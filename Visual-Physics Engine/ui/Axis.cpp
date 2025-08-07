//
//  Axis.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 04.08.25.
//
#include "Axis.hpp"

#include <print>
#include <format>

#include "../Config.hpp"
#include "../core/Scene.hpp"

XAxis::XAxis(std::string name, Scene& scene, sf::Font& font) : m_font(font),
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

    float xMin = translation.x - static_cast<float>(config::window::resolution.x) / 2.f;
    float xMax = translation.x + static_cast<float>(config::window::resolution.x) / 2.f;

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
    
    for (const auto& marker : m_markers) {
        target.draw(marker, states);
    }
}

void XAxis::update() {
    // Axis & Label
    sf::Vector2f translation = m_scene.getTranslation();
    sf::Vector2f viewSize = m_scene.getViewSize();
    
    float y = translation.y / viewSize.y * config::window::size.y;
    
    y = std::clamp(y, -static_cast<float>(config::window::size.y) + config::hud::distanceFromWindowBorder,
                static_cast<float>(config::window::size.y) - config::hud::distanceFromWindowBorder);
    
    setScreenPosition(y);
    
    updateMarkerPositions(y);
}

sf::Vector2f XAxis::getPosition() const {
    return {m_xAxis[0].position.x, m_xAxis[0].position.y};
}


void XAxis::setScreenPosition(double y) {
 
    m_xAxis[0].position.y = static_cast<float>(y);
    m_xAxis[1].position.y = static_cast<float>(y);
    
}

void XAxis::setWorldPosition(double y) {
    
    y = m_scene.worldToScreen({0, static_cast<float>(y)}).y;
    
    m_xAxis[0].position.y = y;
    m_xAxis[1].position.y = y;

}

void XAxis::createMarkers() {
    int markerCount = calculateMarkerCount();
    
    m_markers.reserve(markerCount);
    
    for (size_t i = m_markers.size(); i < markerCount; ++i) {
        
        m_markers.emplace_back(m_scene, m_font, "", sf::Vector2f({0.f, 0.f}));
    }
}

int XAxis::calculateMarkerCount() const {
    
    return static_cast<int>(m_scene.getViewSize().x / calculateMarkerSpacing()) * 2 + 1;
}

float XAxis::calculateMarkerSpacing() const {
    
    return 2.f / config::coordinateSystem::markerCount.x * std::pow(2.f, -std::floor(std::log2(m_scene.getScale().x)));
}

void XAxis::updateMarkerPositions(float yPos) {
    
    float spacing = calculateMarkerSpacing();
    
    createMarkers();
    
    float xMin = m_scene.getTranslation().x - m_scene.getViewSize().x;
    float xMax = m_scene.getTranslation().x + m_scene.getViewSize().x;
    
    float firstX = std::floor(xMin / spacing) * spacing;
    
    int j = 0;
    for (float i = firstX; i <= xMax; i += spacing) {
        
        if (j >= m_markers.size()) {
            
            break;
        }
        
        m_markers[j].setVisible(true);
        m_markers[j].setPosition({m_scene.worldToScreen({i, 0}).x, yPos});
        m_markers[j].setLabel(std::format("{:.2f}", i));
        
        ++j;
    }
    
    for (; j < m_markers.size(); ++j) {
        
        m_markers[j].setVisible(false);
    }
}



//***** YAxis Implementation *****


YAxis::YAxis(std::string name, Scene& scene, sf::Font& font) : m_font(font),
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

    float yMin = translation.y - config::window::size.y;
    float yMax = translation.y + config::window::size.y;

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
    
    for (const auto& marker : m_markers) {
        target.draw(marker, states);
    }
}

void YAxis::update() {
    // Axis & Label
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize = m_scene.getViewSize();
    
    float x = - translation.x / viewSize.x * config::window::size.x;
    
    x = std::clamp(x, -static_cast<float>(config::window::size.x) + config::hud::distanceFromWindowBorder,
                       static_cast<float>(config::window::size.x) - config::hud::distanceFromWindowBorder);
    
    setScreenPosition(x);
    
    updateMarkerPositions(x);
}


void YAxis::setScreenPosition(double x) {
 
    m_yAxis[0].position.x = static_cast<float>(x);
    m_yAxis[1].position.x = static_cast<float>(x);
}

void YAxis::setWorldPosition(double x) {
    
    x = m_scene.worldToScreen({static_cast<float>(x), 0}).x;
    
    m_yAxis[0].position.x = x;
    m_yAxis[1].position.x = x;


}

void YAxis::createMarkers() {
    int markerCount = calculateMarkerCount();
    
    m_markers.reserve(markerCount);
    
    for (size_t i = m_markers.size(); i < markerCount; ++i) {
        
        m_markers.emplace_back(m_scene, m_font, "", sf::Vector2f({0.f, 0.f}));
    }
}

int YAxis::calculateMarkerCount() const {
    
    return static_cast<int>(m_scene.getViewSize().y / calculateMarkerSpacing()) * 2 + 1;
}

float YAxis::calculateMarkerSpacing() const {
    
    return 2.f / config::coordinateSystem::markerCount.y * std::pow(2.f, -std::floor(std::log2(m_scene.getScale().y)));
}

void YAxis::updateMarkerPositions(float xPos) {
    
    float spacing = calculateMarkerSpacing();
    
    createMarkers();
    
    float yMin = m_scene.getTranslation().y - m_scene.getViewSize().y;
    float yMax = m_scene.getTranslation().y + m_scene.getViewSize().y;
    
    float firstY = std::floor(yMin / spacing) * spacing;
    
    int j = 0;
    for (float i = firstY; i <= yMax; i += spacing) {
        
        if (j >= m_markers.size()) {
            
            break;
        }
        
        m_markers[j].setVisible(true);
        m_markers[j].setPosition({xPos, m_scene.worldToScreen({0, i}).y});
        m_markers[j].setLabel(std::format("{:.2f}", i));
        
        ++j;
    }
    
    for (; j < m_markers.size(); ++j) {
        
        m_markers[j].setVisible(false);
    }
}
