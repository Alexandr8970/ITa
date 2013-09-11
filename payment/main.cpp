// lection 5

#include <windows.h>
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<cstdlib>
#include<sstream>
using namespace std;


// --------------------------- MENU -------------------------------------------
int gMainLevelMenu = 0;
int gSubLevelMenu  = 0;

//      MAIN MENU codes
const size_t MAIN_INPUT                     = 1;
const size_t MAIN_READ                      = 2;
const size_t MAIN_HELP                      = 3;
const size_t MAIN_ABOUT                     = 4;
const size_t MAIN_EXIT                      = 5;
const size_t MAIN_INPUT_IN_DATE             = 6;
const size_t MAIN_INPUT_TODAY               = 7;
const size_t MAIN_SELECT_FROM_ALL_SAVING    = 8;
const size_t FOR_ALL_BACK                   = 100;
const size_t NOT_EVENT                      = 101;

struct Menu
{
    std::vector<std::string> mMenuList;
    std::vector<Menu>        mSub;
    std::vector<size_t>      mEventCode;
    int                      mPrevMenuIndex;
};

void addMenu(Menu &menu, std::string Description, size_t eventCode);
void addSubMenu(Menu &menu, Menu &SubMenu);
void initMenu();
void draw_menu();

std::string select_from_all_saved_files();

void input_menu_key(unsigned char &input);

Menu    gMainMenu;
Menu    gCurrentMenu;
Menu    gPrevMenu;
size_t  gCurrentEvent = 0;
bool    gIsExit = true;
//-----------------------------------------------------------------------------

//-------------------------- TEXT UTILS --------------------------------------
enum MY_COLOR
{
    BLACK = 0,
    BLUE,
    GREEN,
    AQUA,
    RED,
    PURPLE,
    YELLOW,
    WHITE,
    GRAY,
    LIGHT_BLUE,
    LIGHT_GREEN,
    LIGHT_AQUA,
    LIGHT_RED,
    LIGHT_PURPLE,
    LIGHT_YELLOW,
    BRIGHT_WHITE
};

static HANDLE hCon;

void print_color_line(std::string text, MY_COLOR color);
void print_color_text(std::string text, MY_COLOR color);
void set_color(MY_COLOR color);
void set_int_color(int color);
//-----------------------------------------------------------------------------


//------------------------ PAYMENT DATA ---------------------------------------
struct ExpenditureInfo
{
    double      mPrice;
    std::string mDescription;
};

struct ExpenditurePerDay
{
    std::string                     mDate;
    std::vector<ExpenditureInfo>    mArrayCosts;
};

ExpenditureInfo fillExpenditureInfo(double &price, std::string &description);
void saveDataPayment(ExpenditurePerDay &dayPayment);
void print_data(ExpenditurePerDay &dayPayment);

//-----------------------------------------------------------------------------


//-------------------------- REDA/INPUT FUNCTIONS -----------------------------
std::string input_expenditure_date(bool isToday);
void input_expenditure(std::string date);

std::vector<std::string> getCurrentDirDataFiles();
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
int print_end_input_menu()
//-----------------------------------------------------------------------------
{
    int input;
    while(true)
    {
        cout << "[0] - show input data;\n";
        cout << "[1] - exit with saving data;\n";
        cout << "[2] - exit without saving data;\n";
        cin >> input;

        if((input >= 0) && (input <= 2))
            return input;

        cout << "choose correct menu;\n";
    }
    return input;
}

//-----------------------------------------------------------------------------
void print_main_menu()
//-----------------------------------------------------------------------------
{
    cout << "Please select mode:\n";

    print_color_text("[0]", GREEN); cout << " - input data\n";
    print_color_text("[1]", GREEN); cout << " - read data\n";
    print_color_text("[2]", GREEN); cout << " - help\n";
    print_color_text("[3]", GREEN); cout << " - about programm\n";
    print_color_text("[4]", RED); cout << " - exit\n";
}


//-----------------------------------------------------------------------------
void read_data()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void print_help()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void print_about_program()
//-----------------------------------------------------------------------------
{
}


int filterException(int code, PEXCEPTION_POINTERS ex) {
    std::cout << "Filtering " << std::hex << code << std::endl;
    return EXCEPTION_EXECUTE_HANDLER;
}




//------------------------- MAIN ----------------------------------------------
int main()
//-----------------------------------------------------------------------------
{
    
    for(int i = 0; i != 255; ++i)
    {
        set_int_color(i);
        cout << "A" << i << " ";
    }
    

    getCurrentDirDataFiles();
    initMenu();

    std::string     date;
    unsigned char input_ch;
    while(gIsExit)
    {
        draw_menu();
        input_ch = getch();
        input_menu_key(input_ch);
        
        switch(gCurrentEvent)
        {
            case MAIN_HELP:
            {
                break;
            }

            case MAIN_ABOUT:
            {
                break;
            }

            case MAIN_EXIT:
            {
                gIsExit = false;
                break;
            }

            case MAIN_INPUT_IN_DATE:
            {
                system("cls");
                date = input_expenditure_date(false);
                system("cls");
                input_expenditure(date);
                break;
            }

            case MAIN_INPUT_TODAY:
            {
                system("cls");
                date = input_expenditure_date(true);
                system("cls");
                input_expenditure(date);
                break;
            }

            case MAIN_SELECT_FROM_ALL_SAVING:
            {
                system("cls");
                date = select_from_all_saved_files();
                if(date == "Back.")
                {
                }
                break;
            };
        }
        gCurrentEvent = 0;
    }

    return 0;
}

//------------------------add MENU --------------------------------------------
void addMenu(Menu &menu, std::string Description, size_t eventCode)
//-----------------------------------------------------------------------------
{
    Menu empty;
    menu.mMenuList.push_back(Description);
    menu.mSub.push_back(empty);
    menu.mEventCode.push_back(eventCode);
}

//------------------------add sub MENU ----------------------------------------
void addSubMenu(Menu &menu, Menu &SubMenu)
//-----------------------------------------------------------------------------
{
    menu = SubMenu;
}

//-----------------------init MENU --------------------------------------------
void initMenu()
//-----------------------------------------------------------------------------
{
    gMainLevelMenu = 0;
    gSubLevelMenu  = 0;

    addMenu(gMainMenu, "Input data.", MAIN_INPUT);
    addMenu(gMainMenu, "Read data.", MAIN_READ);
    addMenu(gMainMenu, "Help.", MAIN_HELP);
    addMenu(gMainMenu, "About program.", MAIN_ABOUT);
    addMenu(gMainMenu, "Exit.", MAIN_EXIT);

    Menu sub1;
    addMenu(sub1," Input date.", MAIN_INPUT_IN_DATE);
    addMenu(sub1," Using today date.", MAIN_INPUT_TODAY);
    addMenu(sub1," Back.", FOR_ALL_BACK);
    addSubMenu(gMainMenu.mSub[0], sub1);

    Menu sub2;
    addMenu(sub2," Select from all saved files.", MAIN_SELECT_FROM_ALL_SAVING);
    addMenu(sub2," Back.", FOR_ALL_BACK);
    addSubMenu(gMainMenu.mSub[1], sub2);

    gCurrentMenu = gMainMenu;
}

//-----------------------draw MENU --------------------------------------------
void draw_menu()
//-----------------------------------------------------------------------------
{
    system("cls");

    for(int i = 0; i < gCurrentMenu.mMenuList.size(); ++i)
    {
        if(i == gSubLevelMenu)
            set_int_color(49);
        else
            set_int_color(31);

        cout << gCurrentMenu.mMenuList[i] << endl;
        set_color(WHITE);
    }
}

//-----------------------input menu key----------------------------------------
void input_menu_key(unsigned char &input)
//-----------------------------------------------------------------------------
{

    int start = 0;
    int end   = gCurrentMenu.mMenuList.size() - 1;

    switch(input)
    {
        case 80:
        {
            if((gSubLevelMenu + 1) > end)
                gSubLevelMenu = start;
            else
                ++gSubLevelMenu;
            break;
        }

        case 72:
        {
            if((gSubLevelMenu - 1) < start)
                gSubLevelMenu = end;
            else
                --gSubLevelMenu;
            break;
        }

        case 13:
        {
            gCurrentEvent = gCurrentMenu.mEventCode[gSubLevelMenu];
            if(gSubLevelMenu == end)
            {
                gCurrentMenu.mMenuList      = gPrevMenu.mMenuList;
                gCurrentMenu.mSub           = gPrevMenu.mSub;
                gCurrentMenu.mEventCode     = gPrevMenu.mEventCode;
                gCurrentMenu.mPrevMenuIndex = gPrevMenu.mPrevMenuIndex;

                gSubLevelMenu = gPrevMenu.mPrevMenuIndex;
            }
            else
            {
                if(gCurrentMenu.mSub.size() > 0)
                {
                    if(gCurrentMenu.mSub[gSubLevelMenu].mMenuList.size() > 0)
                    {
                        gPrevMenu                = gCurrentMenu;
                        gPrevMenu.mPrevMenuIndex = gSubLevelMenu;

                        gCurrentMenu.mMenuList      = gPrevMenu.mSub[gSubLevelMenu].mMenuList;
                        gCurrentMenu.mSub           = gPrevMenu.mSub[gSubLevelMenu].mSub;
                        gCurrentMenu.mEventCode.resize(gPrevMenu.mSub[gSubLevelMenu].mEventCode.size() + 1);
                        gCurrentMenu.mEventCode     = gPrevMenu.mSub[gSubLevelMenu].mEventCode;
                        gCurrentMenu.mPrevMenuIndex = gPrevMenu.mSub[gSubLevelMenu].mPrevMenuIndex;

                        gSubLevelMenu = 0;
                    }
                }
            }
            break;
        }
    }
}


//------------------------ TEXT UTILS -----------------------------------------
//-----------------------------------------------------------------------------
void print_color_line(std::string text, MY_COLOR color) // with enter
//-----------------------------------------------------------------------------
{
    static HANDLE hCon;

    if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hCon, color);
    cout << text << endl;
    SetConsoleTextAttribute(hCon, WHITE);
}

//-----------------------------------------------------------------------------
void print_color_text(std::string text, MY_COLOR color)// without enter
//-----------------------------------------------------------------------------
{
    if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hCon, color);
    cout << text;// << endl;
    SetConsoleTextAttribute(hCon, WHITE);
}

//-----------------------------------------------------------------------------
void set_color(MY_COLOR color)
//-----------------------------------------------------------------------------
{
    if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hCon, color);
}

//-----------------------------------------------------------------------------
void set_int_color(int color)
//-----------------------------------------------------------------------------
{
    if(hCon == NULL)
        hCon = GetStdHandle(STD_OUTPUT_HANDLE);

    SetConsoleTextAttribute(hCon, color);
}


//------------------------ PAYMENT DATA ---------------------------------------
//-----------------------------------------------------------------------------
ExpenditureInfo fillExpenditureInfo(double &price, std::string &description)
//-----------------------------------------------------------------------------
{
    ExpenditureInfo res;
    res.mPrice          = price;
    res.mDescription    = description;
    return res;
}

//-----------------------------------------------------------------------------
void saveDataPayment(ExpenditurePerDay &dayPayment)
//-----------------------------------------------------------------------------
{
    std::string file_name = dayPayment.mDate;
    file_name += ".data";

    ofstream file(file_name, ios::app);
    for(int i = 0; i < dayPayment.mArrayCosts.size(); ++i)
    {
        file << dayPayment.mArrayCosts[i].mPrice << endl;
        file << dayPayment.mArrayCosts[i].mDescription << endl;
    }
    file.close();
}

//-----------------------------------------------------------------------------
void print_data(ExpenditurePerDay &dayPayment)
//-----------------------------------------------------------------------------
{
}
//-----------------------------------------------------------------------------

//---------------------- INPUT FUNCTIONS --------------------------------------
//-----------------------------------------------------------------------------
std::string input_expenditure_date(bool isToday)
//-----------------------------------------------------------------------------
{
    std::string res = "";
    if(isToday)
    {
        time_t     now = time(0);
        struct tm  tstruct;
        char       buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%d.%m.%Y", &tstruct);
        res = buf;
    }
    else
    {
        cout << "Input date:(dd.mm.yyyy - 01.01.2001)" ;
        cin >> res;
    }
    return res;
}

//-----------------------------------------------------------------------------
void input_expenditure(std::string date)
//-----------------------------------------------------------------------------
{
    std::string input_date = date;

    ExpenditurePerDay ourDate;
    ourDate.mDate = date;

    double common_summa = 0.0;
    while(true)
    {
        system("cls");
        set_color(WHITE);
        cout << "date - ";
        set_color(RED);
        cout << date << "(" << common_summa << " UAN)" <<endl;
        set_color(WHITE);
        cout << "Input you expenditur... (For end, input price = -1)" << endl;

        ExpenditureInfo tmpExpenditure;
        cout << "price = ";
        cin >> tmpExpenditure.mPrice;
        if(tmpExpenditure.mPrice == -1.0)
            break;

        cout << "description = ";
        cin >> tmpExpenditure.mDescription;
        common_summa += tmpExpenditure.mPrice;

        ourDate.mArrayCosts.push_back(tmpExpenditure);
    }

    while(true)
    {
        cout << "Do you want save this data(y/n)? ";
        std::string tmp;
        cin >> tmp;
        if(tmp == "y")
        {
            saveDataPayment(ourDate);
            break;
        }
    }
}

//-----------------------------------------------------------------------------
std::vector<std::string> getCurrentDirDataFiles()
//-----------------------------------------------------------------------------
{
    std::vector<std::string> res;
    WIN32_FIND_DATA FindFileData;
    HANDLE            hFile;
    std::string current_dir;
    char tmp[1024] = {0};

    GetCurrentDirectory(1024, &tmp[0]);
    strcat(tmp, "\\*.data");

    hFile = FindFirstFile(tmp, &FindFileData);
    if (hFile == INVALID_HANDLE_VALUE) 
      return res;

    do
    {
        res.push_back(FindFileData.cFileName);
    } while(FindNextFile(hFile, &FindFileData));

    FindClose(hFile);

    return res;
}

//-----------------------------------------------------------------------------
std::string select_from_all_saved_files()
//-----------------------------------------------------------------------------
{
    std::string select_date = "";
    std::vector<std::string> res = getCurrentDirDataFiles();

    res.push_back("Back.");

    int current_position = 0;
    
    unsigned char input = 0;
    while(true)
    {
        system("cls");

        for(int i = 0; i < res.size(); ++i)
        {
            if(i == current_position)
                set_int_color(49);
            else
                set_int_color(31);
            cout << res[i] << endl;
            set_color(WHITE);
        }

        input = getch();

        switch(input)
        {
            case 80:
            {
                if((current_position + 1) >= res.size())
                    current_position = 0;
                else
                    ++current_position;
                break;
            }

            case 72:
            {
                if((current_position - 1) < 0)
                    current_position = res.size() - 1;
                else
                    --current_position;
                break;
            }

            case 13:
                return select_date = res[current_position];
        }
    }

    return select_date;
}

