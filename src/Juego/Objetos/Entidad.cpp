#include "Entidad.hpp"
#include "../../Motor/Componentes/IComponentes.hpp"

namespace IVJ
{
    void Entidad::onUpdate(float dt)
    {
        // Revisar si tiene ciertos componentes para actualizarlos debidamente
        // o hacer una clase hija y ahi revisar

        if(tieneComponente<CE::ISprite>())
        {
            auto sprite = getComponente<CE::ISprite>();
            auto pos = getTransformada()->posicion;
            sprite->m_sprite.setPosition({pos.x, pos.y});
        }
    }

    void Entidad::draw(sf::RenderTarget &target, sf::RenderStates state) const
    {
        state.transform *= getTransform();

        // Revisar si tiene ciertos componentes para renderizarlos debidamente
        // o hacer una clase hija y ahi revisar

        if(tieneComponente<CE::ISprite>())
        {
            auto sprite = getComponente<CE::ISprite>();
            target.draw(sprite->m_sprite);
        }
    }
}


