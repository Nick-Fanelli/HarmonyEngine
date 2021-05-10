#pragma once

#include "harmonypch.h"

#include "Scene.h"

namespace HarmonyEngine::SceneManager {

    static Scene* s_ActiveScenePtr;

    void SetActiveScene(Scene* scenePtr) {
        s_ActiveScenePtr = scenePtr;
    }

    const Scene* GetActiveScenePtr() { return s_ActiveScenePtr; }

    void Update(float deltaTime) {
        if(s_ActiveScenePtr != nullptr)
            s_ActiveScenePtr->Update(deltaTime);
    }
}