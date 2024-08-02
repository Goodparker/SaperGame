#include <iostream>
#include <Windows.h>
#include <vector>
#include <ctime>
#include <conio.h>


void gotoxy(int x, int y)
{
    COORD p = { x, y };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), p);
}



class Map 
{
private:
    const int border{ 100 };
    const int empty{ 0 };
    const int bomb{ 10 };
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

    void initMap()
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
            map.push_back(tmp);
        }
    }

    void show()
    {
        for (int i = 0; i < size; i++)
        {
            for (int j = 0; j < size; j++)
            {
                if (map[j][i] == border)
                    std::cout << "#";
                else if (map[j][i] == empty)
                    std::cout << " ";
                else if (map[j][i] == bomb)
                    std::cout << "*";
                else if (map[j][i] >= 1 && map[j][i] <= 8)
                    std::cout << map[j][i];
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
public:
    void run()
    {
        showLogo();
        Map map;
        Keyboard keyboard;
        Cursor cursor;
        map.initMap();
        map.setRandBomb(25);
        map.setDigits();
        map.show();
      
        cursor.move();

        while (true)
        {
            keyboard.waitKey();
            cursor.save();

            switch (keyboard.getKey())
            {
                case 77: cursor.incX(); break;//право
                case 80: cursor.incY(); break;//вниз
                case 75: cursor.decX(); break;//лево
                case 72: cursor.decY(); break;//верх
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

