# Introduction

[中文版介绍 Chinese Version](#中文版介绍)

A free game engine contain a easy-modified editor with soft render.

Now is still in the development.

# Setup Project

This section will help you setup and build Xenon Engine.

At first, the project is using Cmake as build tool, so you need to download and install Cmake.

https://cmake.org/download/

Now the engine is only support Windows, so I will intro how to use Cmake GUI to build the solution.

Open the git bash in the folder you want to setup the project.

Input the following code:

Clone the project:

1. ```git clone https://github.com/kwh3884858/XenonEngine.git``` 

Into the project folder:

2. ```cd XenonEngine```

Initialize submodule libraries.

3. ```git submodule update --init --recursive```

Generate the solution.

Please make sure all submodule pulled in the folder Library

4. Open Cmake GUI. Set the source code folder as the root of project, set the birary folder as any folder you like, here I set to Build folder. Cmake should create folder if it is not exist. Click Generate.

5. Open Build/XenonEngine.sln, Set XenonEngine as StartUp Project.

Build it! And you will get a error, don`tworry, it means everything is work well.

The reason the project cannot run is it lack of the engine configuration file, you will find a example file in the folder Data. You could simplely copy or in the git bash execute:

6. ```cp Data/EngineConfig.yaml Build/Debug/EngineConfig.yaml```

And you need to open the configuration file to modify the ProjectPath as the root of game project folder, now I set the folder as the project binary folder, Build.You can set as any folder you want, and in the folder Xenon Engine will create a folder named Data to storage the assets and other data.

7. Rerun the project again.

Congratulation! Now Xenon Engine is running on your computer.

## Engine Principles
- Manually control create and destruction of components in an object, don't rely on constructors and destructors, which only take responsibility for object itself.

## New Features
- A game editor and content browser based on ImGUI.
- Three type rendering shader.

## TODO
- Need to support load image.
- Need to support rendering object with material.

## Folder content
- Algorithm, algorithm and data struction
- Mathlab, self-defined data struction for game
- MacOS, MacOS specific render
- Windows, Windows specific render


# 中文版介绍

免费游戏引擎包含带有软渲染的易于修改的编辑器。

现在还在开发中。

# 设置项目

本节将帮助您设置和构建 Xenon 引擎。

首先，项目使用Cmake作为构建工具，所以需要下载安装Cmake。

https://cmake.org/download/

现在引擎只支持Windows，所以我将介绍如何使用Cmake GUI来构建解决方案。

在要存放项目的文件夹中打开 git bash。

输入以下代码：

克隆项目：

1. ```git clone https://github.com/kwh3884858/XenonEngine.git```

进入项目文件夹：

2. ```cd XenonEngine```

初始化子模块库。

3. ```git submodule update --init --recursive```

生成解决方案。

请确保所有子模块都拉入文件夹库中

4. 打开 Cmake GUI。将source code文件夹设置为项目的根目录，将birary文件夹设置为您喜欢的任何文件夹，这里我设置为Build文件夹。如果文件夹不存在，Cmake 会创建文件夹。点击Generate。

5. 打开 Build/XenonEngine.sln，将 XenonEngine 设置为启动项目。

Build it！你会得到一个错误，别担心，这意味着一切正常。

项目无法运行的原因是缺少引擎配置文件，你会在Data文件夹中找到一个示例文件。您可以简单地复制或在 git bash 中执行：

6. ```cp Data/EngineConfig.yaml Build/Debug/EngineConfig.yaml```

并且需要打开配置文件修改ProjectPath为游戏项目根目录，现在我将文件夹设置为项目二进制文件夹，Build.你可以设置为任何你想要的文件夹，Xenon Engine将在你定义的文件夹下创建一个名为 Data 的文件夹，用于存储资产和其他数据。

7. 重新运行项目。

恭喜！现在 Xenon Engine 正在您的计算机上运行。

## 引擎原则
- 手动控制`对象`中`组件`的创建和销毁，不依赖只对`对象`负责的构造函数和析构函数

## 新功能
- 基于 ImGUI 的游戏编辑器和内容浏览器。
- 三个渲染着色器。

## 代办
- 需要支持加载图片。
- 需要支持材质渲染对象。

## 文件夹介绍
- Algorithms、算法和数据结构
- Mathlab，自定义游戏数据结构
- MacOS，MacOS 特定渲染
- Windows，特定于 Windows 的渲染