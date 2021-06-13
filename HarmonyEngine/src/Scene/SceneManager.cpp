#include "SceneManager.h"

using namespace HarmonyEngine;

Scene* SceneManager::s_ActiveScenePtr = nullptr;

void SceneManager::SetActiveScene(Scene* scenePtr) {
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
    if(s_ActiveScenePtr != nullptr)
        s_ActiveScenePtr->OnUpdate(deltaTime);
}

void SceneManager::OnDestroy() {
    if(s_ActiveScenePtr != nullptr)
        s_ActiveScenePtr->OnDestroy();
}