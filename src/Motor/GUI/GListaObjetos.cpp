#include "GListaObjetos.hpp"
#include "../Camaras/CamarasGestor.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

namespace CE
{
    void GListaObjetos::OnInit(const MotorConfig& des) {}
    void GListaObjetos::OnUpdate(float dt) {}
    void GListaObjetos::OnRender(void)
    {
        ImGui::Begin("Objetos", nullptr, 0);
        if(ImGui::TreeNode("Camaras"))
        {
            auto camaras = GestorCamaras::Get().getListaCamaras();
            for(int i=0; i<camaras.size(); i++)
            {
                auto cam = camaras[i];
                auto strCam = cam->nombre;

                // color del boton
                if(cam->esta_activa)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, 0xff00A5FF);
                    ImGui::PushStyleColor(ImGuiCol_Text, 0xff000000);
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, 0xffffbf00);
                    ImGui::PushStyleColor(ImGuiCol_Text, 0xff000000);
                }

                bool boton = ImGui::Button(strCam.c_str(), ImVec2(150, 30));
                if(boton)
                {
                    GestorCamaras::Get().setCamaraAciva(i);
                }
                ImGui::PopStyleColor(2);
            }
            ImGui::TreePop();
            ImGui::Spacing();
        }
        if(ImGui::TreeNode("Objetos"))
        {
            // mas adelante se implementara para que muestre los objetos de la escena
            ImGui::TreePop();
        }
        ImGui::End();
    }
}