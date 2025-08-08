//
//  Application.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Application.hpp"
#include "../objectivecpp/TrackpadZoomBridge.h"

#include <print>

#include "imgui-SFML.h"

#include "../Config.hpp"

Application::Application()
    : m_window(sf::VideoMode(config::window::resolution), config::window::title),
      m_renderer(m_window),
      m_font("assets/Arial.ttf"),
      m_scene(m_font, m_clock, *this),
      m_hud(m_window, m_scene, m_font),
      m_eventHandler(m_window, m_scene),
      m_zoomHandler(m_eventHandler) {
    initialize();
}

Application::~Application() {}

void Application::initialize() {
    m_window.setFramerateLimit(config::window::framerateLimit);
    m_window.setSize(config::window::size);
    m_window.setPosition(config::window::position);
    m_window.setVerticalSyncEnabled(true);

    sf::View view;
    view.setSize({config::window::resolution.x, config::window::resolution.y});
    view.setCenter({0, 0});
    m_window.setView(view);
    
    m_window.requestFocus();

    m_scene.setCallback(m_eventHandler);
    
    std::string windowTitle = config::window::title;
    InitTrackpadZoom(&m_zoomHandler, windowTitle.c_str());
    
    m_renderer.initImGui();
}

void Application::run() {
    sf::Clock frameClock;
    uint64_t frameCount = 0;
    float deltaTime = 0;
    
    sf::Clock deltaClock;

    while (m_window.isOpen()) {
        deltaTime += frameClock.restart().asSeconds();
        frameCount++;

        if (deltaTime >= 1.f) {
            m_hud.setText("FPS", "FPS: " + std::to_string(frameCount));
            frameCount = 0;
            deltaTime = 0;
        }
        
        ImGui::SFML::Update(m_window, deltaClock.restart());

        processEvents();
        update();
        render();
    }
}

void Application::processEvents() {
    m_eventHandler.pollEvents();
}

void Application::update() {
    m_scene.update();
}

void Application::render() {
    
    m_window.clear();

    m_renderer.draw(m_scene);
    m_renderer.draw(m_hud);
    m_renderer.drawAll();
    
    ImGui::SFML::Render(m_window);
    
    m_window.display();
}

void Application::refreshParameterHUDs() {
    m_hud.refreshParameterHUDs();
}
