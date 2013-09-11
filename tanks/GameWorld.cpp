#include "GameWorld.h"

namespace ns_tanks
{

static char *texture_box_path = "textures/box.bmp";

sInputKeyState GameWorld::mstKeyState;

//-----------------------------------------------------------------------------
void GameWorld::stKeyCallback(GLFWwindow *aWindow,
                              int         aKey,
                              int         aScancode,
                              int         aAction,
                              int         aMods)
//-----------------------------------------------------------------------------
{
    if(aAction == GLFW_PRESS)
        mstKeyState.set(aKey);
}

//-----------------------------------------------------------------------------
GameWorld::GameWorld()
    : mGameState(GS_NO_INIT)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
GameWorld::~GameWorld()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void GameWorld::init()
//-----------------------------------------------------------------------------
{
    _test_load();
}

//-----------------------------------------------------------------------------
void GameWorld::game_process()
//-----------------------------------------------------------------------------
{
    _input_processing();

    if(mGameState == GS_RUN)
    {
        // ii

        // input

        _draw();
    }
}

//-----------------------------------------------------------------------------
void GameWorld::_draw()
//-----------------------------------------------------------------------------
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBindTexture(GL_TEXTURE_2D, texture_box);
    double g_x = 0.0;
    double g_y = 0.0;
    double mMapObjectSize = 150;

    glBegin(GL_QUADS);
        glTexCoord2d(0, 1); 
        glVertex2d(g_x, g_y + mMapObjectSize); 
        
        glTexCoord2d(1, 1); 
        glVertex2d(g_x + mMapObjectSize, g_y + mMapObjectSize); 
        
        glTexCoord2d(1, 0); 
        glVertex2d(g_x + mMapObjectSize, g_y);
        
        glTexCoord2d(0, 0); 
        glVertex2d(g_x, g_y);
    glEnd();
}

//-----------------------------------------------------------------------------
void GameWorld::_test_load()
//-----------------------------------------------------------------------------
{
    AUX_RGBImageRec *texture;
    texture = auxDIBImageLoad(texture_box_path);

    glGenTextures(1, &texture_box);
    glBindTexture(GL_TEXTURE_2D, texture_box);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, texture->sizeX, texture->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);
}

//-----------------------------------------------------------------------------
void GameWorld::_input_processing()
//-----------------------------------------------------------------------------
{
    if(mstKeyState.isInput())
    {
        mKeyCodeInput = mstKeyState.get();
        mstKeyState.clear();

        if(mKeyCodeInput == GLFW_KEY_ESCAPE)
            mGameState = GS_PAUSE;
    }
}

//-----------------------------------------------------------------------------
void GameWorld::setGameState(eGAME_STATE aState)
//-----------------------------------------------------------------------------
{
    mGameState = aState;
}

//-----------------------------------------------------------------------------
const eGAME_STATE &GameWorld::getGameState() const
//-----------------------------------------------------------------------------
{
    return mGameState;
}

}
