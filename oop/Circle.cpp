#include "Circle.h"

//-----------------------------------------------------------------------------
Circle::Circle() :
     DrawObj(DrawObj::OBJ_TYPE_CIRCLE)
    ,mRadius(250)
    ,mSegments(2)
//-----------------------------------------------------------------------------
{
    mTypeObj = DrawObj::OBJ_TYPE_CIRCLE;
}

//-----------------------------------------------------------------------------
Circle::~Circle()
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
void Circle::draw()
//-----------------------------------------------------------------------------
{
        float theta = 2 * 3.1415926 / float(mSegments); 
        float c = cosf(theta);
        float s = sinf(theta);
        float t;

        float x = mRadius;
        float y = 0; 

        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glBegin(GL_LINE_LOOP); 
        for(int ii = 0; ii < mSegments; ii++) 
        { 
            glVertex2f(mX + x,mY + y);

            t = x;
            x = c * x - s * y;
            y = s * t + c * y;
        }
        glEnd(); 
}

//-----------------------------------------------------------------------------
void Circle::incSegments()
//-----------------------------------------------------------------------------
{
    if((mSegments + 1) < 360)
        ++mSegments;
}

//-----------------------------------------------------------------------------
void Circle::decSegments()
//-----------------------------------------------------------------------------
{
    if((mSegments - 1) > 1)
        --mSegments;
}


