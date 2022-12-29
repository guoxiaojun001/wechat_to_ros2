#include "mqtt_c.hpp"

Mqtt_C::Mqtt_C(char* address, char* client_id, char* user_name, char* password, char* topic, int qos, MQTTClient_messageArrived *callBack)
: connect_flag_(0) {
    int rc;
    topic_ = topic;
    qos_ = qos;

    MQTTClient_create(&client_, address, client_id, 0, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = user_name;
    conn_opts.password = password;
    MQTTClient_setCallbacks(client_, NULL, NULL, callBack, NULL);
    if ((rc = MQTTClient_connect(client_, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect, return code " << rc << std::endl;
        connect_flag_ = 0;
        return;
    } else {
        std::cout << "Connected to MQTT Broker!" << std::endl;
        connect_flag_ = 1;
    }
    // subscribe topic
    MQTTClient_subscribe(client_, topic, qos);
}

Mqtt_C::Mqtt_C(char* address, char* client_id, char* user_name, char* password, char* topic, int qos, std::function<void (void*, char*, int, MQTTClient_message*)> callBack)
: connect_flag_(0)  {
    int rc;
    topic_ = topic;
    qos_ = qos;

    callBackFun = callBack;

    MQTTClient_create(&client_, address, client_id, 0, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = user_name;
    conn_opts.password = password;
    MQTTClient_setCallbacks(client_, this, NULL, on_message, NULL);
    if ((rc = MQTTClient_connect(client_, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect, return code " << rc << std::endl;
        connect_flag_ = 0;
        return;
    } else {
        std::cout << "Connected to MQTT Broker!" << std::endl;
        connect_flag_ = 1;
    }
    // subscribe topic
    MQTTClient_subscribe(client_, topic, qos);

    
}

Mqtt_C::~Mqtt_C() {
    if(connect_flag_ == 1) {
        MQTTClient_disconnect(client_, TIMEOUT);
        MQTTClient_destroy(&client_);
    }

    std::cout << "Close the mqtt connect" << std::endl;
}

void Mqtt_C::publish(char *topic, char *payload) {
    if(connect_flag_ == 1) {
        MQTTClient_message message = MQTTClient_message_initializer;
        message.payload = payload;
        message.payloadlen = strlen(payload);
        message.qos = qos_;
        message.retained = 0;
        MQTTClient_deliveryToken token;
        MQTTClient_publishMessage(client_, topic, &message, &token);
        MQTTClient_waitForCompletion(client_, token, TIMEOUT);
        std::cout << "Send " << payload << " to topic " << topic << std::endl;
    }
    else {
        std::cout << "Failed to connect" << std::endl;
    }
}

int Mqtt_C::on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
    Mqtt_C* ptr = (Mqtt_C*)context;
    ptr->callBackFun(context, topicName, topicLen, message);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}

void Mqtt_C::run() {
    while(true) {
        sleep(1);
    }
}
