/*******************************************************************************************************************//**
* \file Input.h
* \author Matthew LaDouceur
* \date 6-18-2020
* \brief Handle GLFW input
 **********************************************************************************************************************/

typedef void (*KeyFunction)();
typedef void (*MouseVec2DFunction)(double, double);
typedef int (*MouseButtonFunction)();
typedef void (*MouseButtonIntFunction)(int);


extern MouseVec2DFunction DragCamera;
extern MouseVec2DFunction ZoomCamera;
extern KeyFunction Key_S;
extern KeyFunction Key_G;
extern MouseButtonFunction GrabTileValue;
extern MouseButtonIntFunction PlaceTileValue;


void KeyboardInputCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseCallback(GLFWwindow* window, double xpos, double ypos);
void MouseScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

bool GetEscapeKeyState();
glm::vec2 GetLastMousePos();
