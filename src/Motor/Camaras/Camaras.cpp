#include "Camaras.hpp"
#include "../GUI/GLogger.hpp"

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

    // HIJAS . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
    CamaraCuadro::CamaraCuadro(const Vector2D& pos, const Vector2D& dim)
        : Camara{pos, dim}, limitex{dim.x}, limitey{dim.y}
    {
        nombre = "Camara Cuadro #" + std::to_string(Camara::num_camaras);
    }

    void CamaraCuadro::onUpdate(float dt)
    {
        Camara::onUpdate(dt);
        if (!m_lockObj.lock()) return;

        auto mitad = Vector2D(cam_width, cam_height).escala(0.5f);
        auto obj_trans = m_lockObj.lock()->getTransformada();
        auto opos = obj_trans->posicion;
        std::string log = "OBJ: (" + std::to_string(opos.x) +
            ", " + std::to_string(opos.y) + ")";
        std::string log2 = nombre + ": " + std::to_string(m_transform->posicion.x) +
            ", " + std::to_string(m_transform->posicion.y) + ")";

        GLogger::Get().agregarLog(log, GLogger::Niveles::LOG_DEBUG);
        GLogger::Get().agregarLog(log2, GLogger::Niveles::LOG_DEBUG);

        if (obj_trans->posicion.x > (m_transform->posicion.x + mitad.x))
        {
            m_transform->posicion.x += cam_width;
            limitex += limitex;
        }
    }

    CamaraSeguimiento::CamaraSeguimiento(const Vector2D& pos, const Vector2D& dim)
        : Camara{pos, dim}
    {
        nombre = "Camara Seguimiento #" + std::to_string(Camara::num_camaras);
    }

    void CamaraSeguimiento::onUpdate(float dt)
    {
        // 1. Obtener el shared_ptr del objeto rastreado
        auto locked_obj = m_lockObj.lock();
        
        // 2. Si no hay objeto o el weak_ptr caduco, no hacemos nada.
        if (!locked_obj) 
        {
            Camara::onUpdate(dt);
            return;
        }

        // 3. Obtener la posicion del objeto (asumiendo que tiene transformada)
        auto obj_transform = locked_obj->getTransformada();
        Vector2D posicion_a_seguir = obj_transform->posicion;

        // 4. Actualizar la transformada de la camara a la posicion del objeto
        // La posición de la camara es el centro de la vista.
        m_transform->posicion = posicion_a_seguir;

        // 5. Aplicar la nueva posicion al centro de la vista de SFML
        m_view->setCenter({m_transform->posicion.x, m_transform->posicion.y});
        
        // Opcional: Log para depuracion
        std::string log = nombre + " siguiendo a: (" + std::to_string(posicion_a_seguir.x) + ", " + std::to_string(posicion_a_seguir.y) + ")";
        GLogger::Get().agregarLog(log, GLogger::Niveles::LOG_DEBUG);
    }
}