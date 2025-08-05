//
//  TrackZoomBridge.mm
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//

// TrackpadZoomBridge.mm
#import <Cocoa/Cocoa.h>
#include "TrackpadZoomBridge.h"

static IZoomReceiver* s_receiver = nullptr;

@interface ZoomGestureCatcher : NSView
@end

@implementation ZoomGestureCatcher

- (void)magnifyWithEvent:(NSEvent *)event {
    if (s_receiver) {
        s_receiver->onZoom(event.magnification);
    }
}

@end

void InitTrackpadZoom(void* nativeWindowHandle, IZoomReceiver* receiver) {
    s_receiver = receiver;
    NSWindow* window = (__bridge NSWindow*)nativeWindowHandle;
    ZoomGestureCatcher* zoomView = [[ZoomGestureCatcher alloc] initWithFrame:window.contentView.bounds];
    zoomView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
    [window.contentView addSubview:zoomView];
    [window makeFirstResponder:zoomView];
}
