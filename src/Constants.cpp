//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// popular constants used throughout the game
#include "GDProto.pch.h"
#include "Constants.h"

using namespace mathgp;

namespace Vec
{
    const vector3 up = vector3::coord(0, 0, 1);
    const vector3 zero = vector3::zero();
}

namespace Mat
{
    const mathgp::matrix identity = matrix::identity();
}

namespace Str
{
    const std::string empty;
}
