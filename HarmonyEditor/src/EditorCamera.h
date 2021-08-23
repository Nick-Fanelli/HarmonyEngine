#pragma once

#include <harmonypch.h>

#include <Render/Camera.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class EditorCamera : public PerspectiveCamera {

    public:
        EditorCamera() = default;

        inline bool IsInOperation() const { return m_IsInOperation; }

        void OnUpdate(float deltaTime);

        enum InputStyle {
            InputStyleDefault = 0, InputStyleReversed = 1, InputStyleModern = 2
        };

        static const char* InputStyleFields[];
        static const uint32_t InputStyleCount;

    private:
        bool m_IsInOperation = false;
    };
}