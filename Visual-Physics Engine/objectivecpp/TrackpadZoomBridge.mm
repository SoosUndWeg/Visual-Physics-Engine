//
//  TrackpadZoomBridge.mm
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 03.08.25.
//
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

void InitTrackpadZoom(IZoomReceiver* receiver, const char* windowTitle) {
    s_receiver = receiver;
    NSArray* windows = [NSApp windows];
    for (NSWindow* win in windows) {
        if ([[win title] isEqualToString:[NSString stringWithUTF8String:windowTitle]]) {
            ZoomGestureCatcher* zoomView = [[ZoomGestureCatcher alloc] initWithFrame:win.contentView.bounds];
            zoomView.autoresizingMask = NSViewWidthSizable | NSViewHeightSizable;
            [win.contentView addSubview:zoomView];
            [win makeFirstResponder:zoomView];
            break;
        }
    }
}
