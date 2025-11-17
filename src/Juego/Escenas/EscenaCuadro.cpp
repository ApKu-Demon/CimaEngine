#include "EscenaCuadro.hpp"
#include "../Figuras/Figuras.hpp"
#include "../../Motor/Camaras/CamarasGestor.hpp"
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Utils/Vector2D.hpp"
#include "../../Motor/Primitivos/GestorAssets.hpp"
#include "../Sistemas/Sistemas.hpp"

namespace IVJ
{
    void EscenaCuadros::onInit()
    {
        if(!inicializar) return;

        // cargamos las texturas correspondientes a esta escena
        CE::GestorAssets::Get().agregarTextura("pink", ASSETS "/sprites/aliens/alienPink.png", CE::Vector2D{70, 92}, CE::Vector2D{66, 92});

        // registramos el esquema de los botones
        registrarBotones(sf::Keyboard::Scancode::W, "arriba");
        registrarBotones(sf::Keyboard::Scancode::Up, "arriba");
        registrarBotones(sf::Keyboard::Scancode::S, "abajo");
        registrarBotones(sf::Keyboard::Scancode::Down, "abajo");
        registrarBotones(sf::Keyboard::Scancode::A, "izquierda");
        registrarBotones(sf::Keyboard::Scancode::Left, "arriba");
        registrarBotones(sf::Keyboard::Scancode::D, "derecha");
        registrarBotones(sf::Keyboard::Scancode::Right, "derecha");
        registrarBotones(sf::Keyboard::Scancode::Escape, "circulos");

        // creamos la entidad para probar el sprite
        jugador = std::make_shared<Entidad>();
        jugador->getStats()->hp = 100;
        jugador->setPosicion(500.f, 500.f);
        jugador->addComponente(std::make_shared<CE::ISprite>(CE::GestorAssets::Get().getTextura("pink"), 1.f));
        objetos.agregarPool(jugador);

        // vamos hacer 3 figuras
        /*
        auto fig1 = std::make_shared<Rectangulo>(
            100, 100, sf::Color(255, 0, 0, 255),
            sf::Color(0, 0, 0, 255));
        fig1->setPosicion(300, 400);
        fig1->getStats()->hp = 100;

        auto fig2 = std::make_shared<Rectangulo>(
            200, 100, sf::Color(169, 169, 0, 255),
            sf::Color(0, 0, 0, 255));
        fig2->setPosicion(100, 100);
        fig2->getStats()->hp = 100;

        auto fig3 = std::make_shared<Rectangulo>(
            100, 200, sf::Color(0, 0, 255, 255),
            sf::Color(0, 0, 0, 255));
        fig3->setPosicion(300, 600);
        fig3->getStats()->hp = 100;

        objetos.agregarPool(fig1);
        objetos.agregarPool(fig2);
        objetos.agregarPool(fig3);
        */

        // agregamos una camara
        CE::GestorCamaras::Get().agregarCamara(std::make_shared<CE::CamaraCuadro>(
            CE::Vector2D{540, 360}, CE::Vector2D{1080.f, 720.f}));
        CE::GestorCamaras::Get().setCamaraAciva(1);

        // lockeamos la camara en un objeto
        CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(jugador);
        inicializar = false;
    }

    void EscenaCuadros::onFinal(){}

    void EscenaCuadros::onUpdate(float dt)
    {
        SistemaMovimientoEntes(objetos.getPool(), dt);      //OJO ! ! ! ! ! !
        for(auto &f: objetos.getPool())
            f->onUpdate(dt);
        objetos.borrarPool();
    }

    void EscenaCuadros::onInputs(const CE::Botones& accion)
    {
        auto p = jugador->getTransformada();
        if (accion.getTipo() == CE::Botones::TipoAccion::OnPress)
        {
            if (accion.getNombre() == "arriba")
            {
                p->velocidad.y = -800;
            }
            else if (accion.getNombre() == "derecha")
            {
                p->velocidad.x = 800;
            }
            else if (accion.getNombre() == "abajo")
            {
                p->velocidad.y =+ 800;
            }
            else if (accion.getNombre() == "izquierda")
            {
                p->velocidad.x =- 800;
            }
            else if (accion.getNombre() == "circulos")
            {
                CE::GestorEscenas::Get().cambiarEscena("Circulos");
            }
        }
        else if (accion.getTipo() == CE::Botones::TipoAccion::OnRelease)
        {
            if (accion.getNombre() == "arriba")
            {
                p->velocidad.y = 0;
            }
            else if (accion.getNombre() == "derecha")
            {
                p->velocidad.x += 0;
            }
            else if (accion.getNombre() == "abajo")
            {
                p->velocidad.y = 0;
            }
            else if (accion.getNombre() == "izquierda")
            {
                p->velocidad.x = 0;
            }
        }
    }

    void EscenaCuadros::onRender()
    {
        for (auto &f: objetos.getPool())
            CE::Render::Get().AddToDraw(*f);
    }
}


