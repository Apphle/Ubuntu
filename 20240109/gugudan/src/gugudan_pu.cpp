#include <ros/ros.h>
#include <std_msgs/Int32.h>

int main(int argc, char **argv) 
{
    ros::init(argc, argv, "gugudan_pu");
    ros::NodeHandle n;
    ros::Publisher pub = n.advertise<std_msgs::Int32>("gugudan_result", 10);
    ros::Rate loop_rate(1);  // 1 Hz

    while (ros::ok()) 
    {
        for (int dan = 1; dan <= 9; ++dan) 
        {
            for (int i = 1; i <= 9; ++i) 
            {
                std_msgs::Int32 msg;
                msg.data = dan * i;
                ROS_INFO("Publishing %d * %d = %d", dan, i, msg.data);
                pub.publish(msg);
                ros::spinOnce();
                loop_rate.sleep();
            }
        }
    }

    return 0;
}
