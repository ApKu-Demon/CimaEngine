#pragma once

#include "GLayer.hpp"

namespace CE
{
    class GListaObjetos : public GLayer
    {
        public:
            ~GListaObjetos() override {};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender(void) override;
    };
}