#pragma once

#include "Entity.hpp"

namespace ECS {
    
enum ComponentType
{
    CT_None         = 0x00,
    CT_Transform    = 0x01,
    CT_Camera       = 0x02
};

using ComponentFamily = u32;

class Component
{
public:
    virtual ~Component() = default;
};

}
