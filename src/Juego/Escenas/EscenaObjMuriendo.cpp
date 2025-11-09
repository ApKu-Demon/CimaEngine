#include "EscenaObjMuriendo.hpp" // Incluimos la cabecera
#include "../Figuras/Figuras.hpp"          
#include <random>                          
#include <ctime>                           
#include <iostream>
#include <algorithm> 
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"

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

        // ------------------ Generacion Aleatoria ------------------
        std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
        std::uniform_real_distribution<> distrib_pos_x(50.0, LIMITE_X - 50.0);
        std::uniform_real_distribution<> distrib_pos_y(50.0, LIMITE_Y - 50.0);
        std::uniform_real_distribution<> distrib_radio(10.0, 40.0);
        std::uniform_real_distribution<> distrib_vida(3.0, 15.0); // Vida entre 3 y 15 segundos

        // Creamos y configuramos 50 objetos
        for (int i = 0; i < NUM_OBJETOS; ++i)
        {
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

/*  #2
#include "EscenaObjMuriendo.hpp"
#include "../Figuras/Figuras.hpp"          
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Componentes/IComponentes.hpp" 
#include <random>                          
#include <ctime>                           
#include <iostream>
#include <algorithm> 

namespace IVJ
{
    // Definimos los límites de la pantalla (ajusta si tu ventana es diferente)
    // Asumiendo una ventana de 1280x720 como estándar
    const float LIMITE_X = 1080.0f; 
    const float LIMITE_Y = 720.0f;

    // TimerDeVida extiende CE::ITimer para darle la funcionalidad de cuenta regresiva.
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

        // Decrementa el tiempo restante.
        void onUpdate(float dt) 
        {
            if (expirado) return;

            // CLAVE: Limitamos el dt (salto de tiempo) para evitar la muerte instantánea
            dt = std::min(dt, 0.1f); 
            
            tiempoVidaRestante -= dt;
            if (tiempoVidaRestante <= 0.0f)
            {
                expirado = true;
            }
        }
    };


    void EscenaObjMuriendo::onInit()
    {
        if(!inicializar) return;

        std::cout << "EscenaObjMuriendo: Inicializando..." << std::endl;
        objetos.borrarPool(); 

        // ------------------ Generacion Aleatoria ------------------
        std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
        // Usamos un rango más seguro para evitar que el origen empiece fuera de la pantalla.
        std::uniform_real_distribution<> distrib_pos_x(50.0, LIMITE_X - 50.0);
        std::uniform_real_distribution<> distrib_pos_y(50.0, LIMITE_Y - 50.0);
        std::uniform_real_distribution<> distrib_radio(10.0, 40.0);
        std::uniform_real_distribution<> distrib_vida(3.0, 15.0);

        // Creamos y configuramos 50 objetos
        for (int i = 0; i < NUM_OBJETOS; ++i)
        {
            float radio = static_cast<float>(distrib_radio(gen));
            
            sf::Color color_relleno(
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen))
            );

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
            circulo->addComponente(std::make_shared<TimerDeVida>(static_cast<float>(distrib_vida(gen))));
            circulo->addComponente(std::make_shared<CE::IMotion>());

            objetos.agregarPool(circulo);
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
        // 1. Actualización de Componentes (Temporizadores, etc.)
        for(auto &f: objetos.getPool())
        {
            f->onUpdate(dt);
        }

        // 2. SISTEMA DE MOVIMIENTO/FÍSICA y DETECCIÓN DE LÍMITES
        for(auto &f: objetos.getPool())
        {
            CE::ITransform* t = f->getTransformada().get();
            
            // Aplicar la física (Movimiento)
            t->posicion.x += t->velocidad.x * dt;
            t->posicion.y += t->velocidad.y * dt;

            // Detección y rebote en los límites de la pantalla
            // **SOLUCIÓN SIN USAR EL RADIO:** Verificamos si el origen (el centro) cruza 0 o el límite máximo.

            // Rebote en X
            if (t->posicion.x < 0.0f) 
            {
                t->posicion.x = 0.0f; // Lo reposiciona en el borde
                t->velocidad.x *= -1.0f; // Invierte la dirección
            }
            else if (t->posicion.x > LIMITE_X) 
            {
                t->posicion.x = LIMITE_X; // Lo reposiciona en el borde
                t->velocidad.x *= -1.0f;
            }

            // Rebote en Y
            if (t->posicion.y < 0.0f) 
            {
                t->posicion.y = 0.0f; // Lo reposiciona en el borde
                t->velocidad.y *= -1.0f;
            }
            else if (t->posicion.y > LIMITE_Y) 
            {
                t->posicion.y = LIMITE_Y; // Lo reposiciona en el borde
                t->velocidad.y *= -1.0f;
            }
        }
        
        // 3. SISTEMA DE MUERTE POR TIEMPO: Revisa el Timer y MARCA para borrado (hp=0).
        for(auto &f: objetos.getPool())
        {
            if (auto timer = f->getComponente<TimerDeVida>()) 
            {
                if (timer->estaExpirado())
                {
                    // Si expiró, la escena establece la HP a 0.
                    if (f->getStats())
                    {
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
} */

/* #1
#include "EscenaObjMuriendo.hpp"
#include "../Figuras/Figuras.hpp"          
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Componentes/IComponentes.hpp" // Componentes de Movimiento y Tiempo (ITimer, IMotion)
#include <random>                          
#include <ctime>                           
#include <iostream>

namespace IVJ
{
    // TimerDeVida extiende CE::ITimer para darle la funcionalidad de cuenta regresiva.
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

            // Decrementa el tiempo restante.
            void onUpdate(float dt)
            {
                if (expirado) return;

                // CLAVE para evitar la muerte instantánea: Limitamos el dt (salto de tiempo)
                dt = std::min(dt, 0.1f); // Limita el delta time a 100ms

                tiempoVidaRestante -= dt;
                if (tiempoVidaRestante <= 0.0f)
                {
                    expirado = true;
                }
            }
    };


    void EscenaObjMuriendo::onInit()
    {
        if(!inicializar) return;

        std::cout << "EscenaObjMuriendo: Inicializando..." << std::endl;
        objetos.borrarPool();

        // ------------------ Generacion Aleatoria ------------------
        std::mt19937 gen(static_cast<unsigned int>(std::time(0)));
        std::uniform_real_distribution<> distrib_pos_x(100.0, 1000.0);
        std::uniform_real_distribution<> distrib_pos_y(100.0, 600.0);
        std::uniform_real_distribution<> distrib_radio(10.0, 40.0);
        // El tiempo de vida estará entre 3 y 15 segundos
        std::uniform_real_distribution<> distrib_vida(3.0, 15.0);

        // Creamos y configuramos 50 objetos
        for (int i = 0; i < NUM_OBJETOS; ++i)
        {
            float radio = static_cast<float>(distrib_radio(gen));
            
            // Color aleatorio para el relleno
            sf::Color color_relleno(
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen)),
                static_cast<std::uint8_t>(std::uniform_int_distribution<>(50, 255)(gen))
            );

            auto circulo = std::make_shared<Circulo>(
                radio, color_relleno, sf::Color::Black
            );

            // 1. POSICION Y VELOCIDAD (Usando ITransform, integrado en el objeto)
            circulo->setPosicion(
                static_cast<float>(distrib_pos_x(gen)),
                static_cast<float>(distrib_pos_y(gen))
            );

            // Asignar una velocidad inicial aleatoria
            circulo->getTransformada()->velocidad.x = static_cast<float>(std::uniform_real_distribution<>(-50.0, 50.0)(gen));
            circulo->getTransformada()->velocidad.y = static_cast<float>(std::uniform_real_distribution<>(-50.0, 50.0)(gen));

            // 2. HP (Usando IStats, integrado en el objeto)
            circulo->getStats()->hp = 1;
            
            // 3. TIEMPO DE MUERTE (Usando CE::ITimer a través de TimerDeVida)
            float tiempo_mortal = static_cast<float>(distrib_vida(gen));
            circulo->addComponente(std::make_shared<TimerDeVida>(tiempo_mortal));
            
            // 4. MOVIMIENTO ADICIONAL (Usando CE::IMotion)
            // Se añade el componente para cumplir con el requisito de usarlo.
            circulo->addComponente(std::make_shared<CE::IMotion>());

            objetos.agregarPool(circulo);
        }
        
        std::cout << "EscenaObjMuriendo inicializada. Total de objetos: " << objetos.getPool().size() << std::endl;

        inicializar = false;
    }

    void EscenaObjMuriendo::onFinal()
    {
        // Limpiamos la escena al terminar
        std::cout << "EscenaObjMuriendo: Finalizando..." << std::endl;
        objetos.borrarPool();
    }

    void EscenaObjMuriendo::onUpdate(float dt)
    {
        for(auto &f: objetos.getPool())
        {
            f->onUpdate(dt);
        }

        // 2. SISTEMA DE MOVIMIENTO/FÍSICA: Aplica la velocidad a la posición (MOVIMIENTO)
        // Ya que IMotion no tiene método de actualización, la escena debe aplicar la física.
        for(auto &f: objetos.getPool())
        {
            CE::ITransform* t = f->getTransformada().get();
            t->posicion.x += t->velocidad.x * dt;
            t->posicion.y += t->velocidad.y * dt;
        }
        
        // 3. SISTEMA DE MUERTE POR TIEMPO: Revisa el Timer y MARCA para borrado (hp=0).
        // ESTE BUCLE ES FUNDAMENTAL para que los objetos mueran.
        for(auto &f: objetos.getPool())
        {
            if (auto timer = f->getComponente<TimerDeVida>()) 
            {
                if (timer->estaExpirado())
                {
                    // Si expiró, la escena establece la HP a 0.
                    if (f->getStats())
                    {
                        f->getStats()->hp = 0;
                    }
                }
            }
        }

        // 2. Borramos los objetos cuya HP haya llegado a 0 (los que "murieron" por tiempo)
        objetos.borrarPool();

        // 3. Comprobamos la condicion de cambio de escena
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
*/