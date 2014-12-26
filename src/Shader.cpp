//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// class for a shader
#include "GDProto.pch.h"
#include "Shader.h"

#include "ResourceConstants.h"

using namespace std;

Shader::Shader(ShaderType::Type t)
    : Shader(t, UNNAMED)
{
}

Shader::Shader(ShaderType::Type t, const std::string& name)
    : m_type(t)
    , m_name(name)
{
    switch (t)
    {
    case ShaderType::Vertex:
        m_glType = GL_VERTEX_SHADER;
        break;
    case ShaderType::Pixel:
        m_glType = GL_FRAGMENT_SHADER;
        break;
    default:
        // unknown shader type
        assert(false);
    }

    m_glHandle = glCreateShader(m_glType);

    if (!m_glHandle)
    {
        ostringstream sout;
        sout << "could not create " << (m_type == ShaderType::Vertex ? "vs" : "ps") << " object";
        throw runtime_error(sout.str());
    }
}

Shader::~Shader()
{
    if (m_glHandle)
        glDeleteShader(m_glHandle);
}


void Shader::load(const char* buffer)
{
    glShaderSource(m_glHandle, 1, &buffer, nullptr);
    glCompileShader(m_glHandle);

    GLint isCompiled;
    glGetShaderiv(m_glHandle, GL_COMPILE_STATUS, &isCompiled);

    GLint infoLen = 0;
    glGetShaderiv(m_glHandle, GL_INFO_LOG_LENGTH, &infoLen);

    if (infoLen > 1)
    {
        std::unique_ptr<char[]> infoLog(new char[infoLen]);
        glGetShaderInfoLog(m_glHandle, infoLen, nullptr, infoLog.get());

        if (!isCompiled)
        {
            ostringstream sout;
            sout << "errors compiling " << m_name << ":" << endl;
            sout << infoLog.get() << endl;
            glDeleteShader(m_glHandle);
            throw runtime_error(sout.str());
        }
        else
        {
            cout << "warnings compiling " << m_name << ":" << endl;
            cout << infoLog.get() << endl;
        }
    }
}
