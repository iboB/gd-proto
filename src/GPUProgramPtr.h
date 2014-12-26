//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// shared pointers for GPUProgram
#pragma once

class GPUProgram;

typedef std::shared_ptr<GPUProgram> GPUProgramPtr;
typedef std::shared_ptr<const GPUProgram> ConstGPUProgramPtr;
