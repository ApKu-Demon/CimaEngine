#pragma once
#include "../../Motor/Primitivos/Escena.hpp"
#include "../Objetos/Entidad.hpp"

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
    };
}


