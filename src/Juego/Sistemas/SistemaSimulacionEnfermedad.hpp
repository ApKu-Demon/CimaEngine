#pragma once

#include "../../Motor/Primitivos/Objetos.hpp"
#include "../Componentes/JComponentes.hpp"
#include <vector>
#include <memory>
#include <map>

namespace IVJ
{
    // Constantes de la simulación
    constexpr int POBLACION_SIZE = 15;
    constexpr float TIEMPO_ENFERMO = 3.0f;
    constexpr float TIEMPO_RECUPERACION = 2.0f;
    constexpr float TIEMPO_INM_PARCIAL = 2.0f;

    // Estructura para almacenar estadísticas globales
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
     * @brief Sistema que maneja la lógica de la enfermedad (contagio, recuperación, muerte)
     * y la actualización de estados de las personas.
     * @param poblacion Vector de objetos (personas) en la simulación.
     * @param stats Estructura para actualizar los contadores.
     * @param dt Delta time (tiempo transcurrido desde el último frame).
     */
    void SistemaSimulacionEnfermedad(std::vector<std::shared_ptr<CE::Objeto>>& poblacion, EstadisticasSimulacion& stats, float dt);

    /**
     * @brief Función auxiliar para inicializar la población (15x15) y el 10% de enfermos iniciales.
     * @param poblacion Vector donde se agregarán los objetos Rectangulo.
     */
    void InicializarPoblacion(std::vector<std::shared_ptr<CE::Objeto>>& poblacion);

    /**
     * @brief Función para actualizar el color del objeto visual según su estado.
     * @param persona El objeto Objeto que representa a la persona.
     * @param estado_persona El componente JEstadoPersona.
     */
    void ActualizarColor(CE::Objeto* persona, const JEstadoPersona& estado_persona);
}