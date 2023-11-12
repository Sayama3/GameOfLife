//
// Created by ianpo on 12/11/2023.
//

#pragma once
#include "Voxymore/Voxymore.hpp"
using namespace Voxymore::Core;

namespace Sayama::GameOfLife {


        // ======== CameraControllerSystem ========
        class CameraControllerSystem : public ::Voxymore::Core::GameplaySystem
        {
            VXM_IMPLEMENT_SYSTEM(CameraControllerSystem);
        protected:
            virtual void DeserializeSystem(YAML::Node& componentNode) override;
            virtual void SerializeSystem(YAML::Emitter& emitter) override;
            virtual void ResetSystem() override;
        public:
            virtual bool OnImGuiRender() override;
            virtual void Update(::Voxymore::Core::Scene& scene, ::Voxymore::Core::TimeStep ts) override;
        private:
            float m_Speed = 5.0f;
            ::Voxymore::Core::Key m_ForwardKey = ::Voxymore::Core::Key::W;
            ::Voxymore::Core::Key m_BackwardKey = ::Voxymore::Core::Key::S;
            ::Voxymore::Core::Key m_RightKey = ::Voxymore::Core::Key::D;
            ::Voxymore::Core::Key m_LeftKey = ::Voxymore::Core::Key::A;
            ::Voxymore::Core::Key m_UpKey = ::Voxymore::Core::Key::E;
            ::Voxymore::Core::Key m_DownKey = ::Voxymore::Core::Key::Q;
        };


} // Sayama::GameOfLife
