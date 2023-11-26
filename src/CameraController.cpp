//
// Created by ianpo on 12/11/2023.
//

#include "CameraController.hpp"

namespace Sayama::GameOfLife
{
    // ======== CameraControllerSystem ========

    VXM_CREATE_SYSTEM(CameraControllerSystem)

    void CameraControllerSystem::SerializeSystem(YAML::Emitter &emitter)
    {
        emitter << KEYVAL("Speed", m_Speed);
    }
    void CameraControllerSystem::DeserializeSystem(YAML::Node &componentNode)
    {
        m_Speed = componentNode["Speed"].as<float>();
    }
    void CameraControllerSystem::ResetSystem()
    {
        m_Speed = 5.0f;
    }
    bool CameraControllerSystem::OnImGuiRender()
    {
        return ImGui::DragFloat("Speed", &m_Speed);
    };
    void CameraControllerSystem::Update(Scene &scene, TimeStep ts)
    {
        auto view = scene.GetRegistry().view<TransformComponent, CameraComponent>(entt::exclude<DisableComponent>);

        for (entt::entity entity : view)
        {
            if(!view.get<CameraComponent>(entity).Primary) continue;
            TransformComponent& tc = view.get<TransformComponent>(entity);
            glm::vec3 localMovement(0);
            glm::quat rotation = tc.GetRotation();
            glm::vec3 position = tc.GetPosition();

            if (Input::IsKeyPressed(m_ForwardKey)) localMovement += glm::vec3(0, 0, 1);
            if (Input::IsKeyPressed(m_BackwardKey)) localMovement += glm::vec3(0, 0, -1);
            if (Input::IsKeyPressed(m_RightKey)) localMovement += glm::vec3(1, 0, 0);
            if (Input::IsKeyPressed(m_LeftKey)) localMovement += glm::vec3(-1, 0, 0);
            if (Input::IsKeyPressed(m_UpKey)) localMovement += glm::vec3(0, 1, 0);
            if (Input::IsKeyPressed(m_DownKey)) localMovement += glm::vec3(0, -1, 0);

            position += (rotation * localMovement) * (ts * m_Speed);
            tc.SetPosition(position);
            return;
        }

    }

    void CameraControllerSystem::OnStart(Scene &scene) {
        GameplaySystem::OnStart(scene);
    }

    void CameraControllerSystem::OnStop(Scene &scene) {
        GameplaySystem::OnStart(scene);
    }
} // Sayama::GameOfLife