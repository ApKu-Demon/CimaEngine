#include "EscenaCuadro.hpp"
#include "../Figuras/Figuras.hpp"
#include "../../Motor/Camaras/CamarasGestor.hpp"
#include "../../Motor/Primitivos/GestorEscenas.hpp"
#include "../../Motor/Render/Render.hpp"
#include "../../Motor/Utils/Vector2D.hpp"
#include "../../Motor/Primitivos/GestorAssets.hpp"
#include "../Sistemas/Sistemas.hpp"
#include "../../Motor/Utils/Lerp.hpp"

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
        registrarBotones(sf::Keyboard::Scancode::Left, "izquierda");
        registrarBotones(sf::Keyboard::Scancode::D, "derecha");
        registrarBotones(sf::Keyboard::Scancode::Right, "derecha");
        registrarBotones(sf::Keyboard::Scancode::Escape, "circulos");

        // creamos la entidad para probar el sprite
        jugador = std::make_shared<Entidad>();
        jugador->getStats()->hp = 100;
        jugador->setPosicion(500.f, 500.f);
        jugador->addComponente(std::make_shared<CE::ISprite>(CE::GestorAssets::Get().getTextura("pink"), 1.f));
        objetos.agregarPool(jugador);

        // figuras
        auto fig1 = std::make_shared<Rectangulo>(
            100, 100, sf::Color(255, 0, 0, 255),
            sf::Color(0, 0, 0, 255));
        fig1->setPosicion(100, 400);
        fig1->getStats()->hp = 100;

        auto fig2 = std::make_shared<Rectangulo>(
            200, 100, sf::Color(169, 169, 0, 255),
            sf::Color(0, 0, 0, 255));
        fig2->setPosicion(300, 100);
        fig2->getStats()->hp = 100;

        auto fig3 = std::make_shared<Rectangulo>(
            100, 200, sf::Color(0, 0, 255, 255),
            sf::Color(0, 0, 0, 255));
        fig3->setPosicion(600, 400);
        fig3->getStats()->hp = 100;

        auto fig4 = std::make_shared<Circulo>(
            10, sf::Color(255, 255, 255, 255),
            sf::Color(0, 0, 0, 255));
        fig4->setPosicion(100, 400);
        fig4->getStats()->hp = 100;

        objetos.agregarPool(fig1);
        objetos.agregarPool(fig2);
        objetos.agregarPool(fig3);
        objetos.agregarPool(fig4);

        // agregamos una camara
        CE::GestorCamaras::Get().agregarCamara(std::make_shared<CE::CamaraSeguimientoLerp>(
            CE::Vector2D{540, 360}, CE::Vector2D{1080.f, 720.f}));
        CE::GestorCamaras::Get().setCamaraAciva(1);

        // lockeamos la camara en un objeto
        CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(jugador);
        inicializar = false;

        if(!bg[0].loadTileMap(ASSETS "/mapas/mapa_1_layer_4.txt"))
            exit(EXIT_FAILURE);
        if(!bg[1].loadTileMap(ASSETS "/mapas/mapa_1_layer_2.txt"))
            exit(EXIT_FAILURE);
    }

    void EscenaCuadros::onFinal(){}

    void EscenaCuadros::onUpdate(float dt)
    {
        //SistemaMovimientoEntes(objetos.getPool(), dt);      //OJO ! ! ! ! ! !

        auto p = jugador->getTransformada();
        p->posicion.x += p->velocidad.x * dt;
        p->posicion.y += p->velocidad.y * dt;

        // LERP CUBICA
        // const float max_frames = 120.f;
        // static int tiempo_frame = 0;
        // static unsigned char reversa = 0;
        // const CE::Vector2D inicio = objetos[0]->getTransformada()->posicion;
        // const CE::Vector2D control1 = objetos[1]->getTransformada()->posicion;
        // const CE::Vector2D control2 = objetos[2]->getTransformada()->posicion;
        // const CE::Vector2D destino = objetos[3]->getTransformada()->posicion;
        // auto npos = CE::lerp3(inicio, control1, control2, destino, tiempo_frame / max_frames);
        // objetos[4]->setPosicion(npos.x,npos.y);

        // LERP CUADRADA
        /*
        const float max_frames = 120.f;
        static int tiempo_frame = 0;
        static unsigned char reversa = 0;
        const CE::Vector2D inicio = objetos[0]->getTransformada()->posicion;
        const CE::Vector2D control = objetos[1]->getTransformada()->posicion;
        const CE::Vector2D destino = objetos[2]->getTransformada()->posicion;
        auto npos = CE::lerp2(inicio, control, destino, tiempo_frame / max_frames);
        objetos[4]->setPosicion(npos.x,npos.y);
        */

        // const float max_frames = 120.f;
        // static int tiempo_frame = 0;
        // static unsigned char reversa = 0;
        
        // if(tiempo_frame > max_frames)
        //     reversa = 1;
        // if(tiempo_frame < 0)
        //     reversa = 0;
        // if(reversa == 1)
        //     tiempo_frame--;
        // else
        //     tiempo_frame++;

        //------UPDATE GENERAL DE OBJETOS------
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
                p->velocidad.x =+ 800;
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
                p->velocidad.x = 0;
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
        for(auto b: bg)
            CE::Render::Get().AddToDraw(b);

        for (auto &f: objetos.getPool())
            CE::Render::Get().AddToDraw(*f);
    }
}


