#include "StateMachine.h"
#include <iostream>
#include <string>


StateMachine::StateMachine(int argc, char **argv)
{
	state = StateMachine::EXPLORING;
	ros::init(argc, argv, "statemachine");
    _nh = new ros::NodeHandle;
    _sub = new ros::Subscriber(_nh->subscribe("/rosout", 1000, &StateMachine::stateCallBack,this));
    _rate = new ros::Rate(30);
}

void StateMachine::stateCallBack(const rosgraph_msgs::Log::ConstPtr& msg)
{
    std::string str = msg->msg;
    std::cout<<str<<std::endl;
    if (str.find("Finished exploring")!=std::string::npos)
    {
        std::cout<<"found"<<std::endl;
        if (state == StateMachine::EXPLORING)
            state = StateMachine::VISITING;
    }

}

void StateMachine::Run()
{
    while (ros::ok())
    {
        ros::spinOnce();
        _rate->sleep();
    }
}

StateMachine::~StateMachine()
{
	std::cout<<"Bye!"<<std::endl;
}
