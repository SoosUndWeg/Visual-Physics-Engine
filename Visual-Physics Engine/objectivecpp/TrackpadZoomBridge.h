//
//  TrackpadZoomBridge.h
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//
#pragma once

class IZoomReceiver {
public:
    virtual void onZoom(float delta) = 0;
    virtual ~IZoomReceiver() = default;
};

void InitTrackpadZoom(IZoomReceiver* receiver, const char* windowTitle);

