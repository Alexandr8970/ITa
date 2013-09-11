#ifndef _CIRCLE_H_
#define _CIRCLE_H_

#include "drawobj.h"

class Circle : public DrawObj
{
public:
    Circle();
    ~Circle();

    void draw();
    void incSegments();
    void decSegments();

private:
    int mRadius;
    int mSegments;
};

#endif /* _CIRCLE_H_ */
