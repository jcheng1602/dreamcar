#include "ros/ros.h"
#include "std_msgs/String.h"
//回调函数
void callback(const std_msgs::String::ConstPtr& msg)
{
 ROS_INFO("listener got: [%s]", msg->data.c_str());
}
int main(int argc, char **argv)
{
 ros::init(argc, argv, "listener");
 ros::NodeHandle n;
 //订阅主题
 ros::Subscriber sub = n.subscribe("sensor", 1000, callback);
 ros::spin();
 return 0;
}
