#pragma once

#include <harmonypch.h>

#include <Scene/Entity.h>

#include "EditorPanel.h"

using namespace HarmonyEngine;

class HierarchyEditorPanel : public EditorPanel {

public:
    void OnUpdate() override;
private:
    void DrawEntityNode(Entity& entity);
private:
    Entity m_SelectionContext;

};