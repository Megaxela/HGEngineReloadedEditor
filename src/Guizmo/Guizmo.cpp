// The MIT License(MIT)
//
// Copyright(c) 2016 Cedric Guillemet
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "imgui.h"

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif

#include "imgui_internal.h"
#include <Guizmo/Guizmo.hpp>

// includes patches for multiview from
// https://github.com/CedricGuillemet/ImGuizmo/issues/15

namespace ImGuizmo
{
    static const float ZPI = 3.14159265358979323846f;
    static const float RAD2DEG = (180.f / ZPI);
    static const float DEG2RAD = (ZPI / 180.f);
    static const float gGizmoSizeClipSpace = 0.1f;
    const float screenRotateSize = 0.06f;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // utility and math

    void FPU_MatrixF_x_MatrixF(const float* a, const float* b, float* r)
    {
        r[0] = a[0] * b[0] + a[1] * b[4] + a[2] * b[8] + a[3] * b[12];
        r[1] = a[0] * b[1] + a[1] * b[5] + a[2] * b[9] + a[3] * b[13];
        r[2] = a[0] * b[2] + a[1] * b[6] + a[2] * b[10] + a[3] * b[14];
        r[3] = a[0] * b[3] + a[1] * b[7] + a[2] * b[11] + a[3] * b[15];

        r[4] = a[4] * b[0] + a[5] * b[4] + a[6] * b[8] + a[7] * b[12];
        r[5] = a[4] * b[1] + a[5] * b[5] + a[6] * b[9] + a[7] * b[13];
        r[6] = a[4] * b[2] + a[5] * b[6] + a[6] * b[10] + a[7] * b[14];
        r[7] = a[4] * b[3] + a[5] * b[7] + a[6] * b[11] + a[7] * b[15];

        r[8] = a[8] * b[0] + a[9] * b[4] + a[10] * b[8] + a[11] * b[12];
        r[9] = a[8] * b[1] + a[9] * b[5] + a[10] * b[9] + a[11] * b[13];
        r[10] = a[8] * b[2] + a[9] * b[6] + a[10] * b[10] + a[11] * b[14];
        r[11] = a[8] * b[3] + a[9] * b[7] + a[10] * b[11] + a[11] * b[15];

        r[12] = a[12] * b[0] + a[13] * b[4] + a[14] * b[8] + a[15] * b[12];
        r[13] = a[12] * b[1] + a[13] * b[5] + a[14] * b[9] + a[15] * b[13];
        r[14] = a[12] * b[2] + a[13] * b[6] + a[14] * b[10] + a[15] * b[14];
        r[15] = a[12] * b[3] + a[13] * b[7] + a[14] * b[11] + a[15] * b[15];
    }

    //template <typename T> T LERP(T x, T y, float z) { return (x + (y - x)*z); }
    template<typename T>
    T clamp(T x, T y, T z)
    {
        return ((x < y) ? y : ((x > z) ? z : x));
    }

    template<typename T>
    T max(T x, T y)
    {
        return (x > y) ? x : y;
    }

    template<typename T>
    T min(T x, T y)
    {
        return (x < y) ? x : y;
    }

    template<typename T>
    bool IsWithin(T x, T y, T z)
    {
        return (x >= y) && (x <= z);
    }

    struct matrix_t;

    struct vec_t
    {
    public:
        float x, y, z, w;

        void Lerp(const vec_t& v, float t)
        {
            x += (v.x - x) * t;
            y += (v.y - y) * t;
            z += (v.z - z) * t;
            w += (v.w - w) * t;
        }

        void Set(float v)
        {
            x = y = z = w = v;
        }

        void Set(float _x, float _y, float _z = 0.f, float _w = 0.f)
        {
            x = _x;
            y = _y;
            z = _z;
            w = _w;
        }

        vec_t& operator-=(const vec_t& v)
        {
            x -= v.x;
            y -= v.y;
            z -= v.z;
            w -= v.w;
            return *this;
        }

        vec_t& operator+=(const vec_t& v)
        {
            x += v.x;
            y += v.y;
            z += v.z;
            w += v.w;
            return *this;
        }

        vec_t& operator*=(const vec_t& v)
        {
            x *= v.x;
            y *= v.y;
            z *= v.z;
            w *= v.w;
            return *this;
        }

        vec_t& operator*=(float v)
        {
            x *= v;
            y *= v;
            z *= v;
            w *= v;
            return *this;
        }

        vec_t operator*(float f) const;

        vec_t operator-() const;

        vec_t operator-(const vec_t& v) const;

        vec_t operator+(const vec_t& v) const;

        vec_t operator*(const vec_t& v) const;

        const vec_t& operator+() const
        {
            return (*this);
        }

        float Length() const
        {
            return sqrtf(x * x + y * y + z * z);
        };

        float LengthSq() const
        {
            return (x * x + y * y + z * z);
        };

        vec_t Normalize()
        {
            (*this) *= (1.f / Length());
            return (*this);
        }

        vec_t Normalize(const vec_t& v)
        {
            this->Set(v.x, v.y, v.z, v.w);
            this->Normalize();
            return (*this);
        }

        vec_t Abs() const;

        void Cross(const vec_t& v)
        {
            vec_t res;
            res.x = y * v.z - z * v.y;
            res.y = z * v.x - x * v.z;
            res.z = x * v.y - y * v.x;

            x = res.x;
            y = res.y;
            z = res.z;
            w = 0.f;
        }

        void Cross(const vec_t& v1, const vec_t& v2)
        {
            x = v1.y * v2.z - v1.z * v2.y;
            y = v1.z * v2.x - v1.x * v2.z;
            z = v1.x * v2.y - v1.y * v2.x;
            w = 0.f;
        }

        float Dot(const vec_t& v) const
        {
            return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
        }

        float Dot3(const vec_t& v) const
        {
            return (x * v.x) + (y * v.y) + (z * v.z);
        }

        void Transform(const matrix_t& matrix);

        void Transform(const vec_t& s, const matrix_t& matrix);

        void TransformVector(const matrix_t& matrix);

        void TransformPoint(const matrix_t& matrix);

        void TransformVector(const vec_t& v, const matrix_t& matrix)
        {
            (*this) = v;
            this->TransformVector(matrix);
        }

        void TransformPoint(const vec_t& v, const matrix_t& matrix)
        {
            (*this) = v;
            this->TransformPoint(matrix);
        }

        float& operator[](size_t index)
        {
            return ((float*) &x)[index];
        }

        const float& operator[](size_t index) const
        {
            return ((float*) &x)[index];
        }
    };

    vec_t makeVect(float _x, float _y, float _z = 0.f, float _w = 0.f)
    {
        vec_t res;
        res.x = _x;
        res.y = _y;
        res.z = _z;
        res.w = _w;
        return res;
    }

    vec_t makeVect(ImVec2 v)
    {
        vec_t res;
        res.x = v.x;
        res.y = v.y;
        res.z = 0.f;
        res.w = 0.f;
        return res;
    }

    vec_t vec_t::operator*(float f) const
    {
        return makeVect(x * f, y * f, z * f, w * f);
    }

    vec_t vec_t::operator-() const
    {
        return makeVect(-x, -y, -z, -w);
    }

    vec_t vec_t::operator-(const vec_t& v) const
    {
        return makeVect(x - v.x, y - v.y, z - v.z, w - v.w);
    }

    vec_t vec_t::operator+(const vec_t& v) const
    {
        return makeVect(x + v.x, y + v.y, z + v.z, w + v.w);
    }

    vec_t vec_t::operator*(const vec_t& v) const
    {
        return makeVect(x * v.x, y * v.y, z * v.z, w * v.w);
    }

    vec_t vec_t::Abs() const
    {
        return makeVect(fabsf(x), fabsf(y), fabsf(z));
    }

    vec_t normalized(const vec_t& v)
    {
        vec_t res;
        res = v;
        res.Normalize();
        return res;
    }

    vec_t crossProduct(const vec_t& v1, const vec_t& v2)
    {
        vec_t res;
        res.x = v1.y * v2.z - v1.z * v2.y;
        res.y = v1.z * v2.x - v1.x * v2.z;
        res.z = v1.x * v2.y - v1.y * v2.x;
        res.w = 0.f;
        return res;
    }

    float dotProduct(const vec_t& v1, const vec_t& v2)
    {
        return (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);
    }

    vec_t BuildPlan(const vec_t& p_point1, const vec_t& p_normal)
    {
        vec_t normal, res;
        normal.Normalize(p_normal);
        res.w = normal.Dot(p_point1);
        res.x = normal.x;
        res.y = normal.y;
        res.z = normal.z;
        return res;
    }

    struct matrix_t
    {
    public:

        union
        {
            float m[4][4];
            float m16[16];
            struct
            {
                vec_t right, up, dir, position;
            } v;
            vec_t component[4];
        };

        matrix_t(const matrix_t& other)
        {
            memcpy(&m16[0], &other.m16[0], sizeof(float) * 16);
        }

        matrix_t()
        {
        }

        operator float*()
        {
            return m16;
        }

        operator const float*() const
        {
            return m16;
        }

        void MakeTranslationMatrix(float _x, float _y, float _z)
        {
            this->MakeTranslationMatrix(makeVect(_x, _y, _z));
        }

        void MakeTranslationMatrix(const vec_t& vt)
        {
            v.right.Set(1.f, 0.f, 0.f, 0.f);
            v.up.Set(0.f, 1.f, 0.f, 0.f);
            v.dir.Set(0.f, 0.f, 1.f, 0.f);
            v.position.Set(vt.x, vt.y, vt.z, 1.f);
        }

        void MakeScaleMatrix(float _x, float _y, float _z)
        {
            v.right.Set(_x, 0.f, 0.f, 0.f);
            v.up.Set(0.f, _y, 0.f, 0.f);
            v.dir.Set(0.f, 0.f, _z, 0.f);
            v.position.Set(0.f, 0.f, 0.f, 1.f);
        }

        void MakeScaleMatrix(const vec_t& s)
        {
            MakeScaleMatrix(s.x, s.y, s.z);
        }

        matrix_t& operator*=(const matrix_t& mat)
        {
            matrix_t tmpMat;
            tmpMat = *this;
            tmpMat.Multiply(mat);
            *this = tmpMat;
            return *this;
        }

        matrix_t operator*(const matrix_t& mat) const
        {
            matrix_t matT;
            matT.Multiply(*this, mat);
            return matT;
        }

        void Multiply(const matrix_t& matrix)
        {
            matrix_t tmp;
            tmp = *this;

            FPU_MatrixF_x_MatrixF((float*) &tmp, (float*) &matrix, (float*) this);
        }

        void Multiply(const matrix_t& m1, const matrix_t& m2)
        {
            FPU_MatrixF_x_MatrixF((float*) &m1, (float*) &m2, (float*) this);
        }

        float GetDeterminant() const
        {
            return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1] -
                   m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1];
        }

        float Inverse(const matrix_t& srcMatrix, bool affine = false);

        void SetToIdentity()
        {
            v.right.Set(1.f, 0.f, 0.f, 0.f);
            v.up.Set(0.f, 1.f, 0.f, 0.f);
            v.dir.Set(0.f, 0.f, 1.f, 0.f);
            v.position.Set(0.f, 0.f, 0.f, 1.f);
        }

        void Transpose()
        {
            matrix_t tmpm;
            for (int l = 0; l < 4; l++)
            {
                for (int c = 0; c < 4; c++)
                {
                    tmpm.m[l][c] = m[c][l];
                }
            }
            (*this) = tmpm;
        }

        void RotationAxis(const vec_t& axis, float angle);

        void OrthoNormalize()
        {
            v.right.Normalize();
            v.up.Normalize();
            v.dir.Normalize();
        }
    };

    void vec_t::Transform(const matrix_t& matrix)
    {
        vec_t out;

        out.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + w * matrix.m[3][0];
        out.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + w * matrix.m[3][1];
        out.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + w * matrix.m[3][2];
        out.w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + w * matrix.m[3][3];

        x = out.x;
        y = out.y;
        z = out.z;
        w = out.w;
    }

    void vec_t::Transform(const vec_t& s, const matrix_t& matrix)
    {
        *this = s;
        Transform(matrix);
    }

    void vec_t::TransformPoint(const matrix_t& matrix)
    {
        vec_t out;

        out.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0] + matrix.m[3][0];
        out.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1] + matrix.m[3][1];
        out.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2] + matrix.m[3][2];
        out.w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3] + matrix.m[3][3];

        x = out.x;
        y = out.y;
        z = out.z;
        w = out.w;
    }


    void vec_t::TransformVector(const matrix_t& matrix)
    {
        vec_t out;

        out.x = x * matrix.m[0][0] + y * matrix.m[1][0] + z * matrix.m[2][0];
        out.y = x * matrix.m[0][1] + y * matrix.m[1][1] + z * matrix.m[2][1];
        out.z = x * matrix.m[0][2] + y * matrix.m[1][2] + z * matrix.m[2][2];
        out.w = x * matrix.m[0][3] + y * matrix.m[1][3] + z * matrix.m[2][3];

        x = out.x;
        y = out.y;
        z = out.z;
        w = out.w;
    }

    float matrix_t::Inverse(const matrix_t& srcMatrix, bool affine)
    {
        float det = 0;

        if (affine)
        {
            det = GetDeterminant();
            float s = 1 / det;
            m[0][0] = (srcMatrix.m[1][1] * srcMatrix.m[2][2] - srcMatrix.m[1][2] * srcMatrix.m[2][1]) * s;
            m[0][1] = (srcMatrix.m[2][1] * srcMatrix.m[0][2] - srcMatrix.m[2][2] * srcMatrix.m[0][1]) * s;
            m[0][2] = (srcMatrix.m[0][1] * srcMatrix.m[1][2] - srcMatrix.m[0][2] * srcMatrix.m[1][1]) * s;
            m[1][0] = (srcMatrix.m[1][2] * srcMatrix.m[2][0] - srcMatrix.m[1][0] * srcMatrix.m[2][2]) * s;
            m[1][1] = (srcMatrix.m[2][2] * srcMatrix.m[0][0] - srcMatrix.m[2][0] * srcMatrix.m[0][2]) * s;
            m[1][2] = (srcMatrix.m[0][2] * srcMatrix.m[1][0] - srcMatrix.m[0][0] * srcMatrix.m[1][2]) * s;
            m[2][0] = (srcMatrix.m[1][0] * srcMatrix.m[2][1] - srcMatrix.m[1][1] * srcMatrix.m[2][0]) * s;
            m[2][1] = (srcMatrix.m[2][0] * srcMatrix.m[0][1] - srcMatrix.m[2][1] * srcMatrix.m[0][0]) * s;
            m[2][2] = (srcMatrix.m[0][0] * srcMatrix.m[1][1] - srcMatrix.m[0][1] * srcMatrix.m[1][0]) * s;
            m[3][0] = -(m[0][0] * srcMatrix.m[3][0] + m[1][0] * srcMatrix.m[3][1] + m[2][0] * srcMatrix.m[3][2]);
            m[3][1] = -(m[0][1] * srcMatrix.m[3][0] + m[1][1] * srcMatrix.m[3][1] + m[2][1] * srcMatrix.m[3][2]);
            m[3][2] = -(m[0][2] * srcMatrix.m[3][0] + m[1][2] * srcMatrix.m[3][1] + m[2][2] * srcMatrix.m[3][2]);
        } else
        {
            // transpose matrix
            float src[16];
            for (int i = 0; i < 4; ++i)
            {
                src[i] = srcMatrix.m16[i * 4];
                src[i + 4] = srcMatrix.m16[i * 4 + 1];
                src[i + 8] = srcMatrix.m16[i * 4 + 2];
                src[i + 12] = srcMatrix.m16[i * 4 + 3];
            }

            // calculate pairs for first 8 elements (cofactors)
            float tmp[12]; // temp array for pairs
            tmp[0] = src[10] * src[15];
            tmp[1] = src[11] * src[14];
            tmp[2] = src[9] * src[15];
            tmp[3] = src[11] * src[13];
            tmp[4] = src[9] * src[14];
            tmp[5] = src[10] * src[13];
            tmp[6] = src[8] * src[15];
            tmp[7] = src[11] * src[12];
            tmp[8] = src[8] * src[14];
            tmp[9] = src[10] * src[12];
            tmp[10] = src[8] * src[13];
            tmp[11] = src[9] * src[12];

            // calculate first 8 elements (cofactors)
            m16[0] = (tmp[0] * src[5] + tmp[3] * src[6] + tmp[4] * src[7]) -
                     (tmp[1] * src[5] + tmp[2] * src[6] + tmp[5] * src[7]);
            m16[1] = (tmp[1] * src[4] + tmp[6] * src[6] + tmp[9] * src[7]) -
                     (tmp[0] * src[4] + tmp[7] * src[6] + tmp[8] * src[7]);
            m16[2] = (tmp[2] * src[4] + tmp[7] * src[5] + tmp[10] * src[7]) -
                     (tmp[3] * src[4] + tmp[6] * src[5] + tmp[11] * src[7]);
            m16[3] = (tmp[5] * src[4] + tmp[8] * src[5] + tmp[11] * src[6]) -
                     (tmp[4] * src[4] + tmp[9] * src[5] + tmp[10] * src[6]);
            m16[4] = (tmp[1] * src[1] + tmp[2] * src[2] + tmp[5] * src[3]) -
                     (tmp[0] * src[1] + tmp[3] * src[2] + tmp[4] * src[3]);
            m16[5] = (tmp[0] * src[0] + tmp[7] * src[2] + tmp[8] * src[3]) -
                     (tmp[1] * src[0] + tmp[6] * src[2] + tmp[9] * src[3]);
            m16[6] = (tmp[3] * src[0] + tmp[6] * src[1] + tmp[11] * src[3]) -
                     (tmp[2] * src[0] + tmp[7] * src[1] + tmp[10] * src[3]);
            m16[7] = (tmp[4] * src[0] + tmp[9] * src[1] + tmp[10] * src[2]) -
                     (tmp[5] * src[0] + tmp[8] * src[1] + tmp[11] * src[2]);

            // calculate pairs for second 8 elements (cofactors)
            tmp[0] = src[2] * src[7];
            tmp[1] = src[3] * src[6];
            tmp[2] = src[1] * src[7];
            tmp[3] = src[3] * src[5];
            tmp[4] = src[1] * src[6];
            tmp[5] = src[2] * src[5];
            tmp[6] = src[0] * src[7];
            tmp[7] = src[3] * src[4];
            tmp[8] = src[0] * src[6];
            tmp[9] = src[2] * src[4];
            tmp[10] = src[0] * src[5];
            tmp[11] = src[1] * src[4];

            // calculate second 8 elements (cofactors)
            m16[8] = (tmp[0] * src[13] + tmp[3] * src[14] + tmp[4] * src[15]) -
                     (tmp[1] * src[13] + tmp[2] * src[14] + tmp[5] * src[15]);
            m16[9] = (tmp[1] * src[12] + tmp[6] * src[14] + tmp[9] * src[15]) -
                     (tmp[0] * src[12] + tmp[7] * src[14] + tmp[8] * src[15]);
            m16[10] = (tmp[2] * src[12] + tmp[7] * src[13] + tmp[10] * src[15]) -
                      (tmp[3] * src[12] + tmp[6] * src[13] + tmp[11] * src[15]);
            m16[11] = (tmp[5] * src[12] + tmp[8] * src[13] + tmp[11] * src[14]) -
                      (tmp[4] * src[12] + tmp[9] * src[13] + tmp[10] * src[14]);
            m16[12] = (tmp[2] * src[10] + tmp[5] * src[11] + tmp[1] * src[9]) -
                      (tmp[4] * src[11] + tmp[0] * src[9] + tmp[3] * src[10]);
            m16[13] = (tmp[8] * src[11] + tmp[0] * src[8] + tmp[7] * src[10]) -
                      (tmp[6] * src[10] + tmp[9] * src[11] + tmp[1] * src[8]);
            m16[14] = (tmp[6] * src[9] + tmp[11] * src[11] + tmp[3] * src[8]) -
                      (tmp[10] * src[11] + tmp[2] * src[8] + tmp[7] * src[9]);
            m16[15] = (tmp[10] * src[10] + tmp[4] * src[8] + tmp[9] * src[9]) -
                      (tmp[8] * src[9] + tmp[11] * src[10] + tmp[5] * src[8]);

            // calculate determinant
            det = src[0] * m16[0] + src[1] * m16[1] + src[2] * m16[2] + src[3] * m16[3];

            // calculate matrix inverse
            float invdet = 1 / det;
            for (int j = 0; j < 16; ++j)
            {
                m16[j] *= invdet;
            }
        }

        return det;
    }

    void matrix_t::RotationAxis(const vec_t& axis, float angle)
    {
        float length2 = axis.LengthSq();
        if (length2 < FLT_EPSILON)
        {
            SetToIdentity();
            return;
        }

        vec_t n = axis * (1.f / sqrtf(length2));
        float s = sinf(angle);
        float c = cosf(angle);
        float k = 1.f - c;

        float xx = n.x * n.x * k + c;
        float yy = n.y * n.y * k + c;
        float zz = n.z * n.z * k + c;
        float xy = n.x * n.y * k;
        float yz = n.y * n.z * k;
        float zx = n.z * n.x * k;
        float xs = n.x * s;
        float ys = n.y * s;
        float zs = n.z * s;

        m[0][0] = xx;
        m[0][1] = xy + zs;
        m[0][2] = zx - ys;
        m[0][3] = 0.f;
        m[1][0] = xy - zs;
        m[1][1] = yy;
        m[1][2] = yz + xs;
        m[1][3] = 0.f;
        m[2][0] = zx + ys;
        m[2][1] = yz - xs;
        m[2][2] = zz;
        m[2][3] = 0.f;
        m[3][0] = 0.f;
        m[3][1] = 0.f;
        m[3][2] = 0.f;
        m[3][3] = 1.f;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    enum MOVETYPE
    {
        NONE,
        MOVE_X,
        MOVE_Y,
        MOVE_Z,
        MOVE_YZ,
        MOVE_ZX,
        MOVE_XY,
        MOVE_SCREEN,
        ROTATE_X,
        ROTATE_Y,
        ROTATE_Z,
        ROTATE_SCREEN,
        SCALE_X,
        SCALE_Y,
        SCALE_Z,
        SCALE_XYZ
    };

    struct Context
    {
        Context() : mbUsing(false), mbEnable(true), mbUsingBounds(false)
        {
        }

        ImDrawList* mDrawList;

        MODE mMode;
        matrix_t mViewMat;
        matrix_t mProjectionMat;
        matrix_t mModel;
        matrix_t mModelInverse;
        matrix_t mModelSource;
        matrix_t mModelSourceInverse;
        matrix_t mMVP;
        matrix_t mViewProjection;

        vec_t mModelScaleOrigin;
        vec_t mCameraEye;
        vec_t mCameraRight;
        vec_t mCameraDir;
        vec_t mCameraUp;
        vec_t mRayOrigin;
        vec_t mRayVector;

        float mRadiusSquareCenter;
        ImVec2 mScreenSquareCenter;
        ImVec2 mScreenSquareMin;
        ImVec2 mScreenSquareMax;

        float mScreenFactor;
        vec_t mRelativeOrigin;

        bool mbUsing;
        bool mbEnable;

        // translation
        vec_t mTranslationPlan;
        vec_t mTranslationPlanOrigin;
        vec_t mMatrixOrigin;

        // rotation
        vec_t mRotationVectorSource;
        float mRotationAngle;
        float mRotationAngleOrigin;
        //vec_t mWorldToLocalAxis;

        // scale
        vec_t mScale;
        vec_t mScaleValueOrigin;
        float mSaveMousePosx;

        // save axis factor when using gizmo
        bool mBelowAxisLimit[3];
        bool mBelowPlaneLimit[3];
        float mAxisFactor[3];

        // bounds stretching
        vec_t mBoundsPivot;
        vec_t mBoundsAnchor;
        vec_t mBoundsPlan;
        vec_t mBoundsLocalPivot;
        int mBoundsBestAxis;
        int mBoundsAxis[2];
        bool mbUsingBounds;
        matrix_t mBoundsMatrix;

        //
        int mCurrentOperation;

        float mX = 0.f;
        float mY = 0.f;
        float mWidth = 0.f;
        float mHeight = 0.f;
        float mXMax = 0.f;
        float mYMax = 0.f;
        float mDisplayRatio = 1.f;

        bool mIsOrthographic = false;
    };

    static Context g_context;

    static const float angleLimit = 0.96f;
    static const float planeLimit = 0.2f;

    static const vec_t directionUnary[3] = {makeVect(1.f, 0.f, 0.f), makeVect(0.f, 1.f, 0.f), makeVect(0.f, 0.f, 1.f)};
    static const ImU32 directionColor[3] = {0xFF0000AA, 0xFF00AA00, 0xFFAA0000};

    // Alpha: 100%: FF, 87%: DE, 70%: B3, 54%: 8A, 50%: 80, 38%: 61, 12%: 1F
    static const ImU32 planeColor[3] = {0x610000AA, 0x6100AA00, 0x61AA0000};
    static const ImU32 selectionColor = 0x8A1080FF;
    static const ImU32 inactiveColor = 0x99999999;
    static const ImU32 translationLineColor = 0xAAAAAAAA;
    static const char* translationInfoMask[] = {"X : %5.3f", "Y : %5.3f", "Z : %5.3f",
                                                "Y : %5.3f Z : %5.3f", "X : %5.3f Z : %5.3f", "X : %5.3f Y : %5.3f",
                                                "X : %5.3f Y : %5.3f Z : %5.3f"};
    static const char* scaleInfoMask[] = {"X : %5.2f", "Y : %5.2f", "Z : %5.2f", "XYZ : %5.2f"};
    static const char* rotationInfoMask[] = {"X : %5.2f deg %5.2f rad", "Y : %5.2f deg %5.2f rad",
                                             "Z : %5.2f deg %5.2f rad", "Screen : %5.2f deg %5.2f rad"};
    static const int translationInfoIndex[] = {0, 0, 0, 1, 0, 0, 2, 0, 0, 1, 2, 0, 0, 2, 0, 0, 1, 0, 0, 1, 2};
    static const float quadMin = 0.5f;
    static const float quadMax = 0.8f;
    static const float quadUV[8] = {quadMin, quadMin, quadMin, quadMax, quadMax, quadMax, quadMax, quadMin};
    static const int halfCircleSegmentCount = 64;
    static const float snapTension = 0.5f;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //
    static int GetMoveType(vec_t* gizmoHitProportion);

    static int GetRotateType();

    static int GetScaleType();

    static ImVec2 worldToPos(const vec_t& worldPos, const matrix_t& mat)
    {
        vec_t trans;
        trans.TransformPoint(worldPos, mat);
        trans *= 0.5f / trans.w;
        trans += makeVect(0.5f, 0.5f);
        trans.y = 1.f - trans.y;
        trans.x *= g_context.mWidth;
        trans.y *= g_context.mHeight;
        trans.x += g_context.mX;
        trans.y += g_context.mY;
        return ImVec2(trans.x, trans.y);
    }

    static void ComputeCameraRay(vec_t& rayOrigin, vec_t& rayDir)
    {
        ImGuiIO& io = ImGui::GetIO();

        matrix_t mViewProjInverse;
        mViewProjInverse.Inverse(g_context.mViewMat * g_context.mProjectionMat);

        float mox = ((io.MousePos.x - g_context.mX) / g_context.mWidth) * 2.f - 1.f;
        float moy = (1.f - ((io.MousePos.y - g_context.mY) / g_context.mHeight)) * 2.f - 1.f;

        rayOrigin.Transform(makeVect(mox, moy, 0.f, 1.f), mViewProjInverse);
        rayOrigin *= 1.f / rayOrigin.w;
        vec_t rayEnd;
        rayEnd.Transform(makeVect(mox, moy, 1.f, 1.f), mViewProjInverse);
        rayEnd *= 1.f / rayEnd.w;
        rayDir = normalized(rayEnd - rayOrigin);
    }

    static float GetSegmentLengthClipSpace(const vec_t& start, const vec_t& end)
    {
        vec_t startOfSegment = start;
        startOfSegment.TransformPoint(g_context.mMVP);
        if (fabsf(startOfSegment.w) > FLT_EPSILON) // check for axis aligned with camera direction
            startOfSegment *= 1.f / startOfSegment.w;

        vec_t endOfSegment = end;
        endOfSegment.TransformPoint(g_context.mMVP);
        if (fabsf(endOfSegment.w) > FLT_EPSILON) // check for axis aligned with camera direction
            endOfSegment *= 1.f / endOfSegment.w;

        vec_t clipSpaceAxis = endOfSegment - startOfSegment;
        clipSpaceAxis.y /= g_context.mDisplayRatio;
        float segmentLengthInClipSpace = sqrtf(clipSpaceAxis.x * clipSpaceAxis.x + clipSpaceAxis.y * clipSpaceAxis.y);
        return segmentLengthInClipSpace;
    }

    static float GetParallelogram(const vec_t& ptO, const vec_t& ptA, const vec_t& ptB)
    {
        vec_t pts[] = {ptO, ptA, ptB};
        for (unsigned int i = 0; i < 3; i++)
        {
            pts[i].TransformPoint(g_context.mMVP);
            if (fabsf(pts[i].w) > FLT_EPSILON) // check for axis aligned with camera direction
                pts[i] *= 1.f / pts[i].w;
        }
        vec_t segA = pts[1] - pts[0];
        vec_t segB = pts[2] - pts[0];
        segA.y /= g_context.mDisplayRatio;
        segB.y /= g_context.mDisplayRatio;
        vec_t segAOrtho = makeVect(-segA.y, segA.x);
        segAOrtho.Normalize();
        float dt = segAOrtho.Dot3(segB);
        float surface = sqrtf(segA.x * segA.x + segA.y * segA.y) * fabsf(dt);
        return surface;
    }

    inline vec_t PointOnSegment(const vec_t& point, const vec_t& vertPos1, const vec_t& vertPos2)
    {
        vec_t c = point - vertPos1;
        vec_t V;

        V.Normalize(vertPos2 - vertPos1);
        float d = (vertPos2 - vertPos1).Length();
        float t = V.Dot3(c);

        if (t < 0.f)
            return vertPos1;

        if (t > d)
            return vertPos2;

        return vertPos1 + V * t;
    }

    static float IntersectRayPlane(const vec_t& rOrigin, const vec_t& rVector, const vec_t& plan)
    {
        float numer = plan.Dot3(rOrigin) - plan.w;
        float denom = plan.Dot3(rVector);

        if (fabsf(denom) < FLT_EPSILON)  // normal is orthogonal to vector, cant intersect
            return -1.0f;

        return -(numer / denom);
    }

    static bool IsInContextRect(ImVec2 p)
    {
        return IsWithin(p.x, g_context.mX, g_context.mXMax) && IsWithin(p.y, g_context.mY, g_context.mYMax);
    }

    void SetRect(float x, float y, float width, float height)
    {
        g_context.mX = x;
        g_context.mY = y;
        g_context.mWidth = width;
        g_context.mHeight = height;
        g_context.mXMax = g_context.mX + g_context.mWidth;
        g_context.mYMax = g_context.mY + g_context.mXMax;
        g_context.mDisplayRatio = width / height;
    }

    IMGUI_API void SetOrthographic(bool isOrthographic)
    {
        g_context.mIsOrthographic = isOrthographic;
    }

    void SetDrawlist()
    {
        g_context.mDrawList = ImGui::GetWindowDrawList();
    }

    void BeginFrame()
    {
        ImGuiIO& io = ImGui::GetIO();

        const ImU32 flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar |
                            ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoSavedSettings |
                            ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus;
        ImGui::SetNextWindowSize(io.DisplaySize);
        ImGui::SetNextWindowPos(ImVec2(0, 0));

        ImGui::PushStyleColor(ImGuiCol_WindowBg, 0);
        ImGui::PushStyleColor(ImGuiCol_Border, 0);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);

        ImGui::Begin("gizmo", NULL, flags);
        g_context.mDrawList = ImGui::GetWindowDrawList();
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(2);
    }

    bool IsUsing()
    {
        return g_context.mbUsing || g_context.mbUsingBounds;
    }

    bool IsOver()
    {
        return (GetMoveType(NULL) != NONE) || GetRotateType() != NONE || GetScaleType() != NONE || IsUsing();
    }

    void Enable(bool enable)
    {
        g_context.mbEnable = enable;
        if (!enable)
        {
            g_context.mbUsing = false;
            g_context.mbUsingBounds = false;
        }
    }

    static float GetUniform(const vec_t& position, const matrix_t& mat)
    {
        vec_t trf = makeVect(position.x, position.y, position.z, 1.f);
        trf.Transform(mat);
        return trf.w;
    }

    static void ComputeContext(const float* view, const float* projection, float* matrix, MODE mode)
    {
        g_context.mMode = mode;
        g_context.mViewMat = *(matrix_t*) view;
        g_context.mProjectionMat = *(matrix_t*) projection;

        if (mode == LOCAL)
        {
            g_context.mModel = *(matrix_t*) matrix;
            g_context.mModel.OrthoNormalize();
        } else
        {
            g_context.mModel.MakeTranslationMatrix(((matrix_t*) matrix)->v.position);
        }
        g_context.mModelSource = *(matrix_t*) matrix;
        g_context.mModelScaleOrigin.Set(g_context.mModelSource.v.right.Length(), g_context.mModelSource.v.up.Length(),
                                        g_context.mModelSource.v.dir.Length());

        g_context.mModelInverse.Inverse(g_context.mModel);
        g_context.mModelSourceInverse.Inverse(g_context.mModelSource);
        g_context.mViewProjection = g_context.mViewMat * g_context.mProjectionMat;
        g_context.mMVP = g_context.mModel * g_context.mViewProjection;

        matrix_t viewInverse;
        viewInverse.Inverse(g_context.mViewMat);
        g_context.mCameraDir = viewInverse.v.dir;
        g_context.mCameraEye = viewInverse.v.position;
        g_context.mCameraRight = viewInverse.v.right;
        g_context.mCameraUp = viewInverse.v.up;

        // compute scale from the size of camera right vector projected on screen at the matrix position
        vec_t pointRight = viewInverse.v.right;
        pointRight.TransformPoint(g_context.mViewProjection);
        g_context.mScreenFactor = gGizmoSizeClipSpace / (pointRight.x / pointRight.w -
                                                         g_context.mMVP.v.position.x / g_context.mMVP.v.position.w);

        vec_t rightViewInverse = viewInverse.v.right;
        rightViewInverse.TransformVector(g_context.mModelInverse);
        float rightLength = GetSegmentLengthClipSpace(makeVect(0.f, 0.f), rightViewInverse);
        g_context.mScreenFactor = gGizmoSizeClipSpace / rightLength;

        ImVec2 centerSSpace = worldToPos(makeVect(0.f, 0.f), g_context.mMVP);
        g_context.mScreenSquareCenter = centerSSpace;
        g_context.mScreenSquareMin = ImVec2(centerSSpace.x - 10.f, centerSSpace.y - 10.f);
        g_context.mScreenSquareMax = ImVec2(centerSSpace.x + 10.f, centerSSpace.y + 10.f);

        ComputeCameraRay(g_context.mRayOrigin, g_context.mRayVector);
    }

    static void ComputeColors(ImU32* colors, int type, OPERATION operation)
    {
        if (g_context.mbEnable)
        {
            switch (operation)
            {
            case TRANSLATE:
                colors[0] = (type == MOVE_SCREEN) ? selectionColor : 0xFFFFFFFF;
                for (int i = 0; i < 3; i++)
                {
                    colors[i + 1] = (type == (int) (MOVE_X + i)) ? selectionColor : directionColor[i];
                    colors[i + 4] = (type == (int) (MOVE_YZ + i)) ? selectionColor : planeColor[i];
                    colors[i + 4] = (type == MOVE_SCREEN) ? selectionColor : colors[i + 4];
                }
                break;
            case ROTATE:
                colors[0] = (type == ROTATE_SCREEN) ? selectionColor : 0xFFFFFFFF;
                for (int i = 0; i < 3; i++)
                    colors[i + 1] = (type == (int) (ROTATE_X + i)) ? selectionColor : directionColor[i];
                break;
            case SCALE:
                colors[0] = (type == SCALE_XYZ) ? selectionColor : 0xFFFFFFFF;
                for (int i = 0; i < 3; i++)
                    colors[i + 1] = (type == (int) (SCALE_X + i)) ? selectionColor : directionColor[i];
                break;
            case BOUNDS:
                break;
            }
        } else
        {
            for (int i = 0; i < 7; i++)
                colors[i] = inactiveColor;
        }
    }

    static void ComputeTripodAxisAndVisibility(int axisIndex, vec_t& dirAxis, vec_t& dirPlaneX, vec_t& dirPlaneY,
                                               bool& belowAxisLimit, bool& belowPlaneLimit)
    {
        dirAxis = directionUnary[axisIndex];
        dirPlaneX = directionUnary[(axisIndex + 1) % 3];
        dirPlaneY = directionUnary[(axisIndex + 2) % 3];

        if (g_context.mbUsing)
        {
            // when using, use stored factors so the gizmo doesn't flip when we translate
            belowAxisLimit = g_context.mBelowAxisLimit[axisIndex];
            belowPlaneLimit = g_context.mBelowPlaneLimit[axisIndex];

            dirAxis *= g_context.mAxisFactor[axisIndex];
            dirPlaneX *= g_context.mAxisFactor[(axisIndex + 1) % 3];
            dirPlaneY *= g_context.mAxisFactor[(axisIndex + 2) % 3];
        } else
        {
            // new method
            float lenDir = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), dirAxis);
            float lenDirMinus = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), -dirAxis);

            float lenDirPlaneX = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), dirPlaneX);
            float lenDirMinusPlaneX = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), -dirPlaneX);

            float lenDirPlaneY = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), dirPlaneY);
            float lenDirMinusPlaneY = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f), -dirPlaneY);

            float mulAxis = (lenDir < lenDirMinus && fabsf(lenDir - lenDirMinus) > FLT_EPSILON) ? -1.f : 1.f;
            float mulAxisX = (lenDirPlaneX < lenDirMinusPlaneX && fabsf(lenDirPlaneX - lenDirMinusPlaneX) > FLT_EPSILON)
                             ? -1.f : 1.f;
            float mulAxisY = (lenDirPlaneY < lenDirMinusPlaneY && fabsf(lenDirPlaneY - lenDirMinusPlaneY) > FLT_EPSILON)
                             ? -1.f : 1.f;
            dirAxis *= mulAxis;
            dirPlaneX *= mulAxisX;
            dirPlaneY *= mulAxisY;

            // for axis
            float axisLengthInClipSpace = GetSegmentLengthClipSpace(makeVect(0.f, 0.f, 0.f),
                                                                    dirAxis * g_context.mScreenFactor);

            float paraSurf = GetParallelogram(makeVect(0.f, 0.f, 0.f), dirPlaneX * g_context.mScreenFactor,
                                              dirPlaneY * g_context.mScreenFactor);
            belowPlaneLimit = (paraSurf > 0.0025f);
            belowAxisLimit = (axisLengthInClipSpace > 0.02f);

            // and store values
            g_context.mAxisFactor[axisIndex] = mulAxis;
            g_context.mAxisFactor[(axisIndex + 1) % 3] = mulAxisX;
            g_context.mAxisFactor[(axisIndex + 2) % 3] = mulAxisY;
            g_context.mBelowAxisLimit[axisIndex] = belowAxisLimit;
            g_context.mBelowPlaneLimit[axisIndex] = belowPlaneLimit;
        }
    }

    static void ComputeSnap(float* value, float snap)
    {
        if (snap <= FLT_EPSILON)
            return;
        float modulo = fmodf(*value, snap);
        float moduloRatio = fabsf(modulo) / snap;
        if (moduloRatio < snapTension)
            *value -= modulo;
        else if (moduloRatio > (1.f - snapTension))
            *value = *value - modulo + snap * ((*value < 0.f) ? -1.f : 1.f);
    }

    static void ComputeSnap(vec_t& value, float* snap)
    {
        for (int i = 0; i < 3; i++)
        {
            ComputeSnap(&value[i], snap[i]);
        }
    }

    static float ComputeAngleOnPlan()
    {
        const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector, g_context.mTranslationPlan);
        vec_t localPos = normalized(g_context.mRayOrigin + g_context.mRayVector * len - g_context.mModel.v.position);

        vec_t perpendicularVector;
        perpendicularVector.Cross(g_context.mRotationVectorSource, g_context.mTranslationPlan);
        perpendicularVector.Normalize();
        float acosAngle = clamp(dotProduct(localPos, g_context.mRotationVectorSource), -0.9999f, 0.9999f);
        float angle = acosf(acosAngle);
        angle *= (dotProduct(localPos, perpendicularVector) < 0.f) ? 1.f : -1.f;
        return angle;
    }

    static void DrawRotationGizmo(int type)
    {
        ImDrawList* drawList = g_context.mDrawList;

        // colors
        ImU32 colors[7];
        ComputeColors(colors, type, ROTATE);

        vec_t cameraToModelNormalized;
        if (g_context.mIsOrthographic)
        {
            matrix_t viewInverse;
            viewInverse.Inverse(*(matrix_t*) &g_context.mViewMat);
            cameraToModelNormalized = viewInverse.v.dir;
        } else
        {
            cameraToModelNormalized = normalized(g_context.mModel.v.position - g_context.mCameraEye);
        }

        cameraToModelNormalized.TransformVector(g_context.mModelInverse);

        g_context.mRadiusSquareCenter = screenRotateSize * g_context.mHeight;

        for (int axis = 0; axis < 3; axis++)
        {
            ImVec2 circlePos[halfCircleSegmentCount];

            float angleStart =
                atan2f(cameraToModelNormalized[(4 - axis) % 3], cameraToModelNormalized[(3 - axis) % 3]) + ZPI * 0.5f;

            for (unsigned int i = 0; i < halfCircleSegmentCount; i++)
            {
                float ng = angleStart + ZPI * ((float) i / (float) halfCircleSegmentCount);
                vec_t axisPos = makeVect(cosf(ng), sinf(ng), 0.f);
                vec_t pos =
                    makeVect(axisPos[axis], axisPos[(axis + 1) % 3], axisPos[(axis + 2) % 3]) * g_context.mScreenFactor;
                circlePos[i] = worldToPos(pos, g_context.mMVP);
            }

            float radiusAxis = sqrtf(
                (ImLengthSqr(worldToPos(g_context.mModel.v.position, g_context.mViewProjection) - circlePos[0])));
            if (radiusAxis > g_context.mRadiusSquareCenter)
                g_context.mRadiusSquareCenter = radiusAxis;

            drawList->AddPolyline(circlePos, halfCircleSegmentCount, colors[3 - axis], false, 2);
        }
        drawList->AddCircle(worldToPos(g_context.mModel.v.position, g_context.mViewProjection),
                            g_context.mRadiusSquareCenter, colors[0], 64, 3.f);

        if (g_context.mbUsing)
        {
            ImVec2 circlePos[halfCircleSegmentCount + 1];

            circlePos[0] = worldToPos(g_context.mModel.v.position, g_context.mViewProjection);
            for (unsigned int i = 1; i < halfCircleSegmentCount; i++)
            {
                float ng = g_context.mRotationAngle * ((float) (i - 1) / (float) (halfCircleSegmentCount - 1));
                matrix_t rotateVectorMatrix;
                rotateVectorMatrix.RotationAxis(g_context.mTranslationPlan, ng);
                vec_t pos;
                pos.TransformPoint(g_context.mRotationVectorSource, rotateVectorMatrix);
                pos *= g_context.mScreenFactor;
                circlePos[i] = worldToPos(pos + g_context.mModel.v.position, g_context.mViewProjection);
            }
            drawList->AddConvexPolyFilled(circlePos, halfCircleSegmentCount, 0x801080FF);
            drawList->AddPolyline(circlePos, halfCircleSegmentCount, 0xFF1080FF, true, 2);

            ImVec2 destinationPosOnScreen = circlePos[1];
            char tmps[512];
            ImFormatString(tmps, sizeof(tmps), rotationInfoMask[type - ROTATE_X],
                           (g_context.mRotationAngle / ZPI) * 180.f, g_context.mRotationAngle);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 15, destinationPosOnScreen.y + 15), 0xFF000000, tmps);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 14, destinationPosOnScreen.y + 14), 0xFFFFFFFF, tmps);
        }
    }

    static void DrawHatchedAxis(const vec_t& axis)
    {
        for (int j = 1; j < 10; j++)
        {
            ImVec2 baseSSpace2 = worldToPos(axis * 0.05f * (float) (j * 2) * g_context.mScreenFactor, g_context.mMVP);
            ImVec2 worldDirSSpace2 = worldToPos(axis * 0.05f * (float) (j * 2 + 1) * g_context.mScreenFactor,
                                                g_context.mMVP);
            g_context.mDrawList->AddLine(baseSSpace2, worldDirSSpace2, 0x80000000, 6.f);
        }
    }

    static void DrawScaleGizmo(int type)
    {
        ImDrawList* drawList = g_context.mDrawList;

        // colors
        ImU32 colors[7];
        ComputeColors(colors, type, SCALE);

        // draw
        vec_t scaleDisplay = {1.f, 1.f, 1.f, 1.f};

        if (g_context.mbUsing)
            scaleDisplay = g_context.mScale;

        for (unsigned int i = 0; i < 3; i++)
        {
            vec_t dirPlaneX, dirPlaneY, dirAxis;
            bool belowAxisLimit, belowPlaneLimit;
            ComputeTripodAxisAndVisibility(i, dirAxis, dirPlaneX, dirPlaneY, belowAxisLimit, belowPlaneLimit);

            // draw axis
            if (belowAxisLimit)
            {
                ImVec2 baseSSpace = worldToPos(dirAxis * 0.1f * g_context.mScreenFactor, g_context.mMVP);
                ImVec2 worldDirSSpaceNoScale = worldToPos(dirAxis * g_context.mScreenFactor, g_context.mMVP);
                ImVec2 worldDirSSpace = worldToPos((dirAxis * scaleDisplay[i]) * g_context.mScreenFactor,
                                                   g_context.mMVP);

                if (g_context.mbUsing)
                {
                    drawList->AddLine(baseSSpace, worldDirSSpaceNoScale, 0xFF404040, 3.f);
                    drawList->AddCircleFilled(worldDirSSpaceNoScale, 6.f, 0xFF404040);
                }

                drawList->AddLine(baseSSpace, worldDirSSpace, colors[i + 1], 3.f);
                drawList->AddCircleFilled(worldDirSSpace, 6.f, colors[i + 1]);

                if (g_context.mAxisFactor[i] < 0.f)
                    DrawHatchedAxis(dirAxis * scaleDisplay[i]);
            }
        }

        // draw screen cirle
        drawList->AddCircleFilled(g_context.mScreenSquareCenter, 6.f, colors[0], 32);

        if (g_context.mbUsing)
        {
            //ImVec2 sourcePosOnScreen = worldToPos(g_context.mMatrixOrigin, g_context.mViewProjection);
            ImVec2 destinationPosOnScreen = worldToPos(g_context.mModel.v.position, g_context.mViewProjection);
            /*vec_t dif(destinationPosOnScreen.x - sourcePosOnScreen.x, destinationPosOnScreen.y - sourcePosOnScreen.y);
            dif.Normalize();
            dif *= 5.f;
            drawList->AddCircle(sourcePosOnScreen, 6.f, translationLineColor);
            drawList->AddCircle(destinationPosOnScreen, 6.f, translationLineColor);
            drawList->AddLine(ImVec2(sourcePosOnScreen.x + dif.x, sourcePosOnScreen.y + dif.y), ImVec2(destinationPosOnScreen.x - dif.x, destinationPosOnScreen.y - dif.y), translationLineColor, 2.f);
            */
            char tmps[512];
            //vec_t deltaInfo = g_context.mModel.v.position - g_context.mMatrixOrigin;
            int componentInfoIndex = (type - SCALE_X) * 3;
            ImFormatString(tmps, sizeof(tmps), scaleInfoMask[type - SCALE_X],
                           scaleDisplay[translationInfoIndex[componentInfoIndex]]);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 15, destinationPosOnScreen.y + 15), 0xFF000000, tmps);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 14, destinationPosOnScreen.y + 14), 0xFFFFFFFF, tmps);
        }
    }


    static void DrawTranslationGizmo(int type)
    {
        ImDrawList* drawList = g_context.mDrawList;
        if (!drawList)
            return;

        // colors
        ImU32 colors[7];
        ComputeColors(colors, type, TRANSLATE);

        const ImVec2 origin = worldToPos(g_context.mModel.v.position, g_context.mViewProjection);

        // draw
        bool belowAxisLimit = false;
        bool belowPlaneLimit = false;
        for (unsigned int i = 0; i < 3; ++i)
        {
            vec_t dirPlaneX, dirPlaneY, dirAxis;
            ComputeTripodAxisAndVisibility(i, dirAxis, dirPlaneX, dirPlaneY, belowAxisLimit, belowPlaneLimit);

            // draw axis
            if (belowAxisLimit)
            {
                ImVec2 baseSSpace = worldToPos(dirAxis * 0.1f * g_context.mScreenFactor, g_context.mMVP);
                ImVec2 worldDirSSpace = worldToPos(dirAxis * g_context.mScreenFactor, g_context.mMVP);

                drawList->AddLine(baseSSpace, worldDirSSpace, colors[i + 1], 3.f);

                // Arrow head begin
                ImVec2 dir(origin - worldDirSSpace);

                float d = sqrtf(ImLengthSqr(dir));
                dir /= d; // Normalize
                dir *= 6.0f;

                ImVec2 ortogonalDir(dir.y, -dir.x); // Perpendicular vector
                ImVec2 a(worldDirSSpace + dir);
                drawList->AddTriangleFilled(worldDirSSpace - dir, a + ortogonalDir, a - ortogonalDir, colors[i + 1]);
                // Arrow head end

                if (g_context.mAxisFactor[i] < 0.f)
                    DrawHatchedAxis(dirAxis);
            }

            // draw plane
            if (belowPlaneLimit)
            {
                ImVec2 screenQuadPts[4];
                for (int j = 0; j < 4; ++j)
                {
                    vec_t cornerWorldPos =
                        (dirPlaneX * quadUV[j * 2] + dirPlaneY * quadUV[j * 2 + 1]) * g_context.mScreenFactor;
                    screenQuadPts[j] = worldToPos(cornerWorldPos, g_context.mMVP);
                }
                drawList->AddPolyline(screenQuadPts, 4, directionColor[i], true, 1.0f);
                drawList->AddConvexPolyFilled(screenQuadPts, 4, colors[i + 4]);
            }
        }

        drawList->AddCircleFilled(g_context.mScreenSquareCenter, 6.f, colors[0], 32);

        if (g_context.mbUsing)
        {
            ImVec2 sourcePosOnScreen = worldToPos(g_context.mMatrixOrigin, g_context.mViewProjection);
            ImVec2 destinationPosOnScreen = worldToPos(g_context.mModel.v.position, g_context.mViewProjection);
            vec_t dif = {destinationPosOnScreen.x - sourcePosOnScreen.x, destinationPosOnScreen.y - sourcePosOnScreen.y,
                         0.f, 0.f};
            dif.Normalize();
            dif *= 5.f;
            drawList->AddCircle(sourcePosOnScreen, 6.f, translationLineColor);
            drawList->AddCircle(destinationPosOnScreen, 6.f, translationLineColor);
            drawList->AddLine(ImVec2(sourcePosOnScreen.x + dif.x, sourcePosOnScreen.y + dif.y),
                              ImVec2(destinationPosOnScreen.x - dif.x, destinationPosOnScreen.y - dif.y),
                              translationLineColor, 2.f);

            char tmps[512];
            vec_t deltaInfo = g_context.mModel.v.position - g_context.mMatrixOrigin;
            int componentInfoIndex = (type - MOVE_X) * 3;
            ImFormatString(tmps, sizeof(tmps), translationInfoMask[type - MOVE_X],
                           deltaInfo[translationInfoIndex[componentInfoIndex]],
                           deltaInfo[translationInfoIndex[componentInfoIndex + 1]],
                           deltaInfo[translationInfoIndex[componentInfoIndex + 2]]);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 15, destinationPosOnScreen.y + 15), 0xFF000000, tmps);
            drawList->AddText(ImVec2(destinationPosOnScreen.x + 14, destinationPosOnScreen.y + 14), 0xFFFFFFFF, tmps);
        }
    }

    static bool CanActivate()
    {
        if (ImGui::IsMouseClicked(0) && !ImGui::IsAnyItemHovered() && !ImGui::IsAnyItemActive())
            return true;
        return false;
    }

    static void HandleAndDrawLocalBounds(float* bounds, matrix_t* matrix, float* snapValues, OPERATION operation)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* drawList = g_context.mDrawList;

        // compute best projection axis
        vec_t axesWorldDirections[3];
        vec_t bestAxisWorldDirection = {0.0f, 0.0f, 0.0f, 0.0f};
        int axes[3];
        unsigned int numAxes = 1;
        axes[0] = g_context.mBoundsBestAxis;
        int bestAxis = axes[0];
        if (!g_context.mbUsingBounds)
        {
            numAxes = 0;
            float bestDot = 0.f;
            for (unsigned int i = 0; i < 3; i++)
            {
                vec_t dirPlaneNormalWorld;
                dirPlaneNormalWorld.TransformVector(directionUnary[i], g_context.mModelSource);
                dirPlaneNormalWorld.Normalize();

                float dt = fabsf(
                    dotProduct(normalized(g_context.mCameraEye - g_context.mModelSource.v.position),
                               dirPlaneNormalWorld));
                if (dt >= bestDot)
                {
                    bestDot = dt;
                    bestAxis = i;
                    bestAxisWorldDirection = dirPlaneNormalWorld;
                }

                if (dt >= 0.1f)
                {
                    axes[numAxes] = i;
                    axesWorldDirections[numAxes] = dirPlaneNormalWorld;
                    ++numAxes;
                }
            }
        }

        if (numAxes == 0)
        {
            axes[0] = bestAxis;
            axesWorldDirections[0] = bestAxisWorldDirection;
            numAxes = 1;
        } else if (bestAxis != axes[0])
        {
            unsigned int bestIndex = 0;
            for (unsigned int i = 0; i < numAxes; i++)
            {
                if (axes[i] == bestAxis)
                {
                    bestIndex = i;
                    break;
                }
            }
            int tempAxis = axes[0];
            axes[0] = axes[bestIndex];
            axes[bestIndex] = tempAxis;
            vec_t tempDirection = axesWorldDirections[0];
            axesWorldDirections[0] = axesWorldDirections[bestIndex];
            axesWorldDirections[bestIndex] = tempDirection;
        }

        for (unsigned int axisIndex = 0; axisIndex < numAxes; ++axisIndex)
        {
            bestAxis = axes[axisIndex];
            bestAxisWorldDirection = axesWorldDirections[axisIndex];

            // corners
            vec_t aabb[4];

            int secondAxis = (bestAxis + 1) % 3;
            int thirdAxis = (bestAxis + 2) % 3;

            for (int i = 0; i < 4; i++)
            {
                aabb[i][3] = aabb[i][bestAxis] = 0.f;
                aabb[i][secondAxis] = bounds[secondAxis + 3 * (i >> 1)];
                aabb[i][thirdAxis] = bounds[thirdAxis + 3 * ((i >> 1) ^ (i & 1))];
            }

            // draw bounds
            unsigned int anchorAlpha = g_context.mbEnable ? 0xFF000000 : 0x80000000;

            matrix_t boundsMVP = g_context.mModelSource * g_context.mViewProjection;
            for (int i = 0; i < 4; i++)
            {
                ImVec2 worldBound1 = worldToPos(aabb[i], boundsMVP);
                ImVec2 worldBound2 = worldToPos(aabb[(i + 1) % 4], boundsMVP);
                if (!IsInContextRect(worldBound1) || !IsInContextRect(worldBound2))
                {
                    continue;
                }
                float boundDistance = sqrtf(ImLengthSqr(worldBound1 - worldBound2));
                int stepCount = (int) (boundDistance / 10.f);
                stepCount = min(stepCount, 1000);
                float stepLength = 1.f / (float) stepCount;
                for (int j = 0; j < stepCount; j++)
                {
                    float t1 = (float) j * stepLength;
                    float t2 = (float) j * stepLength + stepLength * 0.5f;
                    ImVec2 worldBoundSS1 = ImLerp(worldBound1, worldBound2, ImVec2(t1, t1));
                    ImVec2 worldBoundSS2 = ImLerp(worldBound1, worldBound2, ImVec2(t2, t2));
                    //drawList->AddLine(worldBoundSS1, worldBoundSS2, 0x000000 + anchorAlpha, 3.f);
                    drawList->AddLine(worldBoundSS1, worldBoundSS2, 0xAAAAAA + anchorAlpha, 2.f);
                }
                vec_t midPoint = (aabb[i] + aabb[(i + 1) % 4]) * 0.5f;
                ImVec2 midBound = worldToPos(midPoint, boundsMVP);
                static const float AnchorBigRadius = 8.f;
                static const float AnchorSmallRadius = 6.f;
                bool overBigAnchor = ImLengthSqr(worldBound1 - io.MousePos) <= (AnchorBigRadius * AnchorBigRadius);
                bool overSmallAnchor = ImLengthSqr(midBound - io.MousePos) <= (AnchorBigRadius * AnchorBigRadius);

                int type = NONE;
                vec_t gizmoHitProportion;

                switch (operation)
                {
                case TRANSLATE:
                    type = GetMoveType(&gizmoHitProportion);
                    break;
                case ROTATE:
                    type = GetRotateType();
                    break;
                case SCALE:
                    type = GetScaleType();
                    break;
                case BOUNDS:
                    break;
                }
                if (type != NONE)
                {
                    overBigAnchor = false;
                    overSmallAnchor = false;
                }


                unsigned int bigAnchorColor = overBigAnchor ? selectionColor : (0xAAAAAA + anchorAlpha);
                unsigned int smallAnchorColor = overSmallAnchor ? selectionColor : (0xAAAAAA + anchorAlpha);

                drawList->AddCircleFilled(worldBound1, AnchorBigRadius, 0xFF000000);
                drawList->AddCircleFilled(worldBound1, AnchorBigRadius - 1.2f, bigAnchorColor);

                drawList->AddCircleFilled(midBound, AnchorSmallRadius, 0xFF000000);
                drawList->AddCircleFilled(midBound, AnchorSmallRadius - 1.2f, smallAnchorColor);
                int oppositeIndex = (i + 2) % 4;
                // big anchor on corners
                if (!g_context.mbUsingBounds && g_context.mbEnable && overBigAnchor && CanActivate())
                {
                    g_context.mBoundsPivot.TransformPoint(aabb[(i + 2) % 4], g_context.mModelSource);
                    g_context.mBoundsAnchor.TransformPoint(aabb[i], g_context.mModelSource);
                    g_context.mBoundsPlan = BuildPlan(g_context.mBoundsAnchor, bestAxisWorldDirection);
                    g_context.mBoundsBestAxis = bestAxis;
                    g_context.mBoundsAxis[0] = secondAxis;
                    g_context.mBoundsAxis[1] = thirdAxis;

                    g_context.mBoundsLocalPivot.Set(0.f);
                    g_context.mBoundsLocalPivot[secondAxis] = aabb[oppositeIndex][secondAxis];
                    g_context.mBoundsLocalPivot[thirdAxis] = aabb[oppositeIndex][thirdAxis];

                    g_context.mbUsingBounds = true;
                    g_context.mBoundsMatrix = g_context.mModelSource;
                }
                // small anchor on middle of segment
                if (!g_context.mbUsingBounds && g_context.mbEnable && overSmallAnchor && CanActivate())
                {
                    vec_t midPointOpposite = (aabb[(i + 2) % 4] + aabb[(i + 3) % 4]) * 0.5f;
                    g_context.mBoundsPivot.TransformPoint(midPointOpposite, g_context.mModelSource);
                    g_context.mBoundsAnchor.TransformPoint(midPoint, g_context.mModelSource);
                    g_context.mBoundsPlan = BuildPlan(g_context.mBoundsAnchor, bestAxisWorldDirection);
                    g_context.mBoundsBestAxis = bestAxis;
                    int indices[] = {secondAxis, thirdAxis};
                    g_context.mBoundsAxis[0] = indices[i % 2];
                    g_context.mBoundsAxis[1] = -1;

                    g_context.mBoundsLocalPivot.Set(0.f);
                    g_context.mBoundsLocalPivot[g_context.mBoundsAxis[0]] = aabb[oppositeIndex][indices[i %
                                                                                                        2]];// bounds[g_context.mBoundsAxis[0]] * (((i + 1) & 2) ? 1.f : -1.f);

                    g_context.mbUsingBounds = true;
                    g_context.mBoundsMatrix = g_context.mModelSource;
                }
            }

            if (g_context.mbUsingBounds)
            {
                matrix_t scale;
                scale.SetToIdentity();

                // compute projected mouse position on plan
                const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector, g_context.mBoundsPlan);
                vec_t newPos = g_context.mRayOrigin + g_context.mRayVector * len;

                // compute a reference and delta vectors base on mouse move
                vec_t deltaVector = (newPos - g_context.mBoundsPivot).Abs();
                vec_t referenceVector = (g_context.mBoundsAnchor - g_context.mBoundsPivot).Abs();

                // for 1 or 2 axes, compute a ratio that's used for scale and snap it based on resulting length
                for (int i = 0; i < 2; i++)
                {
                    int axisIndex1 = g_context.mBoundsAxis[i];
                    if (axisIndex1 == -1)
                        continue;

                    float ratioAxis = 1.f;
                    vec_t axisDir = g_context.mBoundsMatrix.component[axisIndex1].Abs();

                    float dtAxis = axisDir.Dot(referenceVector);
                    float boundSize = bounds[axisIndex1 + 3] - bounds[axisIndex1];
                    if (dtAxis > FLT_EPSILON)
                        ratioAxis = axisDir.Dot(deltaVector) / dtAxis;

                    if (snapValues)
                    {
                        float length = boundSize * ratioAxis;
                        ComputeSnap(&length, snapValues[axisIndex1]);
                        if (boundSize > FLT_EPSILON)
                            ratioAxis = length / boundSize;
                    }
                    scale.component[axisIndex1] *= ratioAxis;
                }

                // transform matrix
                matrix_t preScale, postScale;
                preScale.MakeTranslationMatrix(-g_context.mBoundsLocalPivot);
                postScale.MakeTranslationMatrix(g_context.mBoundsLocalPivot);
                matrix_t res = preScale * scale * postScale * g_context.mBoundsMatrix;
                *matrix = res;

                // info text
                char tmps[512];
                ImVec2 destinationPosOnScreen = worldToPos(g_context.mModel.v.position, g_context.mViewProjection);
                ImFormatString(tmps, sizeof(tmps), "X: %.2f Y: %.2f Z:%.2f",
                               (bounds[3] - bounds[0]) * g_context.mBoundsMatrix.component[0].Length() *
                               scale.component[0].Length(),
                               (bounds[4] - bounds[1]) * g_context.mBoundsMatrix.component[1].Length() *
                               scale.component[1].Length(),
                               (bounds[5] - bounds[2]) * g_context.mBoundsMatrix.component[2].Length() *
                               scale.component[2].Length()
                );
                drawList->AddText(ImVec2(destinationPosOnScreen.x + 15, destinationPosOnScreen.y + 15), 0xFF000000,
                                  tmps);
                drawList->AddText(ImVec2(destinationPosOnScreen.x + 14, destinationPosOnScreen.y + 14), 0xFFFFFFFF,
                                  tmps);
            }

            if (!io.MouseDown[0])
                g_context.mbUsingBounds = false;

            if (g_context.mbUsingBounds)
                break;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //

    static int GetScaleType()
    {
        ImGuiIO& io = ImGui::GetIO();
        int type = NONE;

        // screen
        if (io.MousePos.x >= g_context.mScreenSquareMin.x && io.MousePos.x <= g_context.mScreenSquareMax.x &&
            io.MousePos.y >= g_context.mScreenSquareMin.y && io.MousePos.y <= g_context.mScreenSquareMax.y)
            type = SCALE_XYZ;

        // compute
        for (unsigned int i = 0; i < 3 && type == NONE; i++)
        {
            vec_t dirPlaneX, dirPlaneY, dirAxis;
            bool belowAxisLimit, belowPlaneLimit;
            ComputeTripodAxisAndVisibility(i, dirAxis, dirPlaneX, dirPlaneY, belowAxisLimit, belowPlaneLimit);

            dirAxis.TransformVector(g_context.mModel);

            const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector,
                                                BuildPlan(g_context.mModel.v.position, dirAxis));
            vec_t posOnPlan = g_context.mRayOrigin + g_context.mRayVector * len;

            const ImVec2 posOnPlanScreen = worldToPos(posOnPlan, g_context.mViewProjection);
            const ImVec2 axisStartOnScreen = worldToPos(
                g_context.mModel.v.position + dirAxis * g_context.mScreenFactor * 0.1f, g_context.mViewProjection);
            const ImVec2 axisEndOnScreen = worldToPos(g_context.mModel.v.position + dirAxis * g_context.mScreenFactor,
                                                      g_context.mViewProjection);

            vec_t closestPointOnAxis = PointOnSegment(makeVect(posOnPlanScreen), makeVect(axisStartOnScreen),
                                                      makeVect(axisEndOnScreen));

            if ((closestPointOnAxis - makeVect(posOnPlanScreen)).Length() < 12.f) // pixel size
                type = SCALE_X + i;
        }
        return type;
    }

    static int GetRotateType()
    {
        ImGuiIO& io = ImGui::GetIO();
        int type = NONE;

        vec_t deltaScreen = {io.MousePos.x - g_context.mScreenSquareCenter.x,
                             io.MousePos.y - g_context.mScreenSquareCenter.y, 0.f, 0.f};
        float dist = deltaScreen.Length();
        if (dist >= (g_context.mRadiusSquareCenter - 1.0f) && dist < (g_context.mRadiusSquareCenter + 1.0f))
            type = ROTATE_SCREEN;

        const vec_t planNormals[] = {g_context.mModel.v.right, g_context.mModel.v.up, g_context.mModel.v.dir};

        for (unsigned int i = 0; i < 3 && type == NONE; i++)
        {
            // pickup plan
            vec_t pickupPlan = BuildPlan(g_context.mModel.v.position, planNormals[i]);

            const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector, pickupPlan);
            vec_t localPos = g_context.mRayOrigin + g_context.mRayVector * len - g_context.mModel.v.position;

            if (dotProduct(normalized(localPos), g_context.mRayVector) > FLT_EPSILON)
                continue;
            vec_t idealPosOnCircle = normalized(localPos);
            idealPosOnCircle.TransformVector(g_context.mModelInverse);
            ImVec2 idealPosOnCircleScreen = worldToPos(idealPosOnCircle * g_context.mScreenFactor, g_context.mMVP);

            //g_context.mDrawList->AddCircle(idealPosOnCircleScreen, 5.f, 0xFFFFFFFF);
            ImVec2 distanceOnScreen = idealPosOnCircleScreen - io.MousePos;

            float distance = makeVect(distanceOnScreen).Length();
            if (distance < 8.f) // pixel size
                type = ROTATE_X + i;
        }

        return type;
    }

    static int GetMoveType(vec_t* gizmoHitProportion)
    {
        ImGuiIO& io = ImGui::GetIO();
        int type = NONE;

        // screen
        if (io.MousePos.x >= g_context.mScreenSquareMin.x && io.MousePos.x <= g_context.mScreenSquareMax.x &&
            io.MousePos.y >= g_context.mScreenSquareMin.y && io.MousePos.y <= g_context.mScreenSquareMax.y)
            type = MOVE_SCREEN;

        // compute
        for (unsigned int i = 0; i < 3 && type == NONE; i++)
        {
            vec_t dirPlaneX, dirPlaneY, dirAxis;
            bool belowAxisLimit, belowPlaneLimit;
            ComputeTripodAxisAndVisibility(i, dirAxis, dirPlaneX, dirPlaneY, belowAxisLimit, belowPlaneLimit);
            dirAxis.TransformVector(g_context.mModel);
            dirPlaneX.TransformVector(g_context.mModel);
            dirPlaneY.TransformVector(g_context.mModel);

            const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector,
                                                BuildPlan(g_context.mModel.v.position, dirAxis));
            vec_t posOnPlan = g_context.mRayOrigin + g_context.mRayVector * len;

            const ImVec2 posOnPlanScreen = worldToPos(posOnPlan, g_context.mViewProjection);
            const ImVec2 axisStartOnScreen = worldToPos(
                g_context.mModel.v.position + dirAxis * g_context.mScreenFactor * 0.1f, g_context.mViewProjection);
            const ImVec2 axisEndOnScreen = worldToPos(g_context.mModel.v.position + dirAxis * g_context.mScreenFactor,
                                                      g_context.mViewProjection);

            vec_t closestPointOnAxis = PointOnSegment(makeVect(posOnPlanScreen), makeVect(axisStartOnScreen),
                                                      makeVect(axisEndOnScreen));

            if ((closestPointOnAxis - makeVect(posOnPlanScreen)).Length() < 12.f) // pixel size
                type = MOVE_X + i;

            const float dx = dirPlaneX.Dot3(
                (posOnPlan - g_context.mModel.v.position) * (1.f / g_context.mScreenFactor));
            const float dy = dirPlaneY.Dot3(
                (posOnPlan - g_context.mModel.v.position) * (1.f / g_context.mScreenFactor));
            if (belowPlaneLimit && dx >= quadUV[0] && dx <= quadUV[4] && dy >= quadUV[1] && dy <= quadUV[3])
                type = MOVE_YZ + i;

            if (gizmoHitProportion)
                *gizmoHitProportion = makeVect(dx, dy, 0.f);
        }
        return type;
    }

    static void HandleTranslation(float* matrix, float* deltaMatrix, int& type, float* snap)
    {
        ImGuiIO& io = ImGui::GetIO();
        bool applyRotationLocaly = g_context.mMode == LOCAL || type == MOVE_SCREEN;

        // move
        if (g_context.mbUsing)
        {
            ImGui::CaptureMouseFromApp();
            const float len = fabsf(
                IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector, g_context.mTranslationPlan)); // near plan
            vec_t newPos = g_context.mRayOrigin + g_context.mRayVector * len;



            // compute delta
            vec_t newOrigin = newPos - g_context.mRelativeOrigin * g_context.mScreenFactor;
            vec_t delta = newOrigin - g_context.mModel.v.position;

            // 1 axis constraint
            if (g_context.mCurrentOperation >= MOVE_X && g_context.mCurrentOperation <= MOVE_Z)
            {
                int axisIndex = g_context.mCurrentOperation - MOVE_X;
                const vec_t& axisValue = *(vec_t*) &g_context.mModel.m[axisIndex];
                float lengthOnAxis = dotProduct(axisValue, delta);
                delta = axisValue * lengthOnAxis;
            }

            // snap
            if (snap)
            {
                vec_t cumulativeDelta = g_context.mModel.v.position + delta - g_context.mMatrixOrigin;
                if (applyRotationLocaly)
                {
                    matrix_t modelSourceNormalized = g_context.mModelSource;
                    modelSourceNormalized.OrthoNormalize();
                    matrix_t modelSourceNormalizedInverse;
                    modelSourceNormalizedInverse.Inverse(modelSourceNormalized);
                    cumulativeDelta.TransformVector(modelSourceNormalizedInverse);
                    ComputeSnap(cumulativeDelta, snap);
                    cumulativeDelta.TransformVector(modelSourceNormalized);
                } else
                {
                    ComputeSnap(cumulativeDelta, snap);
                }
                delta = g_context.mMatrixOrigin + cumulativeDelta - g_context.mModel.v.position;

            }

            // compute matrix & delta
            matrix_t deltaMatrixTranslation;
            deltaMatrixTranslation.MakeTranslationMatrix(delta);
            if (deltaMatrix)
                memcpy(deltaMatrix, deltaMatrixTranslation.m16, sizeof(float) * 16);


            matrix_t res = g_context.mModelSource * deltaMatrixTranslation;
            *(matrix_t*) matrix = res;

            if (!io.MouseDown[0])
                g_context.mbUsing = false;

            type = g_context.mCurrentOperation;
        } else
        {
            // find new possible way to move
            vec_t gizmoHitProportion;
            type = GetMoveType(&gizmoHitProportion);
            if (type != NONE)
            {
                ImGui::CaptureMouseFromApp();
            }
            if (CanActivate() && type != NONE)
            {
                g_context.mbUsing = true;
                g_context.mCurrentOperation = type;
                vec_t movePlanNormal[] = {g_context.mModel.v.right, g_context.mModel.v.up, g_context.mModel.v.dir,
                                          g_context.mModel.v.right, g_context.mModel.v.up, g_context.mModel.v.dir,
                                          -g_context.mCameraDir};

                vec_t cameraToModelNormalized = normalized(g_context.mModel.v.position - g_context.mCameraEye);
                for (unsigned int i = 0; i < 3; i++)
                {
                    vec_t orthoVector = crossProduct(movePlanNormal[i], cameraToModelNormalized);
                    movePlanNormal[i].Cross(orthoVector);
                    movePlanNormal[i].Normalize();
                }
                // pickup plan
                g_context.mTranslationPlan = BuildPlan(g_context.mModel.v.position, movePlanNormal[type - MOVE_X]);
                const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector,
                                                    g_context.mTranslationPlan);
                g_context.mTranslationPlanOrigin = g_context.mRayOrigin + g_context.mRayVector * len;
                g_context.mMatrixOrigin = g_context.mModel.v.position;

                g_context.mRelativeOrigin =
                    (g_context.mTranslationPlanOrigin - g_context.mModel.v.position) * (1.f / g_context.mScreenFactor);
            }
        }
    }

    static void HandleScale(float* matrix, float* deltaMatrix, int& type, float* snap)
    {
        ImGuiIO& io = ImGui::GetIO();

        if (!g_context.mbUsing)
        {
            // find new possible way to scale
            type = GetScaleType();
            if (type != NONE)
            {
                ImGui::CaptureMouseFromApp();
            }
            if (CanActivate() && type != NONE)
            {
                g_context.mbUsing = true;
                g_context.mCurrentOperation = type;
                const vec_t movePlanNormal[] = {g_context.mModel.v.up, g_context.mModel.v.dir, g_context.mModel.v.right,
                                                g_context.mModel.v.dir, g_context.mModel.v.up, g_context.mModel.v.right,
                                                -g_context.mCameraDir};
                // pickup plan

                g_context.mTranslationPlan = BuildPlan(g_context.mModel.v.position, movePlanNormal[type - SCALE_X]);
                const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector,
                                                    g_context.mTranslationPlan);
                g_context.mTranslationPlanOrigin = g_context.mRayOrigin + g_context.mRayVector * len;
                g_context.mMatrixOrigin = g_context.mModel.v.position;
                g_context.mScale.Set(1.f, 1.f, 1.f);
                g_context.mRelativeOrigin =
                    (g_context.mTranslationPlanOrigin - g_context.mModel.v.position) * (1.f / g_context.mScreenFactor);
                g_context.mScaleValueOrigin = makeVect(g_context.mModelSource.v.right.Length(),
                                                       g_context.mModelSource.v.up.Length(),
                                                       g_context.mModelSource.v.dir.Length());
                g_context.mSaveMousePosx = io.MousePos.x;
            }
        }
        // scale
        if (g_context.mbUsing)
        {
            ImGui::CaptureMouseFromApp();
            const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector, g_context.mTranslationPlan);
            vec_t newPos = g_context.mRayOrigin + g_context.mRayVector * len;
            vec_t newOrigin = newPos - g_context.mRelativeOrigin * g_context.mScreenFactor;
            vec_t delta = newOrigin - g_context.mModel.v.position;

            // 1 axis constraint
            if (g_context.mCurrentOperation >= SCALE_X && g_context.mCurrentOperation <= SCALE_Z)
            {
                int axisIndex = g_context.mCurrentOperation - SCALE_X;
                const vec_t& axisValue = *(vec_t*) &g_context.mModel.m[axisIndex];
                float lengthOnAxis = dotProduct(axisValue, delta);
                delta = axisValue * lengthOnAxis;

                vec_t baseVector = g_context.mTranslationPlanOrigin - g_context.mModel.v.position;
                float ratio = dotProduct(axisValue, baseVector + delta) / dotProduct(axisValue, baseVector);

                g_context.mScale[axisIndex] = max(ratio, 0.001f);
            } else
            {
                float scaleDelta = (io.MousePos.x - g_context.mSaveMousePosx) * 0.01f;
                g_context.mScale.Set(max(1.f + scaleDelta, 0.001f));
            }

            // snap
            if (snap)
            {
                float scaleSnap[] = {snap[0], snap[0], snap[0]};
                ComputeSnap(g_context.mScale, scaleSnap);
            }

            // no 0 allowed
            for (int i = 0; i < 3; i++)
                g_context.mScale[i] = max(g_context.mScale[i], 0.001f);

            // compute matrix & delta
            matrix_t deltaMatrixScale;
            deltaMatrixScale.MakeScaleMatrix(g_context.mScale * g_context.mScaleValueOrigin);

            matrix_t res = deltaMatrixScale * g_context.mModel;
            *(matrix_t*) matrix = res;

            if (deltaMatrix)
            {
                deltaMatrixScale.MakeScaleMatrix(g_context.mScale);
                memcpy(deltaMatrix, deltaMatrixScale.m16, sizeof(float) * 16);
            }

            if (!io.MouseDown[0])
                g_context.mbUsing = false;

            type = g_context.mCurrentOperation;
        }
    }

    static void HandleRotation(float* matrix, float* deltaMatrix, int& type, float* snap)
    {
        ImGuiIO& io = ImGui::GetIO();
        bool applyRotationLocaly = g_context.mMode == LOCAL;

        if (!g_context.mbUsing)
        {
            type = GetRotateType();

            if (type != NONE)
            {
                ImGui::CaptureMouseFromApp();
            }

            if (type == ROTATE_SCREEN)
            {
                applyRotationLocaly = true;
            }

            if (CanActivate() && type != NONE)
            {
                g_context.mbUsing = true;
                g_context.mCurrentOperation = type;
                const vec_t rotatePlanNormal[] = {g_context.mModel.v.right, g_context.mModel.v.up,
                                                  g_context.mModel.v.dir, -g_context.mCameraDir};
                // pickup plan
                if (applyRotationLocaly)
                {
                    g_context.mTranslationPlan = BuildPlan(g_context.mModel.v.position,
                                                           rotatePlanNormal[type - ROTATE_X]);
                } else
                {
                    g_context.mTranslationPlan = BuildPlan(g_context.mModelSource.v.position,
                                                           directionUnary[type - ROTATE_X]);
                }

                const float len = IntersectRayPlane(g_context.mRayOrigin, g_context.mRayVector,
                                                    g_context.mTranslationPlan);
                vec_t localPos = g_context.mRayOrigin + g_context.mRayVector * len - g_context.mModel.v.position;
                g_context.mRotationVectorSource = normalized(localPos);
                g_context.mRotationAngleOrigin = ComputeAngleOnPlan();
            }
        }

        // rotation
        if (g_context.mbUsing)
        {
            ImGui::CaptureMouseFromApp();
            g_context.mRotationAngle = ComputeAngleOnPlan();
            if (snap)
            {
                float snapInRadian = snap[0] * DEG2RAD;
                ComputeSnap(&g_context.mRotationAngle, snapInRadian);
            }
            vec_t rotationAxisLocalSpace;

            rotationAxisLocalSpace.TransformVector(
                makeVect(g_context.mTranslationPlan.x, g_context.mTranslationPlan.y, g_context.mTranslationPlan.z, 0.f),
                g_context.mModelInverse);
            rotationAxisLocalSpace.Normalize();

            matrix_t deltaRotation;
            deltaRotation.RotationAxis(rotationAxisLocalSpace,
                                       g_context.mRotationAngle - g_context.mRotationAngleOrigin);
            g_context.mRotationAngleOrigin = g_context.mRotationAngle;

            matrix_t scaleOrigin;
            scaleOrigin.MakeScaleMatrix(g_context.mModelScaleOrigin);

            if (applyRotationLocaly)
            {
                *(matrix_t*) matrix = scaleOrigin * deltaRotation * g_context.mModel;
            } else
            {
                matrix_t res = g_context.mModelSource;
                res.v.position.Set(0.f);

                *(matrix_t*) matrix = res * deltaRotation;
                ((matrix_t*) matrix)->v.position = g_context.mModelSource.v.position;
            }

            if (deltaMatrix)
            {
                *(matrix_t*) deltaMatrix = g_context.mModelInverse * deltaRotation * g_context.mModel;
            }

            if (!io.MouseDown[0])
                g_context.mbUsing = false;

            type = g_context.mCurrentOperation;
        }
    }

    void DecomposeMatrixToComponents(const float* matrix, float* translation, float* rotation, float* scale)
    {
        matrix_t mat = *(matrix_t*) matrix;

        scale[0] = mat.v.right.Length();
        scale[1] = mat.v.up.Length();
        scale[2] = mat.v.dir.Length();

        mat.OrthoNormalize();

        rotation[0] = RAD2DEG * atan2f(mat.m[1][2], mat.m[2][2]);
        rotation[1] = RAD2DEG * atan2f(-mat.m[0][2], sqrtf(mat.m[1][2] * mat.m[1][2] + mat.m[2][2] * mat.m[2][2]));
        rotation[2] = RAD2DEG * atan2f(mat.m[0][1], mat.m[0][0]);

        translation[0] = mat.v.position.x;
        translation[1] = mat.v.position.y;
        translation[2] = mat.v.position.z;
    }

    void
    RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix)
    {
        matrix_t& mat = *(matrix_t*) matrix;

        matrix_t rot[3];
        for (int i = 0; i < 3; i++)
            rot[i].RotationAxis(directionUnary[i], rotation[i] * DEG2RAD);

        mat = rot[0] * rot[1] * rot[2];

        float validScale[3];
        for (int i = 0; i < 3; i++)
        {
            if (fabsf(scale[i]) < FLT_EPSILON)
                validScale[i] = 0.001f;
            else
                validScale[i] = scale[i];
        }
        mat.v.right *= validScale[0];
        mat.v.up *= validScale[1];
        mat.v.dir *= validScale[2];
        mat.v.position.Set(translation[0], translation[1], translation[2], 1.f);
    }

    void Manipulate(const float* view,
                    const float* projection,
                    OPERATION operation,
                    MODE mode,
                    float* matrix,
                    float* deltaMatrix,
                    float* snap,
                    float* localBounds,
                    float* boundsSnap)
    {
        ComputeContext(view, projection, matrix, mode);

        // set delta to identity
        if (deltaMatrix)
        {
            ((matrix_t*) deltaMatrix)->SetToIdentity();
        }

        // behind camera
        vec_t camSpacePosition;
        camSpacePosition.TransformPoint(makeVect(0.f, 0.f, 0.f), g_context.mMVP);
        if (!g_context.mIsOrthographic && camSpacePosition.z < 0.001f)
            return;

        // --
        int type = NONE;
        if (g_context.mbEnable)
        {
            if (!g_context.mbUsingBounds)
            {
                switch (operation)
                {
                case ROTATE:
                    HandleRotation(matrix, deltaMatrix, type, snap);
                    break;
                case TRANSLATE:
                    HandleTranslation(matrix, deltaMatrix, type, snap);
                    break;
                case SCALE:
                    HandleScale(matrix, deltaMatrix, type, snap);
                    break;
                case BOUNDS:
                    break;
                }
            }
        }

        if (localBounds && !g_context.mbUsing)
            HandleAndDrawLocalBounds(localBounds, (matrix_t*) matrix, boundsSnap, operation);

        if (!g_context.mbUsingBounds)
        {
            switch (operation)
            {
            case ROTATE:
                DrawRotationGizmo(type);
                break;
            case TRANSLATE:
                DrawTranslationGizmo(type);
                break;
            case SCALE:
                DrawScaleGizmo(type);
                break;
            case BOUNDS:
                break;
            }
        }
    }

    void DrawGrid(const float* view, const float* projection, const float* matrix, const float gridSize)
    {
        matrix_t res = *(matrix_t*) matrix * *(matrix_t*) view * *(matrix_t*) projection;

        for (float f = -gridSize; f <= gridSize; f += 1.f)
        {
            g_context.mDrawList->AddLine(worldToPos(makeVect(f, 0.f, -gridSize), res),
                                         worldToPos(makeVect(f, 0.f, gridSize), res), 0xFF808080);
            g_context.mDrawList->AddLine(worldToPos(makeVect(-gridSize, 0.f, f), res),
                                         worldToPos(makeVect(gridSize, 0.f, f), res), 0xFF808080);
        }
    }
};
