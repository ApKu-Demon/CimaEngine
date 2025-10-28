#include "Camaras.hpp"

namespace CE
{
    int Camara::num_camaras = 0;

    Camara::Camara(float x, float y, float w, float h)
        :cam_width{w}, cam_height{h}, esta_activa{false}
    {
        m_transform = std::make_shared<ITransform>(Vector2D{x, y}, Vector2D{0,0}, 0);
        m_view = std::make_shared<sf::View>(sf::FloatRect{{x, y}, {w, h}});
        m_view->setCenter({cam_width/2.f, cam_height/2.f});
        Camara::num_camaras++;
        nombre = "Camara Default #" + std::to_string(Camara::num_camaras);
    }

    Camara::Camara(const Vector2D& pos, const Vector2D& dim)
        :cam_width{dim.x}, cam_height{dim.y}, esta_activa{false}
    {
        m_transform = std::make_shared<ITransform>(pos, Vector2D{0,0}, 0);
        m_view = std::make_shared<sf::View>(
            sf::FloatRect{{pos.x, pos.y}, {cam_width, cam_height}});
        m_view->setCenter({cam_width/2.f, cam_height/2.f});
        Camara::num_camaras++;
        nombre = "Camara Default #" + std::to_string(Camara::num_camaras);
    }

    void Camara::setViewSize(float x, float y)
    {
        m_view->setSize({x, y});
    }

    void Camara::lockEnObjeto(const std::shared_ptr<Objeto>& obj)
    {
        m_lockObj = obj;
    }

    void Camara::onUpdate(float dt)
    {
        m_view->setCenter({m_transform->posicion.x, m_transform->posicion.y});
    }
}