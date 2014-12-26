//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// gyavol-specific vertex attribute locations
#include "GDProto.pch.h"
#include "VertexAttrib.h"

#include "GPUProgram.h"

namespace Attrib
{
    void BindCustomAttributesToProgram(GPUProgram& p)
    {
        p.bindCustomAttribute("v_pos", Position);
        p.bindCustomAttribute("v_tex_coord", TexCoord);
        p.bindCustomAttribute("v_color", Color);
    }
}