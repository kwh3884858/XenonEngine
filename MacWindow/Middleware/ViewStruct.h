//
//  ViewStruct.h
//  MacWindow
//
//  Created by 威化饼干 on 11/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#ifndef ViewStruct_h
#define ViewStruct_h

#include <stdio.h>
#include "VectorStruct.h"

class ViewStruct {
public:
    Vector2* m_viewSize;
    Vector2* m_origin;

public:
    ViewStruct();
    ViewStruct(const Vector2& viewSize,const Vector2& origin);
    ~ViewStruct();
};

#endif /* ViewStruct_h */
