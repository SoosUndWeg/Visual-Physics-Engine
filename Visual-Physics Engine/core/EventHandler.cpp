//
//  EventHandler.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "EventHandler.hpp"

#include "../Config.hpp"
#include "Scene.hpp"


EventHandler::EventHandler(sf::RenderWindow& window, Scene& scene) : m_window(window), m_scene(scene) {}


void EventHandler::pollEvents() {
    while (std::optional<sf::Event> eventOpt = m_window.pollEvent()) {
        if (eventOpt) {
            handleEvent(*eventOpt);
        }
    }
}

void EventHandler::handleEvent(const sf::Event& event) {
    if (event.is<sf::Event::Closed>()) {
        windowClosed();
        return;
    }
    
    if (auto mouseWheelScrolledEvent = event.getIf<sf::Event::MouseWheelScrolled>()) {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LControl) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RControl)) {

            ctrlMouseWheelScrolled(*mouseWheelScrolledEvent);

        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift) ||
                   sf::Keyboard::isKeyPressed(sf::Keyboard::Key::RShift)) {

            shiftMouseWheelScrolled(*mouseWheelScrolledEvent);

        } else {

            mouseWheelScrolled(*mouseWheelScrolledEvent);
        }

        for (const auto& callback : m_mouseScrolledCallbacks) {
            callback();
        }
    }
}


void EventHandler::subscribe(Listener listener, std::function<void()> callback) {
    switch (listener) {
        case Listener::MouseScrolled:
            m_mouseScrolledCallbacks.push_back(std::move(callback));
            break;
        default:
            throw std::invalid_argument("Unknown listener type");
    }
}


void EventHandler::windowClosed() {
    m_window.close();
}


void EventHandler::ctrlMouseWheelScrolled(const sf::Event::MouseWheelScrolled& event) {
    sf::Vector2f mousePosition = m_window.mapPixelToCoords({event.position.x, event.position.y});

    if (event.wheel == sf::Mouse::Wheel::Vertical) {
        m_scene.scaleAroundMouse({1, 1 + event.delta * config::scene::mouseScaleFactor}, mousePosition);
    }
    if (event.wheel == sf::Mouse::Wheel::Horizontal) {
        m_scene.scaleAroundMouse({1 + event.delta * config::scene::mouseScaleFactor, 1}, mousePosition);
    }
}

void EventHandler::shiftMouseWheelScrolled(const sf::Event::MouseWheelScrolled& event) {
    sf::Vector2f mousePosition = m_window.mapPixelToCoords({event.position.x, event.position.y});

    if (event.wheel == sf::Mouse::Wheel::Vertical ||
        event.wheel == sf::Mouse::Wheel::Horizontal) {

        m_scene.scaleAroundMouse({1 + event.delta * config::scene::mouseScaleFactor,
                                  1 + event.delta * config::scene::mouseScaleFactor}, mousePosition);
    }
}

void EventHandler::trackpadZoomed(float zoomFactor) {
    sf::Vector2f mousePosition = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
    
    m_scene.scaleAroundMouse({1 + zoomFactor * config::scene::trackpadScaleFactor,
                              1 + zoomFactor * config::scene::trackpadScaleFactor},
                             mousePosition);
    
    // Notify all mouse scrolled callbacks
    for (const auto& callback : m_mouseScrolledCallbacks) {
        callback();
    }
}

void EventHandler::mouseWheelScrolled(const sf::Event::MouseWheelScrolled& event) {
    if (event.wheel == sf::Mouse::Wheel::Vertical) {
        m_scene.translate({0, event.delta * config::scene::translationFactor / m_scene.getScale().y});
    }
    if (event.wheel == sf::Mouse::Wheel::Horizontal) {
        m_scene.translate({-event.delta * config::scene::translationFactor / m_scene.getScale().x, 0});
    }
}
