//
// Created by ianpo on 12/11/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>
#include <GameOfLifeComponent.hpp>

using namespace ::Voxymore::Core;

namespace std
{
    template<>
    struct hash<glm::ivec2> {
        inline std::size_t operator()(const glm::ivec2 &vec2) const {
            return std::hash<int>()(vec2.x) ^ std::hash<int>()(vec2.y);
        }

        inline bool operator()(const glm::ivec2 &a, const glm::ivec2 &b) const {
            return a.x == b.x && a.y == b.y;
        }
    };

    template<>
    struct hash<glm::ivec3> {
        inline std::size_t operator()(const glm::ivec3 &vec3) const {
            return std::hash<int>()(vec3.x) ^ std::hash<int>()(vec3.y) ^ std::hash<int>()(vec3.z);
        }

        inline bool operator()(const glm::ivec3 &a, const glm::ivec3 &b) const {
            return a.x == b.x && a.y == b.y && a.z == b.z;
        }
    };
}

namespace Sayama::GameOfLife {

    class GameOfLifeSystem : public GameplaySystem
    {
    VXM_IMPLEMENT_SYSTEM(GameOfLifeSystem)
    public:
        virtual void Update(Scene& scene, TimeStep ts) override;
        virtual void OnStart(Scene& scene) override;
        virtual void OnStop(Scene& scene) override;
        virtual bool OnImGuiRender() override;
    protected:
        virtual void DeserializeSystem(YAML::Node& componentNode) override;
        virtual void SerializeSystem(YAML::Emitter& Emitter) override;
        virtual void ResetSystem() override;
    private:
        bool HasEntity(glm::ivec2 v);
        UUID CreateEntity(Scene& scene, glm::ivec2 v);
        bool DeleteEntity(Scene& scene, glm::ivec2 v);
        int GetNeighboursCount(glm::ivec2 v);
        int GetBlockCount(glm::ivec2 v);
        glm::ivec2 GetMin();
        glm::ivec2 GetMax();
    private:
        std::unordered_map<glm::ivec2, UUID> m_Entities;
        std::vector<UUID> m_PoolEntities;
    private:
        float m_TimePassed = 0;

        float m_TickTime = 2.0f;
        bool m_Pause = true;
    };

} // Sayama
// GameOfLife