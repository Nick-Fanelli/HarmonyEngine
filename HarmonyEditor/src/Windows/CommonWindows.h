#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

using namespace HarmonyEngine;

namespace HarmonyEditor {

    class CommonWindows {

    public:

        CommonWindows() = delete;
        ~CommonWindows() = delete;

        CommonWindows(const CommonWindows&) = delete;

        static void OnImGuiRender();

    };

    class NewComponentWindow {

        friend class CommonWindows;

    public: 
        NewComponentWindow() = delete;
        ~NewComponentWindow() = delete;

        NewComponentWindow(const NewComponentWindow&) = delete;

        static void OpenNewComponentPopup(Entity& entity);
        static void CloseNewComponentPopup();

    private:
        static void OnImGuiRender();

        static bool s_IsVisable;
        static Entity s_CurrentEntity;
    };

}