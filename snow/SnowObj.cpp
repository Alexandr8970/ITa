#include <time.h>
#include "SnowObj.h"

static int _count    = 0;
static int _i        = 0;
static int _end      = 0;
static int _left_x   = 0;
static int _center_x = 0;
static int _right_x  = 0;
static int _down_y   = 0;
static int _rand     = 0;

static bool _isGenRand = true;

unsigned int _res = 0;

static eDIRECTION direction = NOT_MOVE;

//-----------------------------------------------------------------------------
SnowObj::SnowObj(const size_t &aWidth, const size_t &aHeight) 
    : mWidth(aWidth), 
      mHeight(aHeight)
//-----------------------------------------------------------------------------
{
    srand(time(0));

    mMapSnowFlakes.reserve(mWidth * mHeight);

    mStaticSnows.resize(mHeight + 1);
    for(int i = 0; i != mHeight + 1; ++i)
        mStaticSnows[i].resize(mWidth + 1);

    for(int i = 0; i != mWidth; ++i)
        mStaticSnows[mHeight][i] = 1;

    SnowFlakes::setWidthHeight(aWidth, aHeight);
}

//-----------------------------------------------------------------------------
void SnowObj::draw()
//-----------------------------------------------------------------------------
{
    if(_isGenRand)
    {
        _count = rand() % 15 + 1;
        for(_i = 0; _i != _count; ++_i)
        {
            SnowFlakes flake;
            flake.mX = rand() % mWidth + 1;
            flake.mY = 0;

            flake.mPointSize = 1.0f + float((rand() % 8) + 1)/ 10.0f;
            mMapSnowFlakes.push_back(flake);
        }
    }

    glColor3f(1.0f, 1.0f, 1.0f);

    _end = mMapSnowFlakes.size();
    // {
        for(_i = 0; _i != _end; ++_i)
        {
            moving(mMapSnowFlakes[_i]);
            glPointSize(mMapSnowFlakes[_i].mPointSize);
            glBegin(GL_POINTS);
                glVertex2i(mMapSnowFlakes[_i].mX, mHeight - mMapSnowFlakes[_i].mY);
            glEnd();
        }

        _i = 0;
        _end = msnows.size();

        glPointSize(1.0f);
        glBegin(GL_POINTS);
            for(_i = 0; _i != _end; ++_i)
                glVertex2i(msnows[_i].mX, mHeight - msnows[_i].mY - 1);
        glEnd();
    // }
}

//-----------------------------------------------------------------------------
void SnowObj::moving(SnowFlakes &aSnow)
//-----------------------------------------------------------------------------
{
    if(_isGenRand)
        if((aSnow.mY + 1) >= mHeight - 1)
            _isGenRand = false;

    direction = get_way(aSnow.mX, aSnow.mY);

    if(direction == RightDownLetf_MOVE)
        _rand = (rand() % 3);
    else
        _rand = (rand() % 2);

    switch(direction)
    {
        case RightDownLetf_MOVE :
            (_rand == 0) ? aSnow.move_rd() : (_rand == 1) ? aSnow.move_d() : aSnow.move_ld();
            return;

        case LeftDown_MOVE :
            (_rand == 0) ? aSnow.move_ld() : aSnow.move_d();
            return;

        case LeftRight_MOVE :
            (_rand == 0) ? aSnow.move_ld() : aSnow.move_rd();
            return;

        case Left_MOVE :
            aSnow.move_ld();
            return;

        case RightDown_MOVE :
            (_rand == 0) ? aSnow.move_rd() : aSnow.move_d();
            return;

        case Down_MOVE :
            aSnow.move_d();
            return;

        case Right_MOVE :
            aSnow.move_rd();
            return;

        case NOT_MOVE :
            if(mStaticSnows[aSnow.mY][aSnow.mX] == 0)
            {
                mStaticSnows[aSnow.mY][aSnow.mX] = 1;
                msnows.push_back(aSnow);
                aSnow.mX = rand() % mWidth + 1;
                aSnow.mY = 0;
                aSnow.mPointSize = 1.0f + float((rand() % 8) + 1)/ 10.0f;
            }
            return;
    }
}

//-----------------------------------------------------------------------------
eDIRECTION SnowObj::get_way(const int &x, const int &y)
//-----------------------------------------------------------------------------
{
    _res = 0;

    _left_x   = ((x - 1) < 0) ? mWidth - 1 : x - 1;
    _center_x = x;
    _right_x  = ((x + 1) >= mWidth) ? 0 : x + 1;;
    _down_y   = ((y + 1) > mHeight) ? 0 : y + 1;

    _res |= mStaticSnows[_down_y][_left_x]   << 2;
    _res |= mStaticSnows[_down_y][_center_x] << 1;
    _res |= mStaticSnows[_down_y][_right_x];

    return (eDIRECTION)_res;
}

