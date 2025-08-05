//
//  CoordinateSystem.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "CoordinateSystem.hpp"

#include <print>

#include "../Config.hpp"
#include "../core/Scene.hpp"


CoordinateSystem::CoordinateSystem(sf::Font& font, Scene& scene)
    : m_font(font),
      m_scene(scene),
      m_xAxis("X", scene, font),
      m_yAxis("Y", scene, font){
    initialize();
}

void CoordinateSystem::initialize() {
    createXMarkers();
}

void CoordinateSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_xAxis, states);
    target.draw(m_yAxis, states);
    
    for (auto& marker : m_xMarkers) {
        target.draw(marker, states);
    }
}


void CoordinateSystem::update() {
    m_xAxis.update();
    m_yAxis.update();
    
    for (auto& xMarker : m_xMarkers) {
        xMarker.update();
    }
    
}

void CoordinateSystem::createXMarkers() {
    m_xMarkers.clear();
    
    sf::Vector2f translation = m_scene.getTranslation();
    sf::Vector2f zoom = m_scene.getScale();
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / zoom.x;
    viewSize.y = baseViewSize.y / zoom.y;
    
    int numMarkers = (std::round(viewSize.x / 100 / (static_cast<float>(1) - translation.x) / viewSize.x * config::window::size.x)) * 2 + 1;
    std::print("Number of markers: {}\n", numMarkers);
    for (int i = -numMarkers; i < numMarkers; ++i) {
        if ( i == 0) {
            continue;
        }
        
        
        m_xMarkers.emplace_back(m_scene, m_font, m_xAxis, std::to_string(i), static_cast<double>(i));
    }
    
    std::print("Marker count: {}\n", m_xMarkers.size());
}
