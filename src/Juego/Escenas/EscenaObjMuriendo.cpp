#include "EscenaObjMuriendo.hpp" // Incluimos la cabecera
#include "../Figuras/Figuras.hpp"          
#include <random>                          
#include <ctime>                           
#include <iostream>
#include <algorithm> 
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Primitivos/GestorAssets.hpp" 
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ
{
    // Definimos los límites de la pantalla (asumiendo 1280x720)
    const float LIMITE_X = 1280.0f; 
    const float LIMITE_Y = 720.0f;

    // ***************************************************************
    // Implementación de TimerDeVida::onUpdate (Declarado en HPP)
    // ***************************************************************
    void TimerDeVida::onUpdate(float dt)
    {
        if (expirado) return;

        // Limita el delta time (tiempo de salto) para evitar que objetos mueran al instante
        dt = std::min(dt, 0.1f); 
        
        tiempoVidaRestante -= dt;
        if (tiempoVidaRestante <= 0.0f)
        {
            expirado = true;
        }
    }


    void EscenaObjMuriendo::onInit()
    {
        if(!inicializar) return;

        std::cout << "EscenaObjMuriendo: Inicializando..." << std::endl;
        objetos.borrarPool();

        // Usaremos el mismo path para todos, pero diferentes coordenadas y nombres clave.
        const std::string RUTA_ATLAS = ASSETS "/sprites/aliens/enemies.png";

        // Tipo 1: Bat (Vuelo)
        // <SubTexture name="bat_fly.png" x="0" y="0" width="88" height="37"/>
        CE::GestorAssets::Get().agregarTextura("enemigo_bat", RUTA_ATLAS, CE::Vector2D{0, 0}, CE::Vector2D{88, 37});
        
        // Tipo 2: Bee (Normal)
        // <SubTexture name="bee.png" x="315" y="353" width="56" height="48"/>
        CE::GestorAssets::Get().agregarTextura("enemigo_bee", RUTA_ATLAS, CE::Vector2D{315, 353}, CE::Vector2D{56, 48});
        
        // Tipo 3: Frog (Normal)
        // <SubTexture name="frog.png" x="257" y="45" width="58" height="39"/>
        CE::GestorAssets::Get().agregarTextura("enemigo_frog", RUTA_ATLAS, CE::Vector2D{257, 45}, CE::Vector2D{58, 39});
        
        // Tipo 4: Mouse (Caminando)
        // <SubTexture name="mouse_walk.png" x="256" y="475" width="58" height="35"/>
        CE::GestorAssets::Get().agregarTextura("enemigo_mouse", RUTA_ATLAS, CE::Vector2D{256, 475}, CE::Vector2D{58, 35});
        
        // Tipo 5: Spider (Normal)
        // <SubTexture name="spider.png" x="0" y="326" width="71" height="45"/>
        CE::GestorAssets::Get().agregarTextura("enemigo_spider", RUTA_ATLAS, CE::Vector2D{0, 326}, CE::Vector2D{71, 45});

        // 2. LISTA DE CLAVES DE TEXTURAS A USAR
        const std::vector<std::string> CLAVES_ENEMIGOS = {
            "enemigo_bat", 
            "enemigo_bee", 
            "enemigo_frog", 
            "enemigo_mouse", 
            "enemigo_spider"
        };

        // ------------------ Generacion Aleatoria ------------------
        std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
        std::uniform_real_distribution<> distrib_pos_x(50.0, LIMITE_X - 50.0);
        std::uniform_real_distribution<> distrib_pos_y(50.0, LIMITE_Y - 50.0);
        std::uniform_real_distribution<> distrib_escala(0.5, 3.0);
        std::uniform_real_distribution<> distrib_vida(3.0, 15.0); // Vida entre 3 y 15 segundos

        // DISTRIBUCIÓN PARA ELEGIR UNA CLAVE DE LA LISTA
        std::uniform_int_distribution<> distrib_sprite(0, CLAVES_ENEMIGOS.size() - 1);

        const int NUM_ENEMIGOS = 100;

        // Creamos y configuramos 100 objetos
        for (int i = 0; i < NUM_ENEMIGOS; ++i)
        {
            auto alien = std::make_shared<IVJ::Entidad>();
            float escala = static_cast<float>(distrib_escala(gen));

            // --- SELECCIÓN ALEATORIA DEL SPRITE Y ASIGNACIÓN ---
            int indice_sprite = distrib_sprite(gen);
            const std::string& clave_sprite = CLAVES_ENEMIGOS[indice_sprite];
            sf::Texture& textura_enemigo = CE::GestorAssets::Get().getTextura(clave_sprite);

            // AÑADIDO: Añadimos el componente ISprite, pasando la referencia a la textura.
            alien->addComponente(
                std::make_shared<CE::ISprite>(textura_enemigo, escala)
            );
            
            // 1. POSICION Y VELOCIDAD
            alien->setPosicion(
                static_cast<float>(distrib_pos_x(gen)),
                static_cast<float>(distrib_pos_y(gen))
            );

            alien->getTransformada().get()->velocidad.x = static_cast<float>(std::uniform_real_distribution<>(-100.0, 100.0)(gen));
            alien->getTransformada().get()->velocidad.y = static_cast<float>(std::uniform_real_distribution<>(-100.0, 100.0)(gen));
            
            // 2. HP y Componentes
            alien->getStats()->hp = 1;
            // Añadimos el componente TimerDeVida con tiempo aleatorio
            alien->addComponente(std::make_shared<TimerDeVida>(static_cast<float>(distrib_vida(gen)))); 
            alien->addComponente(std::make_shared<CE::IMotion>());

            objetos.agregarPool(alien);

            // Uso de la variable de miembro 'enemigos' para apuntar al primer alien creado
            if (i == 0)
                enemigos = alien;

            /*
            float radio = static_cast<float>(distrib_radio(gen));
            
            sf::Color color_relleno(
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen))
            );

            // Creamos el objeto Circulo
            auto circulo = std::make_shared<Circulo>(
                radio, color_relleno, sf::Color::Black
            );

            // 1. POSICION Y VELOCIDAD
            circulo->setPosicion(
                static_cast<float>(distrib_pos_x(gen)),
                static_cast<float>(distrib_pos_y(gen))
            );

            circulo->getTransformada().get()->velocidad.x = static_cast<float>(std::uniform_real_distribution<>(-100.0, 100.0)(gen));
            circulo->getTransformada().get()->velocidad.y = static_cast<float>(std::uniform_real_distribution<>(-100.0, 100.0)(gen));
            
            // 2. HP y Componentes
            circulo->getStats()->hp = 1;
            // Añadimos el componente TimerDeVida con tiempo aleatorio
            circulo->addComponente(std::make_shared<TimerDeVida>(static_cast<float>(distrib_vida(gen)))); 
            circulo->addComponente(std::make_shared<CE::IMotion>());

            objetos.agregarPool(circulo);
            */
        }
        
        std::cout << "EscenaObjMuriendo inicializada. Total de objetos: " << objetos.getPool().size() << std::endl;

        inicializar = false;
    }

    void EscenaObjMuriendo::onFinal()
    {
        std::cout << "EscenaObjMuriendo: Finalizando..." << std::endl;
        objetos.borrarPool();
    }

    void EscenaObjMuriendo::onUpdate(float dt)
    {
        // 1. Actualización de Componentes (ejecuta TimerDeVida::onUpdate(dt))
        for(auto &f: objetos.getPool())
        {
            f->onUpdate(dt);
            if (auto timer = f->getComponente<TimerDeVida>()) 
                timer->onUpdate(dt);
        }

        // 2. SISTEMA DE MOVIMIENTO/FÍSICA y DETECCIÓN DE LÍMITES
        for(auto &f: objetos.getPool())
        {
            CE::ITransform* t = f->getTransformada().get();
            
            // Aplicar la física (Movimiento)
            t->posicion.x += t->velocidad.x * dt;
            t->posicion.y += t->velocidad.y * dt;

            // Detección y rebote en los límites de la pantalla (usando el origen/centro)
            // Rebote en X
            if (t->posicion.x < 0.0f) 
            {
                t->posicion.x = 0.0f; 
                t->velocidad.x *= -1.0f; 
            }
            else if (t->posicion.x > LIMITE_X) 
            {
                t->posicion.x = LIMITE_X; 
                t->velocidad.x *= -1.0f;
            }

            // Rebote en Y
            if (t->posicion.y < 0.0f) 
            {
                t->posicion.y = 0.0f; 
                t->velocidad.y *= -1.0f;
            }
            else if (t->posicion.y > LIMITE_Y) 
            {
                t->posicion.y = LIMITE_Y; 
                t->velocidad.y *= -1.0f;
            }

            f->setPosition(sf::Vector2f(t->posicion.x, t->posicion.y));
        }
        
        // 3. SISTEMA DE MUERTE POR TIEMPO: Revisa el Timer y MARCA para borrado (hp=0).
        for(auto &f: objetos.getPool())
        {
            // La plantilla getComponente ahora funciona.
            if (auto timer = f->getComponente<TimerDeVida>()) 
            {
                if (timer->estaExpirado())
                {
                    // Si expiró, la escena establece la HP a 0.
                    if (f->getStats())
                    {
                        std::cout << "Objeto ha expirado. HP=0." << std::endl;
                        f->getStats()->hp = 0;
                    }
                }
            }
        }

        // 4. Borramos los objetos cuya HP haya llegado a 0
        objetos.borrarPool();

        // 5. Comprobamos la condicion de cambio de escena
        if (objetos.getPool().size() == 0)
        {
            std::cout << "Todos los objetos han muerto. Cambiando a EscenaCirculos." << std::endl;
            CE::GestorEscenas::Get().cambiarEscena("Circulos");
        }
    }

    void EscenaObjMuriendo::onInputs(const CE::Botones& accion)
    {
        // No hay inputs definidos para esta escena.
    }

    void EscenaObjMuriendo::onRender()
    {
        // Agregamos todos los objetos al renderizador
        for (auto &f: objetos.getPool())
            CE::Render::Get().AddToDraw(*f);
    }
}


