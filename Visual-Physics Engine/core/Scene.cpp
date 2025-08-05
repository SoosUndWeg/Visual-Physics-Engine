//
//  Scene.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Scene.hpp"

#include "../Config.hpp"
#include "EventHandler.hpp"

Scene::Scene(sf::Font& font, sf::Clock& clock) : m_viewSize({config::window::resolution.x / config::window::pixelPerWorldUnit,
                                                 config::window::resolution.y / config::window::pixelPerWorldUnit}),
                                     m_clock(clock),
                                     m_coordinateSystem(font, *this),
                                     m_function1("f", "f(x, t) = sin(x^2 - t)", *this, sf::Color::Green),
                                     m_function2("g", "g(x, t) = sin(x^2 + t)", *this, sf::Color::Red) {
    
    initialize();
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_coordinateSystem, states);
    
    sf::Transform transform = getTransformMatrix();
    for (const auto& shape : m_shapes) {
        target.draw(*shape, transform);
    }

    target.draw(m_function1, states);
    target.draw(m_function2, states);
}

void Scene::initialize() {
    m_shapes.clear();
    
    m_coordinateSystem.update();
}

void Scene::setCallback(EventHandler& eventHandler) {
    eventHandler.subscribe(EventHandler::Listener::MouseScrolled, std::bind(&Scene::setGraphDirty, this));
}

void Scene::update() {
    m_coordinateSystem.update();
    m_graphDirty = true;
    if (m_graphDirty) {
        updateGraph();
        m_graphDirty = false;
    }
}

void Scene::updateGraph() {
    m_function1.calculateInterval(m_clock.getElapsedTime().asSeconds());

    m_function2.calculateInterval(m_clock.getElapsedTime().asSeconds());
}

void Scene::addShape(std::unique_ptr<sf::Drawable> shape) {
    m_shapes.push_back(std::move(shape));
}

void Scene::translate(sf::Vector2f offset) {
    m_translationVector.x += offset.x;
    m_translationVector.y += offset.y;
}

sf::Vector2f Scene::getTranslation() const {
    return m_translationVector;
}

void Scene::setTranslation(sf::Vector2f translation) {
    m_translationVector.x = translation.x;
    m_translationVector.y = translation.y;
}

sf::Transform Scene::getTranslationMatrix() const {
    sf::Transform translateTransform = sf::Transform::Identity;
    translateTransform.translate(m_translationVector);
    return translateTransform;
}

void Scene::scale(sf::Vector2f factor) {
    if (factor.x < 1e-25 || factor.y < 1e-25 || std::isnan(factor.x) || std::isnan(factor.y)) {
        throw std::invalid_argument("Scale factor must be greater than zero.");
    }
    
    m_scaleVector.x *= factor.x;
    m_scaleVector.y *= factor.y;
}

sf::Vector2f Scene::getScale() const {
    if (m_scaleVector.x < 1e-25 || m_scaleVector.y < 1e-25){
        return {1, 1};
    }
    return m_scaleVector;
}

void Scene::setScale(sf::Vector2f scale) {
    m_scaleVector.x = std::max(scale.x, 0.05f);
    m_scaleVector.y = std::max(scale.y, 0.05f);
}

sf::Transform Scene::getScaleMatrix() const {
    sf::Transform scaleTransform = sf::Transform::Identity;
    scaleTransform.scale(m_scaleVector);
    return scaleTransform;
}

sf::Transform Scene::getTransformMatrix() const {
    sf::Transform transform = sf::Transform::Identity;
    transform.translate(m_translationVector);
    transform.scale(m_scaleVector);
    
    return transform;
}

void Scene::scaleAroundMouse(sf::Vector2f factor, sf::Vector2f mousePosition) {
    
    sf::Transform transform = getTransformMatrix();
    sf::Transform inverse = transform.getInverse();
    sf::Vector2f localMouse = inverse.transformPoint(mousePosition);
    
    scale(factor);
    
    sf::Vector2f newMousePosition = transform.transformPoint(localMouse);
    sf::Vector2f offset = mousePosition - newMousePosition;
    
    translate(offset);
    
    sf::Transform translateTransform = sf::Transform::Identity;
    translateTransform.translate(offset);
}


sf::Vector2f Scene::worldToScreen(sf::Vector2f worldPos) const {
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(config::window::size);

    sf::Vector2f viewSizeZoomed = { m_viewSize.x / m_scaleVector.x, m_viewSize.y / m_scaleVector.y };

    float px = (worldPos.x - m_translationVector.x) / viewSizeZoomed.x * windowSize.x;
    float py = -(worldPos.y - m_translationVector.y) / viewSizeZoomed.y * windowSize.y;

    return {px, py};
}

sf::Vector2f Scene::screenToWorld(sf::Vector2f screenPos) const {
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(config::window::size);
    sf::Vector2f viewSizeZoomed = { m_viewSize.x / m_scaleVector.x, m_viewSize.y / m_scaleVector.y };

    float x = ((screenPos.x - windowSize.x / 2.0f) / windowSize.x) * viewSizeZoomed.x + m_translationVector.x;
    float y = -((screenPos.y - windowSize.y / 2.0f) / windowSize.y) * viewSizeZoomed.y + m_translationVector.y;

    return {x, y};
}
