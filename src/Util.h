//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// some utility functions
#pragma once

namespace Util
{
    // loads and entire file to a new char[]
    const char* LoadFile(const char* filename, size_t* outFileSize = nullptr);

    // random float between 0 and 1
    float Rnd01();

    // random float between -1 and 1
    float Rnd11();

    // solves ax^2 + bx + c == 0
    // returns false if there are no real roots
    bool SolveQuadraticEquation(float a, float b, float c, float& outX1, float& outX2);
}
