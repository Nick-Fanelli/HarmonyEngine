#pragma once

#include "harmonypch.h"

#include "Scene.h"

namespace HarmonyEngine {

    class SceneManager {

        static Scene* s_ActiveScenePtr;

    public:

        static void SetActiveScene(Scene* scenePtr);
        static void Update(float deltaTime);
        static void OnDestroy();

        static const Scene* GetActiveScenePtr() { return s_ActiveScenePtr; }
    };
}