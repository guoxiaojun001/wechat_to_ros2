#include <chrono>
#include <functional>
#include <memory>
#include <string>

#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/string.hpp"

#include "MQTTClient.h"
#include "mqtt_c.hpp"

using namespace std::chrono_literals;

class MinimalPublisher : public rclcpp::Node {
public:
    MinimalPublisher() : Node("minimal_publisher") {
        // 初始化ros发布的消息
        publisher_ = this->create_publisher<std_msgs::msg::String>("topic", 10);

        // 初始化mqtt的消息及加载回调函数
        mqtt_ = new Mqtt_C("tcp://broker.emqx.io:1883", "c-client", "test", "test", "/wechat_ros/mqtt", 0, std::bind(&MinimalPublisher::getMqttMessage, 
                                                                                                                     this,
                                                                                                                     std::placeholders::_1,
                                                                                                                     std::placeholders::_2,
                                                                                                                     std::placeholders::_3,
                                                                                                                     std::placeholders::_4));
    }
    
private:
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
    
    Mqtt_C_Ptr mqtt_;

    // 定义mqtt的回调函数
    void getMqttMessage(void *context, char *topicName, int topicLen, MQTTClient_message *message) {
        auto* payload = message->payload;
        std::cout << "Received " << (char*)(payload) << " from " << topicName << " topic" << std::endl;

        // 发布mqtt消息
        mqtt_->publish("/wechat_ros_return/mqtt", "ros2 to wechat");

        // 发布ros消息
        auto ros_message = std_msgs::msg::String();
        ros_message.data = (char*)(payload);
        RCLCPP_INFO(this->get_logger(), "Publishing: '%s'", ros_message.data.c_str());
        publisher_->publish(ros_message);
    }
};

int main(int argc, char * argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<MinimalPublisher>());
    rclcpp::shutdown();
    return 0;
}
