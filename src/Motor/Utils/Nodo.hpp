#pragma once
#include <memory>

namespace CE
{
    // Definicion de la clase lista para poder aseignarla como amigable antes de finirla
    template <typename T> class Lista;

    template <typename T> class Nodo
    {
        // nadie puede instanciar la clase nodo, solo clases amigables
        friend class Lista <T>;

        // para poder usar el constructo en las clases amigables tenemos que usar protected
        protected:
            Nodo(T d): dato {d}
            {
                m_next = nullptr;
                m_prev = nullptr;
            }
        
        public:
            T dato;
            std::shared_ptr<Nodo <T>> m_prev;
            std::shared_ptr<Nodo <T>> m_next;
            unsigned int size;
    };
}


