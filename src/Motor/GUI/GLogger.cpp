#include "GLogger.hpp"
#include <imgui-SFML.h>

namespace CE
{
    GLogger::GLogger()
    :GLayer{}{}

    GLogger& GLogger::Get()
    {
        if(!instancia)
            instancia = new GLogger();
        return *instancia;
    }

    void GLogger::OnInit(const MotorConfig& des){}

    void GLogger::OnUpdate(float dt)
    {
        m_sfps = std::to_string(1.f/dt) + " fps";
    }

    void GLogger::OnRender(void)
    {
        ImGui::Begin("Log", nullptr, 0);
        ImGui::LabelText("", "[%s]", m_sfps.c_str());

        // dequeue a la cola
        while(queue.getSize() > 0)
        {
            auto log = queue.popFrontDato();
            ImVec4 color = nivelColores(log.second);
            ImGui::TextColored(color, "%10s %s", nivelToString(log.second).c_str(), log.first.c_str());
        }

        ImGui::End();
    }

    void GLogger::agregarLog(const std::string& log, GLogger::Niveles n) 
    {
        queue.agregarDato(std::make_pair(log, n));
    }

    ImVec4 GLogger::nivelColores(Niveles n) 
    {
        switch(n) {
            case GLogger::Niveles::LOG:             // gris
                return ImVec4(169, 169, 169, 255);
            case GLogger::Niveles::LOG_DEBUG:       // amarillo
                return ImVec4(255, 255, 0, 255);
            case GLogger::Niveles::LOG_ERROR:       // rojo
                return ImVec4(255, 0, 0, 255);
            case GLogger::Niveles::LOG_SEVERO:      // morado
                return ImVec4(148, 0, 211, 255);
            default:
                return ImVec4(255, 255, 255, 255);  // blanco
        }
    }

    std::string GLogger::nivelToString(Niveles n) {
        switch(n) {
            case GLogger::Niveles::LOG:
                return "[LOG] ";
            case GLogger::Niveles::LOG_DEBUG:
                return "[DEBUG] ";
            case GLogger::Niveles::LOG_ERROR:
                return "[ERROR] ";
            case GLogger::Niveles::LOG_SEVERO:
                return "[SEVERO] ";
            default:
                return "[] ";
        }
    }
}

