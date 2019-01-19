//
//  VectorStruct.cpp
//  MacWindow
//
//  Created by 威化饼干 on 8/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#include "VectorStruct.h"

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

 void Vector2::SwapVector(Vector2* vectorA, Vector2* vectorB){
    Vector2 temp = *vectorA;
    vectorA->x = vectorB->x;
    vectorA->y = vectorB->y;

    vectorB->x = temp.x;
    vectorB->y = temp.y;

}


bool Vector2::LessY(const Vector2& origin, const Vector2& compare){
    return origin.y < compare.y;
}

void Vector2::Exchange(Vector2*const a, Vector2*const b){
    Vector2 temp = *a;
    a -> x = b -> x;
    a -> y = b -> y;
    b -> x = temp.x;
    b -> y = temp.y;
}

Vector2 Vector2::operator+(const Vector2 & v){
    Vector2 vector;
    vector.x = this->x + v.x;
    vector.y = this->y + v.y;
    return  vector;
}

void Vector2::Swap(){
    int temp = x;
    x = y;
    y = temp;
}
//
//Vector2f::Vector2f(){
//    x = 0;
//    y = 0;
//
//}
//
//Vector2f::Vector2f(float ax, float ay){
//    x = ax;
//    y = ay;
//}
//
//Vector2f Vector2f::operator+(const Vector2f &v){
//    Vector2f vector;
//    vector.x = this->x + v.x;
//    vector.y = this->y + v.y;
//    return  vector;
//}
//
//Vector2f::~Vector2f(){
//
//}
