#pragma once
#include "../../Motor/Primitivos/Escena.hpp"

namespace IVJ
{
    // Esta escena crea 50 circulos al azar que se autodestruyen en tiempos aleatorios.
    // Al morir el ultimo objeto, cambia a la escena "Circulos".
    class EscenaObjMuriendo : public CE::Escena
    {
    public:
        EscenaObjMuriendo():CE::Escena{}{}
        void onInit();
        void onFinal();
        void onUpdate(float dt);
        void onInputs(const CE::Botones& accion);
        void onRender();

    private:
        bool inicializar{true};
        const int NUM_OBJETOS = 50;
    };

    class TimerDeVida : public CE::ITimer
    {
        private:
            float tiempoVidaRestante;
            bool expirado = false;

        public:
            // Constructor que inicializa el tiempo de vida
            TimerDeVida(float vida) : tiempoVidaRestante(vida) {}

            // Getter para que el sistema de la escena compruebe si el tiempo expiró.
            bool estaExpirado() const { return expirado; }

            // Decrementa el tiempo restante. Implementación en el CPP.
            void onUpdate(float dt);
    };
}