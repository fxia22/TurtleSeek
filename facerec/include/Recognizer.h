#ifndef RECOGNIZER_H
#define RECOGNIZER_H
#include <iostream>
#include <opencv/highgui.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>
#include <FaceTracker/Tracker.h>
#include <math.h>
#include "SRC/utils.hpp"
#include <ros/package.h>
#include <ros/ros.h>
#include "reading_image/reading_image.h"
#include "std_msgs/String.h"

class Recognizer
{
private:
 std::string package_path_;
 SRCModel *src_model;
 FACETRACKER::Tracker* t_model;
 FACETRACKER::FDet* detector;
 cv::Mat tri,con;
 char ftFile[256],conFile[256],triFile[256];
 double sci_t;
 ros::NodeHandle* n_;
 ros::Rate* rate_;
 ros::Publisher pub_;
 ros::Subscriber sub_;
 ImageConverter* ic_;
 int _buildarchive();
 string guestname;
 int guestpicnum;
 bool archive;
 bool recognize;
 enum _state{
 TRAINING,
 TESTING 
 }state;
 bool model;
 bool findface;
 int count;
public:
 Recognizer():
    package_path_(),
    src_model(NULL),
    t_model(NULL),
    detector(NULL),
    n_(NULL),
    tri(),
    con(),
    pub_(),
    ic_(NULL),
    rate_(NULL),
    state(TRAINING),
    archive(false)
    {};
 bool Init(int argc, char ** argv);
 int Run();
 void Draw(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi,std::string& name);
 void SaveImage(cv::Mat &image,cv::Mat &shape,cv::Mat &con,cv::Mat &tri,cv::Mat &visi,std::string filename);
 void callBack(const std_msgs::String::ConstPtr& msg);
 ~Recognizer(){};
};


#endif
