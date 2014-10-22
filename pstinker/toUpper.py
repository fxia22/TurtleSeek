#!/usr/bin/env python
# -*- coding: utf-8 -*-
# file: toUpper.py
# created by bss at 2014-05-09
# Last modified: 2014-05-09, 17:21:57
# 初始化一组语音识别的任务
#

import sys
import os
import getopt

def Usage():
    print('toUpper.py usage:')
    print('-h,--help: print help message.')
    print('-i: input the text file.')
    print('')
    print('example:')
    print('python toUpper.py -i furoc/answer/gram.jsgf')

def main(argv):
    if len(argv) <= 1:
        Usage()
        sys.exit(2)
    try:
        opts, args = getopt.getopt(argv[1:], 'hi:', ['help'])
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
        elif o in ('-i'):
            processTask(a)
            sys.exit(0)
    Usage()

def processTask(filepath):
    text = ''
    fp = open(filepath, 'r')
    for line in fp.readlines():
        text += line.upper()
    fp.close()
    fp = open(filepath, 'w')
    fp.write(text)
    fp.close()

if __name__ == '__main__':
    main(sys.argv)
