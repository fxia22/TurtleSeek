// project:     followme
// file:        test_reading_image.cpp
// created by bss at 2013-12-21
// Last modified: 2014-07-11, 10:28:57
// description: 

#include "ros/ros.h"
#include <opencv2/opencv.hpp>
#include "reading_image/reading_image.h"

int main(int argc, char** argv)
{
    /* Init */
    printf("Begin init\n");

    ros::init(argc, argv, "test_reading_image");
    ros::NodeHandle n;
    ros::Rate rate(33);

    ImageConverter ic;
    cv::namedWindow("followme image window, test");

    printf("Waiting for ImageConverter\n");
    while (!ic.ready)
    {
        ros::spinOnce();
        rate.sleep();
        if (!ros::ok())
        {
            return 0;
        }
    }
    
    printf("Init OK\n");

    /* Main Loop */
    while (ros::ok())
    {
        if (ic.ready == true)
        {
            cv::imshow("followme image window, test", ic.curr_image);
        }
        ros::spinOnce();
        cv::waitKey(3);
        rate.sleep();
    }

    printf("Bye!\n");
    return 0;
}
