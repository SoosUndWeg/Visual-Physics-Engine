//
//  ParameterHUD.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 07.08.25.
//
#include "ParameterHUD.hpp"

#include <print>

#include "imgui.h"
#include "imgui-SFML.h"

#include "../parser/AST.hpp"
#include "../math/Function.hpp"

ParameterHUD::ParameterHUD(sf::RenderWindow& window, std::shared_ptr<Function> function) : m_window(window), m_function(function) {
    
    initialize();
}
    
void ParameterHUD::initialize() {
    if (m_function && m_function->getFlags() & Function::Flag::NoParameters) {
        return;
    }
    
    Environment& env = m_function->getEnvironment();
    for (auto& parameter : m_function->getParameters()) {
        
        if (m_function->getFlags() & Function::Flag::TimeDependent && parameter == "t")
            continue;
        
        m_parameters.push_back({std::string(parameter), env[parameter]});
    }
}

void ParameterHUD::draw() const {
    if (m_function && m_function->getFlags() & Function::Flag::NoParameters) {
        return;
    }
    
    ImGui::Begin("Parameter HUD");
    
    ImGui::Text("Function: %s", m_function->getName().c_str());
    
    for (auto& parameter : m_parameters) {
        std::string label = std::format("{}: {:.2f}", parameter.first, parameter.second);
        
        ImGui::SliderFloat(label.c_str(), &parameter.second, -10.0f, 10.0f, "%.2f");
    }

    
    ImGui::End();
}
