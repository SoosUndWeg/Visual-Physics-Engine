//
//  Renderer.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <SFML/Graphics.hpp>

class Renderer {
public:
    Renderer(sf::RenderWindow& window);
    
    void initImGui();

    void draw(std::shared_ptr<sf::Drawable> drawable);
    void draw(const sf::Drawable& drawable);

    void drawAll();

    void clear();
    void clear(size_t index);
private:
    std::vector<std::shared_ptr<sf::Drawable>> m_drawables;

    sf::RenderWindow& m_window;
};

#endif // RENDERER_HPP
