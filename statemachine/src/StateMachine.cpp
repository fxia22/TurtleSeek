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
    _rate = new ros::Rate(120);
    _people_sub = new ros::Subscriber(_nh->subscribe("people_tracker_measurements",10,&StateMachine::peopleCallBack,this));
    _tf_listener = new tf::TransformListener;
	_ac = new MoveBaseClient("move_base", true);
	system("espeak \"turtle seek start!\"");
    system("espeak \"I am ready to explore to room!\"");
    _br = new tf::TransformBroadcaster;

}

void StateMachine::peopleCallBack(const people_msgs::PositionMeasurement::ConstPtr& msg)
{
    double distance = sqrt((msg->pos.x)*(msg->pos.x)+ (msg->pos.y)*(msg->pos.y));




    if ((state == StateMachine::EXPLORING)&&(distance < 2.5)&&(msg->reliability>0.7)) 
    {
		tf::Transform tfPeople;
		tfPeople.setOrigin( tf::Vector3(msg->pos.x, msg->pos.y, 0.0) );
        tf::Quaternion q;
        q.setRPY(0, 0, 0);
        tfPeople.setRotation(q.normalized());
        _br->sendTransform(tf::StampedTransform(tfPeople, ros::Time::now(), "camera_depth_frame", msg->name));

        ros::spinOnce(); 
        tf::StampedTransform tfPeopleMap;
        
        try{
                _tf_listener->lookupTransform("map", msg->name,  ros::Time(0), tfPeopleMap);
            }
        catch (tf::TransformException ex){
                ROS_ERROR("%s",ex.what());
                ros::Duration(0.2).sleep();
            }
            
        tf::Vector3 pt = tfPeopleMap.getOrigin();


		double x = pt.x();
		double y = pt.y();
		Point p(x,y);
		std::cout<<"name"<<msg->name<<"x"<<x<<";"<<"y"<<y<<std::endl;
		double mindist = 100;
		for (int i=0;i<plist.size();i++)
		{
			if (p.distance(plist[i])<mindist)
				mindist = p.distance(plist[i]);
		}
		if (((mindist>1) || (plist.size()==0)) &&(abs(p.x)>0.1) &&(abs(p.y)>0.1))
		{
			ROS_INFO("find a new persion at %f,%f\n",p.x,p.y);
			char *voice= new char[100];
            sprintf(voice,"espeak \"find a new person at %.1f,%.1f,reliability %.2f\"",p.x,p.y,msg->reliability);
            system(voice);

            delete [] voice;
            plist.push_back(p);
		}
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
			initVisit();
        }
    };
	
    if (str.find("Finished recoginizing")!=std::string::npos)
    {
		if (state == StateMachine::RECOGNIZING)
			state = StateMachine::VISITING;
	}
	

}

void StateMachine::initVisit()
{
	while(!_ac->waitForServer(ros::Duration(5.0))){
	    ROS_INFO("Waiting for the move_base action server to come up");
	  }
	statenum = 0;
	peoplenum = plist.size();
}
void StateMachine::Run()
{
    
	move_base_msgs::MoveBaseGoal goal;
    while (ros::ok())
    {
        switch (state)
        {
            case StateMachine::EXPLORING:
                break;
            case StateMachine::VISITING:
				goal.target_pose.header.frame_id = "map";
				goal.target_pose.header.stamp = ros::Time::now();
				goal.target_pose.pose.position.x = plist[statenum].x;
				goal.target_pose.pose.position.y = plist[statenum].y;
				goal.target_pose.pose.orientation.w = 1.0;
                ROS_INFO("Sending goal");
				_ac->sendGoal(goal);
				_ac->waitForResult();
				if(_ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				    {
						ROS_INFO("Hooray, the base moved 1 meter forward");
						system("espeak \"come to me ,sweet\"");
						statenum ++;
						if (statenum<peoplenum) state = StateMachine::RECOGNIZING;
						else state = StateMachine::FINISHED;
					}
				  else
				    ROS_INFO("The base failed to move forward 1 meter for some reason");
				
                break;
            case StateMachine::RECOGNIZING:
                break;
			case StateMachine::FINISHED:
				ROS_INFO("Finished");
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
