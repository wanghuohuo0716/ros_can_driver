#include <stdint.h> 
#include <iostream>
#include <stdio.h>
#include <stdlib.h> 
#include <string.h>
#include <sys/types.h> 
//#include <sys/socket.h> 
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>
#include <unistd.h> 
#include <net/if.h> 
//#include <socketcan_bridge/topic_to_socketcan.h>
//#include <socketcan_bridge/socketcan_to_topic.h>
//#include <socketcan_interface/socketcan.h>
#include <sys/ioctl.h> 
#include <linux/can.h> 
#include <linux/can/raw.h> 
#include <ros/ros.h>
#include <ros/spinner.h>
#include <cv_bridge/cv_bridge.h>
#include <ros/package.h>
#include <geometry_msgs/Twist.h>
#include <sensor_msgs/Joy.h>
#include <can_msgs/Frame.h>
#include <fstream>

//#include <tf/transform_broadcaster.h>
//#include <nav_msgs/Odometry.h>




class DecodeFrame
{
	public:
		//struct can_frame frame;//要发送的can对象
		can_msgs::Frame can_frame;
		uint8_t transition[8];//can 8字节传递
		
	    	ros::NodeHandle nh;
		
		ros::Publisher roscan_send_message; //ros_canopen 发送对象话题
		ros::Publisher wheel_vel_pub;
		
		ros::Subscriber socket_can_sub;//can口
		ros::Subscriber roscan_receive_message;//ros_canopen 接收对象的话题		
		geometry_msgs::Twist twist;
		ros::Timer write_socket_can_timer; //can 定时发送计时器

		
		DecodeFrame(void);
		~DecodeFrame(){};
		
		void CB_write_can_Cycle(const ros::TimerEvent& e); 
		void socket_can_sub_callback(const sensor_msgs::Joy::ConstPtr &msg);
		void ros_canopen_receive_callback(const can_msgs::Frame::ConstPtr& msg);
};

