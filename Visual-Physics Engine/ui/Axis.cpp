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
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    sf::Vector2f scaleFactor = m_scene.getScale();
    sf::Vector2f translation = m_scene.getTranslation();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / scaleFactor.x;
    viewSize.y = baseViewSize.y / scaleFactor.y;
    
    // Marker spacing
    m_markerSpacing = (1 / viewSize.x * config::window::size.x);
    
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
    
    // Marker
    createMarkers();
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
    
    // Marker
    /*
    for (auto& marker : m_markers) {
        marker.setVisible(false);
    }
    
    int min = std::floor((- static_cast<float>(config::window::size.x) + translation.x) / m_markerSpacing);
    std::print("Min: {}\n", min);
    int max = std::ceil((static_cast<float>(config::window::size.x) + translation.x) / m_markerSpacing);
    std::print("Max: {}\n", max);
    
    for (int i = min; i <= max; ++i) {
        double x = m_scene.screenToWorld({static_cast<float>(static_cast<double>(i) * m_markerSpacing), 0}).x;
        
        m_markers[i - min].setPosition({static_cast<float>(x), y});
        m_markers[i - min].setVisible(true);
        m_markers[i - min].setLabel(std::format("{:.0f}", i + translation.x/ m_markerSpacing));
    }
    */
    updateMarkerPositions(calculateMarkerSpacing(), y);
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

void XAxis::createMarkers() {
    m_markers.clear();
    
    float markerCount = calculateMarkerCount();
    
    m_markers.reserve(markerCount);
    
    for (int i = 0; i < markerCount; ++i) {
        
        m_markers.emplace_back(m_scene, m_font, "", sf::Vector2f({0.f, 0.f}));
    }
}

int XAxis::calculateMarkerCount() const {
    float worldWidth = m_scene.getViewSize().x / m_scene.getScale().x;
    return static_cast<int>(worldWidth / calculateMarkerSpacing()) * 2 + 4; // Großzügiger Puffer
}

float XAxis::calculateMarkerSpacing() const {
    
    // Umrechnung: Bildschirmbreite -> Weltbreite
    float worldWidth = m_scene.getViewSize().x / m_scene.getScale().x;
    
    // Verhältnis Bildschirm zu Welt
    float ratio = (2.f * config::window::size.x) / worldWidth;
    
    // Dynamischer Abstand
    return m_markerSpacing / ratio;
}

void XAxis::updateMarkerPositions(float spacing, float yPos) {
    sf::Vector2f translation = m_scene.getTranslation();
    float viewWidth = m_scene.getViewSize().x / m_scene.getScale().x;

    // explizite Berechnung linker und rechter Bildschirmrand in Weltkoordinaten
    float leftWorld = translation.x - viewWidth / 2.f;
    float rightWorld = translation.x - viewWidth / 2.f;
    std::print("LeftWorld: {}, RightWorld: {}\n", leftWorld, rightWorld);

    // Erster Marker links des linken Randes
    float startX = spacing * std::floor(leftWorld / spacing) - spacing;
    float endX = spacing * std::ceil(rightWorld / spacing) + spacing;
    std::print("StartX: {}, EndX: {}\n", startX, endX);

    for (auto& marker : m_markers) {
        marker.setVisible(false);
    }
    
    float screenSpacing = m_scene.screenToWorld({spacing, 0}).x;
    std::print("ScreenSpacing: {}, MarkerSpacing: {}\n", screenSpacing, spacing);
    
    int i = 0;
    for (float x = startX; x <= endX; x += spacing * 100) {
        if (i >= m_markers.size()) {
            std::print("Warning: Marker index {} exceeds marker size {}\n", i, m_markers.size());
            break; // Sicherheitsabfrage, um Überlauf zu vermeiden
        }
        std::print("Marker {}: {}\n", i, x);
        sf::Vector2f screenPos = m_scene.worldToScreen({x, yPos});
        m_markers[i].setPosition(screenPos);
        m_markers[i].setLabel(std::format("{:.2f}", x));
        m_markers[i].setVisible(true);
        i++;
    }
    
    /*
    for (size_t i = 0; i < m_markers.size(); ++i) {
        float xPos = startX + static_cast<float>(i) * spacing;

        if (xPos >= leftWorld - spacing && xPos <= rightWorld + spacing) {
            sf::Vector2f screenPos = m_scene.worldToScreen({xPos, yPos});

            m_markers[i].setPosition(screenPos);
            m_markers[i].setLabel(std::format("{:.2f}", xPos));
            m_markers[i].setVisible(true);
        } else {
            m_markers[i].setVisible(false);
        }
    }
     */
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
