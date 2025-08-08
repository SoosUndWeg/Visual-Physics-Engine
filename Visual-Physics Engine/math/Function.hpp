//
//  math.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <atomic>


#include <SFML/Graphics.hpp>


#include "../parser/Parser.hpp"


class Scene;
class ThreadManager;


class Function : public sf::Drawable {
public:
    enum Flag : uint32_t {
        None = 0,
        IntervalCalculated = 1 << 0,
        TimeDependent = 1 << 1,
        NoParameters = 1 << 2,
        Animated = 1 << 3,
        XPlot = 1 << 4,
        Waveform = 1 << 5
    };
public:
    Function(const std::string& name, const std::string& expression, Scene& scene, ThreadManager& threadManager, sf::Color color = sf::Color::Green);

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    
    void update();
    
    
    void setEnvironment(Environment env);
    Environment& getEnvironment();
    void initializeEnvironment();
    
    bool hasVariable(const std::string& variable) const;
    
    void setVariable(const std::string& variable, double value);
    void setVariable(std::pair<std::string, double> variable);
    
    void setTime(double time);
    
    void setFlag(uint32_t flag);
    void addFlag(uint32_t flag);
    uint32_t getFlags() const;
    
    std::string getName() const { return m_name; }
    
    std::vector<std::string> getParameters() const;
        
    void graphDirty(bool dirty = true) { m_graphDirty = dirty; }

private:
    void calculateInterval();
    void calculateInterval(Environment env);
    
    void calculateWave();
    void calculateWave(Environment env);
    
    void addSegment(std::vector<sf::VertexArray>& lines, sf::VertexArray& current);
    std::vector<sf::VertexArray> adaptivePlot(std::string key, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f offset, int depth, int maxDepth, Environment env);
    void adaptivePlot(std::string key, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f offset, int depth, int maxDepth, Environment env, std::vector<sf::VertexArray>& lines, sf::VertexArray& currentLine);

private:
    std::string m_name;
    std::string m_expression;
    
    Environment m_environment;

    std::unique_ptr<FunctionHeaderNode> m_function;
    uint32_t m_flags = None;

    Parser m_parser;
    Scene& m_scene;

    std::vector<sf::VertexArray> m_lines;
    sf::VertexArray m_currentLine;

    sf::Color m_color;
    
    bool m_graphDirty = true;
    
    ThreadManager& m_threadManager;
    
    std::atomic<int> m_emptyCount{0};
};

#endif // FUNCTION_HPP
