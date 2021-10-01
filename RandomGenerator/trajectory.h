#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include "pch.h"
#include "primitives.h"

namespace RandomClientGenerator {

using namespace misc;

class TrajectoryGenerator;
class LocationInfo;

typedef std::shared_ptr<TrajectoryGenerator> TrajectoryPtr;

class TrajectoryGenerator
{
public:
    virtual ~TrajectoryGenerator() {}
    virtual const Point_t nextStep(LocationInfo *si) = 0;
};

class TrajectoryFactory
{
public:
    enum class Type { LinearType=0 };

    static TrajectoryFactory &instance() {
        static TrajectoryFactory myInstance;
        return myInstance;
    }
    TrajectoryPtr GetGenerator(TrajectoryFactory::Type type) {
        assert(mPathFlyweights.find(type)!=mPathFlyweights.end());
        return mPathFlyweights[type];
    }

private:
    TrajectoryFactory();

    TrajectoryFactory(const TrajectoryFactory &) = delete;
    TrajectoryFactory(TrajectoryFactory&&) = delete;
    TrajectoryFactory& operator=(const TrajectoryFactory &) = delete;
    TrajectoryFactory& operator=(TrajectoryFactory &&) = delete;

    std::unordered_map<Type, TrajectoryPtr> mPathFlyweights;
};

class LinearTrajectory : public TrajectoryGenerator
{
public:
    const Point_t nextStep(LocationInfo *si) override;
};

};

#endif // TRAJECTORY_H
