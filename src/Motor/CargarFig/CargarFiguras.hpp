#pragma once
#include <vector>
#include <memory>
#include "../Primitivos/Objetos.hpp"

namespace CE
{
    class CargarFiguras
    {
    public:
        static std::vector<std::shared_ptr<Objeto>> CargarFigurasDesdeArchivo(const std::string& ruta);
    };
}