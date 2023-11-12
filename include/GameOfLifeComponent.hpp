//
// Created by ianpo on 12/11/2023.
//

#pragma once

#include <Voxymore/Voxymore.hpp>

namespace Sayama {
    namespace GameOfLife {

        class GameOfLifeComponent {
            VXM_IMPLEMENT_COMPONENT(GameOfLifeComponent);
        public:
            glm::ivec2 PreviousPosition;
            bool NexState;
        };

    } // Sayama
} // GameOfLife
