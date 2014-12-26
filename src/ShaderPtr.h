//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// shared pointers for Shader
#pragma once

class Shader;

typedef std::shared_ptr<Shader> ShaderPtr;
typedef std::shared_ptr<const Shader> ConstShaderPtr;
