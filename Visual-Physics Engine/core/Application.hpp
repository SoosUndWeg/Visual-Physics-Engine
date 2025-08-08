//
//  Application.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include "Scene.hpp"
#include "Renderer.hpp"
#include "EventHandler.hpp"
#include "../ui/HUD.hpp"
#include "../objectivecpp/ZoomHandler.hpp"

#include <SFML/Graphics.hpp>


class Application {
public:
    Application();
    ~Application();

    void run();

    
    const sf::Font& getFont() const;
    
    void refreshParameterHUDs();

private:
    void initialize();

    void processEvents();
    void update();
    void render();

private:
    Renderer m_renderer;
    HUD m_hud;

    sf::RenderWindow m_window;

    sf::Font m_font;
    
    sf::Clock m_clock;

    Scene m_scene;
    
    EventHandler m_eventHandler;
    ZoomHandler m_zoomHandler;
};

#endif // APPLICATION_HPP
