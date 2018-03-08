#include <ros/ros.h>
#include <geometry_msgs/Twist.h>
#include <std_msgs/String.h> 
#include <sensor_msgs/Joy.h>
#include <iostream>
using namespace std;
 int x,z;
class Teleop
{
    public:
        Teleop();
    private:
    /*data*/
        void callback(const sensor_msgs::Joy::ConstPtr& Joy);
        ros::NodeHandle n;//实例化节点
        ros::Subscriber sub;
        ros::Publisher pub;
        double vlinear,vangular;//控制乌龟的速度,是通过这两个变量调整
        int axis_ang,axis_lin;//axes[]的键

};

Teleop::Teleop()
{
    //我们将这几个变量加上参数,可以方便在launch文件修改
    n.param<int>("axis_linear",axis_lin,4);//默认axes[1]接受速度
    n.param<int>("axis_angular",axis_ang,0);//默认axes[0]接受速度
    n.param<double>("vel_linear",vlinear,1);//默认线速度1单位m/s
    n.param<double>("vel_angular",vangular,1);//默认角速度1单位rad/s
    //发布主题command
    pub=n.advertise<std_msgs::String>("command", 1000);//将速度发布给串口,消息队列1000
    sub=n.subscribe<sensor_msgs::Joy>("joy",10,&Teleop::callback,this);//订阅游戏手柄发来的数据
}
    void Teleop::callback(const sensor_msgs::Joy::ConstPtr& Joy){
       geometry_msgs::Twist v;
       
       v.linear.z=Joy->axes[axis_lin]*vlinear;//将游戏手柄的数据乘以你想要的速度,然后发给乌龟
       v.angular.z=Joy->axes[axis_ang]*vlinear;//将游戏手柄的数据乘以你想要的速度,然后发给乌龟
        v.linear.x=Joy->buttons[3];//Y键表示run
        v.linear.y=Joy->buttons[0];//A键表示后退

       if(v.linear.x>v.linear.y){x=1;}//run,Y按了,A没按
       else if(v.linear.x<v.linear.y){x=2;}//back,A按了,Y没按
       else if(v.linear.x==v.linear.y){x=3;}//break,停车
       
      
       if(v.angular.z>0){z=1;}
       else if(v.angular.z<0){z=2;}
       else if(v.angular.z==0){z=3;}
       ROS_INFO("linear:%.3lf  ,angualr:%.3lf", v.linear.x,v.angular.z);
       std_msgs::String msg;
       std::stringstream ss_data;
       ss_data <<x<<z;
       msg.data = ss_data.str();
       ROS_INFO("%s", msg.data.c_str());
       pub.publish(msg);
      
        
    }

int main (int argc,char** argv)
{
    ros::init(argc,argv, "logteleop");
    Teleop telelog;
    ros::spin();
    return 0;

}


