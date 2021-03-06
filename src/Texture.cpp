//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// the basic texture class
#include "GDProto.pch.h"
#include "Texture.h"

#include "ResourceConstants.h"

using namespace std;

Texture::Texture()
    : Texture(UNNAMED)
{
}

Texture::Texture(const std::string& name)
    : m_name(name)
    , m_width(0)
    , m_height(0)
{
    glGenTextures(1, &m_glHandle);
}

Texture::~Texture()
{
    glDeleteTextures(1, &m_glHandle);
}

void Texture::loadFromFile(const char* filename)
{
    SDL_Surface* image = IMG_Load(filename);
    if (!image)
    {
        const char* err = IMG_GetError();
        throw runtime_error(err);
    }

    GLint internalFormat;
    GLenum glFormat;

    auto sdlFormat = image->format->format;

    switch (sdlFormat)
    {
    case SDL_PIXELFORMAT_RGB888:
        internalFormat = GL_RGB8;
        glFormat = GL_RGB;
        break;
    case SDL_PIXELFORMAT_RGBA8888:
    case SDL_PIXELFORMAT_ABGR8888: // png-s come with this???
        internalFormat = GL_RGBA8;
        glFormat = GL_RGBA;
        break;
    default:
        {
            internalFormat = GL_RGB8;
            glFormat = GL_RGB;

            cout << "unsupported texture format for " << filename << endl;
            assert(false);
        }
        break;
    }

    loadFromData(internalFormat, image->w, image->h, glFormat, GL_UNSIGNED_BYTE, image->pixels);

    m_width = image->w;
    m_height = image->h;

    SDL_FreeSurface(image);
}

void Texture::loadFromData(GLint internalFormat, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* data)
{
    glBindTexture(GL_TEXTURE_2D, m_glHandle);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);

    m_width = width;
    m_height = height;
}

void Texture::setParameter(GLenum param, GLint value)
{
    glTexParameteri(GL_TEXTURE_2D, param, value);
}
