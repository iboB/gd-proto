//
// gd-proto
// Copyright (c) 2014 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
// shared poitner types for Texture
#pragma once

class Texture;

typedef std::shared_ptr<Texture> TexturePtr;
typedef std::shared_ptr<const Texture> ConstTexturePtr;