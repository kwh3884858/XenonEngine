//
//  MathLib.hpp
//  MacWindow
//
//  Created by 威化饼干 on 14/11/2018.
//  Copyright © 2018 威化饼干. All rights reserved.
//

#ifndef MathLib_hpp
#define MathLib_hpp

namespace MathLab{

    struct Vector3f;

     Vector3f RotateXAxis(const Vector3f& vec, const float degree);
     Vector3f RotateYAxis(const Vector3f& vec, const float degree);
     Vector3f RotateZAxis(const Vector3f& vec, const float degree);

     template <typename T>
     const T& clamp(const T& value, const T& min, const T& max) {
         if (value < min)
         {
             return min;
         }
         if (value > max)
         {
             return max;
         }
         return value;
     }

     template <typename T>
     T abs(const T& value)
     {
         return value < 0 ? -value : value;
     }

};
#endif /* MathLib_hpp */
