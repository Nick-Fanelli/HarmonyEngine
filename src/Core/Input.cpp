#include "Input.h"

using namespace HarmonyEngine;

bool Input::s_Keys[NUM_KEYS] = { false };
bool Input::s_KeysLast[NUM_KEYS] = { false };

bool Input::s_MouseButtons[NUM_MOUSE_BUTTONS] = { false };
bool Input::s_MouseButtonsLast[NUM_MOUSE_BUTTONS] = { false };

glm::vec2 Input::s_MousePosition = glm::vec2();
glm::vec2 Input::s_MousePositionLast = glm::vec2();

glm::vec2 Input::s_AbsScrollPosition = glm::vec2();
glm::vec2 Input::s_ScrollPosition = glm::vec2();

void Input::Update() {
    memcpy(&s_KeysLast, &s_Keys, sizeof(s_KeysLast));
    memcpy(&s_MouseButtonsLast, &s_MouseButtons, sizeof(s_MouseButtonsLast));

    memcpy(&s_MousePositionLast, &s_MousePosition, sizeof(s_MousePositionLast));

    constexpr glm::vec2 zero = { 0, 0 };
    s_ScrollPosition = zero;
}

void Input::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if(key <= NUM_KEYS)
        s_Keys[key] = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
}

void Input::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if(button <= NUM_MOUSE_BUTTONS)
        s_MouseButtons[button] = action == GLFW_PRESS;
}

void Input::MousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
    s_MousePosition = { xPos, yPos };
}

void Input::MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    s_ScrollPosition = { xOffset, yOffset };
    s_AbsScrollPosition += glm::vec2(xOffset, yOffset);
}