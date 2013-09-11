// lection ?
// http://orionxl.ru/volnovoj-algoritm.html
// http://astralax.ru/articles/pathway
#include "windows.h"
#include <stdio.h>
#include <conio.h>
#include <ostream>
#include <iostream>
using namespace std;


//------------- GLOBAL VARIABLES ----------------------------------------------
struct SMovingInfo
{
    int  Value;
    int  NextValue;
    bool isMoving;
};
SMovingInfo getMovinfInfo(int dx, int dy);

const int WIDTH_SCREEN  = 100;
const int HEIGHT_SCREEN = 50;

const int WIDTH_MAP  = 30;
const int HEIGHT_MAP = 30;

const int GAME_INIT         = 0;
const int GAME_MENU         = 1;
const int GAME_RUN          = 2;
const int GAME_EXIT         = 3;
const int GAME_WIN          = 4;
const int GAME_NEXT_LEVEL   = 5;

const int M_EMPTY   = 0;
const int M_WALL    = 1;
const int M_PLAYER  = 2;
const int M_FINISH  = 3;
const int M_WAY     = 4;

bool gMainDoing = true;
int  gGameState = GAME_INIT;

HANDLE gConsoleHandle = 0;
COORD  gPrevPosition;
COORD  gCursorPosition;

int  gLevel          = 1;
int  gPlayerPosI     = 0;
int  gPlayerPosJ     = 0;
int *gCurrentMap     = 0;

// 0 - empty
// 1 - wall
// 2 - player
// 3 - finish
// 4 - way
// --------------- LEVEL 1 ----------------------------------------------------
int level_1[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int startPlayerPosI_l1     = 3;
int startPlayerPosJ_l1     = 3;

//-------------  FUNCTIONS ----------------------------------------------------
void init_screen();
void game_init();
void load_level(int level);
void gamer_input();
void render();

void draw_map(int *map);
void draw_empty(int x, int y);
void draw_wall(int x, int y);
void draw_player(int x, int y);
void draw_finish(int x, int y);
void draw_way(int x, int y);
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void game_init()
//-----------------------------------------------------------------------------
{
    system("cls");
    load_level(gLevel);
}

void load_level(int level)
{
    switch(level)
    {
        case 1:
        {
            startPlayerPosI_l1  = gPlayerPosI;
            startPlayerPosJ_l1  = gPlayerPosJ;
            gCurrentMap         = level_1;
            break;
        }
    }
}

//-----------------------------------------------------------------------------
void init_screen()
//-----------------------------------------------------------------------------
{
    CONSOLE_CURSOR_INFO cursor_info;
    gConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
    system("mode con cols=121 lines=60");

    cursor_info.dwSize = 10;
    cursor_info.bVisible = false;
    SetConsoleCursorInfo(gConsoleHandle, &cursor_info);
}

//-----------------------------------------------------------------------------
SMovingInfo getMovinfInfo(int dx, int dy)
//-----------------------------------------------------------------------------
{
    static SMovingInfo info;
    info.Value     = 1;
    info.NextValue = 1;
    info.isMoving  = false;

    info.Value = gCurrentMap[(gPlayerPosJ + dy)*WIDTH_MAP + (gPlayerPosI + dx)];
    if(info.Value == M_WALL)
        return info;
    else
        info.isMoving = true;

    static int index;
    index = (gPlayerPosJ + (dy*2) )*10 + (gPlayerPosI + dx*2);
    if( (index >= 0) && (index < (WIDTH_MAP * HEIGHT_MAP)) )
        info.NextValue = gCurrentMap[(gPlayerPosJ + dy*2)*WIDTH_MAP + (gPlayerPosI + dx*2)];

    return info;
}

//-----------------------------------------------------------------------------
void gamer_input()
//-----------------------------------------------------------------------------
{
    static SMovingInfo movInfo;
    static int dx = 0;
    static int dy = 0;

    int input = getch();
    if((input >= 0) && (input <= 100))
    {
        switch(input)
        {
            case 72: // UP
            {
                dx = 0;
                dy = -1;
                break;
            }

            case 80: // DOWN
            {
                dx = 0;
                dy = 1;
                break;
            }

            case 77: // RIGHT
            {
                dx = 1;
                dy = 0;
                break;
            }

            case 75: // LEFT
            {
                dx = -1;
                dy = 0;
                break;
            }

            case 27:
            {
                gGameState = GAME_EXIT;
                break;
            }
        }
    }
}

//-----------------------------------------------------------------------------
void render()
//-----------------------------------------------------------------------------
{
    draw_map(gCurrentMap);
}

//-----------------------------------------------------------------------------
int main()
//-----------------------------------------------------------------------------
{
    init_screen();

    /*
    for(int i = 0; i != 300; ++i)
    {
        SetConsoleTextAttribute(gConsoleHandle, i);
        cout << i << " ";
    }
    */

    while(gMainDoing)
    {
        switch(gGameState)
        {
            case GAME_INIT:
            {
                game_init();
                gGameState = GAME_RUN;
                render();
                break;
            }

            case GAME_MENU:
            {
                break;
            }

            case GAME_RUN:
            {
                gamer_input();
                render();
                break;
            }

            case GAME_EXIT:
            {
                gMainDoing = false;
                break;
            }

            case GAME_NEXT_LEVEL:
            {
                ++gLevel;
                if(gLevel >= 3)
                    gGameState = GAME_WIN;
                else
                    gGameState = GAME_INIT;
                break;
            }

            case GAME_WIN:
                {
                    system("cls");
                    printf("You win!!! press enter for end\n");
                    int input = getch();
                    gMainDoing = false;
                    break;
                }
        }
    }

    return 0;
}

//-----------------------------------------------------------------------------
void draw_map(int *map)
//-----------------------------------------------------------------------------
{
    for(int j = 0; j < HEIGHT_MAP; ++j)
        for(int i = 0; i < WIDTH_MAP; ++i)
        {
            switch(map[j*WIDTH_MAP + i])
            {
            case M_EMPTY:
                {
                    draw_empty(i, j);
                    break;
                }

            case M_WALL:
                {
                    draw_wall(i, j);
                    break;
                }

            case M_PLAYER:
                {
                    draw_player(i, j);
                    break;
                }

            case M_FINISH:
                {
                    draw_finish(i, j);
                    break;
                }

            case M_WAY:
                {
                    draw_way(i, j);
                    break;
                }
            }
        }
}

//-----------------------------------------------------------------------------
void draw_empty(int x, int y)
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 4;
    pos.Y = y * 2;

    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");
}

//-----------------------------------------------------------------------------
void draw_wall(int x, int y)
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 4;
    pos.Y = y * 2;

    SetConsoleTextAttribute(gConsoleHandle, 85);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_player(int x, int y)
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 4;
    pos.Y = y * 2;

    SetConsoleTextAttribute(gConsoleHandle, 170);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_finish(int x, int y)
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 4;
    pos.Y = y * 2;

    SetConsoleTextAttribute(gConsoleHandle, 204);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_way(int x, int y)
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 4;
    pos.Y = y * 2;

    SetConsoleTextAttribute(gConsoleHandle, 187);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("    ");

    SetConsoleTextAttribute(gConsoleHandle, 15);
}

