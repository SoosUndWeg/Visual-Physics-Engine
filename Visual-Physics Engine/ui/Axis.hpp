//
//  Axis.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 04.08.25.
//
#ifndef AXIS_HPP
#define AXIS_HPP

#include <SFML/Graphics.hpp>


class Scene;


class XAxis : public sf::Drawable {
public:
    
    XAxis(std::string name, Scene& scene, const sf::Font& font);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
    sf::Vector2f getPosition() const;
    
private:
    void setScreenPosition(double y);
    void setWorldPosition(double y);
    
    void initialize();
    
private:
    sf::VertexArray m_xAxis;
    sf::Text m_xAxisLabel;
    
    Scene& m_scene;
    const sf::Font& m_font;
};


class YAxis : public sf::Drawable {
public:
    
    YAxis(std::string name, Scene& scene, const sf::Font& font);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
private:
    void setScreenPosition(double x);
    void setWorldPosition(double x);
    
    void initialize();
    
private:
    sf::VertexArray m_yAxis;
    sf::Text m_yAxisLabel;
    
    Scene& m_scene;
    const sf::Font& m_font;
};

#endif // AXIS_HPP
