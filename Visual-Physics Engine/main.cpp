//
//  main.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include <iostream>

#include <SFML/Graphics.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

#include "core/Application.hpp"

int main(int argc, const char * argv[]) {
    
    try {
        Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "An error occurred: " << e.what() << std::endl;
    }
    
    ImGui::SFML::Shutdown();
    
    return EXIT_SUCCESS;
}
