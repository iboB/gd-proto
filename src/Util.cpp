//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// some utility functions
#include "GDProto.pch.h"
#include "Util.h"

using namespace std;

namespace Util
{
    const char* LoadFile(const char* filename, size_t* outFileSize)
    {
        ifstream fin(filename, ios::in | ios::binary);

        if (!fin.is_open())
        {
            ostringstream sout;
            sout << "cannot open " << filename;
            throw runtime_error(sout.str());
        }

        streamoff begin = fin.tellg();
        fin.seekg(0, ios::end);
        size_t fileSize = size_t(fin.tellg() - begin);
        fin.seekg(0, ios::beg);

        char* fileContents = new char[fileSize + 1];

        fin.read(fileContents, fileSize);

        fileContents[fileSize] = 0;

        if (outFileSize)
            *outFileSize = fileSize;

        return fileContents;
    }

    float Rnd01()
    {
        return float(rand()) / RAND_MAX;
    }

    float Rnd11()
    {
        return Rnd01() * 2 - 1;
    }

    bool SolveQuadraticEquation(float a, float b, float c, float& outX1, float& outX2)
    {
        float discriminant = b*b - 4 * a*c;
        if (discriminant < 0)
        {
            return false;
        }

        float sqrtD = sqrt(discriminant);

        outX1 = (-b + sqrtD) / (2 * a);
        outX2 = (-b - sqrtD) / (2 * a);

        return true;
    }
}
