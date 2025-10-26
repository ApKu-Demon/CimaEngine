
#pragma once
#include "GLayer.hpp"

namespace CE
{
    class GViewport : public GLayer
    {
        public:
            ~GViewport(){};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender() override;
        private:
            MotorConfig motor_info;
            unsigned int vw;
            unsigned int vh;
    };

}
