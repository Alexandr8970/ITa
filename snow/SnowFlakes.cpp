#include "SnowFlakes.h"

int SnowFlakes::mWidth  = 0;
int SnowFlakes::mHeight = 0;

//-----------------------------------------------------------------------------
SnowFlakes::SnowFlakes() 
    : mX(0),
      mY(0),
      mPointSize(1.0f)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void SnowFlakes::setWidthHeight(const int &aWidth, const int &aHeight)
//-----------------------------------------------------------------------------
{
    mWidth  = aWidth;
    mHeight = aHeight;
}

//-----------------------------------------------------------------------------
void SnowFlakes::move_rd()
//-----------------------------------------------------------------------------
{
    mX = ((mX + 1) >= mWidth) ? 0 : mX + 1;
    mY = ((mY + 1) >= mHeight) ? 0 : mY + 1;
}

//-----------------------------------------------------------------------------
void SnowFlakes::move_d()
//-----------------------------------------------------------------------------
{
    mY = ((mY + 1) >= mHeight) ? 0 : mY + 1;
}

//-----------------------------------------------------------------------------
void SnowFlakes::move_ld()
//-----------------------------------------------------------------------------
{
    mX = ((mX - 1) < 0) ? mWidth - 1 : mX -  1;
    mY = ((mY + 1) >= mHeight) ? 0 : mY + 1;
}
