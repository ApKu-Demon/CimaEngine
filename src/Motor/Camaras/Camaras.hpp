#pragma once

#include <SFML/Graphics.hpp>
#include "../Primitivos/Objetos.hpp"
#include "../Componentes/IComponentes.hpp"
#include "../Utils/Vector2D.hpp"

namespace CE
{
    class Camara
    {
        public:
            Camara(float x, float y, float w, float h);
            Camara(const Vector2D& pos, const Vector2D& dim);

            virtual ~Camara(){};

            [[nodiscard]]ITransform& getTransformada()
            {
                return *m_transform;
            }

            [[nodiscard]]sf::View& getView() const
            {
                return *m_view;
            }

            void lockEnObjeto(const std::shared_ptr<Objeto>& obj);
            void setViewSize(float x, float y);
            virtual void onUpdate(float dt);

        public:
            float cam_width;
            float cam_height;
            static int num_camaras;
            std::string nombre;
            bool esta_activa;
        
        protected:
            std::shared_ptr<sf::View> m_view;
            std::weak_ptr<Objeto> m_lockObj;
            // componentes
            std::shared_ptr<ITransform> m_transform;
    };
}

