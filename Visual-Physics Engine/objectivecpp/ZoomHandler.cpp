//
//  ZoomHandler.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//
#include "ZoomHandler.hpp"

#include "../core/EventHandler.hpp"


ZoomHandler::ZoomHandler(EventHandler& eventHandler)
: m_eventHandler(eventHandler) {}


void  ZoomHandler::onZoom(float zoomFactor) {
    m_eventHandler.trackpadZoomed(zoomFactor);
}
    
