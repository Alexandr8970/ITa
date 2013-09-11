#ifndef _GAME_ENGINE_H_
#define _GAME_ENGINE_H_

#include "common.h"

#include "GameWorld.h"

namespace ns_tanks
{

enum eENGINE_STATE
{
     ES_INIT = 0
    ,ES_GAME
    ,ES_MENU
};

class GameEngine
{
public:
    GameEngine();
    ~GameEngine();

    void runWordl();

private:
    static void stKeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods);
    static void stErrorCallback(int aError, const char* aDescription);

private:
    bool _glInit();

    void _loadWorldSettings();
    void _mainWorldLoop();

    void _game_loop();
    void _menu_loop();

    void _drawTest();

private:
    GLFWwindow  *mpGLWindow;
    GLFWmonitor *mpMonitor;

    bool         mbIsFullScreen;
    int          miResolutionWidth;
    int          miResolutionHeight;

    eENGINE_STATE mEngineState;

    GameWorld mGW;
};

}
#endif /* _GAME_ENGINE_H_ */
