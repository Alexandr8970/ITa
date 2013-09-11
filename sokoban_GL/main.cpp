#include "common.h"
#include "Map.h"
#include "OpenGLInit.h"

std::string map_path = "levels.data";
Map map;

const int RESOLUTION_WIDTH  = 800;
const int RESOLUTION_HEIGHT = 600;

int DrawGLScene(GLvoid)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    map.draw();
    return 0;
}

int main()
{
    OpenGLInit glWindow;
    glWindow.setSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);

    MSG  msg;
    BOOL done = false;
    bool fullscreen = false;

    /*
    if(MessageBox(NULL, "Хотите ли Вы запустить приложение в полноэкранном режиме?",  "Запустить в полноэкранном режиме?", MB_YESNO | MB_ICONQUESTION) == IDNO)
        fullscreen = false;
    */

    if(!glWindow.CreateGLWindow(L"NeHe OpenGL окно", fullscreen))
        return 0;


    int current_level = 1;
    map.loadLevels(map_path);
    map.setViewSize(RESOLUTION_WIDTH, RESOLUTION_HEIGHT);
    map.setLevel(current_level);

    while(!done)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
                done = true;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if(OpenGLInit::isActive())
            {
                if(OpenGLInit::mKeys[VK_ESCAPE])
                {
                    done = true;
                }
                else
                {
                    DrawGLScene();
                    SwapBuffers(glWindow.getHDC());
                }
            }

            if(OpenGLInit::mKeys[VK_F1])
            {
                OpenGLInit::mKeys[VK_F1] = false;
                glWindow.KillGLWindow();
                fullscreen = !fullscreen;
                if(!glWindow.CreateGLWindow(L"NeHe OpenGL окно", fullscreen))
                    return 0;
            }
            else if(OpenGLInit::mKeys[VK_UP])
            {
                map.loaderMove(Map::MD_UP);
                OpenGLInit::mKeys[VK_UP] = false;
            }
            else if(OpenGLInit::mKeys[VK_RIGHT])
            {
                map.loaderMove(Map::MD_RIGHT);
                OpenGLInit::mKeys[VK_RIGHT] = false;
            }
            else if(OpenGLInit::mKeys[VK_LEFT])
            {
                map.loaderMove(Map::MD_LEFT);
                OpenGLInit::mKeys[VK_LEFT] = false;
            }
            else if(OpenGLInit::mKeys[VK_DOWN])
            {
                map.loaderMove(Map::MD_DOWN);
                OpenGLInit::mKeys[VK_DOWN] = false;
            }
            else if(OpenGLInit::mKeys[VK_ADD])
            {
                if((current_level + 1) <= map.getEndLevel())
                {
                    ++current_level;
                    map.setLevel(current_level);
                }
                OpenGLInit::mKeys[VK_ADD] = false;
            }
            else if(OpenGLInit::mKeys[VK_SUBTRACT])
            {
                if((current_level - 1) >= map.getStartLevel())
                {
                    --current_level;
                    map.setLevel(current_level);
                }
                OpenGLInit::mKeys[VK_SUBTRACT] = false;
            }
        }
    }
    glWindow.KillGLWindow();

    return 0;
}
