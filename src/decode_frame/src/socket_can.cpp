#include "decode_frame_can.hpp"
#include <cstdio>
#include <cstdlib>



//can socket 发送s_buffer的数据
void DecodeFrame::CB_write_can_Cycle(const ros::TimerEvent& e)
{
    can_frame.id=0x01;//需要修改
    //can_frame.is_extended = true;
    //can_frame.is_rtr = false;   
    //can_frame.is_error = false;
    can_frame.dlc = 3;//有效数据长度
    //使用临时变量进行can data赋值
    for(uint8_t i =0; i<can_frame.dlc; i++)
        {
          can_frame.data[i] = transition[i];
         }
    //can_frame.data[1]=0x10;
    //can_frame.data[2]=0x10;
    roscan_send_message.publish(can_frame); //发布ros canopen话题
    // can赋值
    //TODO
    //nbytes = write(s,&frame,sizeof(struct can_frame)); //发送can
    //ROS_INFO("Wrote %d bytes\n", nbytes);

}


int main (int argc, char** argv)
{
    //初始化节点
    ros::init(argc, argv, "DecodeFrame_node");
    DecodeFrame decoder;

    ros::Rate loop_rate(10);
    while(ros::ok())
  	{
    	ros::spinOnce();
    	loop_rate.sleep();
  	}
    
}



