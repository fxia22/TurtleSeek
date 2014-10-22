Pocketsphinx wrapper for ROS<br />
<br />
以下内容由 bssthu 编辑<br />
下文中 ${task} 代表任务名，如 whoiswho<br />

使用方法
=====
<br />
<br />

首先使用以下命令初始化一个任务的文件夹：<br />

    init.py --init ${task}

此时应该创建了目录 ${pocketsphinx}/furoc/{task}，
其中有文件 sent.txt, gram.jsgf<br />
<br />
编辑 sent.txt 和 gram.jsgf ，方法参见 furoc/README 。<br />
<br />
然后就可以通过以下命令生成 .dict, .fsg, .launch 文件：<br />

    init.py -t ${task}

此时可以用以下命令启用此任务的语音识别节点：<br />

    roslaunch pocketsphinx ${task}.launch


