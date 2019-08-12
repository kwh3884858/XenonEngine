//
//  VectorStruct.cpp
//  MacWindow
//
//  Created by 威化饼干 on 8/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "Vector2.h"
namespace MathLab{
    Vector2::Vector2(){
        x = 0;
        y = 0;

    }

    Vector2::Vector2(int ax, int ay){
        x = ax;
        y = ay;
    }

    Vector2::Vector2(const Vector2& para){
        this->x = para.x;
        this->y = para.y;
    }

    Vector2::~Vector2(){

    }


    void Vector2::Swap(){
        int temp = x;
        x = y;
        y = temp;
    }

    Vector2 operator+(const Vector2& v1,const Vector2& v2)   {
        Vector2 vector;
        vector.x = v1.x + v2.x;
        vector.y = v1.y + v2.y;
        return  vector;
    }

    Vector2 operator-(const Vector2& v1,const Vector2& v2){
        Vector2 vector;
        vector.x = v1.x - v2.x;
        vector.y = v1.y - v2.y;
        return  vector;
    }

    void SwapVector(Vector2* vectorA, Vector2* vectorB){
        Vector2 temp = *vectorA;
        vectorA->x = vectorB->x;
        vectorA->y = vectorB->y;

        vectorB->x = temp.x;
        vectorB->y = temp.y;

    }


    bool LessY(const Vector2& origin, const Vector2& compare){
        return origin.y < compare.y;
    }

    void Exchange(Vector2*const a, Vector2*const b){
        Vector2 temp = *a;
        a -> x = b -> x;
        a -> y = b -> y;
        b -> x = temp.x;
        b -> y = temp.y;
    }

}
