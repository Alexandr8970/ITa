#ifndef _DRAW_OBJ_H_
#define _DRAW_OBJ_H_

#define GLFW_INCLUDE_GLU
#include <GL\glfw3.h>
#include <GL\GLAux.h>


#include <math.h>
#include <time.h>

class DrawObj
{
public:
    enum eTypeObj
    {
         OBJ_TYPE_UNKNOWN = 0
        ,OBJ_TYPE_CIRCLE
        ,OBJ_TYPE_TRIANGLE
        ,OBJ_TYPE_SQUARE
    };

public:
    DrawObj(eTypeObj aType = OBJ_TYPE_UNKNOWN);
    virtual ~DrawObj();

    void setPosition(int aX, int aY);

    const int &getX() const;
    const int &getY() const;

    const eTypeObj &getTypeObj()const;

    void incX();
    void incY();
    void decX();
    void decY();

    virtual void draw();

protected:
    int         mX;
    int         mY;
    eTypeObj    mTypeObj;
};

#endif /* _DRAW_OBJ_H_ */
