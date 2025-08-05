//
//  ZoomHandler.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

#include "TrackpadZoomBridge.h"


class EventHandler;


class ZoomHandler : public IZoomReceiver {
public:
    ZoomHandler(EventHandler& eventHandler);
    
    void onZoom(float zoomFactor) override;
    
private:
    EventHandler& m_eventHandler;
    
};
