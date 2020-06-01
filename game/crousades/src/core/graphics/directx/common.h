#pragma once

#include "core/graphics/graphics_core.h"

#define DEFAULT_BPP_MODE_BIT 32
#define DEFAULT_BPP_MODE_BYTE 4

#define DDRAW_INIT_STRUCT(ddstruct) { memset(&ddstruct,0, \
sizeof(ddstruct)); ddstruct.dwSize=sizeof(ddstruct); }

#define RELEASE_COM(x) if (x) { x->Release(); x = nullptr; }
