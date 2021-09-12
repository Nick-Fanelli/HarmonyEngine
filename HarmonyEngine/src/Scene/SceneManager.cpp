#include "SceneManager.h"

#include <Render/MasterRenderer.h>
#include <Scene/Component.h>

using namespace HarmonyEngine;

// Scene Runtime
void SceneRuntime::StartRuntime() {
    if(s_RunMode == RunMode::RunModeRunning)
        StopRuntime();

    // Position the Camera
    SceneManager::GetActiveScenePtr()->ForEachEntityWithTransform<OrthographicCameraComponent>([&](TransformComponent& transform, OrthographicCameraComponent& component) {
        component.Camera.SetPosition(transform.Transform.Position);
        MasterRenderer::CameraPtr = &component.Camera;
        return;
    });

    // Start the Global Script
    SceneManager::GetActiveScenePtr()->GetGlobalScript().Reload();
    SceneManager::GetActiveScenePtr()->GetGlobalScript().OnCreate();

    s_RunMode = RunMode::RunModeRunning;
}

void SceneRuntime::OnUpdate(float deltaTime) {
    if(s_RunMode == RunMode::RunModeRunning) {
        SceneManager::GetActiveScenePtr()->GetGlobalScript().OnUpdate(deltaTime);
    }
}

void SceneRuntime::StopRuntime() {
    s_RunMode = RunMode::RunModePause;
}

// Scene Manager
void SceneManager::SetActiveScene(Scene* scenePtr) {
    HARMONY_PROFILE_FUNCTION();

    if(scenePtr == nullptr) return;

    if(s_ActiveScenePtr != nullptr) {
        auto ptr = s_ActiveScenePtr;
        s_ActiveScenePtr = nullptr;
        ptr->OnDestroy();
    }
    
    scenePtr->OnCreate();
    s_ActiveScenePtr = scenePtr;
}

void SceneManager::Update(float deltaTime) {
    if(s_ActiveScenePtr != nullptr) {
        s_ActiveScenePtr->OnUpdate(deltaTime);
        SceneRuntime::OnUpdate(deltaTime);
    }
}

void SceneManager::OnDestroy() {
    if(s_ActiveScenePtr != nullptr)
        s_ActiveScenePtr->OnDestroy();
}