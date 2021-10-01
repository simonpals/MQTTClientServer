#include "mqttservice.h"

namespace RandomClientGenerator {

bool PahoMQTT::send(const std::string &topic, const std::string &data)
{
    mqtt::connect_options connOpts;
    connOpts.set_keep_alive_interval(mKeepAliveInterval);
    connOpts.set_clean_session(true);

    bool ok = true;

    try {
        if(!getKeepAlive() || !mpMQTT) {
            mpMQTT = std::make_shared<mqtt::client>(getServerURI(), getClientID());
            mpMQTT->set_callback(mCallback);
            mpMQTT->connect(connOpts);
        }

        auto msg = mqtt::make_message(topic, data);
        msg->set_qos(mQos);

        mpMQTT->publish(msg);

        if(!getKeepAlive())
            mpMQTT->disconnect();
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "Error: " << exc.what() << " ["
                  << exc.get_reason_code() << "]" << std::endl;
        ok = false;
    }

    return ok;
}

};
