#pragma once

#include "../../Motor/Primitivos/Objetos.hpp"
//#include <SFML/Graphics.hpp>

namespace IVJ
{
    class Figuras : public CE::Objeto
    {
        public:
            explicit Figuras(int l, const sf::Color &relleno, const sf::Color &contorno);
            /*
            Figuras(int l, const sf::Color &relleno, const sf::Color &contorno);
            virtual ~Figuras(){};

            virtual void setPosicion(float x, float y) = 0;
            virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const override = 0;
            virtual float getArea() const = 0;
            */
            ~Figuras() override{};
            virtual float getArea() = 0;

            friend std::ostream& operator<<(std::ostream& os, const Figuras& fig)  // revisar: const std::shared_ptr<Figuras>& fig
            {
                os<<"Lados: " << fig.lados <<
                    "Relleno: " << fig.c_relleno.toInteger() <<
                    "Contorno: " << fig.c_contorno.toInteger() <<
                    "\n";
                return os;
            };

        protected:
            int lados;
            sf::Color c_relleno;
            sf::Color c_contorno;
    };

    class Rectangulo : public Figuras
    {
        public:
            /*
            Rectangulo(float ancho, float largo, const sf::Color &relleno, const sf::Color &contorno);
            ~Rectangulo(){};
            */
            explicit Rectangulo(float ancho, float largo, const sf::Color &relleno, const sf::Color &contorno);
            ~Rectangulo() override {};

            float getArea() override;
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
            void onUpdate(float dt) override;

            /*
            void setPosicion(float x, float y) override;
            void draw(sf::RenderTarget &traget, sf::RenderStates state) const override;
            float getArea() const override;
            */

        private:
            sf::RectangleShape imagen;
            float w;
            float h;
    };

    class Circulo : public Figuras
    {
        public:
            explicit Circulo(float radio, const sf::Color &relleno, const sf::Color &contorno);
            ~Circulo() override {};

            float getArea() override;
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
            void onUpdate(float dt) override;
            //void setPosicion(float x, float y) override;

        private:
            sf::CircleShape imagen;
            float r;
    };

    class Triangulo : public Figuras
    {
        public:
            explicit Triangulo(float a, float b, float c, const sf::Color &relleno, const sf::Color &contorno);
            ~Triangulo() override {};

            float getArea() override;
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override;
            void onUpdate(float dt) override;
            //void setPosicion(float x, float y) override;

        private:
            sf::ConvexShape imagen;
            float a;
            float b; 
            float c;
    };
}

