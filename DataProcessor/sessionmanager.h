#ifndef SESSIONMANAGER_H
#define SESSIONMANAGER_H

#include "pch.h"

namespace ldp {

class SessionManagerPrivate;

using MsgHandlerPtr = std::function<void(const std::string&
                                         ,const std::string&)>;

class SessionManagerImpl
{
public:

    explicit SessionManagerImpl(const std::string &serverURI
                                , MsgHandlerPtr handler);

    bool start(const std::vector<std::string> &topics);
    void stop();

private:
    std::shared_ptr<SessionManagerPrivate> pImpl;
};

}

#endif // SESSIONMANAGER_H
