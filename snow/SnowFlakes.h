#ifndef _SHOW_FLAKES_H_
#define _SHOW_FLAKES_H_

struct SnowFlakes
{
    SnowFlakes(void);

    void move_rd();
    void move_d();
    void move_ld();

    static void setWidthHeight(const int &aWidth, const int &aHeight);

    int         mX;
    int         mY;
    float       mPointSize;
    static int  mWidth;
    static int  mHeight;
};
#endif /* _SHOW_FLAKES_H_ */
