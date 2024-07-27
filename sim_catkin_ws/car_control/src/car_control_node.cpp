#include "ros/ros.h"
/*#include "std_msgs/String.h"
#include "std_msgs/Int16.h"
#include "geometry_msgs/Twist.h"
#include "sensor_msgs/LaserScan.h"
#include "sensor_msgs/Range.h"
#include "tf/transform_broadcaster.h"
#include "nav_msgs/Odometry.h"
#include <string.h>  
#include <unistd.h>  
#include <errno.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <linux/i2c-dev.h>  
#include <sys/ioctl.h>  
#include <fcntl.h>  
#include <unistd.h>  
#include <sstream>

//i2c address  
#define ADDRESS 0x05

//I2C bus  
static const char *deviceName = "/dev/i2c-0";

#define DEG2RAD(x) (M_PI/180.0*(x) )
#define RAD2DEG(x) ((x)*180.0/M_PI)

#define Max_Angle_L -30
#define Max_Angle_R  30

union car
{
    short data;
    char  mbyte[2];
}speed, angle;

unsigned char protocal_data[9] ={0,};

////////////////////////////////////////////////////////////////////////
int file_I2C;

int open_I2C(void)
{
   int file;  
   
    if ((file = open( deviceName, O_RDWR ) ) < 0)  
    {  
        fprintf(stderr, "I2C: Failed to access %s\n", deviceName);  
        exit(1);  
    }  
    printf("I2C: Connected\n");  
 
   
    printf("I2C: acquiring buss to 0x%x\n", ADDRESS);  
    if (ioctl(file, I2C_SLAVE, ADDRESS) < 0)  
    {  
        fprintf(stderr, "I2C: Failed to acquire bus access/talk to slave 0x%x\n", ADDRESS);  
        exit(1);  
    }
   
    return file;
}

void close_I2C(int fd)
{
   close(fd);
}
////////////////////////////////////////////////////////////////////////

void cmd_callback(const geometry_msgs::Twist &data)
{
	float linear_data;
    float angular_data;
   
    linear_data  = data;
    angular_data = data;
   
    if(angular_data <= Max_Angle_R)  
    {
		angular_data = Max_Angle_R;
	}
    if(angular_data >= Max_Angle_L)  
    {
		angular_data = Max_Angle_L;  
	}
	
    angle.data = (short)angular_temp;
   
    speed.data = (short)linear_temp;

}

void *read_I2C_thread(void *pt)
{
      int num_bytes = -1;
      unsigned char insert_buf;
      -
      
      sleep(100);
         
}

int main(int argc, char **argv)
{
  file_I2C = open_I2C();
  if(file_I2C < 0)
  {
     printf("Unable to open I2C\n");
     return -1;
  }
  else
  {
     printf("I2C is Connected\n");
  }
 
  ros::init(argc, argv, "car_control");
  ros::NodeHandle n;

  std::string cmd_vel_topic = "/cmd_vel";
 
  pthread_t id_1;
  int ret1 = pthread_create(&id_1,NULL,*read_I2C_thread,NULL);
 
  ros::Subscriber sub1 = n.subscribe(cmd_vel_topic, 20, cmd_callback);
 
  ros::Rate loop_rate(LOOP_RATE);
 
 
  while(ros::ok())
  {
   printf("speed.data : %d\n", speed.data);
   printf("angle.data : %d\n", angle.data);
   
   protocal_data[0] = '#';
   protocal_data[1] = 'C';
   protocal_data[2] = speed.mbyte[0];
   protocal_data[3] = speed.mbyte[1];
   protocal_data[4] = angle.mbyte[0];
   protocal_data[5] = angle.mbyte[1];
   protocal_data[6] = 0;  
   protocal_data[7] = 0;    
   protocal_data[8] = '*';
     
   write(file_I2C, protocal_data, 9);
   read(file_I2C,buf,8);
   close_I2C(file_I2C);
   
   ros::spinOnce();
   loop_rate.sleep();
  }

  return 0;
}
*/
