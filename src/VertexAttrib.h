//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// gyavol-specific vertex attribute locations
#pragma once

class GPUProgram;

namespace Attrib
{
    enum
    {
        Position,
        TexCoord,
        Color,

        Count
    };

    void BindCustomAttributesToProgram(GPUProgram& p);
}
