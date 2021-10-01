#include "dataanalizer.h"
#include "parser.h"
#include "sessionmanager.h"
#include "location.h"

template <typename Func>
void getExecutionTime(const std::string& title, Func func)
{
  const auto sta = std::chrono::steady_clock::now();
  func();
  const std::chrono::duration<double> dur = std::chrono::steady_clock::now() - sta;

  std::stringstream stream;
  stream << "thread:" << std::this_thread::get_id() << ", "
         << title << ": " << dur.count() << " sec. " << std::endl;
  std::cout << stream.str();
}

int main()
{
    using namespace ldp;

    std::shared_ptr<IDataAnalizer> analizer = std::make_shared<DataAnalizer>();
    ParserPtr parser = ParserFactory::factoryMethod(ParserFactory::Format::Version1);

    SessionManagerImpl smi{misc::cServerURI.data(),
                [&](const std::string& topic, const std::string& data)
        {
            //TODO: add new task to thread pool...
            getExecutionTime(" time consuming: ",[&](){
                LocationImpl location {parser->parse(topic, data)};
                analizer->calculationStep(std::move(location));
            });
        }};

    smi.start({ std::string(misc::dataStrTopic)+"#"
                , std::string(misc::configTopic)+"#" });

    getchar();

    smi.stop();

    return 0;
}
