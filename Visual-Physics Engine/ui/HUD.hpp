//
//  HUD.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

class Scene;

class HUD : public sf::Drawable {
public:
    HUD(const sf::Font& font);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setText(const std::string& key, const std::string& text);

private:
    void initialize();

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Drawable>> m_elements;

    const sf::Font& m_font;
};

#endif // HUD_HPP
