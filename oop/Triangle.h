#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include "drawobj.h"

class Triangle : public DrawObj
{
public:
    Triangle();
    ~Triangle();

    void draw();
};

#endif /* _TRIANGLE_H_ */
