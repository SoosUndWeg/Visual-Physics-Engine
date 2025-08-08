//
//  Scene.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef SCENE_HPP
#define SCENE_HPP

#include <SFML/Graphics.hpp>
#include <vector>

#include "../Config.hpp"
#include "../ui/CoordinateSystem.hpp"
#include "../math/Function.hpp"


class Application;
class EventHandler;


class Scene : public sf::Drawable{
public:
    Scene(sf::Font& font, sf::Clock& clock, Application& application);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void initialize();

    void setCallback(EventHandler& eventHandler);

    void update();
    void updateGraph();
    void setGraphDirty();

    auto begin() { return m_shapes.begin(); }
    auto end() { return m_shapes.end(); }
    auto begin() const { return m_shapes.begin(); }
    auto end() const { return m_shapes.end(); }

    void addShape(std::unique_ptr<sf::Drawable> shape);

    void translate(sf::Vector2f offset);
    
    sf::Vector2f getTranslation() const;
    void setTranslation(sf::Vector2f translation);

    void scale(sf::Vector2f factor);
    
    sf::Vector2f getScale() const;
    void setScale(sf::Vector2f scale);
    
    sf::Vector2f getViewSize() const;
    
    size_t getFunctionCount() const;
    std::shared_ptr<Function> getFunction(const std::string& name);
    std::shared_ptr<Function> getFunction(size_t index);

    void scaleAroundMouse(sf::Vector2f factor, sf::Vector2f mousePosition);
    
    sf::Vector2f worldToScreen(sf::Vector2f worldPosition) const;
    sf::Vector2f screenToWorld(sf::Vector2f screenPosition) const;
    
    bool playTime();

private:
    std::vector<std::unique_ptr<sf::Drawable>> m_shapes;

    CoordinateSystem m_coordinateSystem;

    std::vector<std::shared_ptr<Function>> m_functions;
    
    sf::Clock& m_clock;
    
    Application& m_application;

    sf::Vector2f m_translationVector = {0.0f, 0.0f};
    sf::Vector2f m_scaleVector = {1.0f, 1.0f};

    bool m_graphDirty = true;
    
    bool m_playTime = true;
};

#endif // SCENE_HPP
