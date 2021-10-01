#include "location.h"

namespace ldp {

LocationImpl::SharedPtr LocationImpl::sSharedState;
std::mutex LocationImpl::sSharedLock;

struct Location
{
    uint64_t timeStamp;
    misc::Point_t point;
    std::string id;
    std::string sessionID;
};

LocationImpl::LocationImpl(uint64_t timeStamp, const misc::Point_t &point,
                           const std::string &id, const std::string &sessionID)
    : pImpl(std::shared_ptr<Location>(new Location{timeStamp, point, id, sessionID}))
{ }

uint64_t LocationImpl::getTimeStamp() const
{
    return pImpl->timeStamp;
}

misc::Point_t LocationImpl::getPoint() const
{
    return pImpl->point;
}

std::string LocationImpl::getID() const
{
    return pImpl->id;
}

std::string LocationImpl::getSessionID() const
{
    return pImpl->sessionID;
}

void LocationImpl::updateSharedData(const std::string &sessionID,
                                    std::shared_ptr<misc::Region_t> pRegion)
{
    std::lock_guard<std::mutex>  guard(sSharedLock);
    LocationImpl::sSharedState.insert_or_assign(sessionID, pRegion);
}

std::shared_ptr<misc::Region_t> LocationImpl::getRegionPtr(
        const std::string &sessionID)
{
    std::lock_guard<std::mutex>  guard(sSharedLock);

    if(sSharedState.find(sessionID)!=sSharedState.end())
        return sSharedState[sessionID];

    return std::shared_ptr<misc::Region_t>{nullptr};
}

}
