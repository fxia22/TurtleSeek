#!/usr/bin/env python
# created by bss at 2014-03-13
# Last modified: 2014-03-15, 16:55:45

import roslib
import rospy
from std_msgs.msg import String

pub = rospy.Publisher('speech_rec', String)

def callback(data):
    cmd = data.data
    isSend = False

    if cmd.startswith("my name is "):
        name = cmd.split("my name is ")
        print name
        if len(name) == 2:
            name = name[1]
            print name
            #if (len(name) > 0) and (len(name.split(" ")) == 2):
            names = ["tom", "eureka", "bob", "david", "jack", "lucy", "nick","alex"]
            for keyname in names:
               if keyname == name:    
                   isSend = True
    if cmd.startswith("hello "):
        if len(cmd.split(" ")) == 2:
            isSend = True
            print cmd
            cmd = "hello"

    if isSend:
        print cmd
        pub.publish(String(cmd))
    #rospy.loginfo(rospy.get_name() + ": I heard %s" % data.data)


def listener():
    rospy.init_node('speech_recognize', anonymous=True)
    rospy.Subscriber("/recognizer/output", String, callback)
    rospy.spin()


if __name__ == '__main__':
    listener()

