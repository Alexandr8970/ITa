// lection 6
#include "windows.h"
#include <stdio.h>
#include <conio.h>
#include <ostream>
#include <iostream>
using namespace std;
// http://www.denvo.ru/pub/programming/console-decor.html

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

const int WIDTH_MAP  = 10;
const int HEIGHT_MAP = 10;

const int GAME_INIT         = 0;
const int GAME_MENU         = 1;
const int GAME_RUN          = 2;
const int GAME_EXIT         = 3;
const int GAME_WIN          = 4;
const int GAME_NEXT_LEVEL   = 5;

const int M_EMPTY         = 0;
const int M_WALL          = 1;
const int M_BOX           = 2;
const int M_BOX_PLACE     = 3;
const int M_LOADER        = 4;
const int M_FILL_BOX_PLACE= 5;

bool gMainDoing = true;
int  gGameState = GAME_INIT;

HANDLE gConsoleHandle = 0;
COORD  gPrevPosition;
COORD  gCursorPosition;

int  gLevel          = 1;
int  gLoaderPosI     = 0;
int  gLoaderPosJ     = 0;
int *gCurrentMap     = 0;
int  gNeedFillForWin = 0;
int  gCountMoves     = 0;

// 0 - empty
// 1 - wall
// 2 - box
// 3 - box place
// 4 - loader
// 5 - box on place
// --------------- LEVEL 1 ----------------------------------------------------
int level_1[] =
{
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 2, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 3, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};
int lLoaderPosI_1     = 3;
int lLoaderPosJ_1     = 3;
int lNeedFillForWin_1 = 1;


// --------------- LEVEL 2 ----------------------------------------------------
int level_2[] =
{
    0, 0, 1, 1, 1, 1, 1, 0, 0, 0,
    1, 1, 1, 0, 0, 0, 1, 0, 0, 0,
    1, 3, 0, 2, 0, 0, 1, 0, 0, 0,
    1, 1, 1, 0, 2, 3, 1, 0, 0, 0,
    1, 3, 1, 1, 2, 0, 1, 0, 0, 0,
    1, 0, 1, 0, 3, 0, 1, 1, 0, 0,
    1, 2, 0, 5, 2, 2, 3, 1, 0, 0,
    1, 0, 0, 0, 3, 0, 0, 1, 0, 0,
    1, 1, 1, 1, 1, 1, 1, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
int lLoaderPosI_2     = 2;
int lLoaderPosJ_2     = 2;
int lNeedFillForWin_2 = 7;


//-------------  FUNCTIONS ----------------------------------------------------
void init_screen_ascii();
void game_init_ascii();
void load_level(int level);
void menu();
void gamer_input();
void render();

void draw_map(int *map);
void draw_empty(int x, int y);
void draw_wall(int x, int y);
void draw_box(int x, int y);
void draw_box_place(int x, int y);
void draw_fill_box_place(int x, int y);
void draw_loader();

void draw_statistic();
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
void game_init_ascii()
//-----------------------------------------------------------------------------
{
    system("cls");
    load_level(gLevel);
}

//-----------------------------------------------------------------------------
void load_level(int level)
//-----------------------------------------------------------------------------
{
    switch(level)
    {
        case 1:
        {
            gLoaderPosI     = lLoaderPosI_1;
            gLoaderPosJ     = lLoaderPosJ_1;
            gCurrentMap     = level_1;
            gNeedFillForWin = lNeedFillForWin_1;
            break;
        }

        case 2:
        {
            gLoaderPosI     = lLoaderPosI_2;
            gLoaderPosJ     = lLoaderPosJ_2;
            gCurrentMap     = level_2;
            gNeedFillForWin = lNeedFillForWin_2;
            break;
        }
    }

    gCountMoves = 0;
}

//-----------------------------------------------------------------------------
void init_screen_ascii()
//-----------------------------------------------------------------------------
{
    CONSOLE_CURSOR_INFO cursor_info;
    gConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); 
    system("mode con cols=120 lines=50");

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

    info.Value = gCurrentMap[(gLoaderPosJ + dy)*10 + (gLoaderPosI + dx)];
    if(info.Value == M_WALL)
        return info;
    else
        info.isMoving = true;

    static int index;
    index = (gLoaderPosJ + (dy*2) )*10 + (gLoaderPosI + dx*2);
    if( (index >= 0) && (index < (WIDTH_MAP * HEIGHT_MAP)) )
        info.NextValue = gCurrentMap[(gLoaderPosJ + dy*2)*10 + (gLoaderPosI + dx*2)];

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

        movInfo = getMovinfInfo(dx, dy);

        if(movInfo.isMoving)
        {
            if((movInfo.Value == M_EMPTY) || (movInfo.Value == M_BOX_PLACE))
            {
                gLoaderPosI += dx;
                gLoaderPosJ += dy;
                ++gCountMoves;
                return;
            }

            if(movInfo.Value == M_FILL_BOX_PLACE)
            {
                if(movInfo.NextValue == M_EMPTY)
                {
                    gCurrentMap[(gLoaderPosJ + dy)*10     + (gLoaderPosI + dx)]     = M_BOX_PLACE;
                    gCurrentMap[(gLoaderPosJ + (dy*2))*10 + (gLoaderPosI + (dx*2))] = M_BOX;
                    gLoaderPosI += dx;
                    gLoaderPosJ += dy;
                    ++gCountMoves;
                    return;
                }

                if(movInfo.NextValue == M_BOX_PLACE)
                {
                    gCurrentMap[(gLoaderPosJ + dy)*10     + (gLoaderPosI + dx)]     = M_BOX_PLACE;
                    gCurrentMap[(gLoaderPosJ + (dy*2))*10 + (gLoaderPosI + (dx*2))] = M_FILL_BOX_PLACE;
                    gLoaderPosI += dx;
                    gLoaderPosJ += dy;
                    ++gCountMoves;
                    return;
                }
                return;
            }

            if(movInfo.Value == M_BOX)
            {
                if(movInfo.NextValue == M_EMPTY)
                {
                    gCurrentMap[(gLoaderPosJ + dy)*10     + (gLoaderPosI + dx)]     = M_EMPTY;
                    gCurrentMap[(gLoaderPosJ + (dy*2))*10 + (gLoaderPosI + (dx*2))] = M_BOX;
                    gLoaderPosI += dx;
                    gLoaderPosJ += dy;
                    ++gCountMoves;
                    return;
                }

                if(movInfo.NextValue == M_BOX_PLACE)
                {
                    gCurrentMap[(gLoaderPosJ + dy)*10     + (gLoaderPosI + dx)]     = M_EMPTY;
                    gCurrentMap[(gLoaderPosJ + (dy*2))*10 + (gLoaderPosI + (dx*2))] = M_FILL_BOX_PLACE;
                    gLoaderPosI += dx;
                    gLoaderPosJ += dy;
                    ++gCountMoves;
                    return;
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
void draw_statistic()
//-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = 101;
    pos.Y = 10;

    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("Level %i, moves %i", gLevel, gCountMoves);
}

//-----------------------------------------------------------------------------
void render()
//-----------------------------------------------------------------------------
{
    draw_map(gCurrentMap);
    draw_loader();
    draw_statistic();
}

//-----------------------------------------------------------------------------
void draw_map(int *map)
//-----------------------------------------------------------------------------
{
    static int fills = 0;
    fills = 0;
    for(int j = 0; j < HEIGHT_MAP; ++j)
        for(int i = 0; i < WIDTH_MAP; ++i)
        {
            switch(map[j*10 + i])
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

                case M_BOX:
                {
                    draw_box(i, j);
                    break;
                }

                case M_BOX_PLACE:
                {
                    draw_box_place(i, j);
                    break;
                }

                case M_FILL_BOX_PLACE:
                {
                    draw_fill_box_place(i, j);
                    ++fills;
                    break;
                }
            }
        }

    if(fills == gNeedFillForWin)
        gGameState = GAME_NEXT_LEVEL;
}

//-----------------------------------------------------------------------------
int main()
//-----------------------------------------------------------------------------
{
    init_screen_ascii();
    while(gMainDoing)
    {
        switch(gGameState)
        {
            case GAME_INIT:
            {
                game_init_ascii();
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
void draw_empty(int x, int y)
    //-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 10;
    pos.Y = y * 5;

    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");
}

//-----------------------------------------------------------------------------
void draw_wall(int x, int y)
    //-----------------------------------------------------------------------------
{
    //SetConsoleTextAttribute(gConsoleHandle, 119);
    SetConsoleTextAttribute(gConsoleHandle, 118);
    COORD pos;
    pos.X = x * 10;
    pos.Y = y * 5;

    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("          ");
    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_box_place(int x, int y)
    //-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 10;
    pos.Y = y * 5;

    SetConsoleTextAttribute(gConsoleHandle, 10);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 201, 205, 205, 205, 205, 205, 205, 205, 205, 187);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 32, 32, 32, 32, 32, 32, 32, 32);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 32, 32, 32, 32, 32, 32, 32, 32);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 32, 32, 32, 32, 32, 32, 32, 32);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 200, 205, 205, 205, 205, 205, 205, 205, 205, 188);
    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_box(int x, int y)
    //-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 10;
    pos.Y = y * 5;

    SetConsoleTextAttribute(gConsoleHandle, 10);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 201, 205, 205, 205, 205, 205, 205, 205, 205, 187);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 10);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 200, 205, 205, 205, 205, 205, 205, 205, 205, 188);
    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_fill_box_place(int x, int y)
    //-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = x * 10;
    pos.Y = y * 5;

    SetConsoleTextAttribute(gConsoleHandle, 12);
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 201, 205, 205, 205, 205, 205, 205, 205, 205, 187);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 12);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 12);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c", 186);
    SetConsoleTextAttribute(gConsoleHandle, 14);
    printf("%c%c%c%c%c%c%c%c", 219, 219, 219, 219, 219, 219, 219, 219);
    SetConsoleTextAttribute(gConsoleHandle, 12);
    printf("%c", 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 200, 205, 205, 205, 205, 205, 205, 205, 205, 188);
    SetConsoleTextAttribute(gConsoleHandle, 15);
}

//-----------------------------------------------------------------------------
void draw_loader()
    //-----------------------------------------------------------------------------
{
    COORD pos;
    pos.X = gLoaderPosI * 10;
    pos.Y = gLoaderPosJ * 5;

    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 201, 205, 205, 205, 205, 205, 205, 205, 205, 187);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c", 186, 176);
    SetConsoleTextAttribute(gConsoleHandle, 172);
    printf("%c", 254);
    SetConsoleTextAttribute(gConsoleHandle, 15);
    printf("%c%c%c%c", 176, 176, 176, 176);
    SetConsoleTextAttribute(gConsoleHandle, 172);
    printf("%c", 254);
    SetConsoleTextAttribute(gConsoleHandle, 15);
    printf("%c%c", 176, 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 186, 176, 176, 176, 176, 176, 176, 176, 176, 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 186, 176, 176, 91, 32, 32, 93, 176, 176, 186);

    pos.Y += 1;
    SetConsoleCursorPosition(gConsoleHandle, pos);
    printf("%c%c%c%c%c%c%c%c%c%c", 200, 205, 205, 205, 205, 205, 205, 205, 205, 188);
}

