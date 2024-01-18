#include "ros/ros.h"
#include "sensor_msgs/Range.h"
#include "geometry_msgs/Twist.h"

double front_sonar = 0.0;
double front_right_sonar = 0.0;
double front_left_sonar = 0.0;
double steering = 0.0; 

void frontSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_sonar = msg->range;
}

void frontRightSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_right_sonar = msg->range;
}

void frontLeftSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_left_sonar = msg->range;
}

float PID_Control_wall()
{
  static double error_old = 0.0;
  static double error_sum = 0.0;

  double error = front_left_sonar - front_right_sonar;
  double error_d = error;
  double Kp = 0.4;
  double Ki = 0.0;
  double Kd = 0.4;
  
  error_d = error - error_old;
  error_sum += error;
  steering = Kp * error + Kd * error_d + Ki * error_sum;

  error_old = error;

  return steering;
}

int main(int argc, char **argv)
{
  geometry_msgs::Twist msg_cmd;
  
  ros::init(argc, argv, "aeb_control");
  
  ros::NodeHandle n;

  ros::Subscriber sub_front = n.subscribe("range_front", 1000, frontSonarCallback);
  ros::Subscriber sub_front_right = n.subscribe("range_front_right", 1000, frontRightSonarCallback);
  ros::Subscriber sub_front_left = n.subscribe("range_front_left", 1000, frontLeftSonarCallback);
  
  ros::Publisher pub_cmd_vel = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel", 1000);

  ros::Rate loop_rate(10);

  while (ros::ok())
  {
    printf("Left : Front : Right = %.2f : %.2f : %.2f\n", front_left_sonar, front_sonar, front_right_sonar);
    
    if (front_sonar <= 1.3)
    {
      msg_cmd.linear.x = 0.0;
      msg_cmd.angular.z = 0.0;
    }
    else
    {
      msg_cmd.linear.x = 0.5;
      PID_Control_wall();
      msg_cmd.angular.z = steering;
    }
    pub_cmd_vel.publish(msg_cmd);
    ros::spinOnce();
    loop_rate.sleep();
  }

  return 0;
}
