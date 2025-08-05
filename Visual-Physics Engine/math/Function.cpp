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

void Function::addSegment(std::vector<sf::VertexArray>& lines, sf::VertexArray& current) {
    if (current.getVertexCount() > 1) {
        lines.push_back(current);
    }
    current = sf::VertexArray(sf::PrimitiveType::LineStrip);
}

void Function::calculateInterval() {
    calculateInterval(0.0);
}

void Function::calculateInterval(double t) {
    m_currentLine.clear();
    m_lines.clear();

    sf::Vector2f worldOrigin = m_scene.getTranslation();
    sf::Vector2f zoom = m_scene.getScale();
    sf::Vector2f baseViewSize = m_scene.getViewSize();
    
    sf::Vector2f viewSize;
    viewSize.x = baseViewSize.x / zoom.x;
    viewSize.y = baseViewSize.y / zoom.y;
    
    sf::Vector2f windowSize = static_cast<sf::Vector2f>(config::window::size);
    
    double deltaYMax = viewSize.y * config::function::deltaMaxPercent;
    double deltaXMin = viewSize.x * config::function::deltaMinMultiplier;

    
    double xMin = worldOrigin.x - viewSize.x;
    double xMax = worldOrigin.x + viewSize.x;
    
    double gridLength = viewSize.x / config::function::coarseSteps;

    int nSteps = static_cast<int>((xMax - xMin) / gridLength);
    

    bool lastValid = false;
    double lastWorldX = xMin;
    double lastWorldY = m_function->evaluateWithParameters({xMin, t});

    for (int i = 1; i <= nSteps; ++i) {
        double x = xMin + i * gridLength;
        double y = m_function->evaluateWithParameters({x, t});
        bool valid = !std::isnan(y) && !std::isinf(y);

        if (!lastValid && valid) {
            
            double px = (x - worldOrigin.x) / viewSize.x * windowSize.x + windowSize.x / 2.0;
            double py = -(y - worldOrigin.y) / viewSize.y * windowSize.y + windowSize.y / 2.0;
            m_currentLine.append(sf::Vertex(m_scene.worldToScreen({static_cast<float>(px), static_cast<float>(py)}), m_color));
        } else if (lastValid && valid) {
            adaptivePlot(
                sf::Vector2f(lastWorldX, lastWorldY),
                sf::Vector2f(x, y),
                deltaYMax, deltaXMin, 0, config::function::maxDepth,
                worldOrigin, viewSize, windowSize, t
            );
        } else if (lastValid && !valid) {
            addSegment(m_lines, m_currentLine);
        }
        lastValid = valid;
        lastWorldX = x;
        lastWorldY = y;
    }
    addSegment(m_lines, m_currentLine);
}


void Function::adaptivePlot(sf::Vector2f p0, sf::Vector2f p1,
    double deltaYMax, double deltaXMin, int depth, int maxDepth,
    sf::Vector2f worldOrigin, sf::Vector2f viewSize, sf::Vector2f windowSize, double t)
{
    if (depth >= maxDepth) {
        
        double px = (p1.x - worldOrigin.x) / viewSize.x * windowSize.x + windowSize.x / 2.0;
        double py = -(p1.y - worldOrigin.y) / viewSize.y * windowSize.y + windowSize.y / 2.0;
        m_currentLine.append(sf::Vertex(m_scene.worldToScreen({static_cast<float>(px), static_cast<float>(py)}), m_color));
        return;
    }

    if (std::abs(p1.y - p0.y) > deltaYMax || std::abs(p1.x - p0.x) > deltaXMin) {
        // Polstelle
        if (std::abs(p0.y) > config::function::cutoff &&
            std::abs(p1.y) > config::function::cutoff && p0.y * p1.y < 0) {

            addSegment(m_lines, m_currentLine);
            return;
        }
        // Polstelle
        if (std::abs(p1.y - p0.y) > deltaYMax && (std::abs(p0.y) > config::function::cutoff || std::abs(p1.y) > config::function::cutoff)) {
            addSegment(m_lines, m_currentLine);
            return;
        }
        if (std::abs(p1.y - p0.y) > deltaYMax * 200 && std::abs((p1.y - p0.y) / (p1.x - p0.x)) > deltaYMax * 100) {
            addSegment(m_lines, m_currentLine);
            return;
        }

        double xm = (p0.x + p1.x) / 2.f;
        double ym = m_function->evaluateWithParameters({xm, t});
        if (std::isnan(ym) || std::isinf(ym)) {
            addSegment(m_lines, m_currentLine);
            return;
        }
        sf::Vector2f mid(xm, ym);
        adaptivePlot(p0, mid, deltaYMax, deltaXMin, depth + 1, maxDepth, worldOrigin, viewSize, windowSize, t);
        adaptivePlot(mid, p1, deltaYMax, deltaXMin, depth + 1, maxDepth, worldOrigin, viewSize, windowSize, t);
    } else {
        m_currentLine.append(sf::Vertex(m_scene.worldToScreen({static_cast<float>(p1.x), static_cast<float>(p1.y)}), m_color));
    }
}
    

