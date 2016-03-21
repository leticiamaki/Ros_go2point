#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<geometry_msgs/Twist.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Quaternion.h>
#include<tf/tf.h> 

void Callback(const nav_msgs::OdometryConstPtr &msg){
    double yaw; 
    geometry_msgs::Quaternion qt;
    
    qt = msg->pose.pose.orientation;
    yaw = tf::getYaw(qt);
    ROS_INFO("VALUE: %f", yaw);    
}

int main(int argc, char **argv){
    
    ros::init(argc, argv, "quart2yaw");  //node name
    
    ros::NodeHandle node;
    
    ros::Subscriber sub = node.subscribe("/vrep/vehicle/odometry", 1, Callback);
    
    ros::spin();
    
    return 0;
}


