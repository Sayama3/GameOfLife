//
// Created by ianpo on 12/11/2023.
//

#include "GameOfLifeSystem.hpp"


namespace Sayama::GameOfLife {

    VXM_CREATE_SYSTEM(GameOfLifeSystem)

    void GameOfLifeSystem::Update(Scene &scene, TimeStep ts)
    {
        if(m_Pause) return;

        m_TimePassed += ts;
        if(m_TimePassed < m_TickTime) return;
        m_TimePassed = 0;

        auto min = GetMin();
        auto max = GetMax();
        std::vector<glm::ivec2> toEnable;
        std::vector<glm::ivec2> toDisable;

        int realMinX = min.x-1;
        int realMaxX = max.x+1;
        int realMinY = min.y-1;
        int realMaxY = max.y+1;

        for (int x = realMinX; x <= realMaxX; ++x) {
            for (int y = realMinY; y <= realMaxY; ++y) {
                glm::ivec2 pos(x,y);
                if(GetBlockCount(pos) == 0) continue;
                bool alive = HasEntity(pos);
                int neighbour = GetNeighboursCount(pos);

                if(alive)
                {
                    Entity e = scene.GetEntity(m_Entities[pos]);
                    if(neighbour < 2 || neighbour > 3)
                    {
                        e.GetComponent<TransformComponent>().SetPosition({pos.x, 0, pos.y});
                        e.GetComponent<GameOfLifeComponent>().PreviousPosition = pos;
                        e.GetComponent<GameOfLifeComponent>().NexState = false;
                        toDisable.push_back(pos);
                    }
                    else
                    {
                        e.GetComponent<TransformComponent>().SetPosition({pos.x, 0, pos.y});
                        e.GetComponent<GameOfLifeComponent>().PreviousPosition = pos;
                        e.GetComponent<GameOfLifeComponent>().NexState = true;
                        toEnable.push_back(pos);
                    }
                }
                else
                {
                    if(neighbour == 3)
                    {
                        UUID id = CreateEntity(scene, pos);
                        toEnable.push_back(pos);
                    }
                }
            }
        }

        for(auto& pos : toEnable)
        {
            auto id = m_Entities[pos];
            auto e = scene.GetEntity(id);
            e.SetActive(true);
        }

        for(auto& pos : toDisable)
        {
            DeleteEntity(scene, pos);
        }
    }

    void GameOfLifeSystem::OnStart(Scene& scene)
    {
        m_Entities.clear();
        m_PoolEntities.clear();

        auto view = scene.GetRegistry().view<TransformComponent, GameOfLifeComponent>();
        for(entt::entity e : view)
        {
            Entity entity(e,&scene);
            TransformComponent& tc = view.get<TransformComponent>(e);
            GameOfLifeComponent& gol = view.get<GameOfLifeComponent>(e);
            if(!entity.HasComponent<ModelComponent>())
            {
                auto& mc = entity.AddComponent<ModelComponent>();
                mc.SetPath({FileSource::EditorAsset, "Models/Cube_W.glb"});
                mc.SetShader("Default");
            }
            glm::vec3 pos = tc.GetPosition();
            pos.x = std::round(pos.x);
            pos.z = std::round(pos.z);
            glm::ivec2 ipos(pos.x, pos.z);
            tc.SetPosition(pos);
            gol.PreviousPosition = ipos;

            if(entity.IsActive()) m_Entities[ipos] = entity.GetUUID();
            else m_PoolEntities.push_back(entity.GetUUID());
        }
    }

    void GameOfLifeSystem::OnStop(Scene& scene)
    {
        m_PoolEntities.clear();
        m_Entities.clear();
    }

    void GameOfLifeSystem::DeserializeSystem(YAML::Node &componentNode)
    {
        m_TickTime = componentNode["TickTime"] ? componentNode["TickTime"].as<float>() : 2.0f;
        m_Pause = componentNode["Pause"] ? componentNode["Pause"].as<bool>() : true;
    }

    void GameOfLifeSystem::SerializeSystem(YAML::Emitter &out)
    {
        out << KEYVAL("TickTime", m_TickTime);
        out << KEYVAL("Pause", m_Pause);
    }

    bool GameOfLifeSystem::OnImGuiRender()
    {
        bool changed = false;
        changed |= ImGui::DragFloat("Ticks Time", &m_TickTime);
        changed |= ImGui::Checkbox("Pause", &m_Pause);
        return changed;
    }

    void GameOfLifeSystem::ResetSystem()
    {
        m_TickTime = 2;
        m_Pause = true;
    }

    bool GameOfLifeSystem::HasEntity(glm::ivec2 v)
    {
        return m_Entities.contains(v);
    }

    UUID GameOfLifeSystem::CreateEntity(Scene& scene, glm::ivec2 v)
    {
        UUID id;

        if(m_Entities.contains(v))
        {
            id = m_Entities[v];
            auto e = scene.GetEntity(id);
            e.GetComponent<TransformComponent>().SetPosition({v.x, 0, v.y});
            auto& gol = e.GetOrAddComponent<GameOfLifeComponent>();
            gol.PreviousPosition = v;
            gol.NexState = true;
        }
        else if(!m_PoolEntities.empty())
        {
            id = m_PoolEntities[m_PoolEntities.size()-1];
            m_PoolEntities.pop_back();
            auto e = scene.GetEntity(id);
            e.SetActive(true);
            e.GetComponent<TransformComponent>().SetPosition({v.x, 0, v.y});
            auto& gol = e.GetOrAddComponent<GameOfLifeComponent>();
            gol.PreviousPosition = v;
            gol.NexState = true;
            m_Entities[v] = id;
        }
        else
        {
            auto e = scene.CreateEntity();
            e.GetComponent<TransformComponent>().SetPosition({v.x, 0, v.y});
            auto& gol = e.GetOrAddComponent<GameOfLifeComponent>();
            gol.PreviousPosition = v;
            gol.NexState = true;
            auto& mc = e.AddComponent<ModelComponent>();
            mc.SetPath({FileSource::EditorAsset, "Models/Cube_W.glb"});
            mc.SetShader("Default");
            id = e.GetUUID();
            m_Entities[v] = id;
        }

        return id;
    }

    bool GameOfLifeSystem::DeleteEntity(Scene& scene, glm::ivec2 v)
    {
        auto it = m_Entities.find(v);
        if(it != m_Entities.end())
        {
            auto id = it->second;
            auto e = scene.GetEntity(id);
            e.SetActive(false);
            m_PoolEntities.push_back(id);
            m_Entities.erase(it);
        }
        return true;
    }

    int GameOfLifeSystem::GetNeighboursCount(glm::ivec2 v)
    {
        int count = 0;
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                if(x == 0 && z == 0) continue;
                glm::ivec2 checkPos = v + glm::ivec2{x,z};
                if(HasEntity(checkPos)) ++count;
            }
        }
        return count;
    }
    int GameOfLifeSystem::GetBlockCount(glm::ivec2 v)
    {
        int count = 0;
        for (int x = -1; x <= 1; ++x) {
            for (int z = -1; z <= 1; ++z) {
                glm::ivec2 checkPos = v + glm::ivec2{x,z};
                if(HasEntity(checkPos)) ++count;
            }
        }
        return count;
    }

    glm::ivec2 GameOfLifeSystem::GetMin()
    {
        glm::ivec2 pos;
        if(m_Entities.empty()) return pos;
        pos = m_Entities.begin()->first;
        for(auto&&[p, id] : m_Entities)
        {
            if(p.x < pos.x) pos.x = p.x;
            if(p.y < pos.y) pos.y = p.y;
        }
        return pos;
    }

    glm::ivec2 GameOfLifeSystem::GetMax()
    {
        glm::ivec2 pos;
        if(m_Entities.empty()) return pos;
        pos = m_Entities.begin()->first;
        for(auto&&[p, id] : m_Entities)
        {
            if(p.x > pos.x) pos.x = p.x;
            if(p.y > pos.y) pos.y = p.y;
        }
        return pos;
    }
} // Sayama
// GameOfLife