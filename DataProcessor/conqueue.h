#ifndef CONQUEUE_H
#define CONQUEUE_H

#include "pch.h"

namespace ldp {

class LocationImpl;

class IConqurrentQueue
{
public:
    virtual ~IConqurrentQueue() {}
    virtual void push_back(LocationImpl location) = 0;
    virtual LocationImpl pop_front() = 0;
};

class IBalancedTraits
{
public:
    virtual ~IBalancedTraits() {}

    //value which characterized queue loading
    virtual size_t loadingTrait() = 0;
    virtual uint16_t waitingReaders() = 0;
    virtual uint16_t waitingWriters() = 0;
};

class LocationQueue : public IConqurrentQueue, public IBalancedTraits
{
public:
    void push_back(LocationImpl location) override;
    LocationImpl pop_front() override;

    size_t loadingTrait() override {
        std::unique_lock lock(mMutex);
        return mList.size();
    }

    uint16_t waitingReaders() override {
        return mWaitingReaders.load();
    }

    uint16_t waitingWriters() override {
        return mWaitingWriters.load();
    }

private:
    bool writerPred() const {
        return mList.size() < cMaxSize-cWaitToServe || mStopRequest.load();
    };
    bool readerPred() const {
        return mList.size() > cWaitToRead || mStopRequest.load();
    };

    std::list<LocationImpl> mList;
    std::condition_variable mListNotEmpty;
    std::condition_variable mListNotFull;
    std::mutex mMutex;

    std::atomic<int16_t> mWaitingReaders{0};
    std::atomic<int16_t> mWaitingWriters{0};
    std::atomic<bool> mStopRequest{false};

    const static int32_t cMaxSize = 1'000'000;
    const static int32_t cWaitToServe = 10'000;
    const static int32_t cWaitToRead = 10;
    constexpr static int32_t cWaitingTimeMs = 50;
};


//class IThreadBalancer
//{
//public:
//    virtual ~IThreadBalancer() {}
//    virtual bool start() = 0;
//    virtual void stop() = 0;
//};

//class UncopyableMovable
//{
//protected:
//    UncopyableMovable() {}
//    ~UncopyableMovable() {}
//    //allow moving
//    UncopyableMovable(UncopyableMovable &&) {}
//    UncopyableMovable& operator=(UncopyableMovable&&) { return *this;}
//private:
//    UncopyableMovable(const  UncopyableMovable&);
//    UncopyableMovable& operator=(const  UncopyableMovable&);
//};

//class ThreadBalancer : public IThreadBalancer
//        , private UncopyableMovable
//{
//    class ThreadPool
//    {
//    public:
//    private:
//        std::vector<std::thread> pool;
//    };

//public:
//    virtual bool start() override { return false; }
//    virtual void stop() override {}

//private:
//    std::shared_ptr<IBalancedTraits> pTraits;
//};

}

#endif // CONQUEUE_H
