#pragma once
#include "GLayer.hpp"
#include "../Utils/Utils.hpp"

namespace CE
{
    class GDock : public GLayer
    {
        public:
            ~GDock(){};
            void OnInit(const MotorConfig& des) override;
            void OnUpdate(float dt) override;
            void OnRender() override;
        private:
            MotorConfig des;
            unsigned int flags;
            unsigned int dx;
            unsigned int dy;
    };

}
