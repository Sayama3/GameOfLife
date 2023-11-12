//
// Created by ianpo on 12/11/2023.
//

#include "GameOfLifeComponent.hpp"

using namespace Voxymore::Core;

template<>
bool::Voxymore::Core::ComponentCreator<Sayama::GameOfLife::GameOfLifeComponent>::s_Created = false;


namespace Sayama::GameOfLife {
    ::Voxymore::Core::ComponentCreator<GameOfLifeComponent>  GameOfLifeComponent::s_ComponentCreator;
    void GameOfLifeComponent::DeserializeComponent(YAML::Node &componentNode, ::Voxymore::Core::Entity targetEntity)
    {
        auto& c = targetEntity.GetOrAddComponent<GameOfLifeComponent>();
        if(componentNode["PreviousPosition"]) c.PreviousPosition = componentNode["PreviousPosition"].as<glm::ivec2>();
        if(componentNode["NexState"]) c.NexState = componentNode["NexState"].as<bool>();
    }
    void GameOfLifeComponent::SerializeComponent(YAML::Emitter &out, ::Voxymore::Core::Entity sourceEntity)
    {
        auto& c = sourceEntity.GetOrAddComponent<GameOfLifeComponent>();
        out << KEYVAL("PreviousPosition", c.PreviousPosition);
        out << KEYVAL("NexState", c.NexState);

    }
    bool GameOfLifeComponent::OnImGuiRender(::Voxymore::Core::Entity sourceEntity)
    {
        bool changed = false;
        auto& c = sourceEntity.GetOrAddComponent<GameOfLifeComponent>();
        changed |= ImGui::DragInt2("Previous Position", glm::value_ptr(c.PreviousPosition));
        changed |= ImGui::Checkbox("Next State", &c.NexState);
        return changed;
    }
} // Sayama
// GameOfLife