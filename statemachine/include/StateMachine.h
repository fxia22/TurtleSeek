#ifndef _STATEMACHINE_H
#define _STATEMACHINE_H
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "rosgraph_msgs/Log.h"
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
	ros::NodeHandle* _nh;
    ros::Subscriber* _sub; 
    ros::Rate* _rate;
};


#endif
