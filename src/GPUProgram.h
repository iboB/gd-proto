//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// class for a GPU program (effect)
#pragma once

#include "ShaderPtr.h"

class Texture;

class GPUProgram
{
public:
    GPUProgram();
    GPUProgram(const std::string& name); // name is used for debugging purposes
    ~GPUProgram();

    void attachShader(ConstShaderPtr shader);

    void link();
    void use();

    GLint getParameterByName(const char* name) const;

    void bindCustomAttribute(const char* name, int index);

    void setParameter(GLint handle, float value);
    void setParameter(GLint handle, const mathgp::matrix& matrix);
    void setParameter(GLint handle, const mathgp::vector2& vec);
    void setParameter(GLint handle, const mathgp::vector3& vec);
    void setParameter(GLint handle, const mathgp::vector4& vec);
    void setParameter(GLint handle, const Texture& tex);

private:
    friend class ResourceManager;
    const std::string m_name;

    GLuint m_glHandle;

    std::vector<ConstShaderPtr> m_shaders;

    size_t m_boundTextures;
};
