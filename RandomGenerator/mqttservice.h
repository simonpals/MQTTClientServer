#ifndef MQTTSERVICE_H
#define MQTTSERVICE_H

#include <mqtt/client.h>
#include "pch.h"
#include "primitives.h"

namespace RandomClientGenerator {

using namespace misc;

class MQTTService
{
public:
    MQTTService(const std::string &serverURIm, const std::string &clientID, bool keepAlive)
        : mServerURI(serverURIm), mclientID(clientID), mKeepAlive(keepAlive) {}

    virtual ~MQTTService() {}
    virtual bool send(const std::string &topic, const std::string &data) = 0;

    std::string getServerURI() const { return mServerURI; }
    std::string getClientID() const { return mclientID; }
    bool getKeepAlive() const { return mKeepAlive; }

private:
    std::string mServerURI;
    std::string mclientID;
    bool mKeepAlive;
};

class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string& cause) override {
        std::string reason = "\nConnection lost " +
                (!cause.empty() ? "\tcause: " + cause : "" ) + "\n";
        std::cout << reason;
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override {
        std::string reason = "\tDelivery complete for token: " +
                std::to_string(tok ? tok->get_message_id() : -1) + "\n";
        std::cout << reason;
    }
};

class PahoMQTT : public MQTTService
{
public:
    PahoMQTT(const std::string &serverURIm, const std::string &clientID, bool keepAlive)
        : MQTTService(serverURIm, clientID, keepAlive) {}
    virtual ~PahoMQTT() { if(mpMQTT && mpMQTT->is_connected()) mpMQTT->disconnect(); }

    bool send(const std::string &topic, const std::string &data) override;

private:
    const int mQos = 0;
    const int mKeepAliveInterval = 20;

    callback mCallback;
    std::shared_ptr<mqtt::client> mpMQTT;
};

};

#endif // MQTTSERVICE_H
