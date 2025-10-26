#include "Render.hpp"
#include "SFML/Graphics/Drawable.hpp"
#include "SFML/Graphics/RenderTexture.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/ContextSettings.hpp"
#include "SFML/Window/WindowEnums.hpp"

#include<iostream>
#include <memory>
namespace CE
{
    Render& Render::Get()
    {
        if(!instancia)
            instancia = new Render();
        return *instancia;
    }
    void Render::Terminar()
    {
        delete instancia;
    }

    void Render::CrearVentana(const MotorConfig& motor_info)
    {
        sf::ContextSettings videosettings;
        videosettings.antiAliasingLevel=3;

        r_ventana = std::make_unique<sf::RenderWindow>(
                sf::VideoMode({motor_info.vW,motor_info.vH},32),
                sf::String(motor_info.titulo),
                sf::Style::Default,
                sf::State::Windowed,
                videosettings);

        r_ventana->setFramerateLimit(65);
        r_textura = std::make_unique<sf::RenderTexture>(
                sf::Vector2u{motor_info.vH,motor_info.vH},
                videosettings);
    }

    void Render::AutoResizeTextura(unsigned int w, unsigned int h)
    {
        auto cs = r_textura->getSize();
        unsigned cw = cs.x;
        unsigned ch = cs.y;
        if((cw == w && ch == h) || (cw == 0 && ch == 0) ) return;
        sf::ContextSettings video;
        video.antiAliasingLevel=3;
        r_textura->clear(sf::Color::Black);
        if(!r_textura->resize(sf::Vector2u{w,h},video))
            exit(EXIT_FAILURE);
    }
    void Render::AddToDraw(const sf::Drawable& obj)
    {
        r_textura->draw(obj);
    }
    void Render::OnClearColor(const sf::Color& clearcolor)
    {
        r_textura->clear(clearcolor);
        r_ventana->clear(clearcolor);
    }

    void Render::OnDisplayTextura()
    {
        r_textura->display();
    }

    void Render::OnDisplayVentana()
    {
        r_ventana->display();
    }

    sf::RenderWindow& Render::GetVentana()
    {
        return *r_ventana;
    }
    sf::RenderTexture& Render::GetTextura()
    {
        return *r_textura;
    }

}
