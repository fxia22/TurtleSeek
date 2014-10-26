#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rosgraph_msgs/Log.h"
#include "people_msgs/PositionMeasurement.h"
#include <sstream>
#include <vector>
#include <cmath>
#include <tf/transform_listener.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_broadcaster.h>
typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;

class Point
{
public:
	double x,y;
	Point(double xx,double yy){x = xx;y=yy;};
	~Point(){};
	double distance(Point & b){return sqrt((x-b.x)*(x-b.x) + (y-b.y)*(y-b.y)); };
};


class StateMachine
{
public:
	enum State {IDLE,EXPLORING,VISITING,RECOGNIZING,FINISHED};
	State state;
	StateMachine(int argc, char **argv);
	~StateMachine();
    void Run();
    std::vector<Point> plist;
	 
private:
    void stateCallBack(const rosgraph_msgs::Log::ConstPtr& msg);
	void peopleCallBack(const people_msgs::PositionMeasurement::ConstPtr& msg);
	ros::NodeHandle* _nh;
    ros::Subscriber* _sub;
    ros::Subscriber* _people_sub;
    ros::Rate* _rate;
	tf::TransformListener* _tf_listener;
    tf::TransformBroadcaster* _br;
    MoveBaseClient* _ac;
	int statenum;
	int peoplenum;
	void initVisit();
};


#endif
