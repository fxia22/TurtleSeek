#!/usr/bin/env python
# -*- coding: utf-8 -*-
# file: init.py
# created by bss at 2014-05-09
# Last modified: 2014-07-16, 17:57:05
# 初始化一组语音识别的任务
#

import sys
import os
import getopt
import rospkg

def Usage():
    print('init.py usage:')
    print('初始化一组语音识别任务，处理${task}/sent.txt,gram.jsgf')
    print('-h,--help: print help message.')
    print('--init: init a dir for your task')
    print('-t,--task: input the name of the task.')
    print('')
    print('example:')
    print('python init.py --init whoiswho')
    print('python init.py -t whoiswho')

def main(argv):
    if len(argv) <= 1:
        Usage()
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv[1:], 'ht:',
                ['help', 'task=', 'init='])
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
        elif o in ('--init'):
            initDir(a)
            sys.exit(0)
        elif o in ('-t', '--task'):
            processTask(a)
            sys.exit(0)

def initDir(task):
    furocdir = rospkg.RosPack().get_path('pocketsphinx') + '/furoc/'
    taskdir = furocdir + task + '/'
    os.system('mkdir ' + taskdir)
    os.system('touch ' + taskdir + 'sent.txt')
    os.system('touch ' + taskdir + 'gram.jsgf')

    fp = open(taskdir + task + '.launch', 'w')
    fp.write('<launch>\n')
    fp.write('\n')
    fp.write('  <node name="recognizer" pkg="pocketsphinx" type="recognizer.py" output="screen">\n')
    fp.write('    <param name="fsg" value="$(find pocketsphinx)/furoc/'
            + task + '/finite_state.fsg"/>\n')
    fp.write('    <param name="dict" value="$(find pocketsphinx)/furoc/'
            + task + '/words.dic"/>\n')
    fp.write('  </node>\n')
    fp.write('\n')
    fp.write('</launch>\n')
    fp.close()

def processTask(task):
    furocdir = rospkg.RosPack().get_path('pocketsphinx') + '/furoc/'
    taskdir = furocdir + task + '/'
    os.system('python ' + furocdir + 'input2dict.py -t ' + task)
    os.system('python ' + furocdir + 'jsgf2fsg.py -t ' + task)
    

if __name__ == '__main__':
    main(sys.argv)
