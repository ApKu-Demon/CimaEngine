#pragma once

#include <string>
#include "../Utils/Vector2D.hpp"
#include <cinttypes>

namespace CE
{
    // "Clase abstarcta"
    class IComponentes
    {
        public:
            virtual ~IComponentes()=default;    // destructor
    };

    class INombre : public IComponentes
    {
        public:
            INombre(const std::string& nom);
            ~INombre() override {};

        public:
            std::string nombre;
    };

    class ITransform : public IComponentes
    {
        public:
            ITransform(const Vector2D& pos, const Vector2D& vel, float ang);
            ITransform();
            ~ITransform() override {};

        public:
            Vector2D posicion;
            Vector2D pos_prev;  //
            Vector2D velocidad;
            float angulo;
    };

    class ITimer : public IComponentes
    {
        public:
            ITimer();
            ~ITimer() override {};

        public:
            int frame_actual;
    };

    class IMotion : public IComponentes
    {
        public:
            IMotion();
            ~IMotion() override {};

        public:
            CE::Vector2D direccion;
            CE::Vector2D punto_fijo;
            float velocidad;
            bool inicializado;
    };

    class IStats : public IComponentes
    {
        public:
            ~IStats() override {};

            // 8 bits = 255 maximo
            std::uint8_t hp;
            std::uint8_t hp_max;
            std::uint8_t str;
            std::uint8_t def;
            std::uint8_t agi;
    };
}

