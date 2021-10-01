#ifndef DATAANALIZER_H
#define DATAANALIZER_H

#include "pch.h"
#include "location.h"

namespace ldp {

class LocationImpl;

class IDataAnalizer
{
public:
    virtual ~IDataAnalizer() {}
    virtual void calculationStep(LocationImpl && location) = 0;
};

class DataAnalizer : public IDataAnalizer
{
public:
    void calculationStep(LocationImpl && location) override;

private:
    const uint64_t cMaxSensorDelayMs{10000};
    std::unordered_multimap<std::string, LocationImpl> data;
    std::unordered_map<std::string, float> totalDist;
};

}

#endif // DATAANALIZER_H
