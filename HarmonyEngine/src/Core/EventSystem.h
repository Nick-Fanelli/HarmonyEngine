#pragma once

#include "harmonypch.h"

namespace HarmonyEngine {

    template<typename EventType>
    class EventSystem {

    public:
        EventSystem() = default;

        void PushBackCallback(std::function<void(const EventType&)>& function) {
            m_Callbacks.push_back(function);
        }

        void Call(const EventType& event) {
            for(auto& callback : m_Callbacks) {
                *callback(event);
            }
        }

    private:

    private:
        std::vector<std::function<void(const EventType&)>*> m_Callbacks;

    };  

}