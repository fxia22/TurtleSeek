#!/usr/bin/env python
# -*- coding: utf-8 -*-
# file: input2dict.py
# created by bss at 2014-04-30
# Last modified: 2014-05-09, 17:46:48
# 把输入转化为字典
#

import sys
import rospkg
import getopt

def Usage():
    print('input2dict.py usage:')
    print('把输入的句子、单词转化为发音字典')
    print('-h,--help: print help message.')
    print('-t,--task: input the name of the task.')
    print('')
    print('example:')
    print('python input2dict.py -t whoiswho')

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
    fp = open(furocdir + 'cmudict_SPHINX_40', 'r')
    cmudict = {}
    for line in fp.readlines():
        col = line.strip().upper().split('\t', 1)
        cmudict[col[0]] = col[1]
    fp.close()

    fp = open(taskdir + 'sent.txt', 'r')
    words = []
    for line in fp.readlines():
        col = line.strip().upper().split()
        for word in col:
            words.append(word)
    fp.close()

    words = list(set(words))

    fp = open(taskdir + 'words.dic', 'w')
    for word in words:
        isSucceed = False
        try:
            line = word + '\t' + cmudict[word] + '\n'
            fp.write(line)
            isSucceed = True
            for i in range(2, 5):
                key = word + '(' + str(i) + ')'
                line = key + '\t' + cmudict[key] + '\n'
                fp.write(line)
        except:
            pass
        if not isSucceed:
            print("I can't find " + word)
    fp.close()

if __name__ == '__main__':
    main(sys.argv)
