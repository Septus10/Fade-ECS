#pragma once

namespace ECS {
    
enum ComponentFamily
{
    CF_None         = 0x00,
    CF_Transform    = 0x01,
    CF_Camera       = 0x02,
	CF_Velocity		= 0x04,
	CF_Sprite		= 0x08,
	CF_Input		= 0x10,
};

}
