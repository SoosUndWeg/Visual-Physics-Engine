//
//  Scene.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Scene.hpp"

#include <print>

#include "../Config.hpp"
#include "EventHandler.hpp"
#include "Application.hpp"
#include "ThreadManager.hpp"

Scene::Scene(sf::Font& font, sf::Clock& clock, Application& application) :
                                     m_translationVector({0, 0}),
                                     m_scaleVector({1.f, 1.f}),
                                     m_clock(clock),
                                     m_application(application),
                                     m_coordinateSystem(font, *this) {
    initialize();
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_coordinateSystem, states);
    
    for (const auto& shape : m_shapes) {
        target.draw(*shape, states);
    }
    
    for (const auto& function : m_functions) {
        target.draw(*function, states);
    }
}

void Scene::initialize() {
    m_shapes.clear();
    m_functions.clear();
    
    m_functions.push_back(std::make_shared<Function>("f", "f(x, t) = sin(x * sin(t))", *this, m_threadManager, sf::Color::Green));
    m_functions.back()->setFlag(Function::Flag::TimeDependent |
                                Function::Flag::Animated |
                                Function::Flag::NoParameters |
                                Function::Flag::IntervalCalculated);
    m_functions.back()->initializeEnvironment();
    
    m_functions.push_back(std::make_shared<Function>("g", "g(a, b, t) = a * cos(t) * sin(b * t)", *this, m_threadManager, sf::Color::Red));
    m_functions.back()->setFlag(Function::Flag::Animated |
                                Function::Flag::Waveform |
                                Function::Flag::TimeDependent);
    m_functions.back()->initializeEnvironment();
    
    m_application.refreshParameterHUDs();
    
    m_coordinateSystem.update();
}

void Scene::setCallback(EventHandler& eventHandler) {
    eventHandler.subscribe(EventHandler::Listener::MouseScrolled, std::bind(&Scene::setGraphDirty, this));
}


void Scene::update() {
    m_coordinateSystem.update();
    
    updateGraph();
   
}

void Scene::updateGraph() {
    
    for (auto& function : m_functions) {
        
        if (function->getFlags() & Function::Flag::TimeDependent && m_playTime) {
            function->setTime(m_clock.getElapsedTime().asSeconds());
            function->graphDirty();
        }
        
        function->update();
    }
}

void Scene::setGraphDirty() {
    for (auto& function : m_functions) {
        function->graphDirty();
    }
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

sf::Vector2f Scene::getViewSize() const {
    return sf::Vector2f({static_cast<float>(config::window::resolution.x) / m_scaleVector.x / config::window::pixelPerWorldUnit,
                         static_cast<float>(config::window::resolution.y) / m_scaleVector.y / config::window::pixelPerWorldUnit});
}

size_t Scene::getFunctionCount() const {
    if (m_functions.empty()) {
        std::print("Warning: No functions available in the scene.\n");
        return 0;
    }
    return m_functions.size();
}

std::shared_ptr<Function> Scene::getFunction(const std::string& name) {
    
    if (m_functions.empty()) {
        throw std::runtime_error("No functions available in the scene.");
    }
    
    for (auto& function : m_functions) {
        
        if (function->getName() == name) {
            
            return function;
        }
    }
    
    return m_functions.front();
}

std::shared_ptr<Function> Scene::getFunction(size_t index) {
    if (index >= m_functions.size()) {
        throw std::out_of_range("Index out of range for functions in the scene.");
    }
    
    if (m_functions.empty()) {
        throw std::runtime_error("No functions available in the scene.");
    }
    
    if (!m_functions[index]) {
        throw std::runtime_error("Function at index " + std::to_string(index) + " is not initialized.");
    }
    
    return m_functions[index];
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


void Scene::scaleAroundMouse(sf::Vector2f factor, sf::Vector2f mousePosition) {
    
    sf::Vector2f mouseWorldbefore = screenToWorld(mousePosition);
    
    scale(factor);
    
    sf::Vector2f mouseScaled = screenToWorld(mousePosition);
    
    translate(mouseWorldbefore - mouseScaled);
}


sf::Vector2f Scene::worldToScreen(sf::Vector2f worldPos) const {
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(config::window::size);
    sf::Vector2f viewSize = getViewSize();
    
    float px = (worldPos.x - m_translationVector.x) / viewSize.x * windowSize.x;
    float py = -(worldPos.y - m_translationVector.y) / viewSize.y * windowSize.y;

    return {px, py};
}

sf::Vector2f Scene::screenToWorld(sf::Vector2f screenPos) const {
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(config::window::size);
    sf::Vector2f viewSize = getViewSize();

    float x = (screenPos.x / windowSize.x) * viewSize.x + m_translationVector.x;
    float y = -(screenPos.y / windowSize.y) * viewSize.y + m_translationVector.y;

    return {x, y};
}

bool Scene::playTime() {
    
    if (m_playTime) {
        m_playTime = false;
        m_clock.stop();
    } else {
        m_playTime = true;
        m_clock.start();
    }
    
    return m_playTime;
}
