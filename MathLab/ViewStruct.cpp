//
//  ViewStruct.cpp
//  MacWindow
//
//  Created by 威化饼干 on 11/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "ViewStruct.h"
namespace MathLab{

    ViewStruct::ViewStruct(){
    m_origin = new Vector2i();
    m_viewSize = new Vector2i();
    fprintf(stdout, "view size need intialize");
}

ViewStruct::ViewStruct(const Vector2i& viewSize,const Vector2i& origin){
    m_viewSize =new Vector2i(viewSize.x, viewSize.y);
    m_origin = new Vector2i(origin.x, origin.y);
}

ViewStruct::~ViewStruct(){
    delete m_viewSize;
    delete m_origin;
    m_viewSize = nullptr;
    m_origin = nullptr;
}
}
