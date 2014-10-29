#include "StateMachine.h"
#include <iostream>
#include <string>
#include <tf/transform_broadcaster.h>
StateMachine::StateMachine(int argc, char **argv)
{
	state = StateMachine::IDLE;
	ros::init(argc, argv, "statemachine");
    _nh = new ros::NodeHandle;
    _sub = new ros::Subscriber(_nh->subscribe("/rosout", 1000, &StateMachine::stateCallBack,this));
    _rate = new ros::Rate(120);
    _people_sub = new ros::Subscriber(_nh->subscribe("people_tracker_measurements",10,&StateMachine::peopleCallBack,this));
    _tf_listener = new tf::TransformListener;
	_ac = new MoveBaseClient("move_base", true);
	system("espeak --stdout \"turtle seek start!\" | aplay");
    system("espeak --stdout \"I am ready to explore to room!\" | aplay");
    _br = new tf::TransformBroadcaster;
    _marker_pub = new ros::Publisher(_nh->advertise<visualization_msgs::Marker>("people_visualization_marker", 1));

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
                ros::Time latest = ros::Time(0);
                _tf_listener->waitForTransform("map", msg->name,latest, ros::Duration(3.0));
                _tf_listener->lookupTransform("map", msg->name,latest, tfPeopleMap);
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
		if (((mindist>0.5) || (plist.size()==0)) &&((abs(p.x)>0.1) ||(abs(p.y)>0.1)))
		{
			ROS_INFO("find a new persion at %f,%f\n",p.x,p.y);
			char *voice= new char[100];
            sprintf(voice,"espeak -stdout \"find a new person at %.1f,%.1f,probability %.0f percent \" | aplay",p.x,p.y,msg->reliability*100);
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
    
    if ((state==StateMachine::IDLE)&&((str.find("initial point for exploration")!=std::string::npos)))
            {
                state = StateMachine::EXPLORING;
                system("espeak --stdout \"boundary received, start exploring\" | aplay");
            }
    
    
    if (str.find("Finished exploring")!=std::string::npos)
    {
        std::cout<<"found"<<std::endl;
        if (state == StateMachine::EXPLORING)
        { 
            state = StateMachine::VISITING;
            system("espeak --stdout \"finish exploring,ready to recognize\" | aplay");
			initVisit();
        }
    };
	
    if (str.find("finish recognize")!=std::string::npos)
    {
        if (state == StateMachine::RECOGNIZING)
        {
            
			statenum ++;
            if (statenum<peoplenum)
                state = StateMachine::VISITING;
            else state = StateMachine::FINISHED;
        }
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
    char voicecmd[80];
    
    tf::StampedTransform tfRobotMap;
    
        tf::Vector3 ppos;// = tfRobotMap.getOrigin();
        double robotx;// = ppos.x();
        double roboty;// = ppos.y();
        double lambda = 0.1;
    
    
    
    while (ros::ok())
    {
        switch (state)
        {
            case StateMachine::EXPLORING:
                break;
            case StateMachine::VISITING:
				
                
                         
        
        try{
                ros::Time latest = ros::Time(0);
                _tf_listener->waitForTransform("map", "base_link",latest, ros::Duration(3.0));
                _tf_listener->lookupTransform("map", "base_link",latest, tfRobotMap);
        }
        catch (tf::TransformException ex){
                ROS_ERROR("%s",ex.what());
                ros::Duration(0.2).sleep();
            }
                
               ppos = tfRobotMap.getOrigin();
               robotx = ppos.x();
               roboty = ppos.y();
                
                
                
                goal.target_pose.header.frame_id = "/map";
				goal.target_pose.header.stamp = ros::Time::now();
				goal.target_pose.pose.position.x = plist[statenum].x*(1-lambda) + robotx*lambda;
				goal.target_pose.pose.position.y = plist[statenum].y*(1-lambda) + roboty*lambda;
				goal.target_pose.pose.orientation.w = 1.0;
                ROS_INFO("Sending goal");
                sprintf(voicecmd,"espeak --stdout \"visit people number %d, at %.1f %.1f\" | aplay",statenum,plist[statenum].x,plist[statenum].y);
				system(voicecmd);
                
                _ac->sendGoal(goal);
                ros::Duration(3.0).sleep();
				_ac->waitForResult();
				if(_ac->getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				    {
						ROS_INFO("Hooray, the base moved 1 meter forward");
						system("espeak --stdout \"come to me ,sweet\" | aplay");
                        ROS_INFO("ready to recognize");
						if (statenum<peoplenum) state = StateMachine::RECOGNIZING;
					}
				  else
                    {
				    ROS_INFO("The base failed to move forward 1 meter for some reason");
					system("espeak --stdout \"fail to move\" | aplay");
                    }
                break;
            case StateMachine::RECOGNIZING:
                break;
			case StateMachine::FINISHED:
				ROS_INFO("Finished");
				system("espeak --stdout \"task finished,good bye\" | aplay");
                return ; 
                break;
            default:
                break;
        }
        
        publishMarker();
        ros::spinOnce();
        _rate->sleep();
    }
}

void StateMachine::publishMarker()
{
    uint32_t shape = visualization_msgs::Marker::CUBE; 
    visualization_msgs::Marker marker;
    for (int i=0;i<plist.size();i++)
    {
        marker.header.frame_id = "/map";
        marker.header.stamp = ros::Time::now();
        marker.ns = "people";
        marker.id = i;
        marker.type = shape;
        marker.action = visualization_msgs::Marker::ADD;
        marker.pose.position.x = plist[i].x;
        marker.pose.position.y = plist[i].y;
        marker.pose.position.z = 0;
        marker.pose.orientation.x = 0.0;
        marker.pose.orientation.y = 0.0;
        marker.pose.orientation.z = 0.0;
        marker.pose.orientation.w = 1.0;
        marker.scale.x = 0.25;
        marker.scale.y = 0.25;
        marker.scale.z = 0.25;

        // Set the color -- be sure to set alpha to something non-zero!
        marker.color.r = 0.0f;
        marker.color.g = 1.0f;
        marker.color.b = 0.0f;
        marker.color.a = 1.0f;
        marker.lifetime = ros::Duration();
        _marker_pub->publish(marker);
    }

}


StateMachine::~StateMachine()
{
	std::cout<<"Bye!"<<std::endl;
}
