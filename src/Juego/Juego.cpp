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
        // -----------------------------------------------------------------------------------------

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
        // ---------------------------------------------------------------------------------------------------

        // LAB 4: INICIALIZACION DE LA SIMULACION ---
        /*
        objetos.clear();
        
        // Inicializa la cuadricula 15x15 con 10% de enfermos iniciales
        IVJ::InicializarPoblacion(objetos); 

        std::cout << "\nTotal personas en la cuadricula: " << objetos.size() << "\n";
        // ---------------------------------------

        if(!font_juego.openFromFile(ASSETS "/fonts/Saira-VariableFont_wdth,wght.ttf"))
        {
            std::cout<<"ERROR AL CARGAR FONT";
            exit(1);
        }

        // Usaremos el objeto texto existente
        texto_stats.setFont(font_juego);
        texto_stats.setString("Inicializando...");
        texto_stats.setCharacterSize(20); // Tamano mas pequeno para las stats
        texto_stats.setPosition({50.f, 50.f}); // Esquina superior izquierda

        texto_stats.setFillColor(sf::Color::White);
        texto_stats.setStyle(sf::Text::Bold);
        
        // Reiniciar estadisticas al inicio
        stats_simulacion = {0};
        simulacion_activa = true;
        */

        // CAMARAS # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
        
        objetos[2]->getTransformada()->velocidad.x = 150.f;
        CE::GestorCamaras::Get().agregarCamara(
            std::make_shared<CE::CamaraCuadro>(
                CE::Vector2D{540, 360}, CE::Vector2D{1080, 720}
            )
        );
        CE::GestorCamaras::Get().setCamaraAciva(1);
        CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(objetos[2]);

        objetos[5]->getTransformada()->velocidad.x = 5.f;
        CE::GestorCamaras::Get().agregarCamara(
            std::make_shared<CE::CamaraSeguimiento>(
                CE::Vector2D{540, 360}, CE::Vector2D{1080, 720}
            )
        );
        CE::GestorCamaras::Get().setCamaraAciva(2);
        CE::GestorCamaras::Get().getCamaraActiva().lockEnObjeto(objetos[5]);
    }

    void Juego::OnInputs(float dt,std::optional<sf::Event>& eventos)
    {
        if(eventos->getIf<sf::Event::Closed>())
            termino=true;
    }

    void Juego::OnUpdate(float dt)
    {
        // LAB 3
        SistemaMovimientoEntes(objetos, dt);

        const float screenWidth = 1440.f;
        const float centro = screenWidth / 2.f;

        for (auto& obj : objetos)
        {
            // LAB 3
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
        // -----------------------------------------------------------------------------------------------------------

        // LAB 4
        
        if (!simulacion_activa)
        {
            // La simulación termino, no actualizamos los estados de las personas.
            // Opcionalmente, puedes seguir llamando a obj->onUpdate(dt) si los objetos tienen movimiento inactivo.
            return; 
        }

        // 1. Ejecutar la lógica de la enfermedad y actualizar stats_simulacion
        IVJ::SistemaSimulacionEnfermedad(objetos, stats_simulacion, dt);

        // 2. Comprobar la condición de terminación
        int total_poblacion = POBLACION_SIZE * POBLACION_SIZE;
        if (stats_simulacion.enfermo == 0 || stats_simulacion.muerto == total_poblacion)
        {
            simulacion_activa = false; // Detiene la simulacion
            CE::GLogger::Get().agregarLog("--- SIMULACION FINALIZADA ---", CE::GLogger::Niveles::LOG_SEVERO);
        }

        // 3. Actualizar la posicion de los objetos (onUpdate) y el texto de estadisticas
        for (auto& obj : objetos)
        {
            // En este caso, onUpdate solo actualiza la posicion visual, lo cual es necesario.
            obj->onUpdate(dt); 
        }
        
        // 4. Formatear y actualizar el texto de estadisticas para la visualizacion
        std::string stats_str = 
            "--- SIMULACION DE EPIDEMIA ---\n"
            "Tiempo Total: " + std::to_string(static_cast<int>(stats_simulacion.tiempo_total)) + "s\n"
            "NORMAL (Blanco): " + std::to_string(stats_simulacion.normal) + "\n"
            "ENFERMO (Rojo): " + std::to_string(stats_simulacion.enfermo) + "\n"
            "RECUPERACION (Morado): " + std::to_string(stats_simulacion.recuperacion) + "\n"
            "INMUNE (Verde): " + std::to_string(stats_simulacion.inmune) + "\n"
            "MUERTO (Negro): " + std::to_string(stats_simulacion.muerto) + "\n";
            
        if (!simulacion_activa)
            stats_str += "\n*** FIN DE LA SIMULACION!!! ***";
            
        texto_stats.setString(stats_str);
        // -----------------------------------------------------------------------------------------------------------
        

    }

    void Juego::OnRender(float dt)
    {        
        for (auto& obj : objetos)
        {     
            CE::Render::Get().AddToDraw(*obj);
        }

        CE::Render::Get().AddToDraw(texto);
        // Dibuja el panel de estadisticas
        CE::Render::Get().AddToDraw(texto_stats);
    }
}

