## 简介

同济大学2023年《面对对象程序设计》作业。

用C++编写控制台程序，模拟完成一个单人益智小游戏“五子连珠”。

## 开发环境

- macOS Monterey 12.5.1
- cmake 3.22.2
- SFML 2.5.1
- C++11 及以上
- 开发工具：Visual Studio Code 1.78.2

图形库：SFML (https://www.sfml-dev.org/)

SFML 是一个简单、快速、跨平台和面向对象的多媒体 API。它提供对窗口、图形、音频和网络的访问。它是用 C++ 编写的，并且绑定了各种语言，例如 C、.Net、Ruby、Python。

## 运行

```shell
git clone git@github.com:Lei00764/oop-project.git

mkdir build  # 创建 build 文件夹
cd build     # 进入 build 文件夹
cmake ..     # 使用上一层的 CMakeLists.txt 来生成 Makefile
make         # 使用 Makefile 生成可执行文件
./main       # 运行可执行文件
```

## 操作

### 鼠标操作

鼠标点击棋盘，依次选择起点和终点位置。选择完成后，棋子自动开始移动。

### 键盘操作

按下键盘 “Q” 键，游戏结束；按下键盘 “R” 键，游戏重新开始。

## 界面

![截屏2023-05-21 15.52.24](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2023-05-21%2015.52.24.png)

![截屏2023-05-21 15.53.00](https://lei-1306809548.cos.ap-shanghai.myqcloud.com/Obsidian/%E6%88%AA%E5%B1%8F2023-05-21%2015.53.00.png)
