#pragma once

#include "harmonypch.h"

#include "Scene.h"

namespace HarmonyEngine::SceneManager {

    static Scene* s_ActiveScenePtr = nullptr;

    void SetActiveScene(Scene* scenePtr) {
        if(scenePtr == nullptr) return;

        if(s_ActiveScenePtr != nullptr) {
            auto ptr = s_ActiveScenePtr;
            s_ActiveScenePtr = nullptr;
            ptr->OnDestroy();
        }
        
        scenePtr->OnCreate();
        s_ActiveScenePtr = scenePtr;
    }

    const Scene* GetActiveScenePtr() { return s_ActiveScenePtr; }

    void Update(float deltaTime) {
        if(s_ActiveScenePtr != nullptr)
            s_ActiveScenePtr->Update(deltaTime);
    }

    void OnDestroy() {
        if(s_ActiveScenePtr != nullptr)
            s_ActiveScenePtr->OnDestroy();
    }
}