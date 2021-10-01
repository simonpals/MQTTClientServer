#include "sessionmanager.h"
#include "mqtt/async_client.h"

namespace ldp {

class SessionManagerPrivate
{
public:
    explicit SessionManagerPrivate(const std::string &serverURI
                                   , MsgHandlerPtr handler);

    ~SessionManagerPrivate() {
        stop();
    }

    bool start(const std::vector<std::string> &topics);
    void stop();

private:
    const std::string sServerID{"LDPServer"};

    std::string mServerURI;
    MsgHandlerPtr mHandler;
    std::shared_ptr<mqtt::async_client> pMqtt;
};

SessionManagerImpl::SessionManagerImpl(const std::string &serverURI
                                       , MsgHandlerPtr handler)
    : pImpl{std::make_shared<SessionManagerPrivate>(serverURI,handler)}
{
}

bool SessionManagerImpl::start(const std::vector<std::string> &topics)
{
    return pImpl->start(topics);
}

void SessionManagerImpl::stop()
{
    pImpl->stop();
}


SessionManagerPrivate::SessionManagerPrivate(const std::string &serverURI
                                             , MsgHandlerPtr handler)
    : mServerURI(serverURI), mHandler{handler}
{ }

bool SessionManagerPrivate::start(const std::vector<std::string> &topics)
{
    pMqtt = std::make_shared<mqtt::async_client>(mServerURI, sServerID);

    // Connect options for a persistent session and automatic reconnects.
    auto connOpts = mqtt::connect_options_builder()
            .clean_session(false)
            .automatic_reconnect(std::chrono::seconds(2), std::chrono::seconds(30))
            .finalize();

    const std::vector<int> QOS { 0, 1 };

    // Set a callback for connection lost.
    // This just exits the app.
    pMqtt->set_connection_lost_handler([](const std::string&) {
        std::cout << "*** Connection Lost  ***" << std::endl;
        exit(2);
    });

    // Set the callback for incoming messages
    pMqtt->set_message_callback([this](mqtt::const_message_ptr msg) {
        std::cout << msg->get_topic() << " :  " << msg->get_payload_str() << std::endl;
        if(mHandler) {
            mHandler(msg->get_topic(), msg->get_payload_str());
        }
    });

    try {
        std::cout << "Connecting to the MQTT server at " << mServerURI << "..." << std::flush;
        auto rsp = pMqtt->connect(connOpts)->get_connect_response();
        std::cout << "OK\n" << std::endl;

        // Subscribe if this is a new session with the server
        if (!rsp.is_session_present())
            pMqtt->subscribe(mqtt::string_collection::create(topics), QOS);
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
        return false;
    }

    return true;
}

void SessionManagerPrivate::stop()
{
    try {
        mHandler = nullptr;
        if(pMqtt && pMqtt->is_connected()) {
            std::cout << "OK\nDisconnecting..." << std::flush;
            pMqtt->disconnect()->wait();
            std::cout << "OK" << std::endl;
            pMqtt.reset();
        }
    }
    catch (const mqtt::exception& exc) {
        std::cerr << exc.what() << std::endl;
    }
}

}
