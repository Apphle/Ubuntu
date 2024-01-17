#include "ros/ros.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Imu.h"
#include "tf2/LinearMath/Quaternion.h"
#include "tf2/LinearMath/Matrix3x3.h"

#define DEG2RAD(x) (M_PI / 180.0) * x
#define RAD2DEG(x) (180.0 / M_PI) * x

double roll, pitch, yaw;

void imu1Callback(const sensor_msgs::Imu::ConstPtr& msg) 
{

    tf2::Quaternion q
    (msg->orientation.x,
     msg->orientation.y,
     msg->orientation.z,
     msg->orientation.w);
     
    tf2::Matrix3x3 m(q);      

    m.getRPY(roll, pitch, yaw);
     
    double yaw_degree = RAD2DEG(yaw);// yaw가 라디안으로 값이나와서 degree 로 변환
    
    if (yaw_degree > 360)
    {
      yaw_degree = yaw_degree - 360;//값이 0 ~ 360 을 벗어나지 않게하기위해
    }
    else if (yaw_degree < 0)
    {
      yaw_degree = yaw_degree + 360;
    }
    
    printf("degree:%.2f \n", yaw_degree);
}

int main(int argc, char **argv)
{
  int count = 0;
   
  ros::init(argc, argv, "yaw_control");
  ros::NodeHandle n;
  ros::Subscriber yaw_control_sub = n.subscribe("/imu", 1000, imu1Callback);
  ros::Rate loop_rate(30.0);

  while (ros::ok())
  {
    ros::spinOnce();
    loop_rate.sleep();
    ++count;
  }


  return 0;
}
