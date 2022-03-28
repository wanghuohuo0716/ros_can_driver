使用：
1.给外界USB转can设备权限
2.查看can设备id，'can0'
3.修改run.sh文件中对应的can设备id和波特率，运行该脚本，'./run.sh'
4.检查硬件设备连接，启动ros节点
5.'roslaunch decodeframe decode_frame_can_node.launch‘

注意：
decodeframe:
下一个can驱动！！！！



ros_canopen:
1.修改can id
    1.1 源码修改`socketcan_to_topic_node.cpp`和`topic_to_socketcan_node.cpp`两个文件中第45行的can id，如`can0`
    1.2 laucnh文件中修改`rosparam`中的`can_device`的`value`

    <node pkg="socketcan_bridge" name="topic_to_socketcan_node" type="topic_to_socketcan_node" output="screen">
     	<param name="can_device" value="can0"/>
    </node>
 
    <node pkg="socketcan_bridge" name="socketcan_to_topic_node" type="socketcan_to_topic_node" output="screen">
        <param name="can_device" value="can0"/>
    </node>

2.修改topic
    2.1 laucnh文件重映射`topic`名称

    <node pkg="socketcan_bridge" name="topic_to_socketcan_node" type="topic_to_socketcan_node" output="screen">
        <remap from="received_messages" to="/bigrobot2/received_messages"/>
    </node>
 
    <node pkg="socketcan_bridge" name="socketcan_to_topic_node" type="socketcan_to_topic_node" output="screen">
        <remap from="sent_messages" to="/bigrobot2/sent_messages"/>
    </node>



