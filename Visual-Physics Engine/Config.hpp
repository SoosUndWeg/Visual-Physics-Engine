//
//  Config.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <SFML/Graphics.hpp>

namespace config {
    namespace window {
        constexpr sf::Vector2u resolution = {1920, 1080};
        constexpr sf::Vector2u size = {960, 540};
        constexpr sf::Vector2i position = {100, 100};
        constexpr float framerateLimit = 120.f;
        constexpr std::string title = "Visual-Physik Engine";
        constexpr float pixelPerWorldUnit = 100.f;
    }

    namespace scene {
        constexpr float mouseScaleFactor = 0.02f;
        constexpr float trackpadScaleFactor = 0.5f;
        constexpr float translationFactor = 0.1f;
    }

    namespace hud {
        constexpr float distanceFromWindowBorder = 10.f;
    }

    namespace coordinateSystem {
        constexpr float gridSpacing = 100.f;
        constexpr float gridLength = 10.f;
        constexpr float markerLength = 3.f;
        constexpr float markerLabelSize = 20.f;
        constexpr float markerLabelOffset = 0.05f;
        constexpr float windowToCoordinateFactor = 0.01f;
    }

    namespace function {
        constexpr sf::Color color = sf::Color::Green;
        constexpr int coarseSteps = 1000;
        constexpr int cutoff = 1e3;
        constexpr float deltaMaxPercent = 0.02f;
        constexpr int maxDepth = 10;
        constexpr float deltaMinMultiplier = 2.0f;
    }
}

#endif
