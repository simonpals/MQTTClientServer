#ifndef LOCATION_H
#define LOCATION_H

#include "pch.h"
#include "primitives.h"

namespace ldp {

struct Location;

class LocationImpl
{
public:
    using SharedPtr = std::unordered_map<std::string, std::shared_ptr<misc::Region_t>>;

    explicit LocationImpl() : pImpl(nullptr) {}
    explicit LocationImpl(uint64_t timeStamp, const misc::Point_t &point,
                          const std::string &id, const std::string &sessionID);

    operator bool() const { return isValid(); }
    bool isValid() const { return pImpl!=nullptr; }

    uint64_t getTimeStamp() const;
    misc::Point_t getPoint() const;
    std::string getID() const;
    std::string getSessionID() const;

    static std::shared_ptr<misc::Region_t> getRegionPtr(
            const std::string &sessionID);
    static void updateSharedData(const std::string &sessionID,
                                 std::shared_ptr<misc::Region_t>);

private:
    std::shared_ptr<Location> pImpl;
    static SharedPtr sSharedState;
    static std::mutex sSharedLock;
};

}

#endif // LOCATION_H
