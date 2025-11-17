#include "Juego.hpp"
#include <SFML/Graphics.hpp>
#include "../Motor/Render/Render.hpp"
#include <iostream>
#include <fstream>      // Para leer archivos
#include <sstream>      // Para procesar lineas
//#include "../Motor/Utils/Vector2D.hpp"
#include "Sistemas/Sistemas.hpp"
#include "../Motor/CargarFig/CargarFiguras.hpp"
#include "../Motor/Utils/Lista.hpp"
#include "../Motor/GUI/GLogger.hpp"
#include "../Motor/Camaras/CamarasGestor.hpp"
#include "../Motor/Primitivos/GestorEscenas.hpp"
#include "Escenas/EscenaCuadro.hpp"
#include "Escenas/EscenaCirculos.hpp"
#include "Escenas/EscenaObjMuriendo.hpp"

namespace IVJ
{
    Juego::Juego(void)
        :CE::GameLayer{}, texto{font_juego}, texto_stats{font_juego}
    {
        termino=false;
        //objetos = &lista_objetos;
    }

    void Juego::OnInit(void)
    {
        CE::GestorEscenas::Get().registrarEscena("Cuadros", std::make_shared<EscenaCuadros>());
        CE::GestorEscenas::Get().registrarEscena("Circulos", std::make_shared<EscenaCirculos>());
        CE::GestorEscenas::Get().registrarEscena("ObjMuriendo", std::make_shared<EscenaObjMuriendo>());    //
        CE::GestorEscenas::Get().cambiarEscena("ObjMuriendo");
        escena_actual = &CE::GestorEscenas::Get().getEscenaActual();
    }

    void Juego::OnInputs(float dt,std::optional<sf::Event>& eventos)
    {
        if(eventos->getIf<sf::Event::Closed>())
            termino=true;

        // INPUTS a las escenas
        CE::Botones::TipoAccion tipo_accion = CE::Botones::TipoAccion::None;
        std::string strAccion = "None";
        sf::Keyboard::Scancode scan = sf::Keyboard::Scancode::Comma;

        if(eventos->is<sf::Event::KeyPressed>())
        {
            const auto e = eventos->getIf<sf::Event::KeyPressed>();
            tipo_accion = CE::Botones::TipoAccion::OnPress;

            if(e)
            {
                scan = e->scancode;
                if(escena_actual->getBotones().find(scan) == escena_actual->getBotones().end()) return;
                strAccion = escena_actual->getBotones().at(scan);
            }
        }
        else    // on KeyRelease
        {
            const auto e = eventos->getIf<sf::Event::KeyReleased>();
            tipo_accion = CE::Botones::TipoAccion::OnRelease;

            if(e)
            {
                scan = e->scancode;
                if(escena_actual->getBotones().find(scan) == escena_actual->getBotones().end()) return;
                strAccion = escena_actual->getBotones().at(scan);
            }
        }
        escena_actual->onInputs(CE::Botones(strAccion, tipo_accion, scan));
    }

    void Juego::OnUpdate(float dt)
    {
        escena_actual = &CE::GestorEscenas::Get().getEscenaActual();
        escena_actual->onUpdate(dt);
    }

    void Juego::OnRender(float dt)
    {
        escena_actual->onRender();
    }
}


