#include "mas_ros_hackathon/Laser.hpp"

Laser::Laser(ros::NodeHandle n)
{

}

void Laser::laser_scan_callback(const sensor_msgs::LaserScan::ConstPtr& msg)
{
  int start = 40;
  double safe_distance = 0.2;
  for(int i = 0; i < 7; i++)
  {
    set_avg[i] = calculate_avg(&msg->ranges[start + (i*10)],10);
  }

  if(detect_obstacle(set_avg, safe_distance))
  {
    publish_cmd_vel(-0.1, 0.0, 0.0);
  }
}

double Laser::calculate_avg(const float *range,int length)
{
  double average = 0;
  for(int i = 0; i < length; i++)
  {
    average = average + *(range + i);
  }
  return average/10.0;
}

bool Laser::detect_obstacle(double set_avg[7], double safe_distance)
{
  if(set_avg[0] < safe_distance || set_avg[1] < safe_distance || set_avg[2] < safe_distance || set_avg[3] < safe_distance || set_avg[4] < safe_distance || set_avg[5] < safe_distance || set_avg[6] < safe_distance)
  {
    ROS_WARN("Obstacle detected !!");
    return true;
  }
  return false;
}

void Laser::publish_cmd_vel(double x_vel_t, double y_vel_t, double angular_vel_t)
{
  geometry_msgs::Twist vel_msg;
  vel_msg.linear.x = x_vel_t;
  vel_msg.linear.y = y_vel_t;
  vel_msg.angular.z = angular_vel_t;
  velocity_publisher.publish(vel_msg);
}
