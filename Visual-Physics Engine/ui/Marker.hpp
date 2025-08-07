//
//  Marker.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 04.08.25.
//
#ifndef MARKER_HPP
#define MARKER_HPP

#include <SFML/Graphics.hpp>

class Scene;


class XMarker : public sf::Drawable {
public:
    XMarker(Scene& scene, sf::Font& font, const std::string& labelText, sf::Vector2f position);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;
    
    void setLabel(const std::string& labelText);
    void setColor(const sf::Color& color);
    void setVisible(const bool visible);
    
    
private:
    void initialize();

private:
    Scene& m_scene;
    
    sf::VertexArray m_marker;
    sf::Vector2f m_position;
    
    sf::Font& m_font;
    std::optional<sf::Text> m_label;
};



class YMarker : public sf::Drawable {
public:
    YMarker(Scene& scene, sf::Font& font, const std::string& labelText, sf::Vector2f position);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
    void setPosition(sf::Vector2f position);
    sf::Vector2f getPosition() const;
    
    void setLabel(const std::string& labelText);
    void setColor(const sf::Color& color);
    void setVisible(const bool visible);
    
    
private:
    void initialize();

private:
    Scene& m_scene;
    
    sf::VertexArray m_marker;
    sf::Vector2f m_position;
    
    sf::Font& m_font;
    std::optional<sf::Text> m_label;
};



#endif // MARKER_HPP
