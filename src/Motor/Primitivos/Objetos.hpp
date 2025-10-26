#pragma once

#include <SFML/Graphics.hpp>
#include "../Componentes/IComponentes.hpp"
#include <memory>

namespace CE
{
    class Objeto:public sf::Drawable, public sf::Transformable
    {
        public:
            explicit Objeto();
            virtual ~Objeto() = default;    // el destructor
            void draw(sf::RenderTarget &target, sf::RenderStates state) const override; // const para no poder editar als referencias/parametros
            Objeto& addComponente(const std::shared_ptr<IComponentes>& componente);
            
            template<typename T> T* getComponente() const
            {
                static_assert(std::is_base_of<IComponentes, T>::value, "Solo derivados de IComponentes");

                for(auto& c: componentes)
                {
                    T* comp = dynamic_cast<T*>(c.get());
                    if(comp!=nullptr)
                        return comp;
                }
                return nullptr;
            };

            template<typename T> bool tieneComponente() const
            {
                static_assert(std::is_base_of<IComponentes, T>::value, "Solo derivados de IComponentes");

                for(auto& c: componentes)
                {
                    T* comp = dynamic_cast<T*>(c.get());
                    if(comp!=nullptr)
                        return true;
                }
                return false;
            }

            static int getNumObjetos()
            {
                return num_objetos;
            }

            std::shared_ptr<INombre> getNombre()
            {
                return nombre;
            }

            std::shared_ptr<ITransform> getTransformada()
            {
                return transform;
            }

            void setPosicion(float x, float y);
            virtual void onUpdate(float dt){};

            // LAB 3
            virtual std::string toString()
            {
                return nombre->nombre;
            }

        private:
            static int num_objetos;

        protected:
            std::shared_ptr<INombre> nombre;
            std::shared_ptr<ITransform> transform;
            std::vector<std::shared_ptr<IComponentes>> componentes;
    };
}

