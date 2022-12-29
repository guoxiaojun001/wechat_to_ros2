#ifndef MQTT_C_HPP_
#define MQTT_C_HPP_

#include <iostream>
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include <functional>

#include "MQTTClient.h"

typedef int funTest(void* context, int topicLen);

class Mqtt_C {
public:
    Mqtt_C(char* address, char* client_id, char* user_name, char* password, char* topic, int qos, MQTTClient_messageArrived *callBack);
    Mqtt_C(char* address, char* client_id, char* user_name, char* password, char* topic, int qos, std::function<void (void*, char*, int, MQTTClient_message*)> callBack);
    ~Mqtt_C();

    void publish(char *topic, char *payload);

private:
    bool connect_flag_;    //0表示连接失败，1表示成功
    long TIMEOUT = 10000;
    char* topic_;
    MQTTClient client_;
    int qos_;

    std::function<void (void*, char*, int, MQTTClient_message*)> callBackFun;

    static int on_message(void *context, char *topicName, int topicLen, MQTTClient_message *message);

    void run();
};

typedef Mqtt_C* Mqtt_C_Ptr;    //不能使用智能指针，否则会关闭mqtt连接

#endif