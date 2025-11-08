#include "SistemaSimulacionEnfermedad.hpp"
#include "../Figuras/Figuras.hpp" // Para Rectangulo y sf::Color
#include <cstdlib> // para rand()
#include <ctime>   // para time()
#include <iostream>
#include <algorithm> 
#include <random>
#include <numeric>   // para std::iota

namespace IVJ
{
    // Funcion de ayuda para obtener el estado de salud y componente de una persona
    JEstadoPersona* getEstado(CE::Objeto* obj)
    {
        return obj->getComponente<JEstadoPersona>();
    }

    // Definicion de Colores ---
    sf::Color getColorRelleno(EstadoSalud estado)
    {
        switch (estado)
        {
            case EstadoSalud::ENFERMO:
                return sf::Color(255, 0, 0); // Rojo
            case EstadoSalud::RECUPERACION:
                return sf::Color(100, 50, 150); // Morado
            case EstadoSalud::INMUNE_PERMANENTE:
                return sf::Color(173, 255, 47); // Verde Lima 
            case EstadoSalud::MUERTO:
                return sf::Color::Black; // Negro
            case EstadoSalud::NORMAL:
            default:
                return sf::Color::White; // Relleno blanco/neutro
        }
    }
    
    sf::Color getColorContorno(EstadoSalud estado)
    {
        if (estado == EstadoSalud::NORMAL)
            return sf::Color(50, 50, 50);
        return sf::Color::Black;
    }

    void ActualizarColor(CE::Objeto* persona, const JEstadoPersona& estado_persona)
    {
        if (auto rect = dynamic_cast<IVJ::Rectangulo*>(persona))
        {
            sf::Color relleno = getColorRelleno(estado_persona.estado);
            sf::Color contorno = getColorContorno(estado_persona.estado);
            
            if (estado_persona.estado == EstadoSalud::MUERTO)
                contorno = sf::Color(50, 50, 50);

            rect->setColores(relleno, contorno); 
        }
    }
    
    // Logica Principal de la Simulacion ---
    void SistemaSimulacionEnfermedad(std::vector<std::shared_ptr<CE::Objeto>>& poblacion, EstadisticasSimulacion& stats, float dt)
    {
        // Reiniciar estadisticas y acumular tiempo total
        float tiempo_actual = stats.tiempo_total;
        stats = {0}; 
        stats.tiempo_total += tiempo_actual + dt;
        
        // 1. Fase de Contagio (Se basa en el estado del frame anterior)
        // Usamos un vector temporal para marcar las coordenadas de las nuevas infecciones
        std::vector<std::pair<int, int>> nuevas_infecciones; 
        
        // El mecanismo de contagio se activa solo en ciertos "ticks" de tiempo
        bool tick_de_contagio = false;

        for (auto& obj_enfermo : poblacion)
        {
            JEstadoPersona* estado_enfermo = getEstado(obj_enfermo.get());
            if (!estado_enfermo || estado_enfermo->estado != EstadoSalud::ENFERMO) continue;
            
            // *** Mecanismo de Contagio por Segundo (3 segundos) ***
            // Verificamos si el tiempo en estado ha cruzado un umbral de segundo ENTERO.
            // Para esto, necesitamos agregar una variable `float tiempo_ultimo_contagio` en JEstadoPersona
            
            // Si el tiempo en estado ha excedido el siguiente segundo Y es menor o igual a 3
            if (estado_enfermo->tiempo_en_estado < TIEMPO_ENFERMO && 
                estado_enfermo->tiempo_en_estado >= estado_enfermo->tiempo_ultimo_contagio + 1.0f) 
            {
                estado_enfermo->tiempo_ultimo_contagio = std::floor(estado_enfermo->tiempo_en_estado); // Marca el segundo actual (1.0, 2.0, 3.0)
                tick_de_contagio = true; // Solo si al menos un enfermo ha pasado un nuevo segundo
            }

            if (tick_de_contagio)
            {
                // Vecinos (arriba, abajo, izquierda, derecha)
                int dr[] = {-1, 1, 0, 0};
                int dc[] = {0, 0, -1, 1};

                for (int i = 0; i < 4; ++i)
                {
                    int r = estado_enfermo->fila + dr[i];
                    int c = estado_enfermo->columna + dc[i];

                    // Comprobar limites de la cuadricula (15x15)
                    if (r >= 0 && r < POBLACION_SIZE && c >= 0 && c < POBLACION_SIZE)
                    {
                        int indice_vecino = r * POBLACION_SIZE + c;
                        CE::Objeto* obj_vecino = poblacion[indice_vecino].get();
                        JEstadoPersona* estado_vecino = getEstado(obj_vecino);

                        // Solo contagia a personas NORMALES (no enfermas, ni en recuperacion, ni inmunes permanentes)
                        if (estado_vecino && estado_vecino->estado == EstadoSalud::NORMAL)
                        {
                            // Una persona normal puede tener inmunidad parcial (no puede ser contagiada)
                            bool es_vulnerable = !estado_vecino->tiene_inmunidad_parcial;

                            if (es_vulnerable)
                            {
                                // Probabilidad individual de ser contagiado (15% a 30%)
                                float r_prob = static_cast<float>(std::rand()) / RAND_MAX;
                                if (r_prob < estado_vecino->prob_contagio)
                                {
                                    // Marcar para infección en la fase de actualizacion (coordenadas)
                                    nuevas_infecciones.push_back({r, c});
                                }
                            }
                        }
                    }
                }
            }
            // Resetear tick_de_contagio para el siguiente enfermo
            tick_de_contagio = false; 
        }
        
        // 2. Fase de Actualizacion de Estados y Timers
        for (auto& obj : poblacion)
        {
            JEstadoPersona* estado_persona = getEstado(obj.get());
            if (!estado_persona) continue;
            
            // Contador de tiempo en estado
            if (estado_persona->estado != EstadoSalud::MUERTO && estado_persona->estado != EstadoSalud::INMUNE_PERMANENTE)
            {
                estado_persona->tiempo_en_estado += dt;
            }
            
            // Logica de transicion de estados
            switch (estado_persona->estado)
            {
                case EstadoSalud::NORMAL:
                {
                    // Manejar Inmunidad Parcial (duracion de 2 segundos)
                    if (estado_persona->tiene_inmunidad_parcial)
                    {
                        estado_persona->tiempo_inm_parcial_restante -= dt;
                        if (estado_persona->tiempo_inm_parcial_restante <= 0.0f)
                        {
                            estado_persona->tiene_inmunidad_parcial = false; // Inmunidad parcial desaparece
                            estado_persona->tiempo_inm_parcial_restante = 0.0f;
                        }
                    }
                    break;
                }
                case EstadoSalud::ENFERMO:
                {
                    if (estado_persona->tiempo_en_estado >= TIEMPO_ENFERMO) // 3 segundos
                    {
                        estado_persona->estado = EstadoSalud::RECUPERACION;
                        estado_persona->tiempo_en_estado = 0.0f; // Reiniciar timer
                        // Ya no contagia, no necesita resetear tiempo_ultimo_contagio
                    }
                    break;
                }
                case EstadoSalud::RECUPERACION:
                {
                    if (estado_persona->tiempo_en_estado >= TIEMPO_RECUPERACION) // 2 segundos
                    {
                        // 1. Probabilidad de Morir (10%)
                        float r_muerte = static_cast<float>(std::rand()) / RAND_MAX;
                        if (r_muerte < estado_persona->prob_muerte)
                        {
                            estado_persona->estado = EstadoSalud::MUERTO;
                        }
                        else
                        {
                            // 2. Probabilidad de Inmunidad Permanente (1% a 10%)
                            float r_inm = static_cast<float>(std::rand()) / RAND_MAX;
                            if (r_inm < estado_persona->prob_inmunidad_permanente)
                            {
                                estado_persona->estado = EstadoSalud::INMUNE_PERMANENTE;
                            }
                            else
                            {
                                // 3. Obtiene Inmunidad Parcial y pasa a NORMAL
                                estado_persona->estado = EstadoSalud::NORMAL;
                                estado_persona->tiene_inmunidad_parcial = true;
                                estado_persona->tiempo_inm_parcial_restante = TIEMPO_INM_PARCIAL; // 2 segundos
                            }
                        }
                        estado_persona->tiempo_en_estado = 0.0f; // Reiniciar timer
                    }
                    break;
                }
                case EstadoSalud::INMUNE_PERMANENTE: // No cambia
                case EstadoSalud::MUERTO: // No cambia
                default:
                    break;
            }
        }
        
        // 3. Aplicar Nuevas Infecciones (solo si el vecino sigue siendo vulnerable)
        for (const auto& infeccion : nuevas_infecciones)
        {
            int r = infeccion.first;
            int c = infeccion.second;
            int indice = r * POBLACION_SIZE + c;
            
            JEstadoPersona* estado_infectar = getEstado(poblacion[indice].get());
            // Condicion estricta para evitar re-infeccion si cambio de estado en el mismo frame
            if (estado_infectar && estado_infectar->estado == EstadoSalud::NORMAL && !estado_infectar->tiene_inmunidad_parcial)
            {
                estado_infectar->estado = EstadoSalud::ENFERMO;
                estado_infectar->tiempo_en_estado = 0.0f;
                estado_infectar->tiempo_ultimo_contagio = 0.0f; // Empezar el contador de contagio
                estado_infectar->tiene_inmunidad_parcial = false; 
            }
        }
        
        // 4. Actualizar Estadisticas y Colores
        int total_enfermos = 0;
        int total_muertos = 0;
        for (auto& obj : poblacion)
        {
            JEstadoPersona* estado_persona = getEstado(obj.get());
            if (!estado_persona) continue;
            
            // Actualizar Estadisticas
            switch (estado_persona->estado)
            {
                case EstadoSalud::NORMAL: stats.normal++; break;
                case EstadoSalud::ENFERMO: stats.enfermo++; total_enfermos++; break;
                case EstadoSalud::RECUPERACION: stats.recuperacion++; break;
                case EstadoSalud::INMUNE_PERMANENTE: stats.inmune++; break;
                case EstadoSalud::MUERTO: stats.muerto++; total_muertos++; break;
            }
            
            // Actualizar Color Visual
            ActualizarColor(obj.get(), *estado_persona);
        }

        // 5. Condicion de Terminacion
        if (total_enfermos == 0 || total_muertos == POBLACION_SIZE * POBLACION_SIZE)
        {
            // Esta logica DEBE ser manejada en la clase Juego, ya que tiene acceso a 'termino'.
            // Aqui solo emitimos un mensaje para depuracion.
            // std::cout << "--- SIMULACION TERMINADA (Enfermos: " << total_enfermos << ", Muertos: " << total_muertos << ") ---" << std::endl;
        }
    }

    // El codigo de InicializarPoblacion es correcto. Solo añado el uso de std::shuffle/std::iota
    void InicializarPoblacion(std::vector<std::shared_ptr<CE::Objeto>>& poblacion)
    {
        //std::srand(std::time(nullptr)); 
        
        const int total_personas = POBLACION_SIZE * POBLACION_SIZE;
        const int num_enfermos_inicial = static_cast<int>(total_personas * 0.10);

        std::vector<int> indices(total_personas);
        std::iota(indices.begin(), indices.end(), 0);
        
        // 1. Crear un generador de numeros aleatorios
        std::random_device rd;
        std::mt19937 g(rd());

        // 2. Usar std::shuffle para mezclar los índices
        std::shuffle(indices.begin(), indices.end(), g);
        
        poblacion.reserve(total_personas);
        
        const float rect_size = 30.0f;
        const float spacing = 10.0f;
        // Posiciones iniciales centradas para una mejor visualización
        //const float start_x = 1440.0f / 2.0f - (POBLACION_SIZE * (rect_size + spacing)) / 2.0f;
        const float start_x = 500.0f;
        const float start_y = 720.0f / 2.0f - (POBLACION_SIZE * (rect_size + spacing)) / 2.0f;

        for (int r = 0; r < POBLACION_SIZE; ++r)
        {
            for (int c = 0; c < POBLACION_SIZE; ++c)
            {
                int indice = r * POBLACION_SIZE + c;
                
                auto rect = std::make_shared<Rectangulo>(rect_size, rect_size, sf::Color::White, sf::Color::Black);
                rect->setPosicion(start_x + c * (rect_size + spacing), start_y + r * (rect_size + spacing));
                
                auto estado = std::make_shared<JEstadoPersona>(r, c);
                
                // Marcar el 10% inicial de enfermos (usando los índices mezclados)
                // Comprobamos si el índice actual está en el rango de los índices seleccionados para infección
                bool es_enfermo_inicial = std::find(indices.begin(), indices.begin() + num_enfermos_inicial, indice) != indices.begin() + num_enfermos_inicial;

                if (es_enfermo_inicial)
                {
                    estado->estado = EstadoSalud::ENFERMO;
                    estado->tiempo_ultimo_contagio = 0.0f; // Inicializa el tick de contagio
                }
                
                ActualizarColor(rect.get(), *estado);
                
                rect->addComponente(estado);
                poblacion.push_back(rect);
            }
        }
    }
}