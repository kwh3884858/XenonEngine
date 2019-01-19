//
//  ViewStruct.cpp
//  MacWindow
//
//  Created by 威化饼干 on 11/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "ViewStruct.h"
ViewStruct::ViewStruct(){
    m_origin = new Vector2();
    m_viewSize = new Vector2();
    fprintf(stdout, "view size need intialize");
}

ViewStruct::ViewStruct(const Vector2& viewSize,const Vector2& origin){
    m_viewSize =new Vector2(viewSize.x, viewSize.y);
    m_origin = new Vector2(origin.x, origin.y);
}

ViewStruct::~ViewStruct(){
    delete m_viewSize;
    delete m_origin;
    m_viewSize = nullptr;
    m_origin = nullptr;
}
