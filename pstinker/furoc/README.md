下文中${task}代表任务名，如 whoiswho<br />

修改方法
=====
<br />
<br />
dict
-----
编辑${task}/sent.txt，写入可能出现的词语。<br />
示例如下：<br />

    hello tinker
    my name is
    alex
    bob

使用以下命令编译：

    python input2dict.py -t ${task}

<br />
<br />
fsg
-----
编辑gram.jsgf，写入语法。<br />
示例如下：

    #JSGF V1.0;
    grammar furoc;
    public <furocCmd> = <myname> | <hellotinker>;
    <myname> = MY NAME IS <names>;
    <names> = TOM | BOB | JACK;
    <hellotinker> = HELLO <names>;

使用以下命令编译：

    python jsgf2fsg.py -t ${task}

<br />
<br />
注意：编写 gram.jsgf 时，单词请用大写。<br />
最好不要出现 's，一定要出现的话加空格，如 IT 'S <br />

