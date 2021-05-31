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
