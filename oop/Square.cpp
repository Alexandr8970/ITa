#include "Square.h"

//-----------------------------------------------------------------------------
Square::Square() : DrawObj(DrawObj::OBJ_TYPE_SQUARE)
//-----------------------------------------------------------------------------
{
}

//-----------------------------------------------------------------------------
Square::~Square()
//-----------------------------------------------------------------------------
{
}

void Square::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_LINE_LOOP); 
    glVertex2f(mX - 200 , mY - 200);
    glVertex2f(mX - 200, mY + 200);
    glVertex2f(mX + 200, mY + 200);
    glVertex2f(mX + 200, mY - 200);
    glEnd(); 
}
