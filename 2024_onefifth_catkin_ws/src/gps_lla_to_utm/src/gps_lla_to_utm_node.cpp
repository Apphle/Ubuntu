#include "ros/ros.h"
#include "sensor_msgs/NavSatFix.h"
#include <math.h>
#include <iostream>

using namespace std;

#define PI 3.14159265358979323846
#define EQUATORIAL_RADIUS 6378137.0
#define ECCENTRICITY 0.081819190842622

// 구조체 정의
struct gps1 {
    double Latitude;
    double Longitude;
};

// 전역 변수
struct gps1 gps_point;

// UTM 변환 함수
void ToUTM(double latitude, double longitude, double &utmX, double &utmY)
{
    double latitudeRad = latitude * PI / 180.0;
    double longitudeRad = longitude * PI / 180.0;

    int utmZone = (int)((longitude + 180.0) / 6.0) + 1;
    double centralMeridian = PI * (utmZone * 6 - 183) / 180.0;

    double radiusOfCurvatureInPrimeVertical = EQUATORIAL_RADIUS / sqrt(1 - ECCENTRICITY * ECCENTRICITY * sin(latitudeRad) * sin(latitudeRad));
    double tanLatSquared = tan(latitudeRad) * tan(latitudeRad);
    double eccentricityPrimeSquared = ECCENTRICITY * ECCENTRICITY * cos(latitudeRad) * cos(latitudeRad) / (1 - ECCENTRICITY * ECCENTRICITY);
    double longitudeDifference = (longitudeRad - centralMeridian) * cos(latitudeRad);

    double meridianArcDistance = EQUATORIAL_RADIUS * ((1 - ECCENTRICITY * ECCENTRICITY / 4 - 3 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 64 - 5 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 256) * latitudeRad
              - (3 * ECCENTRICITY * ECCENTRICITY / 8 + 3 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 32 + 45 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 1024) * sin(2 * latitudeRad)
              + (15 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 256 + 45 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 1024) * sin(4 * latitudeRad)
              - (35 * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY * ECCENTRICITY / 3072) * sin(6 * latitudeRad));

    utmX = 0.9996 * radiusOfCurvatureInPrimeVertical * (longitudeDifference + (1 - tanLatSquared + eccentricityPrimeSquared) * longitudeDifference * longitudeDifference * longitudeDifference / 6
           + (5 - 18 * tanLatSquared + tanLatSquared * tanLatSquared + 72 * eccentricityPrimeSquared - 58 * ECCENTRICITY * ECCENTRICITY) * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference / 120)
           + 500000.0;

    utmY = 0.9996 * (meridianArcDistance + radiusOfCurvatureInPrimeVertical * tan(latitudeRad) * (longitudeDifference * longitudeDifference / 2 + (5 - tanLatSquared + 9 * eccentricityPrimeSquared + 4 * eccentricityPrimeSquared * eccentricityPrimeSquared) * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference / 24
           + (61 - 58 * tanLatSquared + tanLatSquared * tanLatSquared + 600 * eccentricityPrimeSquared - 330 * ECCENTRICITY * ECCENTRICITY) * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference * longitudeDifference / 720));

    if (latitude < 0)
        utmY += 10000000.0;
}

// ROS 콜백 함수
void gps1fixCallback(const sensor_msgs::NavSatFix::ConstPtr& msg)
{
    gps_point.Latitude = msg->latitude;
    gps_point.Longitude = msg->longitude;

    double utmX, utmY;
    ToUTM(gps_point.Latitude, gps_point.Longitude, utmX, utmY);

    printf("Latitude  = %lf\n", gps_point.Latitude);
    printf("Longitude = %lf\n\n", gps_point.Longitude);
    printf("UTM X     = %lf\n", utmX);
    printf("UTM Y     = %lf\n\n\n\n", utmY);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "gps_lla_to_utm_node");
    ros::NodeHandle n;

    std::string gps1_topic = "/gps1/fix";

    ros::param::get("~gps1_topic", gps1_topic); 

    ros::Subscriber gps1_sub = n.subscribe(gps1_topic, 10, gps1fixCallback);

    ros::Rate loop_rate(25.0);

    while (ros::ok())
    {
        ros::spinOnce();
        loop_rate.sleep();
    }

    return 0;
}
