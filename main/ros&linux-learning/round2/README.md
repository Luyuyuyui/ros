<h1 align="center">第二轮考核</h1>
<p align="center">
  
  <img src="https://img.shields.io/github/languages/count/nanchengcyu/TechMindWave-backend" alt="languages"/>
  <img src="https://img.shields.io/badge/Linux ROS-green" alt="Python"/>
  <img src="https://img.shields.io/badge/Author-wangluyu-orange" alt="Author" />
</p>
<hr>


##  项目列表

###  Linux 部分  🐧 
Linux/
├── Task1/          # Ubuntu系统基础操作任务
│   └── (系统安装、配置、基础命令使用截图)
└── Task2/
    └── neofetch.png    # 系统信息展示截图

### ROS 部分   🤖
ROS/
├── ROS1_Learning_Notes.md    # ROS1 系统学习笔记
├── ROS2_Learning_Notes.md    # ROS2 系统学习笔记
├── Task1/                    # 小乌龟
│   └── (ROS1和ROS2小乌龟运行截图)
├── Task2/                    # 双乌龟圆周运动
│   ├── 圆周运动演示视频.mp4
│   ├── 终端命令截图.png
│   └── 功能包源代码/
├── Task3/                    # 传输通信
│   ├── 功能包文件/
│   └── 运行终端截图.png
├── Task4/                    # 乌龟跟随运动
│   ├── 跟随运动演示视频.mp4
│   ├── 终端命令截图.png
│   └── 功能包文件/
└── Task5/                    # 文件上传进度反馈
    ├── 功能包文件/
    └── 运行终端截图.png
（没有插入图片，本人不太会操作，敬请谅解）

### 学习心得💡

  一开始拿到二轮考核的任务，还是很兴奋的（之前自己探索过一点点ROS1，但仅仅是玩过小乌龟），觉得还挺有意思的，于是很快就完成了任务一；后来开始跟着wiki学习，才发现与我想象中的不太一样；“节点”、“话题”、“服务”、“动作”……每个词都认识，但连在一起就不知道在说什么（）还有CMakeLists.txt和package.xml里面一堆看不懂的代码。。。。。。搜了很多解说，还是不太明白，我还是顺着教程一步一步操作，在实现的过程中，逐渐理解了文件中每一段代码的意思。之后安装了ROS2，发现貌似不能两个环境同时存在，搜索后，通过注释掉其中一个环境解决。任务三当中必须要CMakeLists.txt中添加message_generation依赖，而且还要在package.xml中配置运行依赖，当时缺少了这一步，卡了很久，最后才发现这个问题。任务五是最具挑战的，先去网上搜了Action通信指南，学习了相关知识，要实现动作客户端连续发送状态和当前进度，要添加依赖项，实现回调函数，代码参考了网上的方法。但是最后实现服务器和客户端的时候一直出错，两个终端启动服务器和客户端每次显示No executable found，问了Deepseek换了好多种方法也没有解决，最后直接输入这两个命令才成功运行
`python3 install/test_action/lib/test_action/file_upload_server.py`

`python3 ~/ros2_ws/install/test_action/lib/test_action/file_upload_client.py`

最后，每次编译结束后，都要在终端中输入`source devel/setup.bash`，才能运行成功（每次都忘）
通过这些任务的学习，我对python有了更深入的理解，也对ROS有了更基本的认识。这么多的错误，一次次的修正，我也逐渐理解了代码的运行原理，也对ROS的消息机制有了更深入的理解，未来我会继续学习ROS，探索更多的功能和应用。




> 最后感谢这次考核让我系统性地学习了Linux和ROS，为后续的机器人开发打下了坚实的基础！（虽然现在只是基础，但是我会继续努力，学习更多的知识的）🌟
