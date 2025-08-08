//
//  HUD.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "HUD.hpp"

#include <print>

#include "ImGui.h"

#include "../Config.hpp"
#include "../core/Scene.hpp"

HUD::HUD(sf::RenderWindow& window, Scene& m_scene, const sf::Font& font) :
        m_window(window),
        m_font(font),
        m_scene(m_scene) {
    initialize();
}

void HUD::initialize() {
    sf::Text text(m_font, "FPS: 0", 40);
    text.setFillColor(sf::Color::Red);
    text.setPosition({-950, -530});
    m_elements["FPS"] = std::make_unique<sf::Text>(text);
}

void HUD::draw(sf::RenderTarget& target, sf::RenderStates states) const {

    for (const auto& element : m_elements) {
        target.draw(*element.second, states);
    }
    
    for (const auto& parameterHUD : m_parameterHUDs) {
        parameterHUD.draw();
    }
}

void HUD::setText(const std::string& key, const std::string& text) {
    auto it = m_elements.find(key);
    if (it != m_elements.end()) {
        auto& drawable = it->second;
        if (auto textElement = dynamic_cast<sf::Text*>(drawable.get())) {
            textElement->setString(text);
        }
    }
}

void HUD::refreshParameterHUDs() {
    m_parameterHUDs.clear();
    for (int i = 0; i < m_scene.getFunctionCount(); ++i) {
        m_parameterHUDs.emplace_back(m_window, m_scene.getFunction(i));
    }
}


// ***** ParameterHUD *****


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
    ImGui::Separator();
    
    ImGui::Text("Function: %s", m_function->getName().c_str());
    
    for (auto& parameter : m_parameters) {
        std::string label = std::format("{}: {:.2f}", parameter.first, parameter.second);
        
        ImGui::InputFloat(label.c_str(), &parameter.second);
    }

    
    ImGui::End();
}
