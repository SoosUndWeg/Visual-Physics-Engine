//
//  math.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "Function.hpp"

#include <iostream>

#include "../Config.hpp"
#include "../core/Scene.hpp"


Function::Function(const std::string& name, const std::string& expression, Scene& scene, sf::Color color) :
    m_name(name),
    m_expression(expression),
    m_color(color),
    m_scene(scene) {
        
    m_parser.setExpression(expression);

    try {
        std::unique_ptr<ASTNode> node = m_parser.parse();

        if (auto* raw = dynamic_cast<FunctionHeaderNode*>(node.get())) {
            std::unique_ptr<FunctionHeaderNode> funcHeader(static_cast<FunctionHeaderNode*>(node.release()));
            m_function = std::move(funcHeader);
        }

    } catch (const std::exception& e) {
        std::cerr << "Error parsing function: " << e.what() << std::endl;
        throw;
    }
}


void Function::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    for (const auto& line : m_lines) {
        target.draw(line, states);
    }
}

void Function::update() {
    if (m_graphDirty) {
        m_graphDirty = false;
        
        
        if (m_flags & IntervalCalculated) {
            
            calculateInterval();
            
        } else if (m_flags & Waveform && m_flags & TimeDependent) {
            calculateWave();
        }
    }
}


void Function::addSegment(std::vector<sf::VertexArray>& lines, sf::VertexArray& current) {
    
    if (current.getVertexCount() > 1) {
        
        lines.push_back(current);
    }
    
    current = sf::VertexArray(sf::PrimitiveType::LineStrip);
}

bool Function::hasVariable(const std::string& variable) const {
    return m_environment.contains(variable);
}


void Function::setEnvironment(Environment env) {
    m_environment = env;
    if (m_flags & XPlot) {
        m_environment["x"] = 0.0;
    }
}

Environment& Function::getEnvironment() {
    return m_environment;
}

void Function::initializeEnvironment() {
    for (const auto& parameter : m_function->getParameters()) {
        m_environment[parameter] = 0.0;
    }
}
    

void Function::setVariable(const std::string& variable, double value) {
    m_environment[variable] = value;
}

void Function::setVariable(std::pair<std::string, double> variable) {
    m_environment[variable.first] = variable.second;
}

void Function::setTime(double time) {
    
    if (m_flags & TimeDependent && m_flags & Animated) {
        setVariable("t", time);
        graphDirty();
    }
}


void Function::setFlag(uint32_t flag) {
    m_flags = flag;
}

void Function::addFlag(uint32_t flag) {
    m_flags |= flag;
}

uint32_t Function::getFlags() const {
    return m_flags;
}


std::vector<std::string> Function::getParameters() const {
    
    std::vector<std::string> parameters;
    
    if (m_function) {
        
        parameters = m_function->getParameters();
    }
    
    for (int i = 0; i < parameters.size(); ++i) {
        if (parameters[i] == "x") {
            parameters.erase(parameters.begin() + i);
        } else if (m_flags & TimeDependent && parameters[i] == "t") {
            parameters.erase(parameters.begin() + i);
        }
    }
    
    return parameters;
}

void Function::calculateInterval() {
    
    calculateInterval(m_environment);
}

void Function::calculateInterval(Environment env) {
    
    float& x = env.at("x");
    
    m_currentLine.clear();
    m_lines.clear();

    sf::Vector2f worldOrigin = m_scene.getTranslation();
    sf::Vector2f viewSize = m_scene.getViewSize();

    double xMin = worldOrigin.x - viewSize.x;
    double xMax = worldOrigin.x + viewSize.x;
    
    double gridLength = viewSize.x / config::function::coarseSteps;
    int nSteps = static_cast<int>((xMax - xMin) / gridLength);
    
    bool lastValid = false;
    
    double lastWorldX = xMin;
    double lastWorldY = m_function->evaluate(env);

    for (int i = 1; i <= nSteps; ++i) {
        
        x = xMin + i * gridLength;
        double y = m_function->evaluate(env);
        
        bool valid = !std::isnan(y) && !std::isinf(y);

        if (!lastValid && valid) {
            
            m_currentLine.append(sf::Vertex(m_scene.worldToScreen(sf::Vector2f(x, y)), m_color));
            
        } else if (lastValid && valid) {
            
            adaptivePlot("x", sf::Vector2f(lastWorldX, lastWorldY), sf::Vector2f(x, y), {0.f, 0.f}, 0, config::function::maxDepth, env);
            
        } else if (lastValid && !valid) {
            
            addSegment(m_lines, m_currentLine);
        }
        
        lastValid = valid;
        
        lastWorldX = x;
        lastWorldY = y;
    }
    addSegment(m_lines, m_currentLine);
}


void Function::calculateWave() {
    calculateWave(m_environment);
}

void Function::calculateWave(Environment env) {
    
    float& t = env.at("t");
    
    sf::Vector2f viewSize = m_scene.getViewSize();
    sf::Vector2f worldOrigin = m_scene.getTranslation();

    // t = now is always at x = 0, because t gets translated by t - tauMax before plotting
    // tMin is the left edge of the view, tMax is the current time
    double tauMin = std::max(0.0, static_cast<double>(t + worldOrigin.x - viewSize.x));
    double tauMax = t;
    
    double tau = tauMin;

    double gridLength = viewSize.x / config::function::coarseSteps;
    int nSteps = static_cast<int>((tauMax - tauMin) / gridLength);

    bool lastValid = false;
    double lastT = t, lastY = m_function->evaluate(env);

    m_currentLine.clear();
    m_lines.clear();

    for (int i = 0; i < nSteps; ++i) {
        tau = tauMin + i * gridLength;
        
        if (tau > tauMax) {
            break;
        }

        t = tau;

        double y = m_function->evaluate(env);

        bool valid = !std::isnan(y) && !std::isinf(y);

        if (!lastValid && valid) {
            m_currentLine.append(sf::Vertex(m_scene.worldToScreen(sf::Vector2f(t - tauMax, y)), m_color));
        } else if (lastValid && valid) {
            adaptivePlot("t", sf::Vector2f(lastT, lastY), sf::Vector2f(t, y), {- static_cast<float>(tauMax),0.f}, 0, config::function::maxDepth, env);
        } else if (lastValid && !valid) {
            addSegment(m_lines, m_currentLine);
        }

        lastValid = valid;
        lastT = t;
        lastY = y;
    }
    addSegment(m_lines, m_currentLine);
}


void Function::adaptivePlot(std::string key, sf::Vector2f p0, sf::Vector2f p1, sf::Vector2f offset, int depth, int maxDepth, Environment env) {
    
    sf::Vector2f viewSize = m_scene.getViewSize();
    
    double deltaYMax = viewSize.y * config::function::deltaMaxPercent;
    double deltaXMin = viewSize.x * config::function::deltaMinMultiplier;
    
    if (depth >= maxDepth) {
        
        m_currentLine.append(sf::Vertex(m_scene.worldToScreen({(p1.x + offset.x), p1.y + offset.y}), m_color));
        
        return;
    }

    if (std::abs(p1.y - p0.y) > deltaYMax || std::abs(p1.x - p0.x) > deltaXMin) {
        
        // Polstelle
        if (std::abs(p0.y) > config::function::cutoff &&
            std::abs(p1.y) > config::function::cutoff &&
            p0.y * p1.y < 0) {

            addSegment(m_lines, m_currentLine);
            return;
        }
        
        // Polstelle
        if (std::abs(p1.y - p0.y) > deltaYMax &&
           (std::abs(p0.y) > config::function::cutoff || std::abs(p1.y) > config::function::cutoff)) {
            
            addSegment(m_lines, m_currentLine);
            return;
        }
        
        // Polstelle
        if (std::abs(p1.y - p0.y) > deltaYMax * 200 &&
            std::abs((p1.y - p0.y) / (p1.x - p0.x)) > deltaYMax * 100) {
            
            addSegment(m_lines, m_currentLine);
            return;
        }

        float& xm = env.at(key);
        
        xm = (p0.x + p1.x) / 2.f;
        double ym = m_function->evaluate(env);
        
        if (std::isnan(ym) || std::isinf(ym)) {
            
            addSegment(m_lines, m_currentLine);
            return;
        }
        
        sf::Vector2f mid(xm, ym);
        
        adaptivePlot(key, p0, mid, offset, depth + 1, maxDepth, env);
        adaptivePlot(key, mid, p1, offset, depth + 1, maxDepth, env);
        
    } else {
        
        m_currentLine.append(sf::Vertex(m_scene.worldToScreen({(p1.x + offset.x), p1.y + offset.y}), m_color));
    }
}
