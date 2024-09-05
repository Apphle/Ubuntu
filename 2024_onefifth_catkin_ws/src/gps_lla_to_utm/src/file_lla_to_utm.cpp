#include <stdio.h>
#include <math.h>
#include <string.h>
#include <string>

#define WayPoints_NO 500

struct WayPoints {
    double x;
    double y; 
    double theta;
};

struct WayPoints my_waypoints_list[WayPoints_NO];
int no_waypoints = 0;

void init_waypoints(const std::string& file_path) 
{
    FILE* fp;
    
    double x, y, theta;
    int result;

    fp = fopen(file_path.c_str(), "r");

    if (fp == NULL) 
    {
        printf("파일이 없습니다\n\n");
        return;
    }

    do
    {
		result = fscanf(fp, "%lf %lf %lf", &x, &y, &theta);
		
        if (result == EOF) 
        {
            printf("파일 끝남.\n");
            break;
        }
        else
        {
			my_waypoints_list[no_waypoints].x = x;
			my_waypoints_list[no_waypoints].y = y;
			my_waypoints_list[no_waypoints].theta = theta;
			
			no_waypoints++;
		}
    
    }while(result != EOF);

    fclose(fp);
    
    for(int i = 0; i < no_waypoints; i++)
    {
		printf("my_waypoints_list[%d].x     = %lf\n", i, my_waypoints_list[i].x);
		printf("my_waypoints_list[%d].y     = %lf\n", i, my_waypoints_list[i].y);
		printf("my_waypoints_list[%d].theta = %lf\n\n\n", i, my_waypoints_list[i].theta);
	}
    
}

int main() 
{
    init_waypoints("/home/amap/2024_onefifth_catkin_ws/src/gps_lla_to_utm/shark_full.txt");
    return 0;
}
