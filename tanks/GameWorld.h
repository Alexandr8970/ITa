#ifndef _GAME_WORLD_H_
#define _GAME_WORLD_H_

#include "common.h"

namespace ns_tanks
{

enum eGAME_STATE
{
     GS_NO_INIT = 0
    ,GS_INIT
    ,GS_RUN
    ,GS_PAUSE
    ,GS_WIN
    ,GS_NEXT_LEVEL
};

class GameWorld
{
public:
    static void stKeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods);

private:
    static sInputKeyState mstKeyState;

public:
    GameWorld();
    ~GameWorld();

    void init();
    void game_process();

    const eGAME_STATE &getGameState() const;
    void  setGameState(eGAME_STATE aState);

private:
    void _input_processing();

    void _draw();
    void _test_load();
    GLuint texture_box;


private:
    eGAME_STATE mGameState;
    int         mKeyCodeInput;
};

}

#endif /* _GAME_WORLD_H_ */
