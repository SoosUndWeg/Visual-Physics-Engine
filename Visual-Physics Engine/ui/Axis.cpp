//
//  Axis.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 04.08.25.
//
#include "Axis.hpp"

#include <print>

#include "../Config.hpp"
#include "../core/Scene.hpp"

XAxis::XAxis(std::string name, Scene& scene, const sf::Font& font) : m_font(font),
                                                                            m_scene(scene),
                                                                            m_xAxisLabel(font, name, 30) {
    
    initialize();
}
    
void XAxis::initialize() {
    
    sf::Vector2f translation = m_scene.getTranslation();
    
    if (translation.x < 1e-15)
        translation.x = 0.f;
    if (translation.y < 1e-15)
        translation.y = 0.f;

    float xMin = translation.x - config::window::resolution.x / 2.f;
    float xMax = translation.x + config::window::resolution.x / 2.f;

    // Axis
    m_xAxis.setPrimitiveType(sf::PrimitiveType::Lines);
    m_xAxis.resize(2);

    m_xAxis[0].color = sf::Color::White;
    m_xAxis[0].position = {xMin, 0};

    m_xAxis[1].color = sf::Color::White;
    m_xAxis[1].position = {xMax, 0};
    
    // Label
    m_xAxisLabel.setPosition({xMin + config::hud::distanceFromWindowBorder, config::coordinateSystem::markerLabelOffset * 2.f});
}

void XAxis::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_xAxis, states);
    target.draw(m_xAxisLabel, states);
}

void XAxis::update() {
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    sf::Vector2f scaleFactor = m_scene.getScale();
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize;
    viewSize.y = baseViewSize.y / scaleFactor.y;
    
    float y = translation.y / viewSize.y * config::window::size.y;
    
    if (y < - static_cast<float>(config::window::size.y) + config::hud::distanceFromWindowBorder) {
        
        y = - static_cast<float>(config::window::size.y) + config::hud::distanceFromWindowBorder;
        
    } else if (y > static_cast<float>(config::window::size.y) - config::hud::distanceFromWindowBorder) {
        
        y = static_cast<float>(config::window::size.y) - config::hud::distanceFromWindowBorder;
        
    }
    
    setWorldPosition(y);
}

sf::Vector2f XAxis::getPosition() const {
    return {m_xAxis[0].position.x, m_xAxis[0].position.y};
}


void XAxis::setScreenPosition(double y) {
 
    m_xAxis[0].position.y = m_scene.screenToWorld({0, static_cast<float>(y)}).y;
    m_xAxis[1].position.y = m_scene.screenToWorld({0, static_cast<float>(y)}).y;
}

void XAxis::setWorldPosition(double y) {
    
    m_xAxis[0].position.y = static_cast<float>(y);
    m_xAxis[1].position.y = static_cast<float>(y);


}



YAxis::YAxis(std::string name, Scene& scene, const sf::Font& font) : m_font(font),
                                                                            m_scene(scene),
                                                                            m_yAxisLabel(font, name, 30) {
    
    initialize();
}
    
void YAxis::initialize() {
    
    sf::Vector2f translation = m_scene.getTranslation();
    
    if (translation.x < 1e-15)
        translation.x = 0.f;
    if (translation.y < 1e-15)
        translation.y = 0.f;

    float yMin = translation.y - config::window::resolution.y / 2.f;
    float yMax = translation.y + config::window::resolution.y / 2.f;

    // Axis
    m_yAxis.setPrimitiveType(sf::PrimitiveType::Lines);
    m_yAxis.resize(2);

    m_yAxis[0].color = sf::Color::White;
    m_yAxis[0].position = {0, yMin};

    m_yAxis[1].color = sf::Color::White;
    m_yAxis[1].position = {0, yMax};
    
    // Label
    m_yAxisLabel.setPosition({config::coordinateSystem::markerLabelOffset * 2.f, yMin + config::hud::distanceFromWindowBorder});
    
}

void YAxis::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_yAxis, states);
    target.draw(m_yAxisLabel, states);
}

void YAxis::update() {
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    sf::Vector2f scaleFactor = m_scene.getScale();
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / scaleFactor.x;
    
    float x = - translation.x / viewSize.x * config::window::size.x;
    
    if (x < - static_cast<float>(config::window::size.x) + config::hud::distanceFromWindowBorder) {
        
        x = - static_cast<float>(config::window::size.x) + config::hud::distanceFromWindowBorder;
        
    } else if (x > static_cast<float>(config::window::size.x) - config::hud::distanceFromWindowBorder) {
        
        x = static_cast<float>(config::window::size.x) - config::hud::distanceFromWindowBorder;
        
    }
    setWorldPosition(x);
}


void YAxis::setScreenPosition(double x) {
 
    m_yAxis[0].position.x = m_scene.screenToWorld({static_cast<float>(x), 0}).x;
    m_yAxis[1].position.x = m_scene.screenToWorld({static_cast<float>(x), 0}).x;
}

void YAxis::setWorldPosition(double x) {
    
    m_yAxis[0].position.x = static_cast<float>(x);
    m_yAxis[1].position.x = static_cast<float>(x);


}
