//
//  Renderer.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Renderer.hpp"

Renderer::Renderer(sf::RenderWindow& window)
    : m_window(window) {}

void Renderer::draw(std::shared_ptr<sf::Drawable> drawable) {
    m_drawables.push_back(drawable);
}

void Renderer::draw(const sf::Drawable& drawable) {
    m_window.draw(drawable);
}

void Renderer::drawAll() {
    for (const auto& drawable : m_drawables) {
        m_window.draw(*drawable);
    }
}

void Renderer::clear() {
    m_drawables.clear();
}

void Renderer::clear(size_t index) {
    if (index < m_drawables.size()) {
        m_drawables.erase(m_drawables.begin() + index);
    }
}
