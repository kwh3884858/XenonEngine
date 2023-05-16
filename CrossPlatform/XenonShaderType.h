#pragma once
#include "Algorithms/TypeString.h"
enum ShaderType : unsigned char
{
    ShaderType_Gouraud,
    ShaderType_Flat,
    ShaderType_Wireframe,
};

const Algorithm::TypeString<ShaderType> ShaderTypeString[] = {
	{"Gouraud", ShaderType::ShaderType_Gouraud},
	{"Flat", ShaderType::ShaderType_Flat},
	{"Wireframe", ShaderType::ShaderType_Wireframe}
};