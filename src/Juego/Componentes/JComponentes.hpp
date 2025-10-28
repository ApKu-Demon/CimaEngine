#pragma once

#include "../../Motor/Componentes/IComponentes.hpp"
#include <random>

namespace IVJ
{
    class JComponentes
    {
        public:
            virtual ~JComponentes()=default;    // destructor
    };

    // Estados de salud
    enum class EstadoSalud
    {
        NORMAL,
        ENFERMO,
        RECUPERACION,
        INMUNE_PERMANENTE,
        MUERTO
    };

    // Info del estado de persona
    struct JEstadoPersona : public CE::IComponentes
    {
        // Core State ---
        EstadoSalud estado = EstadoSalud::NORMAL;
        bool tiene_inmunidad_parcial = false; // Bandera para la inmunidad parcial

        // Timers (Controlados por la función de Update) ---
        float tiempo_en_estado = 0.0f;          // Tiempo total en el estado actual.
        float tiempo_inm_parcial_restante = 0.0f; // Para el estado normal con inmunidad parcial.
        float tiempo_ultimo_contagio = 0.0f; // Marca el ultimo segundo entero en que la persona contagió

        // Stats/Probabilities (Individuales y Fijas) ---
        float prob_contagio;            // [0.15, 0.30]
        float prob_muerte = 0.10f;      // 10% Fijo
        float prob_inmunidad_permanente;// [0.01, 0.10]

        // Grid Position ---
        int fila;
        int columna;

        // Constructor para inicializar probabilidades y posición
        JEstadoPersona(int r, int c) : fila(r), columna(c)
        {
            // Inicialización de las distribuciones aleatorias
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dist_contagio(0.15, 0.30);
            std::uniform_real_distribution<> dist_inmunidad(0.01, 0.10);

            // Asignación de probabilidades individuales
            prob_contagio = static_cast<float>(dist_contagio(gen));
            prob_inmunidad_permanente = static_cast<float>(dist_inmunidad(gen));
        }

        ~JEstadoPersona() override = default;
    };
}