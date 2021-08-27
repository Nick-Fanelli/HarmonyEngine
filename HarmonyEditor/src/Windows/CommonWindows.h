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

    class ConfirmationWindow {

        friend class CommonWindows;

    public:
        ConfirmationWindow() = delete;
        ~ConfirmationWindow() = delete;

        ConfirmationWindow(const ConfirmationWindow&) = delete;

        static void Confirm(const std::string& windowTitle, const std::string& message, std::function<void(bool)> functionPtr);
        static void Confirm(const std::string& windowTitle, const std::string& message, std::function<void()> functionPtr);

    private:
        static void OnImGuiRender();

    private:    
        static const char* s_PopupWindowTitle;

        static bool s_ShouldOpen;
        static bool s_UseVoidFunctionPtr;
        static std::string s_PopupMessage;

        static std::function<void(bool)> s_FunctionPtr;
        static std::function<void()> s_VoidFunctionPtr;

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

    private:
        static bool s_IsVisable;
        static Entity s_CurrentEntity;
    };

}