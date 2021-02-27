#include "Matrix3.h"

namespace MathLab {
    Matrix3::Matrix3()
    {
    }

    Matrix3::Matrix3(const Vector3& aV1, const Vector3& aV2, const Vector3& aV3)
    {
        v1 = aV1;
        v2 = aV2;
        v3 = aV3;
    }

    Matrix3::Matrix3(const Matrix3& aMatrix)
    {
        v1 = aMatrix.v1;
        v2 = aMatrix.v2;
        v3 = aMatrix.v3;
    }

    Matrix3::~Matrix3()
    {

    }

    MathLab::Vector3 operator*(const Vector3& lValue, const Matrix3& rValue)
    {
        Vector3 result;
        result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x + lValue.z * rValue.v3.x;
        result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y + lValue.z * rValue.v3.y;
        result.z = lValue.x * rValue.v1.z + lValue.y * rValue.v2.z + lValue.z * rValue.v3.z;
    }

}