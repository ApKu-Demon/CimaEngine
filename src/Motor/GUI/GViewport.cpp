#include "GViewport.hpp"
#include "../Render/Render.hpp"
#include <imgui_internal.h>
#include <imgui-SFML.h>

namespace CE
{
    void GViewport::OnInit(const MotorConfig& des)
    {
        vw=0;
        vh=0;
        motor_info = des;
    }
    void GViewport::OnUpdate(float dt) {}
    void GViewport::OnRender()
    {
#if DEBUG
        ImGui::Begin("GViewPort",nullptr,ImGuiWindowFlags_NoCollapse);
#else
        ImGui::Begin("GViewPort",nullptr,ImGuiWindowFlags_NoCollapse
                | ImGuiWindowFlags_NoDecoration
                | ImGuiWindowFlags_NoTitleBar
                | ImGuiWindowFlags_NoMove);
#endif
        vw = (unsigned int) ImGui::GetContentRegionAvail().x;
        vh = (unsigned int) ImGui::GetContentRegionAvail().y;
        
        ImGui::Image(Render::Get().GetTextura(),
                sf::Vector2f(vw,vh),
                sf::Color::White,
                sf::Color::Transparent);
        Render::Get().AutoResizeTextura(vw,vh);
        ImGui::End();
    }
}
