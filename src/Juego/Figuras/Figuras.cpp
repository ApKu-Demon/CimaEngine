#include "Figuras.hpp"
#include <cmath>
//#include <SFML/Graphics.hpp>

namespace IVJ
{
    Figuras::Figuras(int l, const sf::Color &relleno, const sf::Color &contorno)
        : CE::Objeto{}, lados{l}, c_relleno{std::move(relleno)}, c_contorno{std::move(contorno)}
    {
    }

    Rectangulo::Rectangulo(float ancho, float largo, const sf::Color &relleno, const sf::Color &contorno)
        :Figuras{4, relleno, contorno},
        w{ancho},
        h{largo},
        imagen{sf::RectangleShape{{ancho, largo}}}
        {
            imagen.setFillColor(relleno);
            imagen.setOutlineColor(contorno);
            imagen.setOutlineThickness(2.f);
            imagen.setOrigin({largo/2.f, ancho/2.f});
            nombre->nombre = "Rectangulo " + std::to_string(CE::Objeto::getNumObjetos());
        }
    
    float Rectangulo::getArea()
    {
        return w * h;
    }

    void Rectangulo::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        state.transform *= getTransform();
        target.draw(imagen);

        // pintar el pivote/ancla
        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f, 10.f});
        ancla.setPosition(imagen.getPosition());
        target.draw(ancla);

    }

    void Rectangulo::onUpdate(float dt)
    {
        imagen.setPosition({transform->posicion.x, transform->posicion.y});
    }

    /*
    void Rectangulo::setPosicion(float x, float y)
    {
        imagen.setPosition({x,y});
    }
    */

    Circulo::Circulo(float radio, const sf::Color &relleno, const sf::Color &contorno)
        : Figuras{0, relleno, contorno}, r{radio}, //imagen{radio}
        imagen{sf::CircleShape{radio}}
        {
            imagen.setFillColor(relleno);
            imagen.setOutlineColor(contorno);
            imagen.setOutlineThickness(2.f);
            imagen.setOrigin({radio, radio});
            nombre->nombre = "Circulo " + std::to_string(CE::Objeto::getNumObjetos());
        }

    float Circulo::getArea()
    {
        return 3.14159265f * r * r;
    }

    void Circulo::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        state.transform *= getTransform();
        target.draw(imagen);

        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f, 10.f});
        ancla.setPosition(imagen.getPosition());
        target.draw(ancla);
    }

    void Circulo::onUpdate(float dt)
    {
        imagen.setPosition({transform->posicion.x, transform->posicion.y});
    }

    /*
    void Circulo::setPosicion(float x, float y)
    {
        imagen.setPosition({x, y});
    }
    */

    Triangulo::Triangulo(float a, float b, float c, const sf::Color &relleno, const sf::Color &contorno)
        : Figuras{3, relleno, contorno}, a{a}, b{b}, c{c}, imagen{}
    {
        imagen.setPointCount(3);

        /*
        imagen.setPoint(0, sf::Vector2f(0.f, 0.f));
        imagen.setPoint(1, sf::Vector2f(b, 0.f));
        imagen.setPoint(2, sf::Vector2f(a / 2.f, c));
        */

        imagen.setPoint(0, sf::Vector2f(0.f, c));
        imagen.setPoint(1, sf::Vector2f(b / 2.f, 0.f));
        imagen.setPoint(2, sf::Vector2f(b, c));

        imagen.setFillColor(relleno);
        imagen.setOutlineColor(contorno);
        imagen.setOutlineThickness(2.5f);
        imagen.setOrigin({a / 2.f, c / 2.f});
        nombre->nombre = "Triangulo " + std::to_string(CE::Objeto::getNumObjetos());
    }

    float Triangulo::getArea()
    {
        float s = (a + b + c) / 2.f;
        return std::sqrt(s * (s - a) * (s - b) * (s - c));
    }

    void Triangulo::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        state.transform *= getTransform();
        target.draw(imagen);

        sf::CircleShape ancla{10.f};
        ancla.setFillColor(sf::Color::Black);
        ancla.setOrigin({10.f, 10.f});
        ancla.setPosition(imagen.getPosition());
        target.draw(ancla);
    }

    void Triangulo::onUpdate(float dt)
    {
        imagen.setPosition({transform->posicion.x, transform->posicion.y});
    }

    /*
    void Triangulo::setPosicion(float x, float y)
    {
        imagen.setPosition({x, y});
    }
    */
}

