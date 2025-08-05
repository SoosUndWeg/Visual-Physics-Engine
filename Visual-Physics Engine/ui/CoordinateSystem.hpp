//
//  CoordinateSystem.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef COORDINATESYSTEM_HPP
#define COORDINATESYSTEM_HPP

#include <vector>

#include <SFML/Graphics.hpp>

#include "Axis.hpp"
#include "Marker.hpp"


class Scene;


class CoordinateSystem : public sf::Drawable {
public:
    CoordinateSystem(sf::Font& font, Scene& scene);

    void initialize();

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void update();
    
    void createXMarkers();

private:
    Scene& m_scene;
    
    XAxis m_xAxis;
    YAxis m_yAxis;
    
    std::vector<XMarker> m_xMarkers;

    sf::Font& m_font;
};


#endif // COORDINATESYSTEM_HPP
