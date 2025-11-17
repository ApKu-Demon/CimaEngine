#pragma once

#include "../../Motor/Primitivos/Objetos.hpp"

namespace IVJ
{
    class Entidad : public CE::Objeto
    {
        public:
            void onUpdate(float dt) override;
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
    };
}


