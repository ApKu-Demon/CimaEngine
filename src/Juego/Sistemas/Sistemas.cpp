#include "Sistemas.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ
{
    constexpr float WIDTH = 1440.f;
    constexpr float HEIGHT = 900.f;

    void SistemaMovimientoEntes(const std::vector<std::shared_ptr<CE::Objeto>>& entes, float dt)
    {
        bool state = 0;
        for(auto& e : entes)
        {
            /*
            if(e->tieneComponente<CE::ITimer>())
            {
                auto& timer = *e->getComponente<CE::ITimer>();
                auto& trans = *e->getTransformada();
                trans.posicion.x += 25*dt;
                trans.posicion.y += 5*dt;
                timer.frame_actual++;
            }
            */
            
            if(!e->tieneComponente<CE::ITimer>())
                continue;

            auto& mov = *e->getComponente<CE::IMotion>();
            auto& timer = *e->getComponente<CE::ITimer>();
            auto& trans = *e->getTransformada();
            auto& nombre = *e->getNombre();

            //timer.frame_actual++;

            if (nombre.nombre.find("Rectangulo") != std::string::npos)
            {
                trans.posicion.y += mov.direccion.y * mov.velocidad * dt;

                if (trans.posicion.y > HEIGHT || trans.posicion.y < 0)
                {
                    mov.direccion.y *= -1.f;
                    trans.posicion.y += mov.direccion.y * mov.velocidad * 2 * dt;
                }
            }

            else if (nombre.nombre.find("Triangulo") != std::string::npos)
            {
                trans.posicion.x += mov.direccion.x * mov.velocidad * dt;

                if (!mov.inicializado) 
                {
                    mov.punto_fijo.y = trans.posicion.y;
                    mov.inicializado = true;
                }

                trans.posicion.y = mov.punto_fijo.y + 50.f * std::sin(trans.posicion.x * 0.05f);

                if (trans.posicion.x > WIDTH || trans.posicion.x < 0)
                {
                    mov.direccion.x *= -1.f;
                    trans.posicion.x += mov.direccion.x * mov.velocidad * 2 * dt;
                }
            }

            else if (nombre.nombre.find("Circulo") != std::string::npos)
            {
                timer.frame_actual++;

                float radio = 100.f;
                float angulo = timer.frame_actual * 0.05f;

                if (!mov.inicializado) 
                {
                    mov.punto_fijo = trans.posicion;
                    mov.inicializado = true;
                }

                trans.posicion.x = mov.punto_fijo.x + radio * std::cos(angulo);
                trans.posicion.y = mov.punto_fijo.y + radio * std::sin(angulo);
            }
        }
    }
}