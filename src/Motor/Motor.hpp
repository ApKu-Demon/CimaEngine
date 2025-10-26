#pragma once
#include "Utils/Utils.hpp"
#include "App/ALayer.hpp"
#include "Utils/Utils.hpp"
#include "GUI/GLayer.hpp"
#include <memory>
#include <vector>
namespace CE
{
    class Motor
    {
        public:
            Motor(const MotorConfig& config, std::unique_ptr<GameLayer> ref);
            int OnRun(void);
        private:
            void OnInit(void);
            void OnEventFrame(float dt);
            void OnUpdateFrame(float dt);
            void OnRenderFrame(float dt);
        private:
            MotorConfig motor_info;
            std::unique_ptr<GameLayer> mi_app;
            std::vector<std::shared_ptr<GLayer>> gui_layers;
    };
}
