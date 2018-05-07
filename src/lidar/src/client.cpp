#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"
#include "std_msgs/String.h"
#include <sstream>
#include "myserial/vehicleControl.h"
#define RAD2DEG(x) ((x)*180./M_PI)
//ros::Publisher command_pub;
int a=0;//全局变量
int b=0;
//myserial::vehicleControl control;
void scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
    int count = scan->scan_time / scan->time_increment;
    //ROS_INFO("I heard a laser scan %s[%d]:", scan->header.frame_id.c_str(), count);
    //ROS_INFO("angle_range, %f, %f", RAD2DEG(scan->angle_min), RAD2DEG(scan->angle_max));
     ROS_INFO("count: %d", count);
    for(int i = 0; i < count; i++) {//判断逻辑,在360度以内,有3个小于1.5,则判断有障碍物,否则没有
        float degree = RAD2DEG(scan->angle_min + scan->angle_increment * i);
       // ROS_INFO(": [%f, %f]", degree, scan->ranges[i]);
        if(scan->ranges[i]<1.5){
          // ROS_INFO("there are things,%f",scan->ranges[i]);
          // ROS_INFO("there is one thing: [%f, %f]", degree, scan->ranges[i]);
           //a=1;
           b++;
           if(b==3){
             b=0;
             //ROS_INFO("there are 3things: [%f, %f]",degree,scan->ranges[i]);
             ROS_INFO("count: %d", count);
             a=1;
             break;
           }//b=1,2,3连续3度有障碍物
        }
       /* else{//若前面有100个大于1.5,在该段时间内,a=0
            // ROS_INFO("there is no thing: [%f, %f]",degree,scan->ranges[i]);
               if(b!=3){
                 a=0;
                 b=0;

               }
            }*/
	
	if(i+1==count){a=0;}
    }
}


int main(int argc, char **argv)
{
    ros::Publisher command_pub;
    myserial::vehicleControl control;
    ros::init(argc, argv, "rplidar_node_client");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe<sensor_msgs::LaserScan>("/scan", 1000, scanCallback);
    //ros::Subscriber sub01 = n.subscribe("command", 1000,commandCallback);
//发布主题command
   command_pub = n.advertise<myserial::vehicleControl>("command00", 1000);
   ros::Rate loop_rate(10);
    
  while (ros::ok())
   { 
      
       if(a==1){
	       control.lidar=1;//lidar为1表示有障碍物
	       command_pub.publish(control);
 	       ROS_INFO("has thing");
       }
       else{ 
	ROS_INFO("no thing");
	control.lidar=0;//lidar为0表示没有有障碍物
	command_pub.publish(control); }
       
       ros::spinOnce();//周期性的调用挂起的回调函数
       loop_rate.sleep();

    }
    ros::spin();

    return 0;
}
