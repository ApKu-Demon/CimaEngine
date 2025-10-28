#pragma once

#include "../../Motor/Primitivos/Objetos.hpp"
#include "../Componentes/JComponentes.hpp"
#include <vector>
#include <memory>
#include <map>

namespace IVJ
{
    // Constantes de la simulacion
    constexpr int POBLACION_SIZE = 15;
    constexpr float TIEMPO_ENFERMO = 3.0f;
    constexpr float TIEMPO_RECUPERACION = 2.0f;
    constexpr float TIEMPO_INM_PARCIAL = 2.0f;

    // Estructura para almacenar estadisticas globales
    struct EstadisticasSimulacion
    {
        int normal = 0;
        int enfermo = 0;
        int recuperacion = 0;
        int inmune = 0;
        int muerto = 0;
        float tiempo_total = 0.0f;
    };

    /**
     * Sistema que maneja la logica de la enfermedad (contagio, recuperación, muerte) y la actualizacion de estados de las personas.
     * 
     * poblacion: Vector de objetos (personas) en la simulacion.
     * stats: Estructura para actualizar los contadores.
     * dt: Delta time (tiempo transcurrido desde el ultimo frame).
     */
    void SistemaSimulacionEnfermedad(std::vector<std::shared_ptr<CE::Objeto>>& poblacion, EstadisticasSimulacion& stats, float dt);

    /**
     * Funcion auxiliar para inicializar la poblacion (15x15) y el 10% de enfermos iniciales.
     * 
     * poblacion: Vector donde se agregaran los objetos Rectangulo.
     */
    void InicializarPoblacion(std::vector<std::shared_ptr<CE::Objeto>>& poblacion);

    /**
     * Funcion para actualizar el color del objeto visual segun su estado.
     * 
     * persona: El objeto Objeto que representa a la persona.
     * estado_persona: El componente JEstadoPersona.
     */
    void ActualizarColor(CE::Objeto* persona, const JEstadoPersona& estado_persona);
}