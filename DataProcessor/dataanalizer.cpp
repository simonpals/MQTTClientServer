#include "dataanalizer.h"
#include "location.h"

namespace ldp {

void DataAnalizer::calculationStep(LocationImpl && location)
{
    if(!location) return;

    auto id = location.getID();

    if(data.count(id)==2) {

        float dist{0};
        float speed{0};
        std::string info;

        auto sid = location.getSessionID() + "/" + id ;
        uint64_t timeDiffMs{0};
        uint64_t lastResponceMs{0};

        unsigned long ms_since_epoch =
            std::chrono::system_clock::now().time_since_epoch() /
            std::chrono::milliseconds(1);

        misc::Point_t diff{};
        auto range = data.equal_range(id);

        //reverse order! in loop
        for (auto it = range.first; it != range.second; ++it) {
            diff = it->second.getPoint() - diff;
            lastResponceMs = std::max(ms_since_epoch - it->second.getTimeStamp(), lastResponceMs);
            timeDiffMs = std::max(it->second.getTimeStamp(), timeDiffMs) -
                    std::min(it->second.getTimeStamp(), timeDiffMs);
        }

        if(lastResponceMs > cMaxSensorDelayMs) {
            info = "Sensor " + sid + " disconnected\n";
            if(auto iter = totalDist.find(sid); iter != totalDist.end())
                totalDist.erase(totalDist.find(sid));
        } else {
            dist = static_cast<float>(diff);
            speed = dist / static_cast<float>(timeDiffMs/1000.f);

            auto totalDis = totalDist[sid] + dist;
            totalDist.insert_or_assign(sid, totalDis);

            info = "Sensor with ID: " + sid + " pass distance: " + std::to_string(totalDis)
                    + " , and has speed[unit per sec]: " + std::to_string(speed);
        }

        info += '\n';
        std::cout << info;

        auto its = data.equal_range(id);
        data.erase ( its.first, its.second );

        assert("We must remove info for this sensor" && data.count(id)==0);
    }
    else if(data.count(id)>2) {
        assert("We need just 2 points to analize" && 0);
    }

    data.emplace(std::move(id), std::move(location));
}

}
