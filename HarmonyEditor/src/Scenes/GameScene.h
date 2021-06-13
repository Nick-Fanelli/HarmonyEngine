#pragma once

#include <harmonypch.h>

#include <Core/Display.h>
#include <Core/Input.h>
#include <Core/Assets.h>

#include <Scene/Scene.h>
#include <Scene/Entity.h>
#include <Scene/Component.h>
#include <Scene/Terrain.h>

#include <Render/Shader.h>
#include <Render/Renderer2D.h>
#include <Render/Camera.h>
#include <Render/Renderer.h>

using namespace HarmonyEngine;

class GameScene : public Scene {
public:
    void OnCreate() override;
    void Update(float deltaTime) override;
    void OnDestroy() override;
};