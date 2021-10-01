#include "locationinfo.h"

namespace RandomClientGenerator {

Point_t LocationInfo::makeStep()
{
    mLastPos = mCurrPos;
    mCurrPos = mpTrajGenerator->nextStep(this);

    return mCurrPos;
}

};
