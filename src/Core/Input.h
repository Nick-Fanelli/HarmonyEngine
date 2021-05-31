#pragma once

#include "harmonypch.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "Display.h"

// Taken from glfw.h
#define HARMONY_KEY_SPACE              32
#define HARMONY_KEY_APOSTROPHE         39  /* ' */
#define HARMONY_KEY_COMMA              44  /* , */
#define HARMONY_KEY_MINUS              45  /* - */
#define HARMONY_KEY_PERIOD             46  /* . */
#define HARMONY_KEY_SLASH              47  /* / */
#define HARMONY_KEY_0                  48
#define HARMONY_KEY_1                  49
#define HARMONY_KEY_2                  50
#define HARMONY_KEY_3                  51
#define HARMONY_KEY_4                  52
#define HARMONY_KEY_5                  53
#define HARMONY_KEY_6                  54
#define HARMONY_KEY_7                  55
#define HARMONY_KEY_8                  56
#define HARMONY_KEY_9                  57
#define HARMONY_KEY_SEMICOLON          59  /* ; */
#define HARMONY_KEY_EQUAL              61  /* = */
#define HARMONY_KEY_A                  65
#define HARMONY_KEY_B                  66
#define HARMONY_KEY_C                  67
#define HARMONY_KEY_D                  68
#define HARMONY_KEY_E                  69
#define HARMONY_KEY_F                  70
#define HARMONY_KEY_G                  71
#define HARMONY_KEY_H                  72
#define HARMONY_KEY_I                  73
#define HARMONY_KEY_J                  74
#define HARMONY_KEY_K                  75
#define HARMONY_KEY_L                  76
#define HARMONY_KEY_M                  77
#define HARMONY_KEY_N                  78
#define HARMONY_KEY_O                  79
#define HARMONY_KEY_P                  80
#define HARMONY_KEY_Q                  81
#define HARMONY_KEY_R                  82
#define HARMONY_KEY_S                  83
#define HARMONY_KEY_T                  84
#define HARMONY_KEY_U                  85
#define HARMONY_KEY_V                  86
#define HARMONY_KEY_W                  87
#define HARMONY_KEY_X                  88
#define HARMONY_KEY_Y                  89
#define HARMONY_KEY_Z                  90
#define HARMONY_KEY_LEFT_BRACKET       91  /* [ */
#define HARMONY_KEY_BACKSLASH          92  /* \ */
#define HARMONY_KEY_RIGHT_BRACKET      93  /* ] */
#define HARMONY_KEY_GRAVE_ACCENT       96  /* ` */
#define HARMONY_KEY_WORLD_1            161 /* non-US #1 */
#define HARMONY_KEY_WORLD_2            162 /* non-US #2 */
#define HARMONY_KEY_ESCAPE             256
#define HARMONY_KEY_ENTER              257
#define HARMONY_KEY_TAB                258
#define HARMONY_KEY_BACKSPACE          259
#define HARMONY_KEY_INSERT             260
#define HARMONY_KEY_DELETE             261
#define HARMONY_KEY_RIGHT              262
#define HARMONY_KEY_LEFT               263
#define HARMONY_KEY_DOWN               264
#define HARMONY_KEY_UP                 265
#define HARMONY_KEY_PAGE_UP            266
#define HARMONY_KEY_PAGE_DOWN          267
#define HARMONY_KEY_HOME               268
#define HARMONY_KEY_END                269
#define HARMONY_KEY_CAPS_LOCK          280
#define HARMONY_KEY_SCROLL_LOCK        281
#define HARMONY_KEY_NUM_LOCK           282
#define HARMONY_KEY_PRINT_SCREEN       283
#define HARMONY_KEY_PAUSE              284
#define HARMONY_KEY_F1                 290
#define HARMONY_KEY_F2                 291
#define HARMONY_KEY_F3                 292
#define HARMONY_KEY_F4                 293
#define HARMONY_KEY_F5                 294
#define HARMONY_KEY_F6                 295
#define HARMONY_KEY_F7                 296
#define HARMONY_KEY_F8                 297
#define HARMONY_KEY_F9                 298
#define HARMONY_KEY_F10                299
#define HARMONY_KEY_F11                300
#define HARMONY_KEY_F12                301
#define HARMONY_KEY_F13                302
#define HARMONY_KEY_F14                303
#define HARMONY_KEY_F15                304
#define HARMONY_KEY_F16                305
#define HARMONY_KEY_F17                306
#define HARMONY_KEY_F18                307
#define HARMONY_KEY_F19                308
#define HARMONY_KEY_F20                309
#define HARMONY_KEY_F21                310
#define HARMONY_KEY_F22                311
#define HARMONY_KEY_F23                312
#define HARMONY_KEY_F24                313
#define HARMONY_KEY_F25                314
#define HARMONY_KEY_KP_0               320
#define HARMONY_KEY_KP_1               321
#define HARMONY_KEY_KP_2               322
#define HARMONY_KEY_KP_3               323
#define HARMONY_KEY_KP_4               324
#define HARMONY_KEY_KP_5               325
#define HARMONY_KEY_KP_6               326
#define HARMONY_KEY_KP_7               327
#define HARMONY_KEY_KP_8               328
#define HARMONY_KEY_KP_9               329
#define HARMONY_KEY_KP_DECIMAL         330
#define HARMONY_KEY_KP_DIVIDE          331
#define HARMONY_KEY_KP_MULTIPLY        332
#define HARMONY_KEY_KP_SUBTRACT        333
#define HARMONY_KEY_KP_ADD             334
#define HARMONY_KEY_KP_ENTER           335
#define HARMONY_KEY_KP_EQUAL           336
#define HARMONY_KEY_LEFT_SHIFT         340
#define HARMONY_KEY_LEFT_CONTROL       341
#define HARMONY_KEY_LEFT_ALT           342
#define HARMONY_KEY_LEFT_SUPER         343
#define HARMONY_KEY_RIGHT_SHIFT        344
#define HARMONY_KEY_RIGHT_CONTROL      345
#define HARMONY_KEY_RIGHT_ALT          346
#define HARMONY_KEY_RIGHT_SUPER        347
#define HARMONY_KEY_MENU               348

namespace HarmonyEngine {

    class Input {

        static const size_t NUM_KEYS = 348; // Amount of keys that GLFW can handle
        static const size_t NUM_MOUSE_BUTTONS = 9; // Amount of mouse buttons that GLFW can handle

        static bool s_Keys[NUM_KEYS];
        static bool s_KeysLast[NUM_KEYS];

        static bool s_MouseButtons[NUM_MOUSE_BUTTONS];
        static bool s_MouseButtonsLast[NUM_MOUSE_BUTTONS];

        static glm::vec2 s_MousePosition;
        static glm::vec2 s_MousePositionLast;

        static glm::vec2 s_AbsScrollPosition;
        static glm::vec2 s_ScrollPosition;
    
    public:

        static void Update() {
            memcpy(&s_KeysLast, &s_Keys, sizeof(s_KeysLast));
            memcpy(&s_MouseButtonsLast, &s_MouseButtons, sizeof(s_MouseButtonsLast));

            memcpy(&s_MousePositionLast, &s_MousePosition, sizeof(s_MousePositionLast));

            constexpr glm::vec2 zero = { 0, 0 };
            s_ScrollPosition = zero;
        }

        static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
            if(key <= NUM_KEYS)
                s_Keys[key] = (action == GLFW_PRESS) || (action == GLFW_REPEAT);
        }

        static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
            if(button <= NUM_MOUSE_BUTTONS)
                s_MouseButtons[button] = action == GLFW_PRESS;
        }

        static void MousePositionCallback(GLFWwindow* window, double xPos, double yPos) {
            s_MousePosition = { xPos, yPos };
        }

        static void MouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
            s_ScrollPosition = { xOffset, yOffset };
            s_AbsScrollPosition += glm::vec2(xOffset, yOffset);
        }

        static bool IsKey(int keycode) { return s_Keys[keycode]; }
        static bool IsKeyUp(int keycode) { return !s_Keys[keycode] && s_KeysLast[keycode]; }
        static bool IsKeyDown(int keycode) { return s_Keys[keycode] && !s_KeysLast[keycode]; }

        static bool IsMouseButton(int button) { return s_MouseButtons[button]; }
        static bool IsMouseButtonUp(int button) { return !s_MouseButtons[button] && s_MouseButtonsLast[button]; }
        static bool IsMouseButtonDown(int button) { return s_MouseButtons[button] && !s_MouseButtonsLast[button]; }

        static const glm::vec2& GetMousePosition() { return s_MousePosition; }
        static const glm::vec2& GetScrollPosition() { return s_ScrollPosition; }
        static const glm::vec2& GetAbsScrollPosition() { return s_AbsScrollPosition; }
        static const glm::vec2 GetDeltaMousePosition() { return s_MousePosition - s_MousePositionLast; }

    };

}