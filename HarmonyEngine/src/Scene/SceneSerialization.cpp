#include "SceneSerialization.h"

#include <Scene/Component.h>
#include <Scene/Entity.h>

using namespace HarmonyEngine;

// ======================================================================================================
// Scene Serialization
// ======================================================================================================

SceneSerializer::SceneSerializer(Scene* scenePtr, const std::string& filepath) : m_ScenePtr(scenePtr), m_Filepath(filepath) {

}

void SceneSerializer::SerializeYAML() {

}

void SceneSerializer::SerializeBinary() {

}

void SceneSerializer::LoadYAML() {

}

void SceneSerializer::LoadBinary() {

}