//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// simple resource manager
#include "GDProto.pch.h"
#include "ResourceManager.h"

#include "Util.h"
#include "VertexAttrib.h"

// resources
#include "Texture.h"
#include "Shader.h"
#include "GPUProgram.h"

//#include "LuaManager.h"
//#include <luabind/luabind.hpp>

using namespace std;

#include "Loaders.inl"

class ResourceManager_Impl : public ResourceManager
{
public:
    ResourceManager_Impl()
    {
        //// prepare file formats for lua export
        //using namespace luabind;

        //auto state = LuaManager::instance().state();

        //module(state)
        //[
        //    class_<GPUProgramLoader>("GPUProgramLoader")
        //        .def("set_name", &GPUProgramLoader::setName)
        //        .def("set_vertex_shader", &GPUProgramLoader::setVertexShader)
        //        .def("set_pixel_shader", &GPUProgramLoader::setPixelShader)
        //];
    }

    ConstTexturePtr getTexture(const string& filename) override
    {
        auto t = m_textures.find(filename);

        if (t != m_textures.end())
        {
            return t->second;
        }

        TexturePtr ret = make_shared<Texture>(filename);

        // first insert and then try to load
        // if there's an exception on load the empty texture should still be available
        m_textures.insert({ filename, ret });

        ret->loadFromFile(filename.c_str());
        // here we can try-catch and set some bad texture image

        return ret;
    }

    //ShaderPtr getShader(const string& filename)
    //{
    //    auto s = m_shaders.find(filename);
    //
    //    if (s != m_shaders.end())
    //    {
    //        return s->second;
    //    }
    //
    //    // determine type by vert/frag extension
    //}

    ShaderPtr getShader(const string& filename, ShaderType::Type type) override
    {
        auto s = m_shaders.find(filename);

        if (s != m_shaders.end())
        {
            return s->second;
        }

        return loadShader(filename, type);
    }

    ShaderPtr loadShader(const string& filename, ShaderType::Type type)
    {
        ShaderPtr ret = make_shared<Shader>(type, filename);

        // first insert and then try to load
        // if there's an exception on load the empty shader should still be available
        m_shaders.insert({ filename, ret });

        const char* fileData = Util::LoadFile(filename.c_str());
        ret->load(fileData);
        // here we can try-catch and set some bad shader example

        return ret;
    }

    GPUProgramPtr getGPUProgram(const string& filename) override
    {
        assert(false); // temporarily disabled. When adding lua uncomment code in GPUProgramPtr constructor too
        return nullptr;
        //auto p = m_gpuPrograms.find(filename);

        //if (p != m_gpuPrograms.end())
        //{
        //    return p->second;
        //}

        //// insert first
        //// if there's a load error we don't want to load it over and over again
        //auto inserted = m_gpuPrograms.insert({ filename, m_badProgram });

        //GPUProgramLoader loader;
        //
        //luabind::globals(LuaManager::instance().state())["cur_gpu_program"] = &loader;
        //LuaManager::instance().loadScript(filename);

        //Attrib::BindCustomAttributesToProgram(*loader.program);
        //loader.program->link();

        //inserted.first->second.reset(loader.program);

        //return inserted.first->second;
    }

    unordered_map<string, TexturePtr> m_textures;
    unordered_map<string, ShaderPtr> m_shaders;
    
    unordered_map<string, GPUProgramPtr> m_gpuPrograms;
    GPUProgramPtr m_badProgram;
};

ResourceManager* ResourceManager::m_instance;

void ResourceManager::createInstance()
{
    m_instance = new ResourceManager_Impl;
}

void ResourceManager::destroyInstance()
{
    delete m_instance;
}