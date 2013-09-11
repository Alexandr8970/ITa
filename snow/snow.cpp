#include <windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <gl\glut.h>
#include <gl\glaux.h>

#include <stdio.h>
#include <string>
using namespace std;

#include "SnowObj.h"

#ifndef NULL
#define NULL 0
#endif

//-----------------------------------------------------------------------------
const size_t RESOLUTION_WIDTH  = 800;
const size_t RESOLUTION_HEIGHT = 600;
const size_t RESOLUTION_DEPTH  = 32;

HGLRC      hRC = NULL;
HDC        hDC = NULL;
HWND       hWnd = NULL;
HINSTANCE  hInstance;

bool  keys[256];
bool  active     = true;
bool  fullscreen = true;
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
unsigned long start_clock_count = 0;

unsigned long Get_Clock();
unsigned long Start_Clock();
unsigned long Wait_Clock(unsigned long count);

//-----------------------------------------------------------------------------
LRESULT  CALLBACK WndProc( HWND, UINT, WPARAM, LPARAM ); 

void draw_text(int x, int y, char* text);
//-----------------------------------------------------------------------------

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
    if(height == 0)
        height = 1;

    glViewport(0, 0, width, height);

    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 100.0f);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();
}

int InitGL(GLvoid) // Все установки касаемо OpenGL происходят здесь
{
    //glShadeModel(GL_SMOOTH);                            // Разрешить плавное цветовое сглаживание
    //glClearColor(0.0f, 0.0f, 0.0f, 0.0f);               // Очистка экрана в черный цвет
    //glClearDepth(1.0f);                                 // Разрешить очистку буфера глубины
    //glEnable(GL_DEPTH_TEST);                            // Разрешить тест глубины
    //glDepthFunc(GL_LEQUAL);                             // Тип теста глубины
    //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Улучшение в вычислении перспективы

    glDisable ( GL_DEPTH_TEST );
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, RESOLUTION_WIDTH, 0, RESOLUTION_HEIGHT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    srand(Start_Clock());
    Start_Clock();

    return true; // Инициализация прошла успешно
}

int DrawGLScene(GLvoid) // Здесь будет происходить вся прорисовка
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Очистить экран и буфер глубины
    glLoadIdentity();                                   // Сбросить текущую матрицу


    static SnowObj snows(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    snows.draw();

    // FPS
    static int lastTime = glutGet(GLUT_ELAPSED_TIME);
    static int frames = 0;
    static int fps = 0;

    int time = glutGet(GLUT_ELAPSED_TIME);

    if ((time - lastTime) > 1000.0)
    {
        fps = frames * 1000.0 / (time - lastTime);
        lastTime = time;
        frames = 0;
    }
    ++frames;

    //Wait_Clock(70);

    static char buffer[5] = {};
    itoa(fps, buffer, 10);
    draw_text(RESOLUTION_WIDTH - 85, RESOLUTION_HEIGHT - 25, buffer);
    // FPS

    Sleep(40);
    return true;
}

void draw_text(int x, int y, char* text)
{
    glColor3f(1.0f, 0.0f, 0.0f);
    glRasterPos2i(x, y);

    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'p');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 's');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '-');
    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ' ');

    for(int i = 0; i < strlen(text); ++i)
        glutBitmapCharacter(GLUT_BITMAP_9_BY_15, text[i]);
}

GLvoid KillGLWindow(GLvoid) // Корректное разрушение окна
{
    if(fullscreen) // Мы в полноэкранном режиме?
    {
        ChangeDisplaySettings( NULL, 0 );   // Если да, то переключаемся обратно в оконный режим
        ShowCursor( true );                 // Показать курсор мышки
    }

    if(hRC) // Существует ли Контекст Рендеринга?
    {
        if(!wglMakeCurrent(NULL, NULL)) // Возможно ли освободить RC и DC?
            MessageBox( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        if(!wglDeleteContext(hRC)) // Возможно ли удалить RC?
            MessageBox( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hRC = NULL;              // Установить RC в NULL
    }

    if(hDC && !ReleaseDC(hWnd, hDC)) // Возможно ли уничтожить DC?
    {
        MessageBox( NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hDC=NULL; // Установить DC в NULL
    }

    if(hWnd && !DestroyWindow(hWnd)) // Возможно ли уничтожить окно?
    {
        MessageBox( NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        hWnd = NULL; // Установить hWnd в NULL
    }

    if(!UnregisterClass("OpenGL", hInstance)) // Возможно ли разрегистрировать класс
    {
        MessageBox( NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        hInstance = NULL; // Установить hInstance в NULL
    }
}


BOOL CreateGLWindow( LPCWSTR title, int width, int height, int bits, bool fullscreenflag )
{
    GLuint      PixelFormat;    // Хранит результат после поиска
    WNDCLASS    wc;             // Структура класса окна
    DWORD       dwExStyle;      // Расширенный стиль окна
    DWORD       dwStyle;        // Обычный стиль окна
    RECT        WindowRect;     // Grabs Rectangle Upper Left / Lower Right Values

    WindowRect.left     = (long)0;      // Установить левую составляющую в 0
    WindowRect.right    = (long)width;  // Установить правую составляющую в Width
    WindowRect.top      = (long)0;      // Установить верхнюю составляющую в 0
    WindowRect.bottom   = (long)height; // Установить нижнюю составляющую в Height

    fullscreen = fullscreenflag; // Устанавливаем значение глобальной переменной fullscreen

    hInstance           = GetModuleHandle(NULL);        // Считаем дескриптор нашего приложения
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // Перерисуем при перемещении и создаём скрытый DC
    wc.lpfnWndProc      = (WNDPROC) WndProc;            // Процедура обработки сообщений
    wc.cbClsExtra       = 0;                            // Нет дополнительной информации для окна
    wc.cbWndExtra       = 0;                            // Нет дополнительной информации для окна
    wc.hInstance        = hInstance;                    // Устанавливаем дескриптор
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);  // Загружаем иконку по умолчанию
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);  // Загружаем указатель мышки
    wc.hbrBackground    = NULL;                         // Фон не требуется для GL
    wc.lpszMenuName     = NULL;                         // Меню в окне не будет
    wc.lpszClassName    = "OpenGL";                     // Устанавливаем имя классу

    if(!RegisterClass(&wc)) // Пытаемся зарегистрировать класс окна
    {
        MessageBox( NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false; // Выход и возвращение функцией значения false
    }

    if(fullscreen) // Полноэкранный режим?
    {
        DEVMODE dmScreenSettings; // Режим устройства
        memset( &dmScreenSettings, 0, sizeof( dmScreenSettings ) );    // Очистка для хранения установок

        dmScreenSettings.dmSize         = sizeof( dmScreenSettings );   // Размер структуры Devmode
        dmScreenSettings.dmPelsWidth    = width;                        // Ширина экрана
        dmScreenSettings.dmPelsHeight   = height;                       // Высота экрана
        dmScreenSettings.dmBitsPerPel   = bits;                         // Глубина цвета
        dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;// Режим Пикселя

        // Пытаемся установить выбранный режим и получить результат.  Примечание: CDS_FULLSCREEN убирает панель управления.
        if(ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
        {
            // Если переключение в полноэкранный режим невозможно, будет предложено два варианта: оконный режим или выход.
            if(MessageBox(NULL, 
                          "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                          "NeHe GL", 
                          MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                fullscreen = false;          // Выбор оконного режима (fullscreen = false)
            }
            else
            {
                // Выскакивающее окно, сообщающее пользователю о закрытие окна.
                MessageBox( NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP );
                return false; // Выход и возвращение функцией false
            }
        }
    }

    if(fullscreen) // Мы остались в полноэкранном режиме?
    {
        dwExStyle = WS_EX_APPWINDOW;    // Расширенный стиль окна
        dwStyle   = WS_POPUP;           // Обычный стиль окна
        ShowCursor(false);              // Скрыть указатель мышки
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE; // Расширенный стиль окна
        dwStyle   = WS_OVERLAPPEDWINDOW;                // Обычный стиль окна
    }
    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle ); // Подбирает окну подходящие размеры

    if(!(hWnd = CreateWindowEx( dwExStyle,                              // Расширенный стиль для окна
                                "OpenGL",                               // Имя класса
                                "OpenGL",                               // Заголовок окна
                                WS_CLIPSIBLINGS |                       // Требуемый стиль для окна
                                WS_CLIPCHILDREN |                       // Требуемый стиль для окна
                                dwStyle,                                // Выбираемые стили для окна
                                0, 0,                                   // Позиция окна
                                WindowRect.right - WindowRect.left,     // Вычисление подходящей ширины
                                WindowRect.bottom - WindowRect.top,     // Вычисление подходящей высоты
                                NULL,                                   // Нет родительского
                                NULL,                                   // Нет меню
                                hInstance,                              // Дескриптор приложения
                                NULL ) ) )                              // Не передаём ничего до WM_CREATE (???)
    {
        KillGLWindow(); // Восстановить экран
        MessageBox( NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    static  PIXELFORMATDESCRIPTOR pfd = // pfd сообщает Windows каким будет вывод на экран каждого пикселя
    {
        sizeof(PIXELFORMATDESCRIPTOR),  // Размер дескриптора данного формата пикселей
        1,                              // Номер версии
        PFD_DRAW_TO_WINDOW |            // Формат для Окна
        PFD_SUPPORT_OPENGL |            // Формат для OpenGL
        PFD_DOUBLEBUFFER,               // Формат для двойного буфера
        PFD_TYPE_RGBA,                  // Требуется RGBA формат
        bits,                           // Выбирается бит глубины цвета
        0, 0, 0, 0, 0, 0,               // Игнорирование цветовых битов
        0,                              // Нет буфера прозрачности
        0,                              // Сдвиговый бит игнорируется
        0,                              // Нет буфера накопления
        0, 0, 0, 0,                     // Биты накопления игнорируются
        RESOLUTION_DEPTH,               // 32 битный Z-буфер (буфер глубины)
        0,                              // Нет буфера трафарета
        0,                              // Нет вспомогательных буферов
        PFD_MAIN_PLANE,                 // Главный слой рисования
        0,                              // Зарезервировано
        0, 0, 0                         // Маски слоя игнорируются
    };

    if(!(hDC = GetDC(hWnd)))    // Можем ли мы получить Контекст Устройства?
    {
        KillGLWindow(); // Восстановить экран
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))   // Найден ли подходящий формат пикселя?
    {
        KillGLWindow();                // Восстановить экран
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!SetPixelFormat(hDC, PixelFormat, &pfd)) // Возможно ли установить Формат Пикселя?
    {
        KillGLWindow(); // Восстановить экран
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!(hRC = wglCreateContext(hDC))) // Возможно ли установить Контекст Рендеринга?
    {
        KillGLWindow(); // Восстановить экран
        MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!wglMakeCurrent(hDC, hRC)) // Попробовать активировать Контекст Рендеринга
    {
        KillGLWindow(); // Восстановить экран
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    ShowWindow(hWnd, SW_SHOW);    // Показать окно
    SetForegroundWindow(hWnd);    // Слегка повысим приоритет
    SetFocus(hWnd);               // Установить фокус клавиатуры на наше окно
    ReSizeGLScene(width, height); // Настроим перспективу для нашего OpenGL экрана.

    if(!InitGL()) // Инициализация только что созданного окна
    {
        KillGLWindow(); // Восстановить экран
        MessageBox( NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }
    return true;                  // Всё в порядке!
}

LRESULT CALLBACK WndProc(HWND   hWnd,   // Дескриптор нужного окна
                         UINT   uMsg,   // Сообщение для этого окна
                         WPARAM wParam, // Дополнительная информация
                         LPARAM lParam) // Дополнительная информация
{
    switch(uMsg) // Проверка сообщения для окна
    {
        case WM_ACTIVATE : // Проверка сообщения активности окна
        {
            if(!HIWORD(wParam)) // Проверить состояние минимизации
                active = true; // Программа активна
            else
                active = false; // Программа теперь не активна
        return 0; // Возвращаемся в цикл обработки сообщений
        }

        case WM_SYSCOMMAND : // Перехватываем системную команду
        {
            switch(wParam) // Останавливаем системный вызов
            {
                case SC_SCREENSAVE:   // Пытается ли запустится скринсейвер?
                case SC_MONITORPOWER: // Пытается ли монитор перейти в режим сбережения энергии?
                return 0;             // Предотвращаем это
            }
        break; // Выход
        }

        case WM_CLOSE : // Мы получили сообщение о закрытие?
        {
            PostQuitMessage(0); // Отправить сообщение о выходе
            return 0;           // Вернуться назад
        }

        case WM_KEYDOWN : // Была ли нажата кнопка?
        {
            keys[wParam] = true; // Если так, мы присваиваем этой ячейке true
            return 0;            // Возвращаемся
        }

        case WM_KEYUP : // Была ли отпущена клавиша?
        {
            keys[wParam] = false;   // Если так, мы присваиваем этой ячейке false
            return 0;               // Возвращаемся
        }

        case WM_SIZE : // Изменены размеры OpenGL окна
        {
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));  // Младшее слово=Width, старшее слово=Height
            return 0; // Возвращаемся
        }
    }
    // пересылаем все необработанные сообщения DefWindowProc
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}

//-----------------------------------------------------------------------------
unsigned long Get_Clock()
//-----------------------------------------------------------------------------
{
    // this function returns the current tick count
    // return time
    return(GetTickCount());
}

//-----------------------------------------------------------------------------
unsigned long Start_Clock(void)
//-----------------------------------------------------------------------------
{
    return(start_clock_count = Get_Clock());
}

//-----------------------------------------------------------------------------
unsigned long  Wait_Clock(unsigned long  count)
//-----------------------------------------------------------------------------
{
    while((Get_Clock() - start_clock_count) < count);
    return(Get_Clock());
}


int main()
{
    MSG  msg;           // Структура для хранения сообщения Windows
    BOOL  done = false; // Логическая переменная для выхода из цикла

    // Спрашивает пользователя, какой режим экрана он предпочитает
    if(MessageBox(NULL, "Хотите ли Вы запустить приложение в полноэкранном режиме?",  "Запустить в полноэкранном режиме?", MB_YESNO | MB_ICONQUESTION) == IDNO)
        fullscreen = false; // Оконный режим

    // Создать наше OpenGL окно
    if(!CreateGLWindow(L"NeHe OpenGL окно", RESOLUTION_WIDTH, RESOLUTION_HEIGHT, RESOLUTION_DEPTH, fullscreen))
    {
        return 0; // Выйти, если окно не может быть создано
    }

    while(!done) // Цикл продолжается, пока done не равно true
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Есть ли в очереди какое-нибудь сообщение?
        {
            if(msg.message == WM_QUIT)  // Мы поучили сообщение о выходе?
                done = true;            // Если так, done=true
            else                        // Если нет, обрабатывает сообщения
            {
                TranslateMessage(&msg); // Переводим сообщение
                DispatchMessage(&msg);  // Отсылаем сообщение
            }
        }
        else // Если нет сообщений
        {
            // Прорисовываем сцену.
            if(active)// Активна ли программа?
            {
                if(keys[VK_ESCAPE]) // Было ли нажата клавиша ESC?
                {
                    done = true;    // ESC говорит об останове выполнения программы
                }
                else    // Не время для выхода, обновим экран.
                {
                    DrawGLScene();      // Рисуем сцену
                    SwapBuffers( hDC ); // Меняем буфер (двойная буферизация)
                }
            }

            if(keys[VK_F1]) // Была ли нажата F1?
            {
                keys[VK_F1] = false;        // Если так, меняем значение ячейки массива на false
                KillGLWindow();             // Разрушаем текущее окно
                fullscreen = !fullscreen;   // Переключаем режим
                // Пересоздаём наше OpenGL окно
                if(!CreateGLWindow(L"NeHe OpenGL структура", RESOLUTION_WIDTH, RESOLUTION_HEIGHT, RESOLUTION_DEPTH, fullscreen))
                    return 0;        // Выходим, если это невозможно
            }
        }
    }        
    // Shutdown
    KillGLWindow(); // Разрушаем окно 

    return 0;
}
