//  Image.h
//  XenonEngine
//
//  Created by whkong on 2021-12-11.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "CrossPlatform/SColorRGBA.h"
#include "Algorithms/String.h"
namespace CrossPlatform
{
	class Image
	{
	public:
		Image(const Algorithm::String& fileName);
		Image(const Image& that);
		~Image();
		Image& operator=(Image&& that);

		CrossPlatform::SColorRGBA GetColor(int x, int y)const;
		CrossPlatform::SColorRGBA GetColor(float x, float y)const;
	public:
		unsigned char* m_data = nullptr;
		int m_height = -1;
		int m_width = -1;
		int m_channel = -1;
	};

}