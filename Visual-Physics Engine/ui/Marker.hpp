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
class XAxis;


class XMarker : public sf::Drawable {
public:
    XMarker(Scene& scene, sf::Font& font, XAxis& xAxis, const std::string& labelText, double  position);
    
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
    void setPosition(double x);
    double getPosition() const;
    
    void setLabel(const std::string& labelText);
    void setColor(const sf::Color& color);
    
    
private:
    void initialize();

private:
    Scene& m_scene;
    
    sf::VertexArray m_marker;
    double m_position;
    
    sf::Font& m_font;
    std::optional<sf::Text> m_label;
    XAxis& m_xAxis;
};



#endif // MARKER_HPP
