#include <graphics.h>// ����ͼ�ο�ͷ�ļ�
#include <cstdio>//�������
#include <conio.h>//������
#include <ctime>//ʱ��
#include "data.h"//����


#define rectanglesideLength 20//��������ı߳�����
#define head_GameArea 23//��ͬ�߿��������ĸ߶�
#define wide_GameArea 12//��ͬ�߿��������Ŀ��
#define boundaryWidth 1//�߿�ĺ��

int gameZone[head_GameArea][wide_GameArea] = {0};//��Ϸ����

typedef struct tetris {
    int graphicaltype;//��ǰ����
    int graphicaltype_color;//��ǰ������ɫ
    int next_graphicaltype;//�¸�����
    int next_graphicaltype_color;//�¸�������ɫ
    int x;//��ǰ��������Ͻ�����
    int y;//��ǰ��������Ͻ�����
    int fraction;//����
    bool winOrLose;//��Ӯ
    bool suspend;//��ͣ
} tetris;

tetris data;

void init(int condition);//��Ϸ���ݳ�ʼ��
void reset();//���÷���λ��
void gameLogic();//��Ϸ�߼���ʵ��
void cubeCraze();//��ת����
void directFall();//ʹ����ֱ�Ӵ���
void printframe();//������Ϸ�߿�
void fixedBlock();//�̶��޷��ƶ��ķ���
void drawGameArea();//������Ϸ��
void clearGameArea();//�����Ҫ���»��Ƶ���
void drawotherAreas();//��ʾ����Ϸ����ʾ��Ϣ
int moveBlocks(int key);//ʹ�����ƶ�
void end_DetectionGame();//�����Ϸ�Ƿ����
void takeBlocksAtRandom();//���ѡ���µķ������ɫ
void detectionElimination();//����Ƿ���Ҫ���в����мƷ�
void keyboardinput(int key);//���ݰ���ִ�ж�Ӧ����
bool judgeTheCoincidenceOfBlocks(int key);//�жϷ����Ƿ��غ�
void drawASquare(int x, int y, COLORREF color, int type);//��һ���߳�ΪrectanglesideLength���ص������η���
void printgraphical(int graphical[4][4], float x, float y, COLORREF colour);//����һ������˹�����ͼ��
void button(int x, int y, int wide, int high, char *text, COLORREF color, int type);//����һ����ť

//������˹�����ͼ��
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

//��ʼ��
void init(int condition = 0) {
    //����һ������
    //initgraph((wide_GameArea + 8) * rectanglesideLength, (head_GameArea + 1) * rectanglesideLength);
    if (condition == 0) {
        initgraph(640, 480);
    }
    //��ʼ����Ϸ������
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < wide_GameArea; ++j) {
            gameZone[i][j] = 0;
        }
    }
    //�����߿�
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            gameZone[i][j] = 1;//��߿�
            gameZone[i][wide_GameArea - boundaryWidth + j] = 1;//�ұ߿�
        }
    }

    for (int i = 0; i < boundaryWidth; ++i) {
        for (int j = 0; j < wide_GameArea; ++j) {
            gameZone[head_GameArea - i - 1][j] = 1;//�ײ��߿�
        }
    }

    //���ȡ����
    srand((unsigned) time(NULL));//����
    data.graphicaltype = rand() % 28;
    data.next_graphicaltype = rand() % 28;
    //���ȡ��ɫ
    data.graphicaltype_color = rand() % 15 + 1;
    data.next_graphicaltype_color = rand() % 15 + 1;
    //��ʼ������
    data.fraction = 0;
    //��ʼ����ʼλ��
    data.y = -5;
    data.x = 4;
    //��ʼ����Ӯ
    data.winOrLose = true;
    //��ʼ���Ƿ���ͣ
    data.suspend = false;
    //��ʼ��������
    HWND hWnd = GetHWnd();
    SetWindowText(hWnd, "����˹����         By:С����");
}

//���߿�
void printframe() {
    for (int i = 0; i < head_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            drawASquare(j, i, WHITE, 2);//��
            drawASquare(wide_GameArea - boundaryWidth + j, i, WHITE, 2);//��
        }
    }
    for (int i = 0; i < wide_GameArea; ++i) {
        for (int j = 0; j < boundaryWidth; ++j) {
            drawASquare(i, head_GameArea - boundaryWidth + j, WHITE, 2);//�ײ�
        }
    }

}

//������ type 1 ������ type 2 ��������
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

//�����Ҫ���µ���
void clearGameArea() {
    //��Ϸ������
    clearrectangle(boundaryWidth * rectanglesideLength + 1, 0,
                   (wide_GameArea - boundaryWidth) * rectanglesideLength - 1,
                   (head_GameArea - boundaryWidth) * rectanglesideLength - 1);
    //������
    clearrectangle(wide_GameArea * rectanglesideLength + 1, 0, (wide_GameArea + 8) * rectanglesideLength,
                   (head_GameArea - 4) * rectanglesideLength);

}

//��ӡ��Ϸ��
void drawGameArea() {
    for (int i = 0; i < head_GameArea - boundaryWidth; ++i) {
        for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {
            if (gameZone[i][j] == 1) {
                drawASquare(j, i, LIGHTGRAY);
            }
        }
    }
}

//ʹ�����ƶ�
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

//��ת����
void cubeCraze() {
    int nextGraphIndex =
            data.graphicaltype % 4 == 3 ? data.graphicaltype / 4 * 4 : data.graphicaltype + 1;//��ȡ��ת��ķ���index
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {//����ת��ķ��� ��ԭ����λ�ý��бȽ����غ�(gameZone[j + data.y][i + data.x] == 1)����ת
            if (graphicals[nextGraphIndex][j][i] == 1 && (j + data.y) >= 0 && (i + data.x) >= 0) {
                if (gameZone[j + data.y][i + data.x] == 1) {
                    return;
                }
            }
        }
    }
    data.graphicaltype = nextGraphIndex;//��ת�ɹ� ��ת��ķ���index�滻ԭ���ķ���index
}

//ʹ����ֱ������
void directFall() {
    while (moveBlocks(0)) {
    }
}

//���ݰ���ִ�ж�Ӧ����
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

//�жϷ����Ƿ��غ�
bool judgeTheCoincidenceOfBlocks(int key) {
    if (key == 0) {//�ж��Ƿ���������ƶ�
        for (int i = 0; i < 4; ++i) {
            for (int j = 3; j >= 0; --j) {
                if (graphicals[data.graphicaltype][j][i] == 1) {
                    if (gameZone[j + data.y + 1][i + data.x] == 1) {
                        fixedBlock();//�̶�����
                        takeBlocksAtRandom();//ѡ���µķ���
                        end_DetectionGame();//�����Ӯ
                        reset();//�����µķ���λ��
                        return false;
                    } else {
                        break;
                    }
                }
            }
        }
    } else if (key == 1) {//�ж��Ƿ���������ƶ�
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
    } else if (key == 2) {//�ж��Ƿ���������ƶ�
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

//�̶�����
void fixedBlock() {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (graphicals[data.graphicaltype][i][j] == 1 && (i + data.y) >= 0 && (j + data.x) >= 0) {
                gameZone[i + data.y][j + data.x] = 1;
            }
        }
    }
}


//�µķ��� λ������
void reset() {
    data.x = 4;
    data.y = -5;
}

//����Ƿ���Ҫ����
void detectionElimination() {
    int eliminateRows = 0;
    for (int i = 0; i < head_GameArea - boundaryWidth; ++i) {
        int number = 0;//��¼һ���ж��ٸ�����
        for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {//ͳ��һ�з���ĸ���
            if (gameZone[i][j] == 1) {
                number++;
            }
        }
        if (number == wide_GameArea - 2 * boundaryWidth) {//�Ƚ�һ�з���ĸ����Ƿ�����
            eliminateRows++;
            if (i == 0) {
                for (int j = boundaryWidth; j < wide_GameArea - boundaryWidth; ++j) {//���ǵ�һ�������򽫵�һ�����
                    gameZone[0][j] = 0;
                }
            } else {
                for (int j = i; j > 0; --j) {//����������˵���
                    for (int k = boundaryWidth; k < wide_GameArea - boundaryWidth; ++k) {
                        gameZone[j][k] = gameZone[j - 1][k];
                    }
                }
            }
        }
    }
    data.fraction = eliminateRows * 10 + (eliminateRows == 1 ? 0 : eliminateRows * 5) + data.fraction;
}

//����Ϸ����ʾ��Ϣ
void drawotherAreas() {
    //�¸�����
    outtextxy((wide_GameArea + 3) * rectanglesideLength, 1 * rectanglesideLength, "�¸�����");
    printgraphical(graphicals[data.next_graphicaltype], wide_GameArea + 2.5, 2, colours[data.next_graphicaltype_color]);
    //������ʾ
    outtextxy((wide_GameArea + 3) * rectanglesideLength, 12 * rectanglesideLength, "����");
    char fraction[10];
    itoa(data.fraction, fraction, 10);
    outtextxy((wide_GameArea + 3) * rectanglesideLength + 5, 13 * rectanglesideLength, fraction);
    //����
    outtextxy((wide_GameArea + 12) * rectanglesideLength, 12 * rectanglesideLength, "By: С����");
    //��ʾ��Ϣ
    outtextxy((wide_GameArea + 12) * rectanglesideLength, 1 * rectanglesideLength, "������ʾ");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 2 * rectanglesideLength, "�����ƶ�:�� A");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 3 * rectanglesideLength, "�����ƶ�:�� D");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 4 * rectanglesideLength, "�����ƶ�:�� S");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 5 * rectanglesideLength, "��ת����:�� W");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 6 * rectanglesideLength, "��������: �ո�");
    outtextxy((wide_GameArea + 11) * rectanglesideLength, 7 * rectanglesideLength, "��ͣ��Ϸ: �س�");
}

//���ѡ���µķ���
void takeBlocksAtRandom() {
    int randomNumber1 = rand() % 28;
    int randomNumber2 = rand() % 15 + 1;
    data.graphicaltype = data.next_graphicaltype;
    data.graphicaltype_color = data.next_graphicaltype_color;
    data.next_graphicaltype = randomNumber1;
    data.next_graphicaltype_color = randomNumber2;
}

//�ж���Ϸ�Ƿ����
void end_DetectionGame() {
    for (int i = boundaryWidth; i < wide_GameArea - boundaryWidth; ++i) {
        if (gameZone[1][i] == 1) {//���ڶ�����û�з��� �о���Ϸ����
            data.winOrLose = false;
        }
    }
}

//��ť
void button(int x, int y, int wide, int high, char *text, COLORREF color, int type = 0) {
    //���õ�ǰ�豸ͼ�������������ʱ�ı���ģʽ TRANSPARENT ������͸����
    setbkmode(TRANSPARENT);
    //���õ�ǰ�豸�����ɫ
    setfillcolor(color);
    //���б߿�����Բ�Ǿ��� ellipsewidth ����Բ�Ǿ��ε�Բ�ǵ���Բ�Ŀ��  ellipseheight ����Բ�Ǿ��ε�Բ�ǵ���Բ�ĸ߶ȡ�
    fillroundrect(x, y, x + wide, y + high, 20, 20);
    LOGFONT font;
    char s1[] = "����";
    gettextstyle(&font);
    font.lfQuality = ANTIALIASED_QUALITY;//�����
    //���õ�ǰ������ʽ nHeight ָ���߶� nWidth �ַ���ƽ����� lpszFace ��������
    if (type == 0) {
        settextstyle(30, 0, s1);
    }
    //textwidth() ��ȡ�ַ���ʵ��ռ�õ����ؿ��
    int tx = x + (wide - textwidth(text)) / 2;
    int ty = y + (high - textheight(text)) / 2;
    //��ָ��λ������ַ���
    outtextxy(tx, ty, text);
}

//��Ϸ�߼�ʵ��
void gameLogic() {
    clock_t clockLast, clockNow;//ʱ��
    char s1[] = "����";//����
    settextstyle(0, 0, s1);//���������С
    printframe();//��ӡ�߿�
    int x1 = 290, y1 = 420, x2 = 520, y2 = 420;
    int wide = 80, high = 30;
    button(x1, y1, wide, high, "���¿�ʼ", LIGHTRED, 1);
    button(x2, y2, wide, high, "�˳���Ϸ", LIGHTRED, 1);
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
                case WM_LBUTTONDOWN:
                    if (msg.x >= x1 && msg.x <= x1 + wide && msg.y >= y1 && msg.y <= y1 + high) {
                        init(1);//��Ϸ���¿�ʼ
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
            while (_kbhit()) { // �м�����
                if (data.suspend == false) {//�ж��Ƿ���ͣ
                    keyboardinput(_getch());
                } else {//��Ϸ��ͣ�� ���а�������ֻ�лس���Ч
                    char key = _getch();
                    if (key == 13) {
                        keyboardinput(13);
                    }
                }
            }
            clockNow = clock(); // ��ʱ
            // ���μ�ʱ�ļ������0.45��
            if (clockNow - clockLast > 0.45F * CLOCKS_PER_SEC) {
                clockLast = clockNow;
                // ����������
                if (data.suspend == false) {
                    keyboardinput(80);
                }
            }
            clearGameArea();//�����Ҫ��������
            detectionElimination();//�������
            drawGameArea();//������Ϸ��
            drawotherAreas();//����������
            if (data.winOrLose) {//��Ϸ�����˾Ͳ������ڵķ���λ��
                printgraphical(graphicals[data.graphicaltype], data.x, data.y, colours[data.graphicaltype_color]);
            }
            FlushBatchDraw();//ˢ�·�ֹ��˸
            Sleep(20);//�ȴ�20����
            if (data.winOrLose == false) {//����Ϸ������ӡ��ʾ��
                MessageBox(GetHWnd(), "��Ϸ����", "GAME OVER", MB_OK);//��ӡ��ʾ��
            }
        }
    }
}

int main() {
    init(0);//��ʼ��
    cleardevice();//��ջ�ͼ�豸
    loadimage(NULL, "E:\\Programming\\C\\Tetris\\image.jpg");//��ͼƬ�ļ���ȡͼ��
//    loadimage(NULL, "·��");//��ͼƬ�ļ���ȡͼ�� ·���� \ Ҫ�� \\
    //��ť
    int x1 = 90, y1 = 299, x2 = 370, y2 = 299;//��ť������
    int high = 50, wide = 170;//��ť�ĳ���
    button(x1, y1, wide, high, "��ʼ��Ϸ", LIGHTBLUE, 0);
    button(x2, y2, wide, high, "�˳���Ϸ", LIGHTRED, 0);
    ExMessage msg;
    while (true) {
        if (peekmessage(&msg, EM_MOUSE)) {
            switch (msg.message) {
                case WM_LBUTTONDOWN:
                    if (msg.x >= x1 && msg.x <= x1 + wide && msg.y >= y1 && msg.y <= y1 + high) {
                        cleardevice();//��ջ�ͼ�豸
                        gameLogic();//��Ϸ�߼�
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