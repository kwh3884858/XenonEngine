//
//  ViewStruct.h
//  MacWindow
//
//  To Descript a windows struct
//  Created by 威化饼干 on 11/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#ifndef ViewStruct_h
#define ViewStruct_h

#include <stdio.h>
#include "Vector2i.h"
namespace MathLab{

class ViewStruct {

public:
    Vector2i* m_viewSize;
    Vector2i* m_origin;

public:
    ViewStruct();
    ViewStruct(const Vector2i& viewSize,const Vector2i& origin);
    ~ViewStruct();
};
}
#endif /* ViewStruct_h */
