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


XMarker::XMarker(Scene& scene, sf::Font& font, const std::string& labelText, sf::Vector2f position) :
    m_scene(scene),
    m_font(font),
    m_label(sf::Text(font, labelText, 20)),
    m_position(position) {
    
    initialize();
}

void XMarker::initialize() {
    m_label->setFillColor(sf::Color::Transparent);
    m_label->setPosition({m_position.x, - config::coordinateSystem::markerLabelOffset * 2});
    m_label->setCharacterSize(config::coordinateSystem::markerLabelSize);
    
    m_marker.setPrimitiveType(sf::PrimitiveType::Lines);
    m_marker.resize(2);
    
    m_marker[0].color = sf::Color::Transparent;
    m_marker[1].color = sf::Color::Transparent;
    
    setPosition(m_position);
}

void XMarker::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_label.value(), states);
    target.draw(m_marker, states);
}

void XMarker::update() {
    setPosition(m_position);
}


void XMarker::setPosition(sf::Vector2f position) {
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    sf::Vector2f scaleFactor = m_scene.getScale();
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / scaleFactor.x;
    viewSize.y = baseViewSize.y / scaleFactor.y;
    
    m_marker[0].position = {position.x, m_scene.screenToWorld({0, position.y}).y - config::coordinateSystem::markerLength * 4};
    m_marker[1].position = {position.x, m_scene.screenToWorld({0, position.y}).y + config::coordinateSystem::markerLength * 4};
    m_label->setPosition({(position.x + translation.x) , (config::coordinateSystem::markerLabelOffset * 2 + translation.y) / viewSize.y * config::window::size.y});
    //m_label->setPosition({position.x, position.y + config::coordinateSystem::markerLabelOffset * 2});
    
    m_position = position;
}

sf::Vector2f XMarker::getPosition() const {
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

void XMarker::setVisible(const bool visible) {
    m_label->setFillColor(visible ? sf::Color::Cyan : sf::Color::Transparent);
    m_marker[0].color = visible ? sf::Color::Cyan : sf::Color::Transparent;
    m_marker[1].color = visible ? sf::Color::Cyan : sf::Color::Transparent;
}
