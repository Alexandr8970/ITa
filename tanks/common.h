#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdlib.h>
#include <stdio.h>

#define GLFW_INCLUDE_GLU
#include <GL\glfw3.h>
#include <GL\GLAux.h>

namespace ns_tanks
{

//-----------------------------------------------------------------------------
class sInputKeyState
{
public:
    sInputKeyState()
        : mIsActive(false)
         ,mKey(0)
    {
    }

    const bool &isInput() const
    {
        return mIsActive;
    }

    void set(const int &aKey)
    {
        mIsActive = true;
        mKey      = aKey;
    }

    const int &get() const
    {
        return mKey;
    }

    void clear()
    {
        mKey      = 0;
        mIsActive = false;
    }

private:
    bool mIsActive;
    int  mKey;
};
//-----------------------------------------------------------------------------


}


#endif /* _COMMON_H_ */
