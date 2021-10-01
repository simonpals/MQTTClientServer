#include "conqueue.h"
#include "location.h"

namespace ldp {

void LocationQueue::push_back(LocationImpl location)
{
    std::unique_lock uLock(mMutex);

    if(mList.size() >= cMaxSize) {
        mWaitingWriters.fetch_add(1);
        mListNotEmpty.wait_for(uLock, std::chrono::milliseconds(cWaitingTimeMs),
                               [this]{ return this->writerPred();});

        mWaitingWriters.fetch_add(-1);
    }

    mList.emplace_back(std::move(location));

    if(this->readerPred()) {
        mListNotEmpty.notify_one();
    }
}

LocationImpl LocationQueue::pop_front()
{
    std::unique_lock lock(mMutex);
    if(mList.size()==0) {
        mWaitingReaders.fetch_add(1);
        mListNotEmpty.wait(lock, [this]{
            return this->readerPred();
        });
        mWaitingReaders.fetch_add(-1);
    }

    auto &elem = mList.front();
    mList.pop_front();
    return std::move(elem);
}

}
