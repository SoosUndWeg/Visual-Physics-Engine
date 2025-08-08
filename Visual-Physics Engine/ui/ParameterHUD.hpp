//
//  ParameterHUD.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 07.08.25.
//
#ifndef PARAMETERHUD_HPP
#define PARAMETERHUD_HPP

#include <SFML/Graphics.hpp>

#include "imgui-SFML.h"


class Function;


struct FunctionHeaderNode;

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

#endif
