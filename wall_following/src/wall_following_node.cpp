#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"

double front_sonar = 0.0;
double left_sonar = 0.0;
double right_sonar = 0.0;
double error_old = 0.0;

void FrontSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_sonar = msg->range;
  printf("Front Sonar: %.2f    ", front_sonar);
}

void LeftSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  left_sonar = msg->range;
  printf("Left Sonar:  %.2f    ", left_sonar);
}

void RightSonarCallback(const sensor_msgs::Range::ConstPtr& msg)
{
  right_sonar = msg->range;
  printf("Right Sonar: %.2f\n", right_sonar);
}

geometry_msgs::Twist WallFollowing(double kp, double ki, double kd)//geometry_msgs::Twist 타입의 메세지 반환하는 함수 
{
  geometry_msgs::Twist cmd_vel;
  double error = left_sonar - right_sonar;
  double error_d = error - error_old;  
  double error_sum = 0.0;
  error_sum += error;  
  
  double steering_control = kp * error + ki * error_sum + kd * error_d;

  if (front_sonar < 1.2)
  {
    cmd_vel.linear.x = 0.0;
    cmd_vel.angular.z = 0.0;
  }
  else
  {
    cmd_vel.linear.x = 0.5;
    cmd_vel.angular.z = steering_control;
  }

  error_old = error;

  return cmd_vel;
}

int main(int argc, char **argv)
{
  int count = 0;
  
  ros::init(argc, argv, "wall_following");
  ros::NodeHandle n;
  
  ros::Subscriber front_sonar_sub = n.subscribe("range_front", 1000, FrontSonarCallback);
  ros::Subscriber left_sonar_sub = n.subscribe("range_front_left", 1000, LeftSonarCallback);
  ros::Subscriber right_sonar_sub = n.subscribe("range_front_right", 1000, RightSonarCallback);
  
  ros::Publisher sonar_cmd_vel_pub = n.advertise<geometry_msgs::Twist>("/ackermann_steering_controller/cmd_vel", 1000);

  ros::Rate loop_rate(30.0);  

  // kp, ki, kd 값을 원하는 값으로 설정
  double kp = 0.4;
  double ki = 0.0;
  double kd = 0.4;
  
  while (ros::ok())
  {
    geometry_msgs::Twist cmd_vel = WallFollowing(kp, ki, kd);
    sonar_cmd_vel_pub.publish(cmd_vel);
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }
  return 0;
}
