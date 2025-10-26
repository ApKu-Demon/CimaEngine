#include "CargarFiguras.hpp"
#include "../../Juego/Figuras/Figuras.hpp"
#include <iostream>
#include <fstream>      // Para leer archivos
#include <sstream>      // Para procesar lineas

namespace CE
{
    std::vector<std::shared_ptr<Objeto>> CargarFiguras::CargarFigurasDesdeArchivo(const std::string& ruta)
    {
        std::vector<std::shared_ptr<Objeto>> objetos;
        std::ifstream archivo(ruta);

        if (!archivo.is_open()) {
            std::cerr << "Error al abrir archivo: " << ruta << "\n";
            return objetos;
        }

        std::cout << "Ruta actual: " << std::filesystem::current_path() << "\n";
        std::cout << "\n";

        std::string linea;

        while (std::getline(archivo, linea))
        {
            std::istringstream ss(linea);
            std::string tipo;
            float d1, d2, d3, x, y;
            int r1, g1, b1, r2, g2, b2;

            ss >> tipo;

            if (tipo == "Rectangulo")
            {
                ss >> d1 >> d2 >> x >> y >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;
                
                sf::Color relleno(r1, g1, b1);
                sf::Color borde(r2, g2, b2);

                auto fig = std::make_shared<IVJ::Rectangulo>(d1, d2, relleno, borde);

                fig->addComponente(std::make_shared<ITimer>());
                fig->addComponente(std::make_shared<IMotion>());
                fig->setPosicion(x, y);
                objetos.push_back(fig);

                std::cout << "-> Area " << tipo << ": " << fig->getArea() << "\n";
            }

            else if (tipo == "Circulo")
            {
                ss >> d1 >> x >> y >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;

                sf::Color relleno(r1, g1, b1);
                sf::Color borde(r2, g2, b2);

                auto fig = std::make_shared<IVJ::Circulo>(d1, relleno, borde);

                fig->addComponente(std::make_shared<ITimer>());
                fig->addComponente(std::make_shared<IMotion>());
                fig->setPosicion(x, y);
                objetos.push_back(fig);

                std::cout << "-> Area " << tipo << ": " << fig->getArea() << "\n";
            }

            else if (tipo == "Triangulo")
            {
                ss >> d1 >> d2 >> d3 >> x >> y >> r1 >> g1 >> b1 >> r2 >> g2 >> b2;

                sf::Color relleno(r1, g1, b1);
                sf::Color borde(r2, g2, b2);

                auto fig = std::make_shared<IVJ::Triangulo>(d1, d2, d3, relleno, borde);

                fig->addComponente(std::make_shared<ITimer>());
                fig->addComponente(std::make_shared<IMotion>());
                fig->setPosicion(x, y);
                objetos.push_back(fig);

                std::cout << "-> Area " << tipo << ": " << fig->getArea() << "\n";
            }
        }

        return objetos;
    }
}