#include "GameEngine.h"

namespace ns_tanks
{

//-----------------------------------------------------------------------------
void GameEngine::stKeyCallback(GLFWwindow* aWindow, 
                              int         aKey, 
                              int         aScancode, 
                              int         aAction, 
                              int         aMods)
//-----------------------------------------------------------------------------
{
    if(aAction == GLFW_PRESS)
    {
        if (aKey == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(aWindow, GL_TRUE);
    }
}

//-----------------------------------------------------------------------------
void GameEngine::stErrorCallback(int aError, const char* aDescription)
//-----------------------------------------------------------------------------
{
    fputs(aDescription, stderr);
}


//-----------------------------------------------------------------------------
GameEngine::GameEngine() 
    : mpGLWindow(0)
     ,mpMonitor(0)
     ,mbIsFullScreen(true)
     ,miResolutionWidth(800)
     ,miResolutionHeight(600)
     ,mEngineState(ES_INIT)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
GameEngine::~GameEngine()
//-----------------------------------------------------------------------------
{
    if(mpGLWindow)
        glfwDestroyWindow(mpGLWindow);

    mpGLWindow = 0;
    glfwTerminate();
}

//-----------------------------------------------------------------------------
void GameEngine::runWordl()
//-----------------------------------------------------------------------------
{
    _loadWorldSettings();

    if(!_glInit())
        return;

    _mainWorldLoop();
}

//-----------------------------------------------------------------------------
void GameEngine::_loadWorldSettings()
//-----------------------------------------------------------------------------
{
    mbIsFullScreen  = false;

    mEngineState    = ES_GAME;
    mGW.setGameState(GS_INIT);
}


//-----------------------------------------------------------------------------
bool GameEngine::_glInit()
//-----------------------------------------------------------------------------
{
    glfwSetErrorCallback(stErrorCallback);

    if (!glfwInit())
        return false;

    if(mbIsFullScreen)
        mpMonitor = glfwGetPrimaryMonitor();

    mpGLWindow = glfwCreateWindow(miResolutionWidth, miResolutionHeight, "Tanks", mpMonitor, NULL);
    if (!mpGLWindow)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mpGLWindow);
    glfwSetKeyCallback(mpGLWindow, stKeyCallback);

    glViewport(0, 0, (GLsizei)miResolutionWidth, (GLsizei)miResolutionHeight);

    // Change to the projection matrix, reset the matrix and set up orthagonal projection (i.e. 2D)
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, miResolutionWidth, miResolutionHeight, 0, 0, 1); // Paramters: left, right, bottom, top, near, far
    // ----- OpenGL settings -----
    glfwSwapInterval(1);        // Lock to vertical sync of monitor (normally 60Hz, so 60fps)
    glEnable(GL_SMOOTH);        // Enable (gouraud) shading
    glDisable(GL_DEPTH_TEST);   // Disable depth testing
    glEnable(GL_BLEND);         // Enable blending (used for alpha) and blending function to use
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);   // Enable anti-aliasing on lines
    glEnable(GL_POINT_SMOOTH);  // Enable anti-aliasing on points
    glEnable(GL_TEXTURE_2D);

    return true;
}

//-----------------------------------------------------------------------------
void GameEngine::_mainWorldLoop()
//-----------------------------------------------------------------------------
{
    while (!glfwWindowShouldClose(mpGLWindow))
    {
        switch(mEngineState)
        {
            case ES_GAME:
            {
                _game_loop();
                break;
            }

            case ES_MENU:
            {
                _menu_loop();
                break;
            }
        }

        glfwSwapBuffers(mpGLWindow);
        glfwPollEvents();
    }
}

//-----------------------------------------------------------------------------
void GameEngine::_game_loop()
//-----------------------------------------------------------------------------
{
    switch(mGW.getGameState())
    {
        case GS_INIT:
        {
            mGW.init();
            mGW.setGameState(GS_RUN);
            glfwSetKeyCallback(mpGLWindow, GameWorld::stKeyCallback);
            break;
        }

        case GS_RUN:
        {
            mGW.game_process();
            break;
        }

        case GS_PAUSE:
        {
            glfwSetKeyCallback(mpGLWindow, stKeyCallback);
            mEngineState = ES_MENU;
            break;
        }

        case GS_WIN:
        {
            break;
        }

        case GS_NEXT_LEVEL:
        {
            break;
        }
    }
}

//-----------------------------------------------------------------------------
void GameEngine::_menu_loop()
//-----------------------------------------------------------------------------
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
}

//-----------------------------------------------------------------------------
void GameEngine::_drawTest()
//-----------------------------------------------------------------------------
{
        float ratio;
        int   width, height;

        glfwGetFramebufferSize(mpGLWindow, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

        glBegin(GL_TRIANGLES);
            glColor3f(1.f, 0.f, 0.f);
            glVertex3f(-0.6f, -0.4f, 0.f);
            glColor3f(0.f, 1.f, 0.f);
            glVertex3f(0.6f, -0.4f, 0.f);
            glColor3f(0.f, 0.f, 1.f);
            glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
}

}
