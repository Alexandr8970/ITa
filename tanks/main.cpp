#pragma comment(lib, "glfw3.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glaux.lib")

#include "GameEngine.h"

int main(void)
{
    ns_tanks::GameEngine().runWordl();
    exit(EXIT_SUCCESS);
}
