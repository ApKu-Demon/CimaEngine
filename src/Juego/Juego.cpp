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

namespace IVJ
{
    Juego::Juego(void)
        :CE::GameLayer{}, texto{font_juego}
    {
        termino=false;
        //objetos = &lista_objetos;
    }

    void Juego::OnInit(void)
    {
        // LAB 2
        std::cout<<"\n";

        CE::Vector2D P1{100.3, 200.1};
        CE::Vector2D P2{500.3, 100.1};
        std::cout<<"Magnitud P1 "<<P1.magnitud() <<std::endl;
        std::cout<<"Magnitud P2 "<<P2.magnitud() <<std::endl;
        std::cout<<"Dist P1 a P2 "<<P1.distancia(P2) <<std::endl;
        std::cout<<"Normalizacion P1 "<<P1.normalizacion() <<std::endl;
        std::cout<<"Normalizacion P2 "<<P2.normalizacion() <<std::endl;
        std::cout<<"Norm P1 * 100: "<<P1.escala(100);

        std::cout << "\n\nAreas de las objetos cargados:\n";

        objetos.clear();
        objetos = CE::CargarFiguras::CargarFigurasDesdeArchivo("assets/config/figuras.txt");

        auto prueba = std::make_shared<Rectangulo>(100, 200, sf::Color::Blue, sf::Color::Red);
        prueba->addComponente(std::make_shared<CE::ITimer>());
        prueba->addComponente(std::make_shared<CE::IMotion>());
        prueba->setPosicion(500, 500);
        objetos.push_back(prueba);

        std::cout << "\nTotal objetos cargados: " << objetos.size() << "\n";

        // LAB 3
        CE::Lista<int> listilla;
        listilla.agregarDato(1);
        listilla.agregarDato(2);
        listilla.agregarDato(3);
        listilla.agregarDato(4);
        listilla.agregarDato(5);

        listilla.eliminarDato(2);
        listilla.eliminarDato(2);

        std::cout<<"\n";
        for(auto i = listilla.iterInicio(); !listilla.iterTermino(); i = listilla.iterSiguiente())
        {
            std::cout<<*i<<"\n";
        }
        std::cout<<"\n";
        while(listilla.getSize() > 0)
        {
            std::cout<<listilla.popDato()<<"\n";
        }

        if(!font_juego.openFromFile(ASSETS "/fonts/Saira-VariableFont_wdth,wght.ttf"))  // si esta bien
        {
            std::cout<<"ERROR AL CARGAR FONT";
            exit(1);
        }

        texto.setFont(font_juego);
        texto.setString("HOLA!!!");
        texto.setCharacterSize(50);
        texto.setPosition({500, 300});

        texto.setFillColor(sf::Color::Yellow);
        texto.setStyle(sf::Text::Bold | sf::Text::Underlined);
    }

    void Juego::OnInputs(float dt,std::optional<sf::Event>& eventos)
    {
        if(eventos->getIf<sf::Event::Closed>())
            termino=true;
    }

    void Juego::OnUpdate(float dt)
    {
        /*SistemaMovimientoEntes(objetos, dt);
        int i = 0;
        for (auto& obj : objetos)
        {
            CE::GLogger::Get().agregarLog(obj->toString() + " en Update", (CE::GLogger::Niveles)(i%5));
            obj->onUpdate(dt);
            i++;
        }*/
        SistemaMovimientoEntes(objetos, dt);

        const float screenWidth = 1440.f;
        const float centro = screenWidth / 2.f;

        for (auto& obj : objetos)
        {
            // acceso a nombre y transformada
            auto nombreObj = obj->getNombre();
            auto transformObj = obj->getTransformada();

            if (!nombreObj || !transformObj)
                continue; // si falta algun componente saltar el objeto

            const std::string& nombre = nombreObj->nombre;
            const CE::Vector2D& pos = transformObj->posicion;
            const CE::Vector2D& vel = transformObj->velocidad;

            std::string mensaje = nombre + " | Pos: (" +
                std::to_string(pos.x) + ", " + std::to_string(pos.y) + ") | Vel: (" +
                std::to_string(vel.x) + ", " + std::to_string(vel.y) + ")";

            // mensaje por hubicaciones
            if (pos.x <= 100.f)
            {
                CE::GLogger::Get().agregarLog(mensaje + " cerca del lado izquierdo", CE::GLogger::Niveles::LOG_DEBUG);
            }
            else if (pos.x >= screenWidth - 100.f)
            {
                CE::GLogger::Get().agregarLog(mensaje + " cerca del lado derecho", CE::GLogger::Niveles::LOG_ERROR);
            }
            else if (std::abs(pos.x - centro) <= 50.f)
            {
                CE::GLogger::Get().agregarLog(mensaje + " cerca del centro", CE::GLogger::Niveles::LOG_SEVERO);
            }

            obj->onUpdate(dt);
        }
    }

    void Juego::OnRender(float dt)
    {
        //std::cout << "Áreas de las objetos cargadas:\n";
        
        for (auto& obj : objetos)
        {     
            CE::Render::Get().AddToDraw(*obj);
        }

        CE::Render::Get().AddToDraw(texto);
    }
}

