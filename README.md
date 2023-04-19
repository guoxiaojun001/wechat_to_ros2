
# 运行环境
1. ros2的程序需要在ubuntu20.04下运行；
2. ubuntu需要安装mqtt库(git clone https://github.com/eclipse/paho.mqtt.c.git)
3. 微信小程序需要在windows下安装微信开发者工具

# ubuntu安装依赖
1. git clone https://github.com/eclipse/paho.mqtt.c.git
2. cd paho.mqtt.c
3. mkdir build
4. cd build
5. cmake ..
6. make
7. sudo make install

# 编译并运行ros2程序
1. mkdir ros2_ws
2. cd ros2_ws
3. mkdir src
4. cd src
5. git clone https://gitee.com/jdi-shen/wechat_to_ros2.git
6. cd ..
7. colcon build
8. source install/setup.bash
9. ros2 run ros2_end main



主要事项
在运行ros程序时，需要加载环境变量“export LD_LIBRARY_PATH=/usr/local/lib”；
所有源程序在gitee仓库，https://gitee.com/jdi-shen/wechat_to_ros2.git；
运行ros2程序的ubuntu系统和手机都需要连上互联网；