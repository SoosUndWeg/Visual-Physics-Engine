//
//  HUD.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef HUD_HPP
#define HUD_HPP

#include <SFML/Graphics.hpp>

#include "ParameterHUD.hpp"

class Scene;

class HUD : public sf::Drawable {
public:
    HUD(sf::RenderWindow& window, Scene& m_scene, const sf::Font& font);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setText(const std::string& key, const std::string& text);
    
    void refreshParameterHUDs();

private:
    void initialize();

private:
    std::unordered_map<std::string, std::unique_ptr<sf::Drawable>> m_elements;

    const sf::Font& m_font;
    sf::RenderWindow& m_window;
    Scene& m_scene;
    
    std::vector<ParameterHUD> m_parameterHUDs;
};

#endif // HUD_HPP
