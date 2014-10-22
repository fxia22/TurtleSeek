#!/usr/bin/env python
# -*- coding: utf-8 -*-
# File          : rec_starter_cmd.py
# Project       : speech_recognize
# Author        : bss
# created by bss at 2014-07-16
#  Last modified: 2014-07-20, 04:36:06

import sys
import os
import getopt
import roslib
import rospy
import rospkg
from std_msgs.msg import String
from frmsg.msg import starter_state


pub = rospy.Publisher('/starter/cmd', starter_state)
specified = False
task = ""
TASK_LIST = ['followme', 'pickandplace', 'avoidthat',
        'whatdidyousay', 'emergency']
state = starter_state()


def Usage():
    print('rec_starter_cmd.py usage:')
    print('用语音识别启动机器人')
    print('-h,--help: print help message.')
    print('-t,--task: only work for specified task.')
    print('')
    print('example:')
    print('rosrun speech_recognize rec_starter_cmd.py')
    print('rosrun speech_recognize rec_starter_cmd.py -t "follow me"')
    print('')
    print('task list:')
    print(TASK_LIST)


def publish_state(state):
    rate = rospy.Rate(100)
    for i in range(0, 5):
        pub.publish(state)
        rate.sleep()


def playSound(speech):
    mp3dir = rospkg.RosPack().get_path('speech_recognize') + '/resource/sounds/'
    if os.path.exists(mp3dir + speech + '.mp3'):
        os.system('mplayer "' + mp3dir + speech + '.mp3"')
    else:
        ans_speak = speech.replace("'", '')
        os.system("espeak -s 130 --stdout '" + ans_speak + "' | aplay")


def callback(data):
    cmd = data.data
    cmd = cmd.lower().strip()
    if specified:
        if cmd != task:
            return

    if 'how are you tinker how are you tinker' == cmd:
        print('pocketsphinx is ok.')
        playSound('I hear you.')

    if 'tinker please follow me' == cmd:
        if '' == task or 'followme' == task:
            print(cmd)
            playSound('Ok, I will follow you.')
            state.state = starter_state.FOLLOWME
            publish_state(state)
        else:
            print('not this task!')


def listener():
    rospy.init_node('rec_starter_cmd', anonymous=True)
    rospy.Subscriber("/recognizer/cmd_starter", String, callback)
    rospy.spin()


def main(argv):
    try:
        opts, argv = getopt.getopt(argv[1:], 'ht:', ['help', 'task='])
    except getopt.GetoptError, err:
        print(str(err))
        Usage()
        sys.exit(2)
    except:
        Usage()
        sys.exit(1)

    for o, a in opts:
        if o in ('-h', '--help'):
            Usage()
            sys.exit(0)
        if o in ('-t', '--task'):
            specified = True
            task = a.lower().strip()
    listener()


if __name__ == '__main__':
    main(sys.argv)

