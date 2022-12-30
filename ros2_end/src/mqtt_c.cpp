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

    //将全局函数或类的静态函数
    MQTTClient_setCallbacks(client_, NULL, NULL, callBack, NULL);

    if ((rc = MQTTClient_connect(client_, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect, return code " << rc << std::endl;
        connect_flag_ = 0;
        return;
    } else {
        std::cout << "Connected to MQTT Broker!" << std::endl;
        connect_flag_ = 1;
    }
    // 开始订阅对应topic的消息
    MQTTClient_subscribe(client_, topic, qos);
}

Mqtt_C::Mqtt_C(char* address, char* client_id, char* user_name, char* password, char* topic, int qos, std::function<void (void*, char*, int, MQTTClient_message*)> callBack)
: connect_flag_(0)  {
    int rc;
    topic_ = topic;
    qos_ = qos;

    // 将需要回调的函数进行绑定
    callBackFun = callBack;

    MQTTClient_create(&client_, address, client_id, 0, NULL);
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    conn_opts.username = user_name;
    conn_opts.password = password;

    // 将类的this指针传入on_message函数中，以便静态函数调用类的非静态成员
    MQTTClient_setCallbacks(client_, this, NULL, on_message, NULL);

    if ((rc = MQTTClient_connect(client_, &conn_opts)) != MQTTCLIENT_SUCCESS) {
        std::cout << "Failed to connect, return code " << rc << std::endl;
        connect_flag_ = 0;
        return;
    } else {
        std::cout << "Connected to MQTT Broker!" << std::endl;
        connect_flag_ = 1;
    }
    // 开始订阅对应topic的消息
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
    // 调用绑定的回调函数
    Mqtt_C* ptr = (Mqtt_C*)context;
    ptr->callBackFun(context, topicName, topicLen, message);

    MQTTClient_freeMessage(&message);
    MQTTClient_free(topicName);

    return 1;
}
