#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

using namespace std;

int dir, num = 6;
const int w = 15, h = 60;
vector<vector<char>> arr(w, vector<char>(h, ' '));
bool isThere;

struct Point {int x; int y;};
vector<Point> s(100);

struct Fructs {
    int x, y;
    void New() {
        x = 1 + rand() % (w - 2);
        y = 1 + rand() % (h - 2);
    }
} m;

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    cout << "\033[2J\033[1;1H";
#endif
}

void FillArray() {
    for (int i = 0; i < w; ++i)
        for (int j = 0; j < h; ++j) arr[i][j] = ' ';
    
    arr[m.x][m.y] = '@';
    for (int i = 0; i < w; ++i) {
        arr[i][0] = 'X';
        arr[i][h - 1] = 'X';
    }
    for (int j = 0; j < h; ++j) {
        arr[0][j] = 'X';
        arr[w - 1][j] = 'X';
    }
}

void PrintArray() {
    clearScreen();
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) cout << arr[i][j];
        cout << endl;
    }
}

void DrawSnake() {
    for (int i = 0; i < num; ++i) {
        arr[s[i].y][s[i].x] = '*';
        if (s[i + 1].x == 0 && s[i + 1].y == 0) break;
    }
}

void Display() {
    FillArray();
    DrawSnake();
    PrintArray();
}

void Tick() {
    for (int i = num; i > 0; --i) {
        s[i].x = s[i - 1].x;
        s[i].y = s[i - 1].y;
    }
    if ((s[0].x == m.y) && (s[0].y == m.x)) {
        ++num;
        m.New();
        for (int j = 0; j < num; j++) if (s[j].x == m.y && s[j].y == m.x) isThere = true;
        
        while (isThere) {
            for (int i = 0; i < num; i++) {
                while (s[i].x == m.y && s[i].y == m.x) m.New();
                isThere = false;
                for (int j = 0; j < num; j++) if (s[j].x == m.y && s[j].y == m.x) isThere = true;
            }
        }
    }
    if (dir == 0) s[0].y -= 1;
    if (dir == 1) s[0].x -= 1;
    if (dir == 2) s[0].x += 1;
    if (dir == 3) s[0].y += 1;
    if (s[0].x == h - 1 || s[0].x == 0 || s[0].y == w - 1 || s[0].y == 0) {
        clearScreen();
        cout << "Game Over!" << endl;
        exit(0);
    }
    for (int i = 1; i < num; ++i) {
        if (s[0].x == s[i].x && s[0].y == s[i].y) {
            clearScreen();
            cout << "Game Over!" << endl;
            exit(0);
        }
    }
}

#ifdef _WIN32
int kbhit() return _kbhit();
int getch() return _getch();

#else
int kbhit() {
    struct termios oldt, newt;
    int ch;
    int oldf;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);
    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
}

int getch() {
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

void KeyboardEvent() {
    if (kbhit()) {
        switch (getch()) {
        case 'w': if (dir != 3) dir = 0; break;
        case 'a': if (dir != 2) dir = 1; break;
        case 'd': if (dir != 1) dir = 2; break;
        case 's': if (dir != 0) dir = 3; break;
        }
    }
}

void Timer() {
    Display();
    KeyboardEvent();
    Tick();
    this_thread::sleep_for(chrono::milliseconds(100));
}

int main() {
    srand(static_cast<unsigned int>(time(0)));
    m.New();
    s[0].x = 4;
    s[0].y = 9;
    while (true) Timer();
    return 0;
}
