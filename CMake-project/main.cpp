#include <graphics.h>// 引用图形库头文件
#include <cstdio>//输入输出
#include <conio.h>//输入检测
#include <ctime>//时间
#include "data.h"//数据


#define rectanglesideLength 20//单个方块的边长像素
#define head_GameArea 23//连同边框整个区的高度
#define wide_GameArea 12//连同边框整个区的宽度
#define boundaryWidth 1//边框的厚度

int gameZone[head_GameArea][wide_GameArea] = {0};//游戏数据

typedef struct tetris {
    int graphicaltype;//当前方块
    int graphicaltype_color;//当前方块颜色
    int next_graphicaltype;//下个方块
    int next_graphicaltype_color;//下个方块颜色
    int x;//当前方块的右上角坐标
    int y;//当前方块的右上角坐标
    int fraction;//分数
    bool winOrLose;//输赢
    bool suspend;//暂停
} tetris;

tetris data;

void init(int condition);//游戏数据初始化
void reset();//重置方块位置
void gameLogic();//游戏逻辑的实现
void cubeCraze();//旋转方块
void directFall();//使方块直接触底
void printframe();//绘制游戏边框
void fixedBlock();//固定无法移动的方块
void drawGameArea();//绘制游戏区
void clearGameArea();//清楚需要更新绘制的区
void drawotherAreas();//显示非游戏区提示信息
int moveBlocks(int key);//使方块移动
void end_DetectionGame();//检测游戏是否结束
void takeBlocksAtRandom();//随机选择新的方块和颜色
void detectionElimination();//检测是否需要消行并进行计分
void keyboardinput(int key);//根据按键执行对应操作
bool judgeTheCoincidenceOfBlocks(int key);//判断方块是否重合
void drawASquare(int x, int y, COLORREF color, int type);//画一个边长为rectanglesideLength像素的正方形方块
void printgraphical(int graphical[4][4], float x, float y, COLORREF colour);//绘制一个俄罗斯方块的图形
void button(int x, int y, int wide, int high, char *text, COLORREF color, int type);//绘制一个按钮

//画俄罗斯方块的图形
void printgraphical(int graphical[4][4], float x = 0, float y = 0, COLORREF colour = RED) {
    setfillcolor(colour);
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (graphical[i][j] == 1) {
                fillrectangle((j + x) * rectanglesideLength,
                              (i + y) * rectanglesideLength,
                              (j + x + 1) * rectanglesideLength,
                              (i + y + 1) * rectanglesideLength);
            }
        }
    }
}

//初始化
void init(int condition = 0) {
    //创建一个窗口
    //initgraph((wide_GameArea + 8) * rectanglesideLength, (head_GameArea + 1) * rectanglesideLength);
    if (condition == 0) {
        initgraph(640, 480);
    }
    //初始化游戏区数组
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < wide_GameArea; ++j) {
            gameZone[i][j] = 0;
        }
    }
    //建立边框
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            gameZone[i][j] = 1;//左边框
            gameZone[i][wide_GameArea - boundaryWidth + j] = 1;//右边框
        }
    }

    for (int i = 0; i < boundaryWidth; ++i) {
        for (int j = 0; j < wide_GameArea; ++j) {
            gameZone[head_GameArea - i - 1][j] = 1;//底部边框
        }
    }

    //随机取方块
    srand((unsigned) time(NULL));//种子
    data.graphicaltype = rand() % 28;
    data.next_graphicaltype = rand() % 28;
    //随机取颜色
    data.graphicaltype_color = rand() % 15 + 1;
    data.next_graphicaltype_color = rand() % 15 + 1;
    //初始化分数
    data.fraction = 0;
    //初始化开始位置
    data.y = -5;
    data.x = 4;
    //初始化输赢
    data.winOrLose = true;
    //初始化是否暂停
    data.suspend = false;
    //初始化窗口名
    HWND hWnd = GetHWnd();
    SetWindowText(hWnd, "俄罗斯方块         By:小肥仔");
}

//画边框
void printframe() {
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            drawASquare(j, i, WHITE, 2);//左
            drawASquare(wide_GameArea - boundaryWidth + j, i, WHITE, 2);//右
        }
    }
    for (int i = 0; i < wide_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            drawASquare(i, head_GameArea - boundaryWidth + j, WHITE, 2);//底部
        }
    }

}

//画矩形 type 1 填充矩形 type 2 无填充矩形
void drawASquare(int x, int y, COLORREF color, int type = 1) {
    setfillcolor(color);
    if (type == 1) {
        fillrectangle(x * rectanglesideLength, y * rectanglesideLength, x * rectanglesideLength + rectanglesideLength,
                      y * rectanglesideLength + rectanglesideLength);
    } else if (type == 2) {
        rectangle(x * rectanglesideLength, y * rectanglesideLength, x * rectanglesideLength + rectanglesideLength,
                  y * rectanglesideLength + rectanglesideLength);
    }

}

//清楚需要更新的区
void clearGameArea() {
    //游戏方块区
    clearrectangle(boundaryWidth * rectanglesideLength + 1, 0,
                   (wide_GameArea - boundaryWidth) * rectanglesideLength - 1,
                   (head_GameArea - boundaryWidth) * rectanglesideLength - 1);
    //其他区
    clearrectangle(wide_GameArea * rectanglesideLength + 1, 0, (wide_GameArea + 8) * rectanglesideLength,
                   (head_GameArea - 4) * rectanglesideLength);

}

//打印游戏区
void drawGameArea() {
    for (int i = 0; i < head_GameArea - boundaryWidth; ++i) {
        for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {
            if (gameZone[i][j] == 1) {
                drawASquare(j, i, LIGHTGRAY);
            }
        }
    }
}

//使方块移动
int moveBlocks(int key) {
    if (judgeTheCoincidenceOfBlocks(key)) {
        if (key == 0) {
            data.y++;
            return 1;
        } else if (key == 1) {
            data.x--;
            return 1;
        } else if (key == 2) {
            data.x++;
            return 1;
        }
    } else {
        return 0;
    }
}

//旋转方块
void cubeCraze() {
    int nextGraphIndex =
            data.graphicaltype % 4 == 3 ? data.graphicaltype / 4 * 4 : data.graphicaltype + 1;//获取旋转后的方块index
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {//用旋转后的方块 与原来的位置进行比较若重合(gameZone[j + data.y][i + data.x] == 1)则不旋转
            if (graphicals[nextGraphIndex][j][i] == 1 && (j + data.y) >= 0 && (i + data.x) >= 0) {
                if (gameZone[j + data.y][i + data.x] == 1) {
                    return;
                }
            }
        }
    }
    data.graphicaltype = nextGraphIndex;//旋转成功 旋转后的方块index替换原来的方块index
}

//使方块直接下落
void directFall() {
    while (moveBlocks(0)) {
    }
}

//根据按键执行对应操作
void keyboardinput(int key) {
    printf("%d ", key);
    switch (key) {
        case 13:
            data.suspend == false ? data.suspend = true : data.suspend = false;
            break;
        case 'w':
        case 'W':
        case 72:
            cubeCraze();
            break;
        case 's':
        case 'S':
        case 80:
            moveBlocks(0);
            break;
        case 'a':
        case 'A':
        case 75:
            moveBlocks(1);
            break;
        case 'd':
        case 'D':
        case 77:
            moveBlocks(2);
            break;
        case 32:
            directFall();
            break;
    }
}

//判断方块是否重合
bool judgeTheCoincidenceOfBlocks(int key) {
    if (key == 0) {//判断是否可以向下移动
        for (int i = 0; i < 4; ++i) {
            for (int j = 3; j >= 0; --j) {
                if (graphicals[data.graphicaltype][j][i] == 1) {
                    if (gameZone[j + data.y + 1][i + data.x] == 1) {
                        fixedBlock();//固定方块
                        takeBlocksAtRandom();//选择新的方块
                        end_DetectionGame();//检测输赢
                        reset();//重置新的方块位置
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    } else if (key == 1) {//判断是否可以向左移动
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (graphicals[data.graphicaltype][i][j] == 1) {
                    if (gameZone[i + data.y][j + data.x - 1] == 1) {
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    } else if (key == 2) {//判断是否可以向右移动
        for (int i = 0; i < 4; ++i) {
            for (int j = 3; j >= 0; --j) {
                if (graphicals[data.graphicaltype][i][j] == 1) {
                    if (gameZone[i + data.y][j + data.x + 1] == 1) {
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    }
    return true;
}

//固定方块
void fixedBlock() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (graphicals[data.graphicaltype][i][j] == 1 && (i + data.y) >= 0 && (j + data.x) >= 0) {
                gameZone[i + data.y][j + data.x] = 1;
            }
        }
    }
}


//新的方块 位置重置
void reset() {
    data.x = 4;
    data.y = -5;
}

//检测是否需要消行
void detectionElimination() {
    int eliminateRows = 0;
    for (int i = 0; i < head_GameArea - boundaryWidth; ++i) {
        int number = 0;//记录一行有多少个方块
        for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {//统计一行方块的个数
            if (gameZone[i][j] == 1) {
                number++;
            }
        }
        if (number == wide_GameArea - 2 * boundaryWidth) {//比较一行方块的个数是否满了
            eliminateRows++;
            if (i == 0) {
                for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {//若是第一行满了则将第一行清楚
                    gameZone[0][j] = 0;
                }
            } else {
                for (int j = i; j > 0; --j) {//清楚其他满了的行
                    for (int k = boundaryWidth; k < wide_GameArea - boundaryWidth; ++k) {
                        gameZone[j][k] = gameZone[j - 1][k];
                    }
                }
            }
        }
    }
    data.fraction = eliminateRows * 10 + (eliminateRows == 1 ? 0 : eliminateRows * 5) + data.fraction;
}

//非游戏区提示信息
void drawotherAreas() {
    //下个方块
    outtextxy((wide_GameArea + 3) * rectanglesideLength, 1 * rectanglesideLength, "下个方块");
    printgraphical(graphicals[data.next_graphicaltype], wide_GameArea + 2.5, 2, colours[data.next_graphicaltype_color]);
    //分数显示
    outtextxy((wide_GameArea + 3) * rectanglesideLength, 12 * rectanglesideLength, "分数");
    char fraction[10];
    itoa(data.fraction, fraction, 10);
    outtextxy((wide_GameArea + 3) * rectanglesideLength + 5, 13 * rectanglesideLength, fraction);
    //作者
    outtextxy((wide_GameArea + 12) * rectanglesideLength, 12 * rectanglesideLength, "By: 小肥仔");
    //提示信息
    outtextxy((wide_GameArea + 12) * rectanglesideLength, 1 * rectanglesideLength, "操作提示");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 2 * rectanglesideLength, "向左移动:← A");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 3 * rectanglesideLength, "向右移动:→ D");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 4 * rectanglesideLength, "向下移动:↓ S");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 5 * rectanglesideLength, "旋转方块:↑ W");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 6 * rectanglesideLength, "方块下落: 空格");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 7 * rectanglesideLength, "暂停游戏: 回车");
}

//随机选择新的方块
void takeBlocksAtRandom() {
    int randomNumber1 = rand() % 28;
    int randomNumber2 = rand() % 15 + 1;
    data.graphicaltype = data.next_graphicaltype;
    data.graphicaltype_color = data.next_graphicaltype_color;
    data.next_graphicaltype = randomNumber1;
    data.next_graphicaltype_color = randomNumber2;
}

//判断游戏是否结束
void end_DetectionGame() {
    for (int i = boundaryWidth; i < wide_GameArea - boundaryWidth; ++i) {
        if (gameZone[1][i] == 1) {//检测第二行有没有方块 有就游戏结束
            data.winOrLose = false;
        }
    }
}

//按钮
void button(int x, int y, int wide, int high, char *text, COLORREF color, int type = 0) {
    //设置当前设备图案填充和文字输出时的背景模式 TRANSPARENT 设置是透明的
    setbkmode(TRANSPARENT);
    //设置当前设备填充颜色
    setfillcolor(color);
    //画有边框的填充圆角矩形 ellipsewidth 构成圆角矩形的圆角的椭圆的宽度  ellipseheight 构成圆角矩形的圆角的椭圆的高度。
    fillroundrect(x, y, x + wide, y + high, 20, 20);
    LOGFONT font;
    char s1[] = "黑体";
    gettextstyle(&font);
    font.lfQuality = ANTIALIASED_QUALITY;//抗锯齿
    //设置当前文字样式 nHeight 指定高度 nWidth 字符的平均宽度 lpszFace 字体名称
    if (type == 0) {
        settextstyle(30, 0, s1);
    }
    //textwidth() 获取字符串实际占用的像素宽度
    int tx = x + (wide - textwidth(text)) / 2;
    int ty = y + (high - textheight(text)) / 2;
    //在指定位置输出字符串
    outtextxy(tx, ty, text);
}

//游戏逻辑实现
void gameLogic() {
    clock_t clockLast, clockNow;//时间
    char s1[] = "黑体";//字体
    settextstyle(0, 0, s1);//重置字体大小
    printframe();//打印边框
    int x1 = 290, y1 = 420, x2 = 520, y2 = 420;
    int wide = 80, high = 30;
    button(x1, y1, wide, high, "重新开始", LIGHTRED, 1);
    button(x2, y2, wide, high, "退出游戏", LIGHTRED, 1);
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
                case WM_LBUTTONDOWN:
                    if (msg.x >= x1 && msg.x <= x1 + wide && msg.y >= y1 && msg.y <= y1 + high) {
                        init(1);//游戏重新开始
                        break;
                    }
                    if (msg.x >= x2 && msg.x <= x2 + wide && msg.y >= y1 && msg.y <= y2 + high) {
                        exit(0);
                        break;
                    }
                    break;
                default:
                    break;
            }
        } else if (data.winOrLose) {
            while (_kbhit()) { // 有键按下
                if (data.suspend == false) {//判断是否暂停
                    keyboardinput(_getch());
                } else {//游戏暂停了 所有按键功能只有回车有效
                    char key = _getch();
                    if (key == 13) {
                        keyboardinput(13);
                    }
                }
            }
            clockNow = clock(); // 计时
            // 两次记时的间隔超过0.45秒
            if (clockNow - clockLast > 0.45F * CLOCKS_PER_SEC) {
                clockLast = clockNow;
                // 方块往下移
                if (data.suspend == false) {
                    keyboardinput(80);
                }
            }
            clearGameArea();//清楚需要更新区域
            detectionElimination();//检测消除
            drawGameArea();//画出游戏区
            drawotherAreas();//画出其他区
            if (data.winOrLose) {//游戏结束了就不画现在的方块位置
                printgraphical(graphicals[data.graphicaltype], data.x, data.y, colours[data.graphicaltype_color]);
            }
            FlushBatchDraw();//刷新防止闪烁
            Sleep(20);//等待20毫秒
            if (data.winOrLose == false) {//当游戏结束打印提示框
                MessageBox(GetHWnd(), "游戏结束", "GAME OVER", MB_OK);//打印提示框
            }
        }
    }
}

int main() {
    init(0);//初始化
    cleardevice();//清空绘图设备
    loadimage(NULL, "E:\\Programming\\C\\Tetris\\image.jpg");//从图片文件获取图像
//    loadimage(NULL, "路径");//从图片文件获取图像 路径的 \ 要用 \\
    //按钮
    int x1 = 90, y1 = 299, x2 = 370, y2 = 299;//按钮的坐标
    int high = 50, wide = 170;//按钮的长宽
    button(x1, y1, wide, high, "开始游戏", LIGHTBLUE, 0);
    button(x2, y2, wide, high, "退出游戏", LIGHTRED, 0);
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
                case WM_LBUTTONDOWN:
                    if (msg.x >= x1 && msg.x <= x1 + wide && msg.y >= y1 && msg.y <= y1 + high) {
                        cleardevice();//清空绘图设备
                        gameLogic();//游戏逻辑
                        return 0;
                    }
                    if (msg.x >= x2 && msg.x <= x2 + wide && msg.y >= y1 && msg.y <= y2 + high) {
                        return 0;
                    }
                    break;
                default:
                    break;
            }
        }

    }
}