#!/usr/bin/env python
# -*- coding: utf-8 -*-
# file: jsgf2fsg.py
# created by bss at 2014-04-30
# Last modified: 2014-05-09, 10:57:52
# 把 jsgf 转化为 fsg 语法格式
#

import sys
import os
import rospkg
import getopt

def Usage():
    print('jsgf2fsg.py usage:')
    print('把jsgf语法转换为fsg语法文件')
    print('-h,--help: print help message.')
    print('-t,--task: input the name of the task.')
    print('')
    print('example:')
    print('python jsgf2fsg.py -t whoiswho')

def main(argv):
    if len(argv) <= 1:
        Usage()
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv[1:], 'ht:', ['help', 'task='])
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
        elif o in ('-t', '--task'):
            processTask(a)
            sys.exit(0)

def processTask(task):
    furocdir = rospkg.RosPack().get_path('pocketsphinx') + '/furoc/'
    taskdir = furocdir + task + '/'
    os.system('sphinx_jsgf2fsg -jsgf ' + taskdir + 'gram.jsgf -fsg '
        + taskdir + 'finite_state.fsg')

if __name__ == '__main__':
    main(sys.argv)

