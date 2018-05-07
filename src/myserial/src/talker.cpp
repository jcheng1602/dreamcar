#include "ros/ros.h"
#include "std_msgs/String.h"
#include <sstream>
int main(int argc, char **argv)
{
 ros::init(argc, argv, "talker");
 ros::NodeHandle n;
 //发布主题command
 ros::Publisher command_pub = n.advertise<std_msgs::String>("command", 1000);
 ros::Rate loop_rate(10);
 int count =10;
 while (ros::ok())
 {
   std_msgs::String msg;
   std::stringstream ss_data;
   
   //ss_data << "talker publish command " << count;
   ss_data<< count;
   msg.data = ss_data.str();
   ROS_INFO("%s", msg.data.c_str());
   command_pub.publish(msg);
   ros::spinOnce();
   loop_rate.sleep();
    ++count;
  }
  return 0;
}
