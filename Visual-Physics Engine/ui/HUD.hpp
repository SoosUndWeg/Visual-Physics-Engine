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
class Function;
class ParameterHUD;



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



class ParameterHUD {
public:
    
    ParameterHUD(sf::RenderWindow& window, std::shared_ptr<Function> function);

    void initialize();
    
    void draw() const;
    

private:
    sf::RenderWindow& m_window;
    
    std::shared_ptr<Function> m_function;
    
    std::vector<std::pair<std::string, float&>> m_parameters;
};

#endif // HUD_HPP
