//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// inline file with loaders for the different resource types

struct GPUProgramLoader
{
    GPUProgramLoader() : program(nullptr) 
    {
    }
    
    void setName(const std::string& name)
    {
        program = new GPUProgram(name);
    }

    void setVertexShader(const std::string& filename)
    {
        assert(program);
        auto shader = ResourceManager::instance().getShader(filename, ShaderType::Vertex);
        program->attachShader(shader);
    }

    void setPixelShader(const std::string& filename)
    {
        assert(program);
        auto shader = ResourceManager::instance().getShader(filename, ShaderType::Pixel);
        program->attachShader(shader);
    }

    GPUProgram* program;
};
