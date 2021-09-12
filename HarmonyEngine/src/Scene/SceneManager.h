#pragma once

#include "harmonypch.h"

#include "Scene.h"

namespace HarmonyEngine {

    class SceneRuntime {

    public:
        SceneRuntime() = delete;
        SceneRuntime(const SceneRuntime&) = delete;

        enum RunMode {
            RunModeRunning,
            RunModePause
        };

        static void StartRuntime();
        static void OnUpdate(float deltaTime);
        static void StopRuntime();

        static inline RunMode GetRunMode() { return s_RunMode; }
        static inline bool IsRunning() { return s_RunMode == RunModeRunning; }

    private:
        static inline RunMode s_RunMode = RunModePause;


    };

    class SceneManager {

        static inline Scene* s_ActiveScenePtr = nullptr;

    public:

        static void SetActiveScene(Scene* scenePtr);
        static void Update(float deltaTime);
        static void OnDestroy();

        static Scene* GetActiveScenePtr() { return s_ActiveScenePtr; }
    };
}