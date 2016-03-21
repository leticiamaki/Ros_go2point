#include<ros/ros.h>
#include<std_msgs/Float64.h>
#include<geometry_msgs/Twist.h>
#include<nav_msgs/Odometry.h>
#include<geometry_msgs/Quaternion.h>
#include<tf/tf.h>

ros::Publisher pub;


void Callback(const nav_msgs::OdometryConstPtr &msg){
    double yaw, d, teta, xf = 4, yf = 1.5, deltaTeta, k = 1, r = 0.1, v, w, x, y; 
    geometry_msgs::Quaternion qt;
    geometry_msgs::Point position;
    geometry_msgs::Twist twist_msg; 
    
    qt = msg->pose.pose.orientation;
    position= msg->pose.pose.position;
    yaw = tf::getYaw(qt);
    ROS_INFO("VALUE YAW: %f", yaw);   
    
    d = sqrt(pow(xf - position.x, 2) + pow(yf - position.y, 2));
    teta = atan2(yf - position.y, xf - position.x);
    deltaTeta = teta - yaw;
    ROS_INFO("DELTA TETA: %F", deltaTeta);
    
    if (d > 0.1){     // ainda não chegou
        if(deltaTeta > M_PI){
            v = -1;   // alinhado no angulo
            w = 0;
        }
        if(deltaTeta < M_PI){
            v = 1;
            
        }
        else{
            v = 0;  // não alinhado no angulo
            w = (0.1);
        }
    }
    else{   // encontrou a posição de chegada
        v = 0;
        w = 0;
    }
    
    //w = k * deltaTeta;
    //  v = w * r;
    
    twist_msg.linear.x = v;
    twist_msg.angular.z = w; 
    pub.publish(twist_msg);    
    
}

int main(int argc, char **argv){
    
    ros::init(argc, argv, "quart2yaw");  //node name
    
    ros::NodeHandle node;
    
    ros::Subscriber sub = node.subscribe("/vrep/vehicle/odometry", 1, Callback);
    
    pub = node.advertise<geometry_msgs::Twist>("go_to_point",1);
    
    ros::spin();
    
    return 0;
}

