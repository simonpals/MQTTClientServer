#ifndef SESSIONINFO_H
#define SESSIONINFO_H

#include "locationinfo.h"
#include "mqttservice.h"

namespace RandomClientGenerator {

struct SessionInfo
{
    SessionInfo(std::shared_ptr<LocationInfo> pLocationInfo,
                std::shared_ptr<MQTTService> pMQTTervice)
        : mpLocationInfo{pLocationInfo}, mpMQTTervice{pMQTTervice} {}

    std::shared_ptr<LocationInfo> mpLocationInfo;
    std::shared_ptr<MQTTService> mpMQTTervice;
    std::atomic<bool> mBusy=false;
};

};

#endif // SESSIONINFO_H
