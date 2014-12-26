//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// simple resource manager
// a PIMPL singleton
#pragma once

#include "ExplicitSingleton.h"

#include "ShaderType.h"

#include "TexturePtr.h"
#include "ShaderPtr.h"
#include "GPUProgramPtr.h"

class ResourceManager
{
protected:
    ResourceManager() {}
    virtual ~ResourceManager() {}
public:
    static void createInstance();
    static void destroyInstance();
    static ResourceManager& instance() { return *m_instance; }

    virtual ConstTexturePtr getTexture(const std::string& filename) = 0;
    //virtual ShaderPtr getShader(const std::string& filename) = 0;
    virtual ShaderPtr getShader(const std::string& filename, ShaderType::Type type) = 0;
    virtual GPUProgramPtr getGPUProgram(const std::string& filename) = 0;

private:
    static ResourceManager* m_instance;
};
