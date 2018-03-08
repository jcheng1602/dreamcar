#include "ros/ros.h"  
#include <serial/serial.h>  
#include <std_msgs/String.h>  
#include <std_msgs/Empty.h>  
#include "sensor_msgs/JointState.h"  
#include <string>  
#include <sstream>  
serial::Serial ros_ser;

void callback(const std_msgs::String::ConstPtr& msg){
     ROS_INFO_STREAM("Write to serial port" << msg->data);
     ros_ser.write(msg->data);
 }
int main (int argc, char** argv){
     ros::init(argc, argv, "my_serial_node");
     ros::NodeHandle n;
     
     ros::Subscriber command_sub = n.subscribe("command", 1000, callback);//回调函数
     
     ros::Publisher sensor_pub = n.advertise<std_msgs::String>("sensor", 1000);
     try
     {
         ros_ser.setPort("/dev/ttyUSB0");//串口号可以修改
         ros_ser.setBaudrate(115200);
         serial::Timeout to = serial::Timeout::simpleTimeout(1000);
         ros_ser.setTimeout(to);
         ros_ser.open();
     }
     catch (serial::IOException& e)
     {
         ROS_ERROR_STREAM("Unable to open port ");
         return -1;
     }
     if(ros_ser.isOpen()){
         ROS_INFO_STREAM("Serial Port opened");
     }else{
         return -1;
     }
     ros::Rate loop_rate(10);
     while(ros::ok()){
         ros::spinOnce();
         if(ros_ser.available()){
            // ROS_INFO_STREAM("Reading from serial port");
             std_msgs::String serial_data;
             
             serial_data.data = ros_ser.read(ros_ser.available());
             ROS_INFO_STREAM("Read: " << serial_data.data);
             
             sensor_pub.publish(serial_data);
         }
         loop_rate.sleep();
     }
 }
