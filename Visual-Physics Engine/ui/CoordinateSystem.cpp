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
    
}

void CoordinateSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_xAxis, states);
    target.draw(m_yAxis, states);
}


void CoordinateSystem::update() {
    m_xAxis.update();
    m_yAxis.update();
}
