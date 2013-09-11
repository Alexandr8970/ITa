#include "OpenGLInit.h"


bool OpenGLInit::mIsActive = true;
bool OpenGLInit::mKeys[256] = {0};

//-----------------------------------------------------------------------------
OpenGLInit::OpenGLInit() 
    : mhRC(0)
     ,mhDC(0)
     ,mhWnd(0)
     ,mWidth(800)
     ,mHeight(600)
     ,mDepth(32)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
OpenGLInit::~OpenGLInit()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void OpenGLInit::setSize(size_t aWidth, size_t aHeight)
//-----------------------------------------------------------------------------
{
    mWidth  = aWidth;
    mHeight = aHeight;
}

//-----------------------------------------------------------------------------
LRESULT CALLBACK OpenGLInit::WndProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//-----------------------------------------------------------------------------
{
    switch(uMsg)
    {
        case WM_ACTIVATE :
        {
            if(!HIWORD(wParam))
                mIsActive = true;
            else
                mIsActive = false;
            return 0;
        }

        case WM_SYSCOMMAND :
        {
            switch(wParam)
            {
                case SC_SCREENSAVE:
                case SC_MONITORPOWER:
                return 0;
            }
        break;
        }

        case WM_CLOSE :
        {
            PostQuitMessage(0);
            return 0;
        }

        case WM_KEYDOWN :
        {
            mKeys[wParam] = true;
            return 0;
        }

        case WM_KEYUP :
        {
            mKeys[wParam] = false;
            return 0;
        }

        case WM_SIZE :
        {
            ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
            return 0;
        }
    }
    return DefWindowProc( hWnd, uMsg, wParam, lParam );
}


//-----------------------------------------------------------------------------
bool OpenGLInit::CreateGLWindow(LPCWSTR title, bool fullscreenflag)
//-----------------------------------------------------------------------------
{
    GLuint      PixelFormat;
    WNDCLASS    wc;
    DWORD       dwExStyle;
    DWORD       dwStyle;
    RECT        WindowRect;

    WindowRect.left     = (long)0;
    WindowRect.right    = (long)mWidth;
    WindowRect.top      = (long)0;
    WindowRect.bottom   = (long)mHeight;

    mIsFullScreen = fullscreenflag;

    mhInstance           = GetModuleHandle(NULL);
    wc.style            = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc      = (WNDPROC) WndProc1;
    wc.cbClsExtra       = 0;
    wc.cbWndExtra       = 0;
    wc.hInstance        = mhInstance;
    wc.hIcon            = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor          = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground    = NULL;
    wc.lpszMenuName     = NULL;
    wc.lpszClassName    = "OpenGL";

    if(!RegisterClass(&wc))
    {
        MessageBox( NULL, "Failed To Register The Window Class.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    if(mIsFullScreen)
    {
        DEVMODE dmScreenSettings;
        memset( &dmScreenSettings, 0, sizeof(dmScreenSettings));

        dmScreenSettings.dmSize         = sizeof( dmScreenSettings );
        dmScreenSettings.dmPelsWidth    = mWidth;
        dmScreenSettings.dmPelsHeight   = mHeight;
        dmScreenSettings.dmBitsPerPel   = mDepth;
        dmScreenSettings.dmFields       = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;

        if(ChangeDisplaySettings( &dmScreenSettings, CDS_FULLSCREEN ) != DISP_CHANGE_SUCCESSFUL)
        {
            if(MessageBox(NULL, 
                          "The Requested Fullscreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?",
                          "NeHe GL", 
                          MB_YESNO | MB_ICONEXCLAMATION) == IDYES)
            {
                mIsFullScreen = false;
            }
            else
            {
                MessageBox( NULL, "Program Will Now Close.", "ERROR", MB_OK | MB_ICONSTOP );
                return false;
            }
        }
    }

    if(mIsFullScreen)
    {
        dwExStyle = WS_EX_APPWINDOW;
        dwStyle   = WS_POPUP;
        ShowCursor(false);
    }
    else
    {
        dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
        dwStyle   = WS_OVERLAPPEDWINDOW;
    }
    AdjustWindowRectEx( &WindowRect, dwStyle, false, dwExStyle );

    if(!(mhWnd = CreateWindowEx( dwExStyle, "OpenGL", "OpenGL",
                                WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
                                0, 0,
                                WindowRect.right - WindowRect.left,
                                WindowRect.bottom - WindowRect.top,
                                NULL, NULL,
                                mhInstance,
                                NULL)))
    {
        KillGLWindow();
        MessageBox( NULL, "Window Creation Error.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }

    static  PIXELFORMATDESCRIPTOR pfd =
    {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        mDepth,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        mDepth,
        0, 0,
        PFD_MAIN_PLANE,
        0, 0, 0, 0
    };

    if(!(mhDC = GetDC(mhWnd)))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Create A GL Device Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!(PixelFormat = ChoosePixelFormat(mhDC, &pfd)))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Find A Suitable PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!SetPixelFormat(mhDC, PixelFormat, &pfd))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Set The PixelFormat.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!(mhRC = wglCreateContext(mhDC)))
    {
        KillGLWindow();
        MessageBox( NULL, "Can't Create A GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    if(!wglMakeCurrent(mhDC, mhRC))
    {
        KillGLWindow();
        MessageBox(NULL, "Can't Activate The GL Rendering Context.", "ERROR", MB_OK | MB_ICONEXCLAMATION);
        return false;
    }

    ShowWindow(mhWnd, SW_SHOW);
    SetForegroundWindow(mhWnd);
    SetFocus(mhWnd);
    ReSizeGLScene(mWidth, mHeight);

    if(!InitGL())
    {
        KillGLWindow();
        MessageBox( NULL, "Initialization Failed.", "ERROR", MB_OK | MB_ICONEXCLAMATION );
        return false;
    }
    return true;
}

const bool &OpenGLInit::isActive()
{
    return mIsActive;
}


HDC &OpenGLInit::getHDC()
{
    return mhDC;
}

//-----------------------------------------------------------------------------
GLvoid OpenGLInit::KillGLWindow(GLvoid)
//-----------------------------------------------------------------------------
{
    if(mIsFullScreen)
    {
        ChangeDisplaySettings(NULL, 0);
        ShowCursor(true);
    }

    if(mhRC)
    {
        if(!wglMakeCurrent(NULL, NULL))
            MessageBox( NULL, "Release Of DC And RC Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );

        if(!wglDeleteContext(mhRC))
            MessageBox( NULL, "Release Rendering Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        mhRC = NULL;
    }

    if(mhDC && !ReleaseDC(mhWnd, mhDC))
    {
        MessageBox( NULL, "Release Device Context Failed.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        mhDC=NULL;
    }

    if(mhWnd && !DestroyWindow(mhWnd))
    {
        MessageBox( NULL, "Could Not Release hWnd.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION );
        mhWnd = NULL;
    }

    if(!UnregisterClass("OpenGL", mhInstance))
    {
        MessageBox( NULL, "Could Not Unregister Class.", "SHUTDOWN ERROR", MB_OK | MB_ICONINFORMATION);
        mhInstance = NULL;
    }
}

//-----------------------------------------------------------------------------
int OpenGLInit::InitGL(GLvoid)
//-----------------------------------------------------------------------------
{
    glDisable ( GL_DEPTH_TEST );
    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, mWidth, 0, mHeight);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    glEnable(GL_TEXTURE_2D);
    return true; // Инициализация прошла успешно
}

//-----------------------------------------------------------------------------
GLvoid OpenGLInit::ReSizeGLScene(GLsizei width, GLsizei height)
//-----------------------------------------------------------------------------
{
    if(height == 0)
        height = 1;

    glViewport(0, 0, width, height);
}
