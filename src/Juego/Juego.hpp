#pragma once 
#include "../Motor/App/ALayer.hpp"
#include <optional>
#include "Figuras/Figuras.hpp"      // para que el compilador no lo deseche al hacer el ensamble
#include "../Motor/Primitivos/Objetos.hpp"


namespace IVJ
{
    class Juego: public CE::GameLayer
    {
        public:
            explicit Juego(void);
            void OnInit(void) override ;
            void OnInputs(float dt,std::optional<sf::Event>& eventos) override;
            void OnUpdate(float dt) override;
            void OnRender(float dt) override;

        private:
            std::vector<std::shared_ptr<CE::Objeto>> objetos;
            sf::Font font_juego;
            sf::Text texto;
    };
}

