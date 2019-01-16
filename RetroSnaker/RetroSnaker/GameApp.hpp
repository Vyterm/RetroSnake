#pragma once

// 贪吃蛇大作战，功能描述
// 1) 根据坐标点绘制圆形的蛇头与蛇身，通过蛇头与其他所有点进行碰撞检测实现交互功能，包括检查是否死亡，是否吃到了食物增加蛇身
// 2) 每个贪吃蛇都有一个匀速移动，作用于蛇头，蛇身将沿着前面的点进行移动，提供旋转方向的功能，同时提供一个加速的功能，按下加速键时将以两倍速度移动
//

// 控制台版贪吃蛇，功能描述
// 1) 在有限的窗口内绘制边框以及一些简单的障碍物，在右边绘制积分，支持WASD及上下左右二人同玩，单人模式不绘制积分
// 2) 当蛇头移动至可吞食的点处将不造成尾部的位移(相当于增长)，当蛇头碰到不可吞食的点(尾部，边框，障碍物)时将结束游戏
// 3) 使用'@'标示蛇头，使用'a'标示可吞食的点，使用'#'标示边框及障碍物，使用'O'标示蛇尾

// Golly版贪吃蛇， 功能描述
// 1) 在Golly的框架下创建一种可以被控制移动的点(淡蓝色尖角蛇头)，在遇到普通的点(白色方块)时会在尾部形成点(深蓝色圆形蛇尾)
// 2) 普通的点移动至蛇尾时将产生截断，视截断的长度给予其他部位一定回合的刚体效果(体现为暖色)
// 3) 刚体效果时吞食冷色点将降低刚体持续时间增加长度，吞食暖色点将提高持续时间减少长度，刚体期间将摧毁普通点而不产生副作用
// 在Golly的世界中享受吞食的快感吧
