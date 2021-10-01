#include "sessioninfo.h"

const unsigned int cSensorsCount{2};
const unsigned int cThreadsCount{ std::min(std::thread::hardware_concurrency(), cSensorsCount) };
const unsigned int cDelayInMs{200}; // send data in each thread with delay

const unsigned int cMax{100};
const RandomClientGenerator::Region_t cRegion{{0,cMax,cMax},{cMax,0,0}}; //{x,y,z}
const RandomClientGenerator::Point_t cBegPoint{50,50,0};

const std::string cFormatVersion{"1"};
const bool cKeepAliveMQTTSession{false};
const std::string configTopic{"simulation/config/"};
const std::string dataStrTopic{"simulation/data/"};
const std::string dataBlobTopic{"simulation/dataBlob/"};

std::atomic<bool> bTerminate = false;
std::function<void(void)> shutdown_handler;

//allow run few processes
pid_t getPID()
{
    return getpid();
}


//Press Ctrl+z to finish
int main(/*int argc, char* argv[]*/)
{
    using namespace RandomClientGenerator;

    {
        PahoMQTT mqtt{cServerURI.data(),"0",false};
        std::cout << "Sending configurations...\n";
        std::cout << "Connecting to server '" << cServerURI.data() << "\n" << std::flush;
        mqtt.send(configTopic+std::to_string(getPID()), cFormatVersion+cDelim
                  +static_cast<std::string>(cRegion));
    }

    std::vector<std::shared_ptr<SessionInfo>> sessionList;
    sessionList.reserve(cSensorsCount);

    for(unsigned int iSession=0; iSession<cSensorsCount; iSession++) {

        std::string idStr = std::to_string(generateUniqueID());

        sessionList.emplace(sessionList.end(), std::make_shared<SessionInfo>
                            (std::make_shared<LocationInfo>(idStr,cBegPoint,cBegPoint,fabsf(floatRand()),cRegion,
                                                            TrajectoryFactory::instance().GetGenerator(TrajectoryFactory::Type::LinearType))
                             , std::make_shared<PahoMQTT>(cServerURI.data(),idStr,cKeepAliveMQTTSession))
                            );
    }

    std::vector<std::future<void>> threads;
    threads.reserve(cThreadsCount);
    std::atomic<uint16_t> index{0};

    std::cout << "Running threads...\n";
    for(unsigned int iThread=0; iThread<cThreadsCount; iThread++) {
        threads.emplace(threads.end(), std::async(std::launch::async, [&](unsigned int iIndex){
            while(!bTerminate.load()) {
                uint16_t oldValue = index.fetch_add(1);
                uint16_t newValue = oldValue % cSensorsCount;

                const auto &job = sessionList[newValue];

                int accessCnt = 0;
                bool expected = false;
                while(!job->mBusy.compare_exchange_weak(expected, true /*desired*/)) {
                    expected = false; // active waiting
                    if(++accessCnt >= 10)
                        std::this_thread::sleep_for(std::chrono::milliseconds(1));
                }

                std::string text = "Thread number: " + std::to_string(iIndex) +
                        " works with session index: " + std::to_string(newValue) + "\n";
                std::cout << text;

                int64_t ms_since_epoch =
                        std::chrono::system_clock::now().time_since_epoch() /
                        std::chrono::milliseconds(1);

                const auto &point = job->mpLocationInfo->makeStep();
                job->mpMQTTervice->send(dataStrTopic+std::to_string(getPID())+'/'
                                        +job->mpLocationInfo->getID(), cFormatVersion
                                        +cDelim+static_cast<std::string>(point)
                                        +cDelim+std::to_string(ms_since_epoch));

                job->mBusy.store(false);

                unsigned int delay = static_cast<unsigned int>(cDelayInMs * fabsf(floatRand()));
                std::this_thread::sleep_for(std::chrono::milliseconds(delay+1));
            }
            std::string text = "Thread number: " + std::to_string(iIndex) + " finished\n";
            std::cout << text;
        }, iThread));
    }

    shutdown_handler = [&threads]{
        for(auto & thread : threads) {
            thread.get();
        }
        std::cout << "All jobs done\n";
    };

    if(std::signal(SIGTSTP, [](int){
                   std::cout << "Stop signal\n";
                   bTerminate.store(true);
                   shutdown_handler(); }
                   ) == SIG_ERR)
    {
        std::cerr << "\nError installing handler\n";
    }

    getchar();

    return 0;
}
