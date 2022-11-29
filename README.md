# BlackStar_ustc_robogame_2022
A final version of Team_BlackStar for ustc_robogame_2022 competition.

-------

## 队伍信息

竞技组主题: 冰壶机器人;

参赛队名称：黑星队;

参赛队队徽(@long-knight绘制)：
<div align="center"> <img src="https://github.com/Deagle-PrintStream/BlackStar_ustc_robogame_2022/blob/main/reference/pictures/blackstar.jpg" width = 300 height = 300 /> </div>

队标拟人形象(novelAI生成)：
<div align="center"> <img src="https://github.com/Deagle-PrintStream/BlackStar_ustc_robogame_2022/blob/main/reference/pictures/blackstar_novelAI_waifu2x_1.png" width = 300 height = 300 /> </div>

队伍编号：17;

小组队员：
* @Swayee2022(https://github.com/Deagle-PrintStream)
* @stubborncake(https://github.com/stubborncake)
* @long-knight(https://github.com/long-knight)

## 审核成绩排名

|  流程 | 审核日期  | 名次  | 参与队数 |
| :--:  | :---  | :--:  | :--:  |
| 计划书 | 2022.5.15 |  43 |  46  |
| 一审   | 2022.7.28 |  24 |  44  |
| 二审 | 2022.9.16 |  21 |  42  |
| 三审 | 2022.10.5 |  9 |  30  |
| 预赛 | 2022.11.26 |  9 |  28  |
| 决赛 | 2022.11.26 |  - |  8  |

*note1: 比赛过程中持续有退赛的队伍，队伍数量逐渐边少*

*note2: 由于过程大部分队伍进度较慢，最终决赛时间延期一周*

*note3: 由于疫情影响，原定的预赛日期延期，同时场地改为中区创新基地，无观众，仅直播展示*

*note4: 16强进8强的比赛为积分赛，因一分之差惜败于第九名的成绩，无缘决赛*

## 规则简介

1. 自制机器人，使得完成巡线，识别冰壶，取冰壶，放置冰壶，推冰壶的操作。
2. 巡线部分大多为直角弯，从起点到取壶区的路程比较复杂，为折线和弧线的组合，可以选择不巡线；
3. 需要通过颜色或者冰壶上的二维码来识别正确的冰壶，默认红色为我方冰壶，黄色为敌方；抬起错误的壶则之后的得分无效，只能从起点重跑；
4. 冰壶上有握把，可以提起来，也可以整个抬起；
5. 冰壶要么抬起来，要么放置在发射区内；若壶在移动过程中脱落，则之后得分无效，同上；
6. 四个冰壶推出后，根据最终的位置计算得分，在大本营内的冰壶根据环数，可以获得1，2，3分；
7. 在大本营的正中心，和发射区中间各有一个障碍壶；大本营后方的围栏是不能碰撞的，一旦发送的冰壶撞到底线，则直接移除该冰壶。

场地简图：

![robogame map](https://github.com/Deagle-PrintStream/BlackStar_ustc_robogame_2022/blob/main/reference/pictures/robogame%20map.jpg)

