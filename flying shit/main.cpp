
#include <iostream>
#include <conio.h>
#include <graphics.h>
#include <cmath>
const unsigned int width = 720;
const unsigned int high = 480;
const unsigned char hole = 100;
using namespace std;
bool flag_over = 1;
int timer0 = 0;
int timer1 = 0;
IMAGE pheart;
IMAGE chacter[2];
IMAGE BK;


inline void dh(int cx)
{
	loadimage(&pheart, "./heart.png", 20, 20);
	for (int temp = width - 20; temp > width - 20 - cx * 20; temp -= 20)
	{
		putimage(temp, 20, &pheart);
	}
}
inline void draw_bk(int x)
{
	loadimage(&BK, "bkg.png", 720, 320+20);
	putimage(x, 0, &BK);
}

class person
{

private:
	int height;
	int y;
	int r;
	int ct_over;
	int hreat;
	bool StateDraw;
public:
	int jump;
	int state;//0为常规 1为无敌状态
	int TextState;//0为常规无显示，1为跳跃，2为向左，3为向右
	person()
	{
		jump = 0;
		height = 2 * high / 3 - 100;
		y = width / 3;
		ct_over = 0;
		hreat = 3;
		r = 18;
		state = 0;
		TextState = 0;
		StateDraw = 0;
	}
	void f_SD()
	{
		if (StateDraw) StateDraw = 0;
		else StateDraw = 1;

	}
	int getr()
	{
		return r;
	}
	int get_hreat()
	{
		return hreat;
	}
	void addhreat(int x)
	{
		hreat += x;
	}
	void draw_hreat()
	{
		for (int i = hreat; i > 0; --i)
		{
			dh(i);
		}
	}
	void left(unsigned char x)
	{
		y -= x;
	}
	void right(unsigned char x)
	{
		y += x;
	}
	inline int getheight()
	{
		return height;
	}
	inline int gety()
	{
		return y;
	}
	inline void addct()
	{
		ct_over++;
	}
	inline void draw()
	{
		//circle(y, height, r);
		loadimage(&chacter[0], "./shit.png", 50, 40);
		loadimage(&chacter[1], "./shit1.png", 50, 40);
		putimage(y - r, height - r, &chacter[StateDraw]);
	}
	inline void jumping()//1上升0下降
	{
		if (jump == 1 && timer1)
		{
			height -= 5;
			timer1--;
			if (timer1 == 0) jump = 0;
		}
		else if (jump == 0)
		{
			height += 2;
		}
	}

	inline void reset()
	{
		height = 2 * high / 3 -100;
		y = width / 3;
		ct_over = 0;
		hreat = 3;
		r = 18;
		state = 0;
		TextState = 0;
	}
	inline void out_crossed()
	{
		char str[10];
		sprintf_s(str, " %d", ct_over);
		outtextxy(0, 0, " Crossed:");
		outtextxy(60, 0, str);


	}
	inline void change_state(int x)
	{
		state = x;
	}
};

person* a = new person();

class retangle
{
public:
	int x1, y1, x2, y2;
	int y3;
	float speed;
public:
	retangle()
	{
		x1 = width;
		y1 = 2 * high / 3 + a->getr();
		x2 = x1 + 20;
		y2 = y1 - 50;
		y3 = y2 - hole;
		speed = 3;
	}
	inline void retangle_go()
	{
		speed += 0.001;
		x1 -= speed;
		x2 -= speed;
		char str2[10];
		sprintf_s(str2, "%.2lf", speed - 3);
		outtextxy(0, 20, "Time:");
		outtextxy(60, 20, str2);

	}
	inline void draw()
	{
		setfillcolor(RGB(2, 158, 245));
		solidrectangle(x1, y2, x2, y1);
		solidrectangle(x1, 0, x2, y3);
	}
	inline void back()
	{
		if (x1 <= 0)
		{
			x1 = width;
			x2 = x1 + 20;
			y2 = y1 - (rand() % 250);
			y3 = y2 - hole;
			flag_over = 1;
		}
	}
	inline void reset()
	{
		x1 = width;
		y1 = 2 * high / 3 + a->getr();
		x2 = x1 + 20;
		y2 = y1 - 50;
		y3 = y2 - hole;
		speed = 3;
	}

};

retangle* obstacle = new retangle();

inline void objdraw()
{
	//draw_bk(0);
	line(0, 2 * high / 3 + a->getr(), width, 2 * high / 3 + a->getr());
	a->draw();
	a->draw_hreat();
	a->out_crossed();
	obstacle->draw();
}
inline bool test()//1受伤过0正常  
{
	if (a->getheight() + a->getr() > 2*high/3 + a->getr() || a->getheight() - a->getr() < 0) a->addhreat(-4);
	if (a->gety() + a->getr() <= obstacle->x1 ||( a->getheight() + a->getr() <= obstacle->y2 && a->getheight() - a->getr() >=obstacle->y3)|| a->gety() - a->getr() >= obstacle->x2)
	{
		if (a->gety() - a->getr() < obstacle->x2) a->state = 0;
		if (a->gety() > obstacle->x1 && a->gety() < obstacle->x2 && flag_over == 1)
		{
			//obstacle->speed += 0.01;
			flag_over = 0;
			a->addct();		
		}
		return 0;
	}
	else return 1;
}

inline void kbin()
{
	if (timer0 > 0)
	{
		timer0--;
	}
	if (_kbhit())
	{
		timer0 = 30;
		int key = _getch();
		if (key == ' ')
		{
			timer1 = 8;
			a->jump = 1;
			a->TextState = 1;;
		}
		else if (key == 'a' && a->gety() > 10)
		{
			a->TextState = 2;
			a->left(10);
		}
		else if (key == 'd' && a->gety() < width - 20)
		{
			a->TextState = 3;
			a->right(20);
		}
		else if (key == 'j')
		{
			a->TextState = 4;
			obstacle->speed += 0.01;
		}
	}
}inline void text()
{
	outtextxy(0, high - 20, "按空格跳跃,按J加速");
	if (timer0 == 0) a->TextState = 0;
	if (a->TextState == 1)
	{
		outtextxy(width - 100, high - 20, "Jumping");
	}
	else if (a->TextState == 2)
	{
		outtextxy(width - 100, high - 20, "←");
	}
	else if (a->TextState == 3)
	{
		outtextxy(width - 100, high - 20, "→");
	}
	else if (a->TextState == 4)
	{
		outtextxy(width - 100, high - 20, "Accelerating!");
	}
}



inline void update()
{
	a->f_SD();
	obstacle->retangle_go();
	obstacle->back();
	//person->poistion update
	//state machine 0->walking	1->jumping 2->falling
	kbin();
	text();
	a->jumping();

}

inline bool restart()
{
	outtextxy(width / 2 - 100, high / 3 - 10, "YOU DIED");
	outtextxy(width / 2 - 150, high / 3 + 10, "PRESS 'R' TO RESTAR");
	obstacle->reset();
	a->reset();
	int ch = _getch();
	if (ch == 'r' || ch == ' ')
	{
		return 1;
	}
	else return 0;
}

//主函数循环实现功能：
//1——清屏
//2——检测输入
//3——画图
//4——

int main(void)
{
	while (1)
	{
		initgraph(width, high);
		draw_bk(0);
		while (1)
		{
			BeginBatchDraw();
			objdraw();
			update();
			if (test() && !a->state)
			{
				a->addhreat(-1);
				a->change_state(1);
			}
			if (a->get_hreat() <= 0) break;
			FlushBatchDraw();
			Sleep(1);

			cleardevice();
		}
		EndBatchDraw();

		if (restart())
		{
			continue;
		}
		else break;
	}
	delete obstacle;
	delete a;
	_getch();
	closegraph();
	return 0;
}
