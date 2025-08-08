//
//  HUD.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "HUD.hpp"

#include <print>

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
