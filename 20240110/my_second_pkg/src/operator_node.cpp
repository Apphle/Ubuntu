#include "ros/ros.h"
#include "std_msgs/Float32.h"
#include "std_msgs/String.h"

float x = 0.0;
float y = 0.0;
std::string operator_value = "";


void xCallback(const std_msgs::Float32::ConstPtr& msg)
{
    x = msg->data;
}

void operatorCallback(const std_msgs::String::ConstPtr& msg)
{
    operator_value = msg->data;
}

void yCallback(const std_msgs::Float32::ConstPtr& msg)
{
    y = msg->data;

    double result = 0.0;

    if (operator_value == "+") 
    {
        result = x + y;
    } 
    else if (operator_value == "-")
    {
        result = x - y;
    } 
    else if (operator_value == "x")
    {
        result = x * y;
    } 
    else if (operator_value == "/") 
    {
        if (y != 0.0) 
        {
            result = x / y;
        } else 
        {
            ROS_WARN("error");
            return;
        }
    } 
    else 
    {
        ROS_WARN_STREAM("error " << operator_value);
        return;
    }

    printf("%.0f %s %.0f = %.1f\n", x, operator_value.c_str(), y, result);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "subscriber_node");
    ros::NodeHandle nh;

    ros::Subscriber x_sub = nh.subscribe("/float32/x", 1, xCallback);
    ros::Subscriber operator_sub = nh.subscribe("/string/operator", 1, operatorCallback);
    ros::Subscriber y_sub = nh.subscribe("/float32/y", 1, yCallback);

    ros::spin();

    return 0;
}
