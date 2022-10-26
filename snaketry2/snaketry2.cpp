#include<iostream>
#include<time.h>
#include<Windows.h>
using namespace std;
int dir, num = 6;
const int w = 15, h = 60;
char arr[w][h];
bool isThere;
struct
{
	int x;
	int y;
} s[100];
struct Fructs
{
	int x, y;
	void New()
	{
		x = 1 + rand() % (w - 2);
		y = 1 + rand() % (h - 2);
	}
} m;
void FillArrary()
{
	for (int i = 0; i < w; ++i)
	{
		for (int j = 0; j < h; ++j)
		{
			arr[i][j] = ' ';
		}
	}
	arr[m.x][m.y] = '@';
	for (int i = 0; i < w; ++i)
	{
		arr[i][0] = 'X';
		arr[i][59] = 'X';
	}
	for (int j = 0; j < h; ++j)
	{
		arr[0][j] = 'X';
		arr[14][j] = 'X';
	}

}
void PrintArray()
{
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			cout << arr[i][j];
		}
		cout << endl;
	}
}
void DrawSnake()
{
	for (int i = 0; i < num; ++i)
	{
		arr[s[i].y][s[i].x] = '*';
		if (s[i + 1].x == 0 && s[i + 1].y == 0)
		{
			break;
		}
	}
}
void Display()
{
	system("cls");
	FillArrary();
	DrawSnake();
	PrintArray();
}
void Tick()
{
	for (int i = num; i > 0; --i)
	{
		s[i].x = s[i - 1].x;
		s[i].y = s[i - 1].y;
	}
	if ((s[0].x == m.y) && (s[0].y == m.x))
	{
		++num;
		m.New();
		for (int j = 0; j < num; j++)
		{
			if (s[j].x == m.y && s[j].y == m.x)
			{
				isThere = true;
			}
		}
		while (isThere)
		{
			for (int i = 0; i < num; i++)
			{
				while (s[i].x == m.y && s[i].y == m.x)
				{
					m.New();
				}
				isThere = false;
				for (int j = 0; j < num; j++)
				{
					if (s[j].x == m.y && s[j].y == m.x)
					{
						isThere = true;
					}
				}
			}
		}
	}
	if (dir == 0) s[0].y -= 1;
	if (dir == 1) s[0].x -= 1;
	if (dir == 2) s[0].x += 1;
	if (dir == 3) s[0].y += 1;
	if (s[0].x == h - 1 || s[0].x == 0 || s[0].y == w - 1 || s[0].y == 0)
	{
		system("cls");
		cout << "Game Over!" << endl;
		exit(0);
	}
	for (int i = 1; i < num; ++i)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y)
		{
			system("cls");
			cout << "Game Over!" << endl;
			exit(0);
		}
	}
}
void KeyboardEvent()
{
	Sleep(100);
	if (GetAsyncKeyState(VK_DOWN) && dir != 0)
	{
		dir = 3;
	}
	else if (GetAsyncKeyState(VK_UP) && dir != 3)
	{
		dir = 0;
	}
	else if (GetAsyncKeyState(VK_RIGHT) && dir != 1)
	{
		dir = 2;
	}
	else if (GetAsyncKeyState(VK_LEFT) && dir != 2)
	{
		dir = 1;
	}
}
void Timer()
{
	Display();
	KeyboardEvent();
	Tick();
}
int main()
{
	HANDLE hWndCon = GetStdHandle(STD_OUTPUT_HANDLE);   //прячем курсор
	CONSOLE_CURSOR_INFO structCursorInfo;
	GetConsoleCursorInfo(hWndCon, &structCursorInfo);
	structCursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(hWndCon, &structCursorInfo);
	srand(time(0));
	m.New();
	s[0].x = 4;    //начальные координаты
	s[0].y = 9;
	while (true)
	{
		Timer();
	}
	return 0;
}