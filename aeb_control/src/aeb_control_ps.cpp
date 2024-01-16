#include "ros/ros.h"
#include "sensor_msgs/Range.h"

void ultrasonicCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  float distance = msg->range;
  ROS_INFO("Ultrasonic distance: %f", distance);
}

int main(int argc, char **argv)
{
  ros::init(argc, argv, "ultrasonic_listener");
  ros::NodeHandle n;

  ros::Subscriber ultrasonic_sub = n.subscribe<sensor_msgs::Range>("/ultrasonic_sensor", 10, ultrasonicCallback);

  ros::spin();

  return 0;
}
