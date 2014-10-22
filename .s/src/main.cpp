// project:     followme
// file:        main.cpp
// created by bss at 2014-03-13
// Last modified: 2014-03-15, 14:36:23
// description: 

#include <stdio.h>
#include <ros/ros.h>
#include <ros/package.h>
#include <std_msgs/String.h>

void recogCallback(const std_msgs::String::ConstPtr& msg)
{
    msg.split();
    ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char** argv)
{
    ros::init(argc, argv, "speech_recognize");
    ros::NodeHandle n;

    ros::Subscriber sub = n.subscribe("/recognizer/output", 1000, recogCallback);

    ros::spin();

    return 0;
}

