//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// main application class declaration
#pragma once

#include "ImplicitSingleton.h"

class MainWindow;

class Application : public ImplicitSingleton<Application>
{
public:
    Application();
    ~Application();

    void run();

    const mathgp::uvector2& screenSize() const;

private:
    void initialize();
    void deinitialize();

    MainWindow* m_mainWindow;

    bool m_isRunning;

    //////////////////////////////////////
    // input
    void handleInput();

    //////////////////////////////////////
    // rendering
    void drawFrame();

    //////////////////////////////////////
    // time
public:
    uint currentFrameTime() const { return m_currentFrameTime; }

private:
    uint m_currentFrameTime; // start of current frame (ms)
    uint m_timeSinceLastFrame;

    //////////////////////////////////////
    // fps stuff
    uint  m_desiredFrameTime; // the application will try to work at this time per frame

    void updateFPSData();

    uint m_frameCount; // number of frames since the app has started
    uint m_lastFrameEnd; // end time of the last frame
    uint m_lastFPSStatusUpdateTime; // last time the fps status was updated
    uint m_lastFPSStatusUpdateFrameCount; // frame count of the last time the stasus was updated

    //////////////////////////////////////
    // debugging and so forth
    bool m_isWireframe;
};
