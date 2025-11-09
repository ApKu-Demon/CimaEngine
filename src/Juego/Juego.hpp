#pragma once 
#include "../Motor/App/ALayer.hpp"                      //
#include <optional>
#include "Figuras/Figuras.hpp"
#include "../Motor/Primitivos/Objetos.hpp"
#include "Sistemas/SistemaSimulacionEnfermedad.hpp"
#include "../Motor/Primitivos/Escena.hpp"               //

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
            CE::Escena* escena_actual;

            std::vector<std::shared_ptr<CE::Objeto>> objetos;
            sf::Font font_juego;
            sf::Text texto;

            // Para la simulacion
            sf::Text texto_stats;
            EstadisticasSimulacion stats_simulacion;
            bool simulacion_activa = true; // Controlar si el OnUpdate se ejecuta
            // ------------------------------------
    };
}


