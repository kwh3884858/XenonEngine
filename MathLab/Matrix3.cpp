#include "Matrix3.h"

namespace MathLab {
    Matrix3::Matrix3()
    {
    }

    Matrix3::Matrix3(const Vector3f& aV1, const Vector3f& aV2, const Vector3f& aV3)
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

    MathLab::Vector3f operator*(const Vector3f& lValue, const Matrix3& rValue)
    {
        Vector3f result;
        result.x = lValue.x * rValue.v1.x + lValue.y * rValue.v2.x + lValue.z * rValue.v3.x;
        result.y = lValue.x * rValue.v1.y + lValue.y * rValue.v2.y + lValue.z * rValue.v3.y;
        result.z = lValue.x * rValue.v1.z + lValue.y * rValue.v2.z + lValue.z * rValue.v3.z;
        return result;
    }

}