#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rosgraph_msgs/Log.h"
#include "people_msgs/PositionMeasurement.h"
#include <sstream>
class StateMachine
{
public:
	enum State {IDLE,EXPLORING,VISITING,RECOGNIZING};
	State state;
	
	StateMachine(int argc, char **argv);
	~StateMachine();
    void Run();
    void stateCallBack(const rosgraph_msgs::Log::ConstPtr& msg);
private:
    void peopleCallBack(const people_msgs::PositionMeasurement::ConstPtr& msg);
	ros::NodeHandle* _nh;
    ros::Subscriber* _sub;
    ros::Subscriber* _people_sub;
    ros::Rate* _rate;
};


#endif
