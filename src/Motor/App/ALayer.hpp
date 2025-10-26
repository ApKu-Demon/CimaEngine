#pragma once
#include <SFML/Graphics.hpp>

namespace CE
{
    class GameLayer
    {
        public:
            virtual ~GameLayer(){};
            virtual void OnInit(void){};
            virtual bool EstaCorriendo(void){return !termino;};
            virtual void OnInputs(float dt, 
                    std::optional<sf::Event>&eventos){};
            virtual void OnUpdate(float dt){};
            virtual void OnRender(float dt){};
        protected:
            bool termino;

    };
}
