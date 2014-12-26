//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// render interface for libRocket
#include "GDProto.pch.h"
#include "GUIRenderInterface.h"

#include "Application.h"
#include "Texture.h"
#include "GUIAttribs.h"
#include "GPUProgram.h"
#include "Shader.h"

#include <Rocket/Core/Context.h>

using namespace std;
using namespace mathgp;

// The commented out line "precision mediump float;" was issuing a warning
// on Windows.
// Think of a way to gave it for opengl es only
static const char* FragmentTextureShaderSource =
"\
    /*precision mediump float;*/ \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    uniform sampler2D u_texture; \
    \
    void main() \
    { \
        gl_FragColor = texture2D( u_texture, v_texCoord ) * v_color; \
    } \
";

static const char* FragmentColorShaderSource =
"\
    /*precision mediump float;*/ \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    \
    void main() \
    { \
        gl_FragColor = v_color; \
    } \
";

static const char* VertexShaderSource =
"\
    attribute vec2 a_position; \
    attribute vec2 a_texCoord; \
    attribute vec4 a_color; \
    varying vec2 v_texCoord; \
    varying vec4 v_color; \
    varying vec2 v_position; \
    uniform mat4 u_projection; \
    uniform vec2 u_translation; \
    \
    void main() \
    { \
        v_position = a_position + u_translation; \
        gl_Position = u_projection * vec4(v_position, 0.0, 1.0); \
        v_texCoord = a_texCoord; \
        v_color = a_color; \
    }\
";


GUIRenderInterface::GUIRenderInterface()
{
    glGetError(); // clear error state so as to not confuse opengl about the compilation errors

    auto texturePixelShader = make_shared<Shader>(ShaderType::Pixel, "gui texture pixel shader");
    texturePixelShader->load(FragmentTextureShaderSource);

    auto colorPixelShader = make_shared<Shader>(ShaderType::Pixel, "gui texture color shader");
    colorPixelShader->load(FragmentColorShaderSource);

    auto vertexShader = make_shared<Shader>(ShaderType::Vertex, "gui vertex shader");
    vertexShader->load(VertexShaderSource);

    ///////////////////////////////////////////////////
    // create the color program
    m_colorProgram = make_shared<GPUProgram>("gui color program");

    m_colorProgram->attachShader(vertexShader);
    m_colorProgram->attachShader(colorPixelShader);

    m_colorProgram->bindCustomAttribute("a_position", Attrib_Position);
    m_colorProgram->bindCustomAttribute("a_texCoord", Attrib_TexCoord);
    m_colorProgram->bindCustomAttribute("a_color", Attrib_Color);

    m_colorProgram->link();

    m_colorProjection = m_colorProgram->getParameterByName("u_projection");
    m_colorTranslation = m_colorProgram->getParameterByName("u_translation");

    ///////////////////////////////////////////////////
    // create the texture program
    m_textureProgram = make_shared<GPUProgram>("gui color program");

    m_textureProgram->attachShader(vertexShader);
    m_textureProgram->attachShader(texturePixelShader);

    m_textureProgram->bindCustomAttribute("a_position", Attrib_Position);
    m_textureProgram->bindCustomAttribute("a_texCoord", Attrib_TexCoord);
    m_textureProgram->bindCustomAttribute("a_color", Attrib_Color);

    m_textureProgram->link();

    m_texture = m_textureProgram->getParameterByName("u_texture");
    m_textureProjection = m_textureProgram->getParameterByName("u_projection");
    m_textureTranslation = m_textureProgram->getParameterByName("u_translation");
}


void GUIRenderInterface::RenderGeometry(Rocket::Core::Vertex* vertices, int numVertices,
    int* indices, int numIndices, Rocket::Core::TextureHandle textureHandle,
    const Rocket::Core::Vector2f& translation)
{
    auto size = GetContext()->GetDimensions();

    // custom ortho 2d matrix
    const matrix projection = matrix::rows(
        2.f/size.x, 0,          0,     -1,
        0,          -2.f/size.y,0,      1,
        0,          0,          0,      0,
        0,          0,          0,      1
    );

    if (textureHandle)
    {
        m_textureProgram->use();
        m_textureProgram->setParameter(m_textureProjection, projection);
        m_textureProgram->setParameter(m_textureTranslation, vc(translation.x, translation.y));
    }
    else
    {
        m_colorProgram->use();
        m_colorProgram->setParameter(m_colorProjection, projection);
        m_colorProgram->setParameter(m_colorTranslation, vc(translation.x, translation.y));
    }

    glVertexAttribPointer(Attrib_Position, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices->position);    
    glVertexAttribPointer(Attrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Rocket::Core::Vertex), &vertices->colour);

    if (textureHandle)
    {
        auto tex = reinterpret_cast<Texture*>(textureHandle);
        m_textureProgram->setParameter(m_texture, *tex);
        glVertexAttribPointer(Attrib_TexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(Rocket::Core::Vertex), &vertices->tex_coord);
        glEnableVertexAttribArray(Attrib_TexCoord);
    }
    else
    {
        glDisableVertexAttribArray(Attrib_TexCoord);
    }

    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, indices);
}

Rocket::Core::CompiledGeometryHandle GUIRenderInterface::CompileGeometry(Rocket::Core::Vertex*, int, int*, int, Rocket::Core::TextureHandle)
{
    // no obvious need to support this yet
    return 0;
}
void GUIRenderInterface::RenderCompiledGeometry(Rocket::Core::CompiledGeometryHandle, const Rocket::Core::Vector2f&) {}
void GUIRenderInterface::ReleaseCompiledGeometry(Rocket::Core::CompiledGeometryHandle) {}

void GUIRenderInterface::EnableScissorRegion(bool enable)
{
    if (enable)
    {
        glEnable(GL_SCISSOR_TEST);
    }
    else
    {
        glDisable(GL_SCISSOR_TEST);
    }
}

void GUIRenderInterface::SetScissorRegion(int x, int y, int width, int height)
{
    glScissor(x, -y, width, height);
}

bool GUIRenderInterface::LoadTexture(Rocket::Core::TextureHandle& outTextureHandle,
    Rocket::Core::Vector2i& outTextureDimensions, const Rocket::Core::String& resourcePath)
{
    auto tex = new Texture(resourcePath.CString());

    tex->loadFromFile(resourcePath.CString());

    outTextureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(tex);

    return !!tex;
}

bool GUIRenderInterface::GenerateTexture(Rocket::Core::TextureHandle& outTextureHandle, const Rocket::Core::byte* data,
    const Rocket::Core::Vector2i& sourceDimensions)
{
    auto tex = new Texture("libRocket generated");
    tex->loadFromData(GL_RGBA, sourceDimensions.x, sourceDimensions.y, GL_RGBA, GL_UNSIGNED_BYTE, data);

    outTextureHandle = reinterpret_cast<Rocket::Core::TextureHandle>(tex);

    return !!tex;
}

void GUIRenderInterface::ReleaseTexture(Rocket::Core::TextureHandle textureHandle)
{
    auto tex = reinterpret_cast<Texture*>(textureHandle);
    delete tex;
}
