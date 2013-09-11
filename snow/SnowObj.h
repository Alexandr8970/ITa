#ifndef _SHOW_OBJECT_H_
#define _SHOW_OBJECT_H_

#include <windows.h>
#include <gl\gl.h>

#include <vector>
#include <map>
using namespace std;

#include "SnowFlakes.h"

enum eDIRECTION
{
    RightDownLetf_MOVE  = 0,
    LeftDown_MOVE       = 1,
    LeftRight_MOVE      = 2,
    Left_MOVE           = 3,
    RightDown_MOVE      = 4,
    Down_MOVE           = 5,
    Right_MOVE          = 6,
    NOT_MOVE            = 7
};

class SnowObj
{
public:
    SnowObj(const size_t &aWidth, const size_t &aHeight);

    void draw();

private:
    void moving(SnowFlakes &aSnow);

    eDIRECTION get_way(const int &x, const int &y);

private:
    int mWidth;
    int mHeight;

    std::vector<SnowFlakes>         mMapSnowFlakes;
    std::vector< std::vector<int> > mStaticSnows;
    std::vector<SnowFlakes>         msnows;
};

#endif /*_SHOW_OBJECT_H_*/
