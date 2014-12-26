//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// main application class definition
#include "GDProto.pch.h"
#include "Application.h"
#include "MainWindow.h"

#include "VersionInfo.h"

#include "GameStateManager.h"
#include "ResourceManager.h"
//tmp #include "LuaManager.h"
//tmp #include "PerFrameMemoryPool.h"

#include "GUI.h"

#include "MainMenuState.h"
//#include "InGameState.h"

//tmp #include "DebugVisualization.h"

using namespace std;
using namespace mathgp;

Application::Application()
    : m_mainWindow(nullptr)
    , m_isRunning(false)
    , m_currentFrameTime(0)
    , m_desiredFrameTime(10) // work with 100 fps
    // fps stuff
    , m_frameCount(0)
    , m_lastFrameEnd(0)
    , m_lastFPSStatusUpdateTime(0)
    , m_lastFPSStatusUpdateFrameCount(0)
    , m_timeSinceLastFrame(0)
    // debug stuff
    , m_isWireframe(false)
{
    //tmp boost::mixin::add_new_mutation_rule(new boost::mixin::mandatory_mixin<DebugVisualization>());
}

Application::~Application()
{
}

void Application::run()
{
    initialize();

    m_currentFrameTime = SDL_GetTicks() - 1; // otherwise first frame time is too long (or zero if no -1)

    m_isRunning = true;
    while (m_isRunning)
    {
        uint now = SDL_GetTicks();
        m_timeSinceLastFrame = now - m_currentFrameTime;
        m_currentFrameTime = now;

        handleInput();

        GameStateManager::instance().updateStates(m_timeSinceLastFrame);

        drawFrame();

        //tmp PerFrameMemoryPool::instance().reset();

        ////////////////////////////////////////////////////////////////////////////
        updateFPSData();

        // force framerate
        uint frameLength = SDL_GetTicks() - m_currentFrameTime;
        if (frameLength < m_desiredFrameTime)
        {
            SDL_Delay(m_desiredFrameTime - frameLength);
        }
    }

    deinitialize();
}

void Application::initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        throw runtime_error("couldn't init sdl");
    }

    MainWindow::CreationParameters mwc;
    mwc.clientAreaSize = v(800u, 600u);
    mwc.isFullScreen = false;
    mwc.title = APP_NAME;
    m_mainWindow = new MainWindow(mwc);

#if defined(_WIN32)
    if (glewInit() != GLEW_OK)
    {
        throw runtime_error("couldn't init glew");
    }
#endif

    //////////////////////////////////////
    // scripts
    //tmp LuaManager::instance().loadScript("scripts/common.lua");
    //tmp LuaManager::instance().loadScript("scripts/loaders.lua");

    //////////////////////////////////////
    // graphhics

    // global program features
    glEnable(GL_DEPTH_TEST); // z buffer
    glEnable(GL_CULL_FACE); // cull back (CW) faces

    glClearColor(0.6f, 0.6f, 0.6f, 1); // backbuffer clear color
    glClearDepth(1); // z buffer clear value

    //////////////////////////////////////
    // explicit singletons
    ResourceManager::createInstance();
    //tmp PerFrameMemoryPool::createInstance(1024 * 1024);

    //////////////////////////////////////
    // gui
    GUI::createInstance();
    GUI::instance().loadFont("gui/fonts/atari.ttf");

    //////////////////////////////////////
    // states
    GameStateManager::createInstance();
    GameStateManager::instance().setState(new MainMenuState);    
}

void Application::drawFrame()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    GameStateManager::instance().drawFrame();

    m_mainWindow->swapBuffers();
}

void Application::deinitialize()
{
    GameStateManager::destroyInstance();
    GUI::destroyInstance();

    //tmp PerFrameMemoryPool::destroyInstance();
    ResourceManager::destroyInstance();
    safe_delete(m_mainWindow);
    SDL_Quit();
}

void Application::updateFPSData()
{
    ++m_frameCount;

    const uint now = SDL_GetTicks();
    const uint frameTime = now - m_lastFrameEnd;
    const uint fpsStatusUpdateTimeDelta = now - m_lastFPSStatusUpdateTime;

    if (fpsStatusUpdateTimeDelta > 1000) // update once per second
    {
        const uint framesDelta = m_frameCount - m_lastFPSStatusUpdateFrameCount;
        const uint meanFrameTime = fpsStatusUpdateTimeDelta / framesDelta;
        const uint fps = (framesDelta * 1000) / fpsStatusUpdateTimeDelta;

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
    while (SDL_PollEvent(&event))
    {
        if (GameStateManager::instance().processEvent(event))
        {
            continue;
        }

        if (event.type == SDL_QUIT)
        {
            m_isRunning = false;
        }
        else if (event.type == SDL_KEYUP)
        {
            switch (event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                m_isRunning = false;
                break;
            case SDLK_p:
                m_isWireframe = !m_isWireframe;
                glPolygonMode(GL_FRONT_AND_BACK, m_isWireframe ? GL_LINE : GL_FILL);
                break;
            }
        }
    }
}

const uvector2& Application::screenSize() const
{
    return m_mainWindow->clientAreaSize();
}
