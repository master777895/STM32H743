# STM32H743 伞兵三号飞控
学生维护不易，如果觉得有帮助还请点星星支持一下，感谢。



## **项目介绍: **

**基于STM32H7芯片，CUBEMX平台，CLION编辑器，内嵌FreeRTOS系统的飞控项目。**



## **飞控方向定义：**

+ ### PPM通道：

  + 通道一是ROLL；

    在内部程序处理后，向左拨动是负，向右是正。(绝对值<=35)

  + 通道二是PITCH；

    在内部程序处理后，向下拨动是负，向上是正。(绝对值<=35)

  + 通道三是THROTTLE；向左拨动是负，向右是正。

    在内部程序处理后，向上是正。(0—950)

  + 通道四是YAW；向左拨动是负，向右是正。

    在内部程序处理后，向左拨动是负，向右是正。(绝对值<=200)

  + 通道五六七八就是有个高低位即可。

+ ### 飞机姿态：

  机头在前，在俯视的情况下。(该数据可以通过OLED屏幕观察到)

  + 左边低过右边：ROLL为负数。同理，从水平到这个动作过程中，ROLL_GYRO也是负数；
  + 左高右低：ROLL为正数；同理，从水平到这个动作过程中，ROLL_GYRO也是正数；
  + 前低头：PITCH为正数；同理，从水平到这个动作过程中，PITCH_GYRO也是正数；
  + 前抬头：PITCH为负数；同理，从水平到这个动作过程中，PITCH_GYRO也是负数；
  + 向左转：YAW减小；向右转：YAW增加；同理得YAW_GYRO的极性；
  
+ ### 控制器输出极性：

  在前面的定义下。在不接动力，不打杆时。

  + 在当前姿态数值是负数时，输出为正。其他同理。

## 更新日志：

**2021/8/7** : 

开发飞控。起步阶段。

分为四层：

+ APP_layer 			   ——用户上层	
+ Compute_layer	  ——飞行解算层
+ System_layer  		——系统层		
+ Hardware_layer 	——底层与芯片相关的驱动层



**2021/8/10**：

+ 首次更新底层PWM，ppm，I2C，RGB。
+ 上传原理图，引脚配置



**2021/8/27：**

+ 添加FreeRTOS系统，创建主流程。
+ 添加IMU读取以及相关滤波。
+ 添加姿态解算功能，采用的是Madgwick作者的梯度下降姿态融合算法。
+ 添加遥控器解析，解锁上锁识别功能。



**2021/9/2:**

+ 添加电调校准模式
+ 添加PID控制算法，采用两环串型PID控制。
+ 添加PWM总输出函数，至此，初步完成飞控主体



**代办事务**：

+ 飞机PID参数调试

  

