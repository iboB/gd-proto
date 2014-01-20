//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// main application class definition
#pragma once

#include "GDProto.pch.h"
#include "Application.h"
#include "MainWindow.h"

#include "VersionInfo.h"

using namespace std;
using namespace mathgp;

Application::Application()
    : m_mainWindow(nullptr)
    , m_mainScene(nullptr)
    , m_isRunning(false)
    , m_desiredFrameTime(10) // work with 100 fps
    // fps stuff
    , m_frameCount(0)
    , m_lastFrameEnd(0)
    , m_lastFPSStatusUpdateTime(0)
    , m_lastFPSStatusUpdateFrameCount(0)
    // debug stuff
    , m_isWireframe(false)    
{
}

Application::~Application()
{
}

void Application::run()
{
    initialize();

    m_isRunning = true;
    while(m_isRunning)
    {
        unsigned frameStart = SDL_GetTicks();

        handleInput();
        drawFrame();
        updateFPSData();

        unsigned frameLength = SDL_GetTicks() - frameStart;
        if(frameLength < m_desiredFrameTime)
        {
            SDL_Delay(m_desiredFrameTime - frameLength);
        }
    }

    deinitialize();
}

void Application::initialize()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw runtime_error("couldn't init sdl");
    }

    MainWindow::CreationParameters mwc;
    mwc.clientAreaSize = v(1024u, 768u);
    mwc.isFullScreen = false;
    mwc.title = APP_NAME;
    m_mainWindow = new MainWindow(mwc);

#if defined(_WIN32)
    if(glewInit() != GLEW_OK)
    {
        throw runtime_error("couldn't init glew");
    }
#endif

    // global program features
    glEnable(GL_DEPTH_TEST); // z buffer
    glEnable(GL_CULL_FACE); // cull back (CW) faces

    glClearColor(0, 0, 0.6f, 0); // backbuffer clear color
    glClearDepth(1); // z buffer clear value
}

void Application::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_mainWindow->swapBuffers();
}

void Application::deinitialize()
{
    safe_delete(m_mainWindow);
    SDL_Quit();
}

void Application::updateFPSData()
{
    ++m_frameCount;
    
    const unsigned now = SDL_GetTicks();
    const unsigned frameTime = now - m_lastFrameEnd;
    const unsigned fpsStatusUpdateTimeDelta = now - m_lastFPSStatusUpdateTime;

    if(fpsStatusUpdateTimeDelta > 1000) // update once per second
    {
        const unsigned framesDelta = m_frameCount - m_lastFPSStatusUpdateFrameCount;
        const unsigned meanFrameTime = fpsStatusUpdateTimeDelta/framesDelta;
        const unsigned fps = (framesDelta*1000)/fpsStatusUpdateTimeDelta;

        string windowTitle;
        ostringstream sout;
        sout << APP_NAME << " " << APP_VERSION_STRING << " | mean frame time: " << meanFrameTime << " ms | fps: " << fps;

        m_mainWindow->setTitle(sout.str().c_str());

        m_lastFPSStatusUpdateTime = now;
        m_lastFPSStatusUpdateFrameCount = m_frameCount;
    }
}

void Application::handleInput()
{
    SDL_Event event;
    bool handledHere = false;
    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
        {
            m_isRunning = false;
            handledHere = true;
        }
        else if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_isRunning = false;
                handledHere = true;
                break;
            case SDLK_p:
                m_isWireframe = !m_isWireframe;
                glPolygonMode(GL_FRONT_AND_BACK, m_isWireframe ? GL_LINE : GL_FILL);
                handledHere = true;
                break;
            }
        }

        if(!handledHere)
        {
            // handle in some input controller
        }
    }
}