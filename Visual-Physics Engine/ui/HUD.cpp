//
//  HUD.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "HUD.hpp"

#include "../Config.hpp"
#include "../core/Scene.hpp"

HUD::HUD(const sf::Font& font) : m_font(font) {
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
