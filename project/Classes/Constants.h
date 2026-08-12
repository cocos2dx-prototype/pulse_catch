//
//  Constants.h
//  PulseCatch-desktop
//
//  Created by hoangbui-fe on 11/8/26.
//

#ifndef Constants_h
#define Constants_h

#include "cocos2d.h"

namespace PulseCatch
{
    constexpr float DESIGN_WIDTH = 960.0f;
    constexpr float DESIGN_HEIGHT = 640.0f;

    constexpr float CENTER_X = DESIGN_WIDTH * 0.5f;
    constexpr float CENTER_Y = DESIGN_HEIGHT * 0.5f;

    constexpr float RING_MAX_RADIUS = 600.0f;
    constexpr float INITIAL_RING_SPEED = 200.0f;
    constexpr float SPEED_STEP = 10.0f;
    constexpr int SUCCESSFUL_TAPS_PER_SPEED_STEP = 5;

    constexpr float TARGET_RADIUS = 16.0f;
    constexpr float INITIAL_VISIBILITY_WINDOW = 3.0f;
    constexpr float VISIBILITY_STEP = 0.1f;
    constexpr float MIN_VISIBILITY_WINDOW = 1.5f;
    constexpr int SUCCESSFUL_TAPS_PER_VISIBILITY_STEP = 5;

    constexpr float PULSE_TOLERANCE = 20.0f;
    constexpr int INITIAL_LIVES = 3;

    constexpr float RING_STROKE_WIDTH = 3.0f;
    constexpr unsigned int RING_SEGMENTS = 128;
    constexpr unsigned int TARGET_SEGMENTS = 48;

    constexpr float HUD_MARGIN = 20.0f;
    constexpr float HUD_FONT_SIZE = 30.0f;
    constexpr float GAME_OVER_FONT_SIZE = 42.0f;
    constexpr float RESULT_FONT_SIZE = 26.0f;

    const cocos2d::Vec2 SCREEN_CENTER(CENTER_X, CENTER_Y);

    const cocos2d::Color4F BACKGROUND_COLOR(
        0.005f,
        0.02f,
        0.10f,
        1.0f
    );

    const cocos2d::Color4F WHITE(
        1.0f,
        1.0f,
        1.0f,
        1.0f
    );

    const cocos2d::Color4F CORAL_RED(
        1.0f,
        0.30f,
        0.28f,
        1.0f
    );
}

#endif /* Constants_h */
