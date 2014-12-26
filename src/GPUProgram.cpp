//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// class for a GPU program (effect)
#include "GDProto.pch.h"
#include "GPUProgram.h"

#include "ResourceConstants.h"

#include "Texture.h"
#include "Shader.h"

using namespace std;
using namespace mathgp;

GPUProgram::GPUProgram()
    : GPUProgram(UNNAMED)
{

}

GPUProgram::GPUProgram(const std::string& name)
    : m_name(name)
    , m_boundTextures(0)
{
    m_glHandle = glCreateProgram();

    if (!m_glHandle)
    {
        throw(runtime_error("could not create gl program object"));
    }

    m_shaders.reserve(ShaderType::Count);
}

GPUProgram::~GPUProgram()
{
    if (m_glHandle)
        glDeleteProgram(m_glHandle);
}

void GPUProgram::link()
{
    glLinkProgram(m_glHandle);

    GLint isLinked;
    glGetProgramiv(m_glHandle, GL_LINK_STATUS, &isLinked);

    if (!isLinked)
    {
        GLint infoLen = 0;
        glGetShaderiv(m_glHandle, GL_INFO_LOG_LENGTH, &infoLen);

        if (infoLen > 1)
        {
            std::unique_ptr<char[]> infoLog(new char[infoLen]);
            glGetShaderInfoLog(m_glHandle, infoLen, nullptr, infoLog.get());

            ostringstream sout;
            sout << "errors linking " << m_name << ":" << endl;
            sout << infoLog.get() << endl;
            glDeleteProgram(m_glHandle);
            throw runtime_error(sout.str());
        }
        else
        {
            ostringstream sout;
            sout << "unkown errors linking " << m_name;
            glDeleteProgram(m_glHandle);
            throw runtime_error(sout.str());
        }
    }
}

void GPUProgram::attachShader(ConstShaderPtr shader)
{
    m_shaders.push_back(shader);

    glAttachShader(m_glHandle, shader->m_glHandle);
}

void GPUProgram::use()
{
    glUseProgram(m_glHandle);
    m_boundTextures = 0;
}

void GPUProgram::bindCustomAttribute(const char* name, int index)
{
    glBindAttribLocation(m_glHandle, index, name);
}

GLint GPUProgram::getParameterByName(const char* name) const
{
    return glGetUniformLocation(m_glHandle, name);
}

void GPUProgram::setParameter(GLint handle, float value)
{
    glUniform1f(handle, value);
}

void GPUProgram::setParameter(GLint param, const matrix& matrix)
{
    glUniformMatrix4fv(param, 1, false, matrix.as_array());
}

void GPUProgram::setParameter(GLint param, const vector2& vec)
{
    glUniform2fv(param, 1, vec.as_array());
}

void GPUProgram::setParameter(GLint param, const vector3& vec)
{
    glUniform3fv(param, 1, vec.as_array());
}

void GPUProgram::setParameter(GLint param, const vector4& vec)
{
    glUniform4fv(param, 1, vec.as_array());
}

void GPUProgram::setParameter(GLint param, const Texture& tex)
{
    glActiveTexture(GL_TEXTURE0 + m_boundTextures);
    glBindTexture(tex.type(), tex.glHandle());
    glUniform1i(param, m_boundTextures);

    ++m_boundTextures;
}
