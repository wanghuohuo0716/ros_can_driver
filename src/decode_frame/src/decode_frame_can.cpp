#include "decode_frame_can.hpp"


//构造函数
DecodeFrame::DecodeFrame(void){
	memset(&transition,0,sizeof(transition));//初始化can发送的 date临时数组 

	socket_can_sub = nh.subscribe("/joy", 100, &DecodeFrame::socket_can_sub_callback,this);
	roscan_receive_message = nh.subscribe("/received_messages",100,&DecodeFrame::ros_canopen_receive_callback,this);

        roscan_send_message = nh.advertise<can_msgs::Frame>("sent_messages",100); // 订阅ros can open 发布话题
        write_socket_can_timer = nh.createTimer(ros::Duration(0.01), &DecodeFrame::CB_write_can_Cycle, this);//socket can 定时发送；
	wheel_vel_pub  = nh.advertise<geometry_msgs::Twist>("tank/cmd_vel", 100);
	
}


void DecodeFrame::socket_can_sub_callback(const sensor_msgs::Joy::ConstPtr& msg)
{
	memset(&transition,0,sizeof(transition));//初始化can发送的 date临时数组
	

	float left1 = msg->axes[3];
	float right1 = msg->axes[3];

	float left2 = -msg->axes[2];
	float right2 = msg->axes[2];

	float left = left1 + left2;
	float right = right1 + right2;

	if(left > 0)
		transition[1] = int(left*40) | 0x80; // 首位为1,0-80
	else
		transition[1] = -1 * int(left*40); // 首位为0,0-80


	if(right > 0)
		transition[2] = int(right*40) | 0x80;
	else
		transition[2] =  -1 * int(right*40);


	transition[0]=0;

	// ROS_INFO("AXES:%d", s_buffer[21]);
}

//解析ros canopen 发送过来的can_frame话题
void DecodeFrame::ros_canopen_receive_callback(const can_msgs::Frame::ConstPtr& msg )
{
	uint32_t id;
  	bool is_rtr;
  	bool is_extended;
  	bool is_error;
  	uint8_t dlc;
  	uint8_t data;

	id = msg->id;
  	is_rtr = msg->is_rtr;
        is_extended = msg->is_extended;
        dlc = msg->dlc;

	if(id==17){
		//data = msg->data[0];
		//TODO 发送Twist话题
		// ROS_INFO("AXES:%d", s_buffer[21]);
		twist.linear.x = msg->data[1];
		twist.linear.y = msg->data[2];
		int temp_left = uint8_t(twist.linear.x);
		int temp_right = uint8_t(twist.linear.y);

		// 左侧电机转速反馈
		if(temp_left >= 128)
			temp_left = temp_left & 0x7f; // 正转,去掉字节高位的1
		else
			temp_left = -1 * temp_left; // 反转,高位为0

		// 右侧电机转速反馈
		if(temp_right >= 128)
			temp_right = temp_right & 0x7f; // 正转,去掉字节高位的1
		else
			temp_right = -1 * temp_right; // 反转,高位为0

		twist.linear.x = double(temp_left);
		twist.linear.y = double(temp_right);

		wheel_vel_pub.publish(twist);

		// ROS_INFO("left:%d",temp_left);
		// ROS_INFO("left:%d",temp_right);
	}
}

