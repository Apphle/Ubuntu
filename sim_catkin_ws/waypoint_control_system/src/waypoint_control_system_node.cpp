#include "ros/ros.h"

int main(int argc, char **argv)
{

    int count = 0;
    
    ros::init(argc, argv, "waypoint_control_system");
    ros::NodeHandle n;

   ros::Rate loop_rate(100.0);
  
   while (ros::ok())
   {   
    
    ros::spinOnce();
    loop_rate.sleep();
    ++count;       
    }
    return 0;
}
