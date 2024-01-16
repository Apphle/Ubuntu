#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float64.h"
#include "std_msgs/Bool.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"

double front_sonar = 0.0;

void sonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_sonar = msg->range;
}

int main(int argc, char **argv)
{
  geometry_msgs::Twist msg_cmd;
  
  ros::init(argc, argv, "aeb_control");
  
  ros::NodeHandle n;

  ros::Subscriber sub = n.subscribe("range_front", 1000, sonarCallback);
  
  ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    printf("Front Sonar Distance: %.2f\n", front_sonar);
	
	if(front_sonar <= 1.0)
	{
		msg_cmd.linear.x = 0.0;
	}
	else
	{
		msg_cmd.linear.x = 0.5;
	}
	pub_cmd_vel.publish(msg_cmd);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
