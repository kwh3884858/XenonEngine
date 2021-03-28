//
//  VectorStruct.cpp
//  MacWindow
//
//  Created by 威化饼干 on 8/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "Vector2i.h"
namespace MathLab{
    Vector2i::Vector2i(){
        x = 0;
        y = 0;

    }

    Vector2i::Vector2i(int ax, int ay){
        x = ax;
        y = ay;
    }

    Vector2i::Vector2i(const Vector2i& para){
        this->x = para.x;
        this->y = para.y;
    }

    Vector2i::~Vector2i(){

    }


    void Vector2i::Swap(){
        int temp = x;
        x = y;
        y = temp;
    }

    Vector2i operator+(const Vector2i& v1,const Vector2i& v2)   {
        Vector2i vector;
        vector.x = v1.x + v2.x;
        vector.y = v1.y + v2.y;
        return  vector;
    }

    Vector2i operator-(const Vector2i& v1,const Vector2i& v2){
        Vector2i vector;
        vector.x = v1.x - v2.x;
        vector.y = v1.y - v2.y;
        return  vector;
    }

    void SwapVector(Vector2i* vectorA, Vector2i* vectorB){
        Vector2i temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;

        vectorB->x = temp.x;
        vectorB->y = temp.y;

    }


    bool LessY(const Vector2i& origin, const Vector2i& compare){
        return origin.y < compare.y;
    }

    void Exchange(Vector2i*const a, Vector2i*const b){
        Vector2i temp = *a;
        a -> x = b -> x;
        a -> y = b -> y;
        b -> x = temp.x;
        b -> y = temp.y;
    }

}
