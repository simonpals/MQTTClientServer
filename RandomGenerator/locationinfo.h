#ifndef LOCATIONINFO_H
#define LOCATIONINFO_H

#include "trajectory.h"

namespace RandomClientGenerator {

class LocationInfo
{
public:
    explicit LocationInfo(const std::string &id, const Point_t &curr, const Point_t &last
                          , float speedFactor, Region_t region, TrajectoryPtr trajPtr)
        : mID(id), mCurrPos(curr), mLastPos(last), mRegion(region)
        , mpTrajGenerator(trajPtr), mSpeedFactor(speedFactor) {}

    std::string getID() const { return mID; }

    Point_t getCurrentPosition() const { return mCurrPos; }
    Point_t getLastPosition() const { return mLastPos; }
    Region_t getRestrictedRegion() const { return mRegion; }

    float getSpeedFactor() const { return mSpeedFactor; }

    Point_t makeStep();

private:
    const std::string mID;

    Point_t mCurrPos;
    Point_t mLastPos;
    //actual area
    const Region_t mRegion;

    const TrajectoryPtr mpTrajGenerator;
    const float mSpeedFactor; // from 0 to 1
};

};

#endif // LOCATIONINFO_H
