#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File          : fake_recognizer.py
# Author        : bss
# Creation date : 2014-05-09
#  Last modified: 2014-05-09, 16:58:20
# Description   : send /recognizer/output. debug only.
#

import sys
import os
import rospkg
import rospy
from std_msgs.msg import String

def main(argv):
    # Listen to /recognizer/output from pocketsphinx, task:answer
    rospy.init_node('fake_recognizer', anonymous=True)
    pub = rospy.Publisher('/recognizer/output', String)
    while True:
        cmd = raw_input().strip()
        pub.publish(cmd)

if __name__ == '__main__':
    main(sys.argv)
