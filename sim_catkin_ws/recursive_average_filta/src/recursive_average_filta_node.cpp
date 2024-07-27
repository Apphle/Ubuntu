#include "ros/ros.h"
#include "std_msgs/Int8.h"
#include "std_msgs/Float64.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/Range.h"

double front_sonar = 0.0;
double sonar[100] = {0,0};

void Front_Sonar_Callback(const sensor_msgs::Range::ConstPtr& msg)
{
  front_sonar = msg->range;
  for(int i = 0; i < 100; i++)
  {
	  sonar[i] = msg->range;
  }
   
  printf("Front_Sonar Range: [%f]\n", front_sonar);
}

int main(int argc, char **argv)
{
    int count = 0;
    ros::init(argc, argv, "recursive_average_filta");
    ros::NodeHandle n;
    
    std::string front_sonar_topic             = "/range_front";
    
    ros::param::get("~front_sonar_topic",            front_sonar_topic);
    
    ros::Subscriber front_sonar_sub          = n.subscribe(front_sonar_topic, 1, Front_Sonar_Callback);
    
    ros::Rate loop_rate(5.0);// 5.0Hz
    
    while (ros::ok())
    {
        double Xk_average;
        double X[10] = {1, 2.2, 3.7, 4, 5.5, 6, 7.2, 8, 9, 10};
        int k = 50;//내가 원하는 시작점
        int n = 30;// 계산할 범위
        
        Xk_average =  sonar[k - 1] + (1.0 / n) * (sonar[k] - sonar[k - n]);
        printf("Xk_average = %.2lf\n", Xk_average);
        printf("range_front = %.2lf\n", front_sonar);
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }

    return 0;
}

