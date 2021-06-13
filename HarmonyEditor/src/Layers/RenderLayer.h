#include <harmonypch.h>

#include <Render/Camera.h>

using namespace HarmonyEngine;

namespace RenderLayer {

    static Camera* m_CameraPtr;

    void OnCreate(Camera* camera);
    void OnUpdate(float deltaTime);
    void OnDestroy();

};