//  Image.h
//  XenonEngine
//
//  Created by whkong on 2021-12-11.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "MathLab/Vector.h"
#include "Algorithms/String.h"
namespace CrossPlatform
{
	class Image
	{
	public:
		Image(const Algorithm::String& fileName);
		~Image() = default;

		MathLab::TVector4f GetColor(int x, int y)const;
	public:
		unsigned char* m_data = nullptr;
		int m_height = -1;
		int m_width = -1;
		int m_channel = -1;
	};

}