#pragma once
#include "../../Motor/Primitivos/Escena.hpp"
#include "../Objetos/Entidad.hpp"
#include "../Objetos/TileMap.hpp"
#include <memory>

namespace IVJ
{
    class EscenaCuadros : public CE::Escena
    {
        public:
            EscenaCuadros():CE::Escena{}{};
            void onInit();
            void onFinal();
            void onUpdate(float dt);
            void onInputs(const CE::Botones& accion);
            void onRender();

        private:
            bool inicializar{true};
            std::shared_ptr<Entidad> jugador;
            // agregamos el mapa
            TileMap bg[2];
    };
}


