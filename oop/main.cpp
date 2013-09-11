#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include <stdlib.h>
#include <stdio.h>

#include "DrawObj.h"
#include "Circle.h"
#include "Triangle.h"
#include "Square.h"

GLFWwindow  *mpGLWindow;
GLFWmonitor *mpMonitor;

const int miResolutionWidth  = 800;
const int miResolutionHeight = 600;
const bool mbIsFullScreen = false;

DrawObj  *obj      = 0;
Circle   *circle   = 0;
Triangle *triangle = 0;
Square   *square   = 0;

void stKeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods);

//-----------------------------------------------------------------------------
void stErrorCallback(int aError, const char* aDescription)
//-----------------------------------------------------------------------------
{
    fputs(aDescription, stderr);
}

bool initGL()
{
    glfwSetErrorCallback(stErrorCallback);

    if (!glfwInit())
        return false;

    if(mbIsFullScreen)
        mpMonitor = glfwGetPrimaryMonitor();

    mpGLWindow = glfwCreateWindow(miResolutionWidth, miResolutionHeight, "OOP", mpMonitor, NULL);
    if (!mpGLWindow)
    {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mpGLWindow);
    glfwSetKeyCallback(mpGLWindow, stKeyCallback);

    glViewport(0, 0, (GLsizei)miResolutionWidth, (GLsizei)miResolutionHeight);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, miResolutionWidth, miResolutionHeight, 0, 0, 1);
    glfwSwapInterval(1);
    glEnable(GL_SMOOTH);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_TEXTURE_2D);

    return true;
}

//-----------------------------------------------------------------------------
void stKeyCallback(GLFWwindow* aWindow, int aKey, int aScancode, int aAction, int aMods)
//-----------------------------------------------------------------------------
{
    if(aAction == GLFW_PRESS)
    {
        switch(aKey)
        {
            case GLFW_KEY_ESCAPE:
            {
                glfwSetWindowShouldClose(aWindow, GL_TRUE);
                break;
            }

            case GLFW_KEY_C:
            {
                if(circle == 0)
                    circle = new Circle();

                circle->setPosition(obj->getX(), obj->getY());
                obj = circle;
                break;
            }

            case GLFW_KEY_T:
            {
                if(triangle == 0)
                    triangle = new Triangle();

                triangle->setPosition(obj->getX(), obj->getY());
                obj = triangle;
                break;
            }

            case GLFW_KEY_S:
            {
                if(square == 0)
                    square = new Square();

                square->setPosition(obj->getX(), obj->getY());
                obj = square;
                break;
            }

            case GLFW_KEY_KP_ADD:
            {
                if(obj->getTypeObj() == DrawObj::OBJ_TYPE_CIRCLE)
                    circle->incSegments();
                break;
            }

            case GLFW_KEY_KP_SUBTRACT:
            {
                if(obj->getTypeObj() == DrawObj::OBJ_TYPE_CIRCLE)
                    circle->decSegments();
                break;
            }

            case GLFW_KEY_RIGHT:
            {
                obj->incX();
                break;
            }

            case GLFW_KEY_LEFT:
            {
                obj->decX();
                break;
            }

            case GLFW_KEY_DOWN:
            {
                obj->incY();
                break;
            }

            case GLFW_KEY_UP:
            {
                obj->decY();
                break;
            }
        }
    }
}

int main()
{
    if(!initGL())
        return 0;

    obj = new DrawObj();

    while (!glfwWindowShouldClose(mpGLWindow))
    {
        if(obj)
            obj->draw();

        glfwSwapBuffers(mpGLWindow);
        glfwPollEvents();
    }

    return 1;
}
