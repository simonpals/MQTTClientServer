#include "trajectory.h"
#include "locationinfo.h"

namespace RandomClientGenerator {

TrajectoryFactory::TrajectoryFactory()
    : mPathFlyweights{{Type::LinearType, std::make_shared<LinearTrajectory>()}} { }


const Point_t LinearTrajectory::nextStep(LocationInfo *si)
{
    const auto &curr = si->getCurrentPosition();
    const auto &prev = si->getLastPosition();

    float step = si->getRestrictedRegion().diagonal()*cEps*si->getSpeedFactor();
    assert("step - should be greate than zero" && step>0);

    auto point = curr - prev; // remain the same direction
    Point_t newPoint = (static_cast<float>(point)==0 ?
                            curr+Point_t::randUnitVector()*step : curr+point);

    while(!si->getRestrictedRegion().isInsideRegion(newPoint)) {
        newPoint = curr + Point_t::randUnitVector() * step;
        std::string mes = "Sensor: " + si->getID() + ", point: " +
                static_cast<std::string>(newPoint) + " is outside of the region \n";
        std::cout << mes;
    }

    return newPoint;
}

};
