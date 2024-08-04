#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <conio.h>
#include <stack>


void gotoxy(int x, int y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}

enum ConsloneColor
{
    Black = 0,
    Green = 2,
    Cyan = 3,
    Light_cyan = 11,
    Dark_grey = 8,
    Light_green = 10,
    Blue = 1,
    Red = 4,
    Magenta = 5,
    Brown = 6,
    Yellow = 14,
    White = 15,
    Light_blue = 9
};
void setColor(int background, int text)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}
const int border{ 100 };
const int empty{ 0 };
const int bomb{ 10 };

class Map 
{
private:
    int size{ 0 };
    std::vector <std::vector<int>> map;
    std::vector <std::vector<int>> mask;
public:
    Map()
    {
        size = 20;
    }
    bool isBorder(int x, int y)
    {
        if (x < 0 || x >= size)
            return false;

        if (y < 0 || y >= size)
            return false;

        if (map[x][y] == border)
            return true;
        return false;
    }

    int openCell(int x, int y)
    {
        int rez { 1 };
        mask[x][y] = 1;
        if (map[x][y] == bomb)
        {
            rez = bomb;
        }
        else if (map[x][y] == empty)
        {
            rez = empty;
        }
        show();
        return rez;
    }
    int openBomb(int x, int y)
    {
        int rez{ 1 };
        mask[x][y] = 1;
        if (map[x][y] == bomb)
        {
            rez = bomb;
        }
        else if (map[x][y] == empty)
        {
            rez = empty;
        }
        show();
        return rez;
    }


    void initMap()
    {
        initVec(map);
    }
    void initMask()
    {
        initVec(mask);
    }
    void initVec(std::vector <std::vector<int>> &vec)
    {
        for (int i = 0; i < size; i++)
        {
            std::vector<int> tmp;
            for (int j = 0; j < size; j++)
            {
                if (i == 0 || j == 0 || i == size - 1 || j == size - 1)
                    tmp.push_back(border);
                else
                    tmp.push_back(empty);
            }
            vec.push_back(tmp);
        }
    }

    void coutColor(char ch, int color)
    {
        setColor(Black, color);
        std::cout << ch;
        setColor(Black, White);
    }
    void show()
    {
        gotoxy(0,0);
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (mask[j][i] == empty)
                {
                    std::cout << ".";
                    continue;
                }
                if (map[j][i] == border)
                    coutColor('#', Light_blue);
                else if (map[j][i] == empty)
                    std::cout << " ";
                else if (map[j][i] == bomb)
                    coutColor('*', Yellow);
                else if (map[j][i] == 1)
                    coutColor('1', Blue);
                else if (map[j][i] == 2)
                    coutColor('2', Green);
                else if (map[j][i] == 3)
                    coutColor('3', Red);
                else if (map[j][i] == 4)
                    coutColor('4', Magenta);
                else if (map[j][i] == 5)
                    coutColor('5', Brown);
                else if (map[j][i] == 6)
                    coutColor('1', Dark_grey);
                else if (map[j][i] == 7)
                    coutColor('1', Light_green);
                else if (map[j][i] == 8)
                    coutColor('1', Light_cyan);                
            }
            std::cout << "\n";
        }
    }

    void setRandBomb(int numBomb)
    {
        if (numBomb >= (size - 2) * (size - 2))
            return;
        for (int i = 0; i < numBomb; i++)
        {
            int x{ 0 }, y{ 0 };
            do
            {
                x = rand() % (size - 2) + 1;
                y = rand() % (size - 2) + 1;
            } while (map[x][y] == bomb);
            map[x][y] = bomb;
        }
    }
    void setDigits()
    {
        int d{ 0 };
        for (int i = 1; i < size - 1; i++)
        {
            for (int j = 1; j < size - 1; j++)
            {
                if (map[j][i] == bomb)
                    continue;
                if (map[j][i + 1] == bomb)
                    d++;
                if (map[j][i - 1] == bomb)
                    d++;
                if (map[j + 1][i + 1] == bomb)
                    d++;
                if (map[j + 1][i - 1] == bomb)
                    d++;
                if (map[j - 1][i + 1] == bomb)
                    d++;
                if (map[j - 1][i - 1] == bomb)
                    d++;
                if (map[j + 1][i] == bomb)
                    d++;
                if (map[j - 1][i] == bomb)
                    d++;
                map[j][i] = d;
                d = 0;
            }
        }
    }
    void fill(int px, int py)
    {
        int x{ 0 }, y{ 0 };
        std::stack <int> stk;
        stk.push(px);
        stk.push(py);
        while (true)
        {
            y = stk.top();
            stk.pop();
            x = stk.top();
            stk.pop();

            if (map[x][y + 1] == empty && mask[x][y + 1] == 0)
            {
                stk.push(x);
                stk.push(y + 1);
            }
            mask[x][y + 1] = 1;
            if (map[x][y - 1] == empty && mask[x][y - 1] == 0)
            {
                stk.push(x);
                stk.push(y - 1);
            }
            mask[x][y - 1] = 1;
            if (map[x + 1][y + 1] == empty && mask[x + 1][y + 1] == 0)
            {
                stk.push(x + 1);
                stk.push(y + 1);   
            }
            mask[x + 1][y + 1] = 1;
            if (map[x + 1][y - 1] == empty && mask[x + 1][y - 1] == 0)
            {
                stk.push(x + 1);
                stk.push(y - 1);
            }
            mask[x + 1][y - 1] = 1;
            if (map[x - 1][y + 1] == empty && mask[x - 1][y + 1] == 0)
            {
                stk.push(x - 1);
                stk.push(y + 1);
            }
            mask[x - 1][y + 1] = 1;
            if (map[x - 1][y - 1] == empty && mask[x - 1][y - 1] == 0)
            {
                stk.push(x - 1);
                stk.push(y - 1);
            }
            mask[x - 1][y - 1] = 1;
            if (map[x - 1][y] == empty && mask[x - 1][y] == 0)
            {
                stk.push(x - 1);
                stk.push(y);
            }
            mask[x - 1][y] = 1;
            if (map[x + 1][y] == empty && mask[x+1][y] == 0)
            {
                stk.push(x + 1);
                stk.push(y);
            }
            mask[x + 1][y] = 1;

            if (stk.empty())
                break;
        }
    }
};

class Keyboard
{
private:
    int ch{ 0 };
public:
    Keyboard()
    {
        ch = 0;
    }

    void waitKey()
    {
        ch = _getch();
    }
    int getKey()
    {
        return ch;
    }

};

class Cursor 
{
private:
    int x{ 1 }, y{ 1 }, tx{ 1 }, ty{ 1 };
public:
    void save()
    {
        tx = x;
        ty = y;
    }
    void undo()
    {
        x = tx;
        y = ty;
    }
    void incX()
    {
        x++;
    }
    void decX()
    {
        x--;
    }
    void incY()
    {
        y++;
    }
    void decY()
    {
        y--;
    }
    int getX()
    {
        return x;
    }
    int getY()
    {
        return y;
    }
    void move()
    {
        gotoxy(x, y);
    }
};

class Game
{
private:
    void showLogo()
    {
        gotoxy(40,9);
        std::cout << "Saper Game\n";
        Sleep(2000);
        system("cls");
    }
    void gameOver()
    {
        gotoxy(40, 9);
        std::cout << "Game Over\n";
        Sleep(2000);
        gotoxy(0, 15);
        system("pause");
    }
public:
    void run()
    {
        showLogo();
        Map map;
        Keyboard keyboard;
        Cursor cursor;
        map.initMap();
        map.initMask();
        map.setRandBomb(50);
        map.setDigits();
        map.show();
 
        
      
        cursor.move();

        bool exit = false;
        while (!exit)
        {
            keyboard.waitKey();
            cursor.save();

            switch (keyboard.getKey())
            {
                case 9: map.openBomb(cursor.getX(), cursor.getY()); break;
                case 77: cursor.incX(); break;//право
                case 80: cursor.incY(); break;//вниз
                case 75: cursor.decX(); break;//лево
                case 72: cursor.decY(); break;//верх
                case 13: 
                    int rez = map.openCell(cursor.getX(), cursor.getY());
                    if (rez == bomb)
                        {
                            gameOver();
                            exit = true;
                        }
                    if (rez == empty)
                    {
                        map.fill(cursor.getX(), cursor.getY());
                        map.show();
                    }
                    break;

            }
            if (map.isBorder(cursor.getX(), cursor.getY()))
            {
                cursor.undo();
            }
            cursor.move();
        }
    }
};

int main()
{
    srand(time(0));
    Game game;
    game.run();
    return 0;
}

