#pragma once

#include <U8g2lib.h>

class Screen {
public:
    virtual ~Screen() = default;

    virtual void onEnter() {}
    virtual void onExit() {}

    virtual void update() {}
    virtual void draw(U8G2& display) = 0;

    virtual void onRotate(int direction) {}
    virtual void onClick() {}
};