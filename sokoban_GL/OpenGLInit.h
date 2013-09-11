#ifndef _OPEN_GL_INIT_H_
#define _OPEN_GL_INIT_H_

#include "common.h"

class OpenGLInit
{
public:
    OpenGLInit();
    ~OpenGLInit();

    void setSize(size_t aWidth, size_t aHeight);

    int InitGL(GLvoid);
    //int DrawGLScene(GLvoid);

    bool CreateGLWindow( LPCWSTR title, bool fullscreenflag);
    GLvoid KillGLWindow(GLvoid);

    static const bool &isActive();

    HDC &getHDC();

private:
    static GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
    static LRESULT CALLBACK WndProc1(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    static bool mKeys[256];

private:
     HGLRC      mhRC;
     HDC        mhDC;
     HWND       mhWnd;
     HINSTANCE  mhInstance;

     size_t     mWidth;
     size_t     mHeight;
     size_t     mDepth;
     bool       mIsFullScreen;

     static bool mIsActive;
};

#endif /* _OPEN_GL_INIT_H_ */
