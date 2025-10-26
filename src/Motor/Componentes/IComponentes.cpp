#include "IComponentes.hpp"

namespace CE
{
    INombre::INombre(const std::string& nom)
        :nombre{std::move(nom)} {}

    ITransform::ITransform(const CE::Vector2D& pos, const CE::Vector2D& vel, float ang)
        :posicion{pos}, pos_prev{pos}, velocidad{vel}, angulo{ang} {}   //

    ITransform::ITransform()
        : posicion{0.f, 0.f}, pos_prev{0.f, 0.f}, velocidad{0.f, 0.f}, angulo{0.f} {}

    ITimer::ITimer()
        :frame_actual{0} {}

    IMotion::IMotion()
    : direccion{1.f, 1.f}, punto_fijo{0.f, 0.f}, velocidad{100.f}, inicializado{false} {}
}

