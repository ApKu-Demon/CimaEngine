#include "Motor.hpp"
#include<utility>
#include<chrono>
#include "Render/Render.hpp"
#include "GUI/GDock.hpp"
#include "GUI/GViewport.hpp"
#include "SFML/System/Clock.hpp"
#include <imgui.h>
#include <imgui-SFML.h>
#include <imgui_internal.h>
#include "GUI/GLogger.hpp"
#include "Camaras/CamarasGestor.hpp"

namespace CE
{
    Motor::Motor(const MotorConfig& config, std::unique_ptr<GameLayer> ref)
        :motor_info(std::move(config)),mi_app(std::move(ref))
    {
    }
    int Motor::OnRun(void)
    {
        OnInit();
        auto frame_ant = std::chrono::high_resolution_clock::now();
        float ms = 0.16667f;
        sf::Clock clock = sf::Clock();
        while(mi_app->EstaCorriendo())
        {
            OnEventFrame(ms);
            
            ImGui::SFML::Update(
                    Render::Get().GetVentana(),
                    clock.restart());

            OnUpdateFrame(ms);
            OnRenderFrame(ms);
            auto frame_act = std::chrono::high_resolution_clock::now();
            ms = std::chrono::duration_cast<std::chrono::duration<float>>(frame_act-frame_ant).count();
            frame_ant = frame_act;
        }
        Render::Get().Terminar();
        ImGui::SFML::Shutdown();
        return 0;
    }
    void Motor::OnInit(void)
    {
        Render::Get().CrearVentana(motor_info);
        if(!ImGui::SFML::Init(Render::Get().GetVentana()))
            exit(-1);
        
        // ImGui Configuration
        auto& io=ImGui::GetIO();(void)io;
        io.ConfigFlags |=ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |=ImGuiConfigFlags_ViewportsEnable;
        io.ConfigDockingWithShift = true;

        gui_layers.push_back(std::make_shared<GDock>());
        gui_layers.push_back(std::make_shared<GViewport>());

        // Camaras
        GestorCamaras::Get().agregarCamara(
            std::make_shared<Camara> (
                Vector2D{540, 360}, Vector2D{1440, 900} // Original: 1080, 720
            )
        );
        GestorCamaras::Get().setCamaraAciva(0);

        for(auto& gui: gui_layers)
            gui->OnInit(motor_info);
        GLogger::Get().OnInit(motor_info);

        mi_app->OnInit(); //
    }
    void Motor::OnEventFrame(float dt)
    {
        while(auto eventos=Render::Get().GetVentana().pollEvent())
        {
            ImGui::SFML::ProcessEvent(
                    Render::Get().GetVentana(),
                    *eventos);
            mi_app->OnInputs(dt,eventos);
        }
    }
    void Motor::OnUpdateFrame(float dt)
    {
        // Camaras
        GestorCamaras::Get().onUpdateCamaras(dt);

        mi_app->OnUpdate(dt);
        for(auto& gui: gui_layers)
            gui->OnUpdate(dt);
        GLogger::Get().OnUpdate(dt);
    }
    void Motor::OnRenderFrame(float dt)
    {
        Render::Get().OnClearColor(sf::Color(118,118,255));
        // Camaras
        GestorCamaras::Get().onRenderCamara(
            Render::Get().GetTextura());
        mi_app->OnRender(dt);
        for(auto& gui:gui_layers)
            gui->OnRender();
        GLogger::Get().OnRender();
        Render::Get().OnDisplayTextura();

        ImGui::Begin("Test",nullptr,0);
        ImGui::End();

        Render::Get().OnDisplayTextura();
        if(ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
        ImGui::SFML::Render(Render::Get().GetVentana());
        Render::Get().OnDisplayVentana();
    }
}
