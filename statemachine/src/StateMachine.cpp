#include "StateMachine.h"
#include <iostream>
#include <string>
#include <tf/transform_broadcaster.h>
StateMachine::StateMachine(int argc, char **argv)
{
	state = StateMachine::EXPLORING;
	ros::init(argc, argv, "statemachine");
    _nh = new ros::NodeHandle;
    _sub = new ros::Subscriber(_nh->subscribe("/rosout", 1000, &StateMachine::stateCallBack,this));
    _rate = new ros::Rate(30);
    _people_sub = new ros::Subscriber(_nh->subscribe("people_tracker_measurements",1000,&StateMachine::peopleCallBack,this));
    system("espeak \"turtle seek start!\"");
    system("espeak \"now ready to explore!\"");
}

void StateMachine::peopleCallBack(const people_msgs::PositionMeasurement::ConstPtr& msg)
{
    if (state == StateMachine::EXPLORING)
    {


    }


}

void StateMachine::stateCallBack(const rosgraph_msgs::Log::ConstPtr& msg)
{
    std::string str = msg->msg;
    std::cout<<str<<std::endl;
    if (str.find("Finished exploring")!=std::string::npos)
    {
        std::cout<<"found"<<std::endl;
        if (state == StateMachine::EXPLORING)
        {
            state = StateMachine::VISITING;
            system("espeak \"finish exploring,ready to recognize\"");
        }
    }

}

void StateMachine::Run()
{
    while (ros::ok())
    {
        switch (state)
        {
            case StateMachine::EXPLORING:
                break;
            case StateMachine::VISITING:
                break;
            case StateMachine::RECOGNIZING:
                break;
            default:
                break;
        }
        ros::spinOnce();
        _rate->sleep();
    }
}

StateMachine::~StateMachine()
{
	std::cout<<"Bye!"<<std::endl;
}
