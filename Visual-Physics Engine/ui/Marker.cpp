//
//  Marker.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 04.08.25.
//
#include "Marker.hpp"

#include <print>

#include "../Config.hpp"
#include "../core/Scene.hpp"
#include "Axis.hpp"


XMarker::XMarker(Scene& scene, sf::Font& font, XAxis& xAxis, const std::string& labelText, double position) :
    m_xAxis(xAxis),
    m_scene(scene),
    m_font(font),
    m_label(sf::Text(font, labelText, 20)),
    m_position(position) {
    
    initialize();
}

void XMarker::initialize() {
    m_label->setFillColor(sf::Color::White);
    m_label->setPosition({static_cast<float>(m_position), - config::coordinateSystem::markerLabelOffset * 2});
    m_label->setCharacterSize(config::coordinateSystem::markerLabelSize);
    
    m_marker.setPrimitiveType(sf::PrimitiveType::Lines);
    m_marker.resize(2);
    
    m_marker[0].color = sf::Color::White;
    m_marker[1].color = sf::Color::White;
    
    setPosition(m_position);
}

void XMarker::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_label.value(), states);
    target.draw(m_marker, states);
}

void XMarker::update() {
    setPosition(m_position);
}

void XMarker::setPosition(double x) {
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    sf::Vector2f scaleFactor = m_scene.getScale();
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / scaleFactor.x;
    viewSize.y = baseViewSize.y / scaleFactor.y;
    
    float tmp = static_cast<float>(translation.y) / viewSize.y * config::window::size.y;
    
    if (tmp < - static_cast<float>(config::window::size.y) + config::hud::distanceFromWindowBorder) {
        
        translation.y = - tmp + config::hud::distanceFromWindowBorder - 50;
        std::print("Translation y: {}\n", translation.y);
    } else if (tmp > static_cast<float>(config::window::size.y) -config::hud::distanceFromWindowBorder) {
        
        translation.y = tmp - config::hud::distanceFromWindowBorder;
        std::print("Translation y: {}\n", translation.y);
    }
    
    m_marker[0].position = {(static_cast<float>(x) - translation.x) / viewSize.x * config::window::size.x, (-config::coordinateSystem::markerLabelOffset + translation.y) / viewSize.y * config::window::size.y};
    m_marker[1].position = {(static_cast<float>(x) - translation.x) / viewSize.x * config::window::size.x,  (config::coordinateSystem::markerLabelOffset + translation.y) /viewSize.y * config::window::size.y};
    m_label->setPosition({(static_cast<float>(x) - translation.x) / viewSize.x * config::window::size.x, (config::coordinateSystem::markerLabelOffset * 2 + translation.y) / viewSize.y * config::window::size.y});
    
    m_position = x;
}

double XMarker::getPosition() const {
    return m_position;
}

void XMarker::setLabel(const std::string& labelText) {
    if (m_label) {
        m_label->setString(labelText);
    }
}

void XMarker::setColor(const sf::Color &color) {
    m_marker[0].color = color;
    m_marker[1].color = color;
}
