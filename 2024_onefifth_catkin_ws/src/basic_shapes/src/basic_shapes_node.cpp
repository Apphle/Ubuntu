#include <ros/ros.h>
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "std_msgs/Float32.h"
#include <sensor_msgs/NavSatFix.h>
#include <geometry_msgs/Pose2D.h>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <vector>
#include <math.h>

// UTM 변환에 필요한 상수 정의
#define UTM_PI 3.14159265358979323846
#define UTM_A 6378137.0
#define UTM_E 0.081819191
#define UTM_K0 0.9996

// Point 구조체 정의
struct Point
{
    float x;
    float y;
    float z;
};

double latitude, longitude, altitude; // GPS 데이터 저장 변수
double utm_east, utm_north; // UTM 좌표 저장 변수
bool new_gps_data = false; // 새로운 GPS 데이터 플래그

ros::Publisher utm_pub; // UTM 좌표 퍼블리셔
ros::Publisher lla_pub; // 위도-경도 좌표 퍼블리셔

// Datum 좌표 저장 변수
double datum_lat = 0.0;
double datum_lon = 0.0;
double datum_utm_east = 0.0;
double datum_utm_north = 0.0;
bool datum_set = false; // Datum 설정 여부 플래그

ros::Publisher marker_pub; // RViz 시각화 퍼블리셔
std::vector<Point> trajectory; // 경로 저장 벡터

// GPS Fix 데이터에서 위도와 경도를 추출하는 함수
void gps_fix_lla(const sensor_msgs::NavSatFix& gps_fix, double* latitude, double* longitude) 
{
    *latitude = gps_fix.latitude;
    *longitude = gps_fix.longitude;

    printf("Received GPS data: Latitude = %9.6lf, Longitude = %9.6lf\n", *latitude, *longitude);
}

// GPS 데이터 콜백 함수
void gps1_Callback(const sensor_msgs::NavSatFix::ConstPtr& msg) 
{
    gps_fix_lla(*msg, &latitude, &longitude);
    new_gps_data = true;
}

// 위도와 경도를 UTM 좌표로 변환하는 함수
void ToUTM(double latitude, double longitude, double& utmX, double& utmY) 
{
    double lat = latitude * UTM_PI / 180.0;
    double lon = longitude * UTM_PI / 180.0;

    int zone = (int)((longitude + 180) / 6) + 1;
    double lon0 = (zone * 6 - 183) * UTM_PI / 180.0;

    double N = UTM_A / sqrt(1 - UTM_E * UTM_E * sin(lat) * sin(lat));
    double T = tan(lat) * tan(lat);
    double C = UTM_E * UTM_E * cos(lat) * cos(lat) / (1 - UTM_E * UTM_E);
    double A = (lon - lon0) * cos(lat);

    double M = UTM_A * ((1 - UTM_E * UTM_E / 4 - 3 * UTM_E * UTM_E * UTM_E * UTM_E / 64 - 5 * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E / 256) * lat
              - (3 * UTM_E * UTM_E / 8 + 3 * UTM_E * UTM_E * UTM_E * UTM_E / 32 + 45 * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E / 1024) * sin(2 * lat)
              + (15 * UTM_E * UTM_E * UTM_E * UTM_E / 256 + 45 * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E / 1024) * sin(4 * lat)
              - (35 * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E * UTM_E / 3072) * sin(6 * lat));

    utmX = UTM_K0 * N * (A + (1 - T + C) * A * A * A / 6
           + (5 - 18 * T + T * T + 72 * C - 58 * UTM_E * UTM_E) * A * A * A * A * A / 120)
           + 500000.0;

    utmY = UTM_K0 * (M + N * tan(lat) * (A * A / 2 + (5 - T + 9 * C + 4 * C * C) * A * A * A * A / 24
           + (61 - 58 * T + T * T + 600 * C - 330 * UTM_E * UTM_E) * A * A * A * A * A * A / 720));

    if (latitude < 0) 
    {
        utmY += 10000000.0;
    }
}

// Datum 좌표를 설정하고 변환하는 함수
void set_datum(double lat, double lon)
{
    datum_lat = lat;
    datum_lon = lon;
    ToUTM(datum_lat, datum_lon, datum_utm_east, datum_utm_north);
    datum_set = true;
    ROS_INFO("Datum set: Latitude = %f, Longitude = %f", datum_lat, datum_lon);
    ROS_INFO("Datum UTM: East = %f, North = %f", datum_utm_east, datum_utm_north);
}

// RViz에 경로를 시각화하는 함수
void pub_markers() 
{
    visualization_msgs::MarkerArray marker_array;
    
    for (size_t i = 0; i < trajectory.size(); i++) 
    {
        visualization_msgs::Marker marker;
        marker.header.frame_id = "map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "gps_trajectory";
        marker.id = i;
        marker.type = visualization_msgs::Marker::SPHERE;
        marker.action = visualization_msgs::Marker::ADD;
        
        marker.pose.position.x = trajectory[i].x;
        marker.pose.position.y = trajectory[i].y;
        
        marker.pose.orientation.w = 1.0;
        
        marker.scale.x = 0.05;
        marker.scale.y = 0.05;
        
        marker.color.r = 0.0;
        marker.color.g = 1.0;
        marker.color.b = 0.0;
        marker.color.a = 1.0;
        
        marker_array.markers.push_back(marker);
    }
    
    marker_pub.publish(marker_array);
}

// 위도-경도 좌표를 퍼블리시하는 함수
void pub_lla(double latitude, double longitude) 
{
    geometry_msgs::Pose2D lla_msg;
    lla_msg.x = latitude;
    lla_msg.y = longitude;

    lla_pub.publish(lla_msg);
    printf("Published LLA: Latitude = %9.6lf, Longitude = %9.6lf\n", latitude, longitude);
}

// UTM 좌표를 퍼블리시하는 함수
void pub_utm(double utmX, double utmY) 
{
    geometry_msgs::Pose2D utm_msg;
    utm_msg.x = utmX;
    utm_msg.y = utmY;

    utm_pub.publish(utm_msg);
    printf("Published UTM: utm_East = %9.6lf, utm_North = %9.6lf\n", utmX, utmY);
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "basic_shapes_node");
    ros::NodeHandle n;

    std::string gps1_fix_topic = "/gps1/fix";
    std::string utm_pub_topic = "/gps1/utm";
    std::string lla_pub_topic = "/gps1/lla";
    std::string marker_topic = "gps_markers";
    
    // 파라미터 서버에서 토픽 이름을 읽어옴
    ros::param::get("~gps1_fix_topic", gps1_fix_topic);
    ros::param::get("~utm_pub_topic", utm_pub_topic);
    ros::param::get("~lla_pub_topic", lla_pub_topic);
    ros::param::get("~marker_topic", marker_topic);

    // 구독자와 퍼블리셔 초기화
    ros::Subscriber sub_gps1_fix = n.subscribe(gps1_fix_topic, 1, gps1_Callback);
    
    utm_pub = n.advertise<geometry_msgs::Pose2D>(utm_pub_topic, 10);
    lla_pub = n.advertise<geometry_msgs::Pose2D>(lla_pub_topic, 10);
    marker_pub = n.advertise<visualization_msgs::MarkerArray>(marker_topic, 1);
        
    ros::Rate loop_rate(25.0); // 루프 주기 설정

    while (ros::ok())
    {
        if (new_gps_data)
        {
            ToUTM(latitude, longitude, utm_east, utm_north);
            printf("Converted to UTM : utm_East = %9.6lf utm_North = %9.6lf\n\n", utm_east, utm_north);
            
            if (!datum_set) 
            {
                set_datum(latitude, longitude);
            }
            
            // Datum에 대해 상대 좌표 계산
            Point p;
            p.x = utm_east - datum_utm_east;
            p.y = utm_north - datum_utm_north;
            p.z = 0;
            
            trajectory.push_back(p);
            
            ROS_INFO("Relative position: East = %f, North = %f", p.x, p.y);
            
            pub_markers(); // RViz에 경로 시각화
            
            new_gps_data = false;
            
            pub_lla(latitude, longitude); // 위도-경도 퍼블리시
            pub_utm(utm_east, utm_north); // UTM 좌표 퍼블리시

            new_gps_data = false;
        }
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}
