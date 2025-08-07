//
//  math.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef FUNCTION_HPP
#define FUNCTION_HPP


#include <SFML/Graphics.hpp>


#include "../parser/Parser.hpp"


class Scene;


class Function : public sf::Drawable {
public:
    Function(const std::string& name, const std::string& expression, Scene& scene, sf::Color color = sf::Color::Green);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void calculateInterval();
    void calculateInterval(Environment env);
    
    void setEnvironment(Environment env);
    
    void setVariable(const std::string& variable, double value);
    void setVariable(std::pair<std::string, double> variable);
        

private:
    void addSegment(std::vector<sf::VertexArray>& lines, sf::VertexArray& current);
    void adaptivePlot(sf::Vector2f p0, sf::Vector2f p1, double deltaYMax, double deltaXMin, int depth, int maxDepth, Environment env);

private:
    std::string m_name;
    std::string m_expression;
    
    Environment m_environment;

    std::unique_ptr<FunctionHeaderNode> m_function;

    Parser m_parser;
    Scene& m_scene;

    std::vector<sf::VertexArray> m_lines;
    sf::VertexArray m_currentLine;

    sf::Color m_color;
};

#endif // FUNCTION_HPP
