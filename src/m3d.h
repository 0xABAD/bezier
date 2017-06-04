#ifndef __GUARD_MATH3D_H__
#define __GUARD_MATH3D_H__

#ifdef M3D_STATIC
    #define M3D_DEF static
#else
    #define M3D_DEF extern
#endif

union Vec2 {
    struct { float x, y; };
    struct { float s, t; };
    float data[2];

    float   operator [] (size_t i) const { return data[i]; }
    float & operator [] (size_t i)       { return data[i]; }
};

union Vec3 {
    struct { float x, y, z; };
    struct { Vec2 xy; float __ignored0; };
    struct { float __ignored1; Vec2 yz; };

    struct { float r, g, b; };
    struct { Vec2 rg; float __ignored2; };
    struct { float __ignored3; Vec2 gb; };
    
    struct { float s, t, u; };
    struct { Vec2 st; float __ignored4; };
    struct { float __ignored5; Vec2 tu; };
    
    float data[3];

    float   operator [] (size_t i) const { return data[i]; }
    float & operator [] (size_t i)       { return data[i]; }
};

union Vec4 {
    struct { float x, y, z, w; };
    struct { Vec3 xyz; float __ignored0; };
    struct { float __ignored1; Vec3 yzw; };
    struct { Vec2 xy; Vec2 zw; };
    struct { float __ignored2; Vec2 yz; float __ignored3; };

    struct { float r, g, b, a; };
    struct { Vec3 rgb; float __ignored4; };
    struct { float __ignored5; Vec3 gba; };
    struct { Vec2 rg; Vec2 ba; };
    struct { float __ignored6; Vec2 gb; float __ignored7; };

    struct { float s, t, u, v; };
    struct { Vec3 stu; float __ignored8; };
    struct { float __ignored9; Vec3 tuv; };
    struct { Vec2 st; Vec2 uv; };
    struct { float __ignored10; Vec2 tu; float __ignored11; };

    float data[4];

    float   operator [] (size_t i) const { return data[i]; }
    float & operator [] (size_t i)       { return data[i]; }
};


M3D_DEF Vec2  vec2(float x, float y);
M3D_DEF Vec2  operator - (Vec2 a);
M3D_DEF Vec2  operator - (Vec2 a, Vec2 b);
M3D_DEF Vec2  operator + (Vec2 a, Vec2 b);
M3D_DEF Vec2  operator * (float scale, Vec2 a);
M3D_DEF Vec2  operator * (Vec2 a, float scale);
M3D_DEF Vec2  operator / (Vec2 a, float scale);
M3D_DEF Vec2& operator += (Vec2 &a, Vec2 b);
M3D_DEF Vec2& operator *= (Vec2 &a, float scale);
M3D_DEF Vec2  hadamard(Vec2 a, Vec2 b);
M3D_DEF float dot(Vec2 a, Vec2 b);
M3D_DEF float len_sq(Vec2 v);
M3D_DEF float length(Vec2 v);
M3D_DEF Vec2  normalize(Vec2 v);


M3D_DEF Vec3  vec3(float x, float y, float z);
M3D_DEF Vec3  vec3(Vec2 v2);
M3D_DEF Vec3  vec3(Vec2 v2, float z);
M3D_DEF Vec3  operator - (Vec3 a);
M3D_DEF Vec3  operator - (Vec3 a, Vec3 b);
M3D_DEF Vec3  operator + (Vec3 a, Vec3 b);
M3D_DEF Vec3  operator * (float scale, Vec3 a);
M3D_DEF Vec3  operator * (Vec3 a, float scale);
M3D_DEF Vec3  operator / (Vec3 a, float scale);
M3D_DEF Vec3& operator += (Vec3 &a, Vec3 b);
M3D_DEF Vec3& operator *= (Vec3 &a, float scale);
M3D_DEF Vec3  hadamard(Vec3 a, Vec3 b);
M3D_DEF float dot(Vec3 a, Vec3 b);
M3D_DEF Vec3  cross(Vec3 a, Vec3 b);
M3D_DEF float len_sq(Vec3 v);
M3D_DEF float length(Vec3 v);
M3D_DEF Vec3  normalize(Vec3 v);


M3D_DEF Vec4  vec4(float x, float y, float z, float w);
M3D_DEF Vec4  vec4(Vec2 v2);
M3D_DEF Vec4  vec4(Vec2 v2, float z, float w);
M3D_DEF Vec4  vec4(Vec3 v3);
M3D_DEF Vec4  vec4(Vec3 v3, float w);
M3D_DEF Vec4  operator - (Vec4 a);
M3D_DEF Vec4  operator - (Vec4 a, Vec4 b);
M3D_DEF Vec4  operator + (Vec4 a, Vec4 b);
M3D_DEF Vec4  operator * (float scale, Vec4 a);
M3D_DEF Vec4  operator * (Vec4 a, float scale);
M3D_DEF Vec4  operator / (Vec4 a, float scale);
M3D_DEF Vec4& operator += (Vec4 &a, Vec4 b);
M3D_DEF Vec4& operator *= (Vec4 &a, float scale);
M3D_DEF Vec4  hadamard(Vec4 a, Vec4 b);
M3D_DEF float dot(Vec4 a, Vec4 b);
M3D_DEF float len_sq(Vec4 v);
M3D_DEF float length(Vec4 v);
M3D_DEF Vec4  normalize(Vec4 v);


struct Mat4 {
    float data[16];

    float at(int row, int col) const {
        return data[(col * 4) + row];
    }

    float & at(int row, int col) {
        return data[(col * 4) + row];
    }
};

M3D_DEF Mat4 identity();
M3D_DEF Mat4 orthoGL(float left, float right, float top, float bottom, float near, float far);
M3D_DEF Mat4 perspectiveGL(float left, float right, float top, float bottom, float near, float far);
M3D_DEF Mat4 perspectiveGL(float horizontal_fov, float aspect_ht_over_wd, float near, float far);
M3D_DEF Mat4 translate(float x, float y, float z);
M3D_DEF Mat4 rotation(float angle, Vec3 axis);
M3D_DEF Mat4 scale(float x, float y, float z);
M3D_DEF Mat4 inverse(Mat4 const &A, bool *isInvertible = nullptr);
M3D_DEF Mat4 operator + (Mat4 const &A, Mat4 const &B);
M3D_DEF Mat4 operator * (Mat4 const &A, Mat4 const &B);
M3D_DEF Vec4 operator * (Mat4 const &A, Vec4 const &b);


M3D_DEF float to_radians(float angle_in_degrees);
M3D_DEF float clamp(float val, float a, float b);

M3D_DEF float min_of(float a, float b);
M3D_DEF Vec2  min_of(Vec2 a, Vec2 b);
M3D_DEF float max_of(float a, float b);
M3D_DEF Vec2  max_of(Vec2 a, Vec2 b);

M3D_DEF float lerp(float t, float a, float b);
M3D_DEF Vec2  lerp(float t, Vec2 a, Vec2 b);
M3D_DEF Vec3  lerp(float t, Vec3 a, Vec3 b);
M3D_DEF Vec4  lerp(float t, Vec4 a, Vec4 b);

#endif // __GUARD_MATH3D_H__


#if defined(M3D_IMPLEMENTATION)


#if !defined(M3D_INVERSE_MATRIX_EPSILON)
    #define M3D_INVERSE_MATRIX_EPSILON 0.00001f
#endif

#if !defined(M3D_DO_NOT_USE_C_MATH_LIB)
    #include <math.h>

    #define M3D_COSF  cosf
    #define M3D_SINF  sinf
    #define M3D_TANF  tanf
    #define M3D_FABSF fabsf
    #define M3D_SQRTF sqrtf
#endif

const float M3D_PI           = 3.14159265359f;
const float M3D_PI_DEG_RATIO = M3D_PI / 180.0f;


/**** BEGIN Miscellaneous definitions ****/

inline M3D_DEF float min_of(float a, float b) { return a < b ? a : b; }
inline M3D_DEF float max_of(float a, float b) { return a > b ? a : b; }

inline M3D_DEF Vec2 min_of(Vec2 a, Vec2 b)
{
    return vec2(min_of(a.x, b.x), min_of(a.y, b.y));
}

inline M3D_DEF Vec2 max_of(Vec2 a, Vec2 b)
{
    return vec2(max_of(a.x, b.x), max_of(a.y, b.y));
}

inline M3D_DEF float to_radians(float angle_in_deg)
{
    return angle_in_deg * M3D_PI_DEG_RATIO;
}

inline M3D_DEF float clamp(float val, float a, float b)
{
    return min_of(max_of(val, a), b);
}

inline M3D_DEF float lerp(float t, float a, float b) { return ((1 - t) * a) + (t * b); }
inline M3D_DEF Vec2  lerp(float t, Vec2 a, Vec2 b)   { return ((1 - t) * a) + (t * b); }
inline M3D_DEF Vec3  lerp(float t, Vec3 a, Vec3 b)   { return ((1 - t) * a) + (t * b); }
inline M3D_DEF Vec4  lerp(float t, Vec4 a, Vec4 b)   { return ((1 - t) * a) + (t * b); }


/**** END Miscellaneous definitions ****/


/**** BEGIN Mat4 definitions ****/
inline M3D_DEF Mat4 identity()
{
    return Mat4 {
        {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        }
    };
}

M3D_DEF Mat4 orthoGL(float l, float r, float t, float b, float n, float f)
{
    Mat4 result = Mat4{};

    // Main diagonal
    result.at(0, 0) = 2.0f / (r - l);
    result.at(1, 1) = 2.0f / (t - b);
    result.at(2, 2) = -2.0f / (f - n);

    // column 4
    result.at(0, 3) = -(r + l) / (r - l);
    result.at(1, 3) = -(t + b) / (t - b);
    result.at(2, 3) = -(f + n) / (f - n);
    result.at(3, 3) = 1.0f;
    
    return result;
}

M3D_DEF Mat4 perspectiveGL(float l, float r, float t, float b, float n, float f)
{
    Mat4 result = Mat4 {};

    // x/y diagonal
    result.at(0, 0) = (2*n) / (r - l);
    result.at(1, 1) = (2*n) / (t - b);
    
    // column 3
    result.at(0, 2) = (r + l) / (r - l);
    result.at(1, 2) = (t + b) / (t - b);
    result.at(2, 2) = -(f + n) / (f - n);
    result.at(3, 2) = -1.0f;

    // column 4
    result.at(2, 3) = (-2*f*n) / (f - n);
    result.at(3, 3) = 0.0f;

    return result;;
}

M3D_DEF Mat4 perspectiveGL(float horiz_fov, float aspect_ht_over_wd, float n, float f)
{
    Mat4  result = identity();
    float wd     = M3D_TANF(to_radians(horiz_fov / 2.0f));

    // x/y diagonal
    result.at(0, 0) = 1.0f / wd;
    result.at(1, 1) = 1.0f / (wd * aspect_ht_over_wd);
    
    // column 3
    result.at(2, 2) = -(f + n) / (f - n);
    result.at(3, 2) = -1.0f;

    // column 4
    result.at(2, 3) = (-2*f*n) / (f - n);
    result.at(3, 3) = 0.0f;
    
    return result;
}

inline M3D_DEF Mat4 translate(float x, float y, float z)
{
    Mat4 result = identity();

    result.at(0, 3) = x;
    result.at(1, 3) = y;
    result.at(2, 3) = z;

    return result;
}

M3D_DEF Mat4 rotation(float angle, Vec3 axis)
{
    axis = normalize(axis);
    
    Mat4  R  = identity();
    float ng = to_radians(angle);
    float c  = M3D_COSF(ng);
    float s  = M3D_SINF(ng);
    float x  = axis.x;
    float y  = axis.y;
    float z  = axis.z;

    // column 1
    R.at(0,0) = c + (1 - c) * x*x;
    R.at(1,0) = (1 - c) * x*y + s*z;
    R.at(2,0) = (1 - c) * x*z - s*y;

    // column 2
    R.at(0,1) = (1 - c) * x*y - s*z;
    R.at(1,1) = c + (1 - c) * y*y;
    R.at(2,1) = (1 - c) * y*z + s*x;

    // column 3
    R.at(0,2)  = (1 - c) * x*z + s*y;
    R.at(1,2)  = (1 - c) * y*z - s*x;
    R.at(2,2) = c + (1 - c) * z*z;

    return R;
}

inline M3D_DEF Mat4 scale(float x, float y, float z)
{
    Mat4 result = identity();

    result.at(0,0) = x;
    result.at(1,1) = y;
    result.at(2,2) = z;

    return result;
}

M3D_DEF Mat4 inverse(Mat4 const &A, bool *isInvertible)
{
    Mat4 inv;

    inv.at(0,0) = ((A.at(1,1) * A.at(2,2) * A.at(3,3)) - 
                   (A.at(1,1) * A.at(3,2) * A.at(2,3)) - 
                   (A.at(1,2) * A.at(2,1) * A.at(3,3)) + 
                   (A.at(1,2) * A.at(3,1) * A.at(2,3)) +
                   (A.at(1,3) * A.at(2,1) * A.at(3,2)) - 
                   (A.at(1,3) * A.at(3,1) * A.at(2,2)));

    inv.at(0,1) = ((-A.at(0,1) * A.at(2,2) * A.at(3,3)) + 
                   (A.at(0,1) * A.at(3,2) * A.at(2,3)) + 
                   (A.at(0,2) * A.at(2,1) * A.at(3,3)) - 
                   (A.at(0,2) * A.at(3,1) * A.at(2,3)) - 
                   (A.at(0,3) * A.at(2,1) * A.at(3,2)) + 
                   (A.at(0,3) * A.at(3,1) * A.at(2,2)));

    inv.at(0,2) = ((A.at(0,1) * A.at(1,2) * A.at(3,3)) - 
                   (A.at(0,1) * A.at(3,2) * A.at(1,3)) - 
                   (A.at(0,2) * A.at(1,1) * A.at(3,3)) + 
                   (A.at(0,2) * A.at(3,1) * A.at(1,3)) + 
                   (A.at(0,3) * A.at(1,1) * A.at(3,2)) - 
                   (A.at(0,3) * A.at(3,1) * A.at(1,2)));

    inv.at(0,3) = ((-A.at(0,1) * A.at(1,2) * A.at(2,3)) + 
                   (A.at(0,1) * A.at(2,2) * A.at(1,3)) +
                   (A.at(0,2) * A.at(1,1) * A.at(2,3)) - 
                   (A.at(0,2) * A.at(2,1) * A.at(1,3)) - 
                   (A.at(0,3) * A.at(1,1) * A.at(2,2)) + 
                   (A.at(0,3) * A.at(2,1) * A.at(1,2)));

    inv.at(1,0) = ((-A.at(1,0) * A.at(2,2) * A.at(3,3)) + 
                   (A.at(1,0) * A.at(3,2) * A.at(2,3)) + 
                   (A.at(1,2) * A.at(2,0) * A.at(3,3)) - 
                   (A.at(1,2) * A.at(3,0) * A.at(2,3)) - 
                   (A.at(1,3) * A.at(2,0) * A.at(3,2)) + 
                   (A.at(1,3) * A.at(3,0) * A.at(2,2)));

    inv.at(1,1) = ((A.at(0,0) * A.at(2,2) * A.at(3,3)) - 
                   (A.at(0,0) * A.at(3,2) * A.at(2,3)) - 
                   (A.at(0,2) * A.at(2,0) * A.at(3,3)) + 
                   (A.at(0,2) * A.at(3,0) * A.at(2,3)) + 
                   (A.at(0,3) * A.at(2,0) * A.at(3,2)) - 
                   (A.at(0,3) * A.at(3,0) * A.at(2,2)));

    inv.at(1,2) = ((-A.at(0,0) * A.at(1,2) * A.at(3,3)) + 
                   (A.at(0,0) * A.at(3,2) * A.at(1,3)) + 
                   (A.at(0,2) * A.at(1,0) * A.at(3,3)) - 
                   (A.at(0,2) * A.at(3,0) * A.at(1,3)) - 
                   (A.at(0,3) * A.at(1,0) * A.at(3,2)) + 
                   (A.at(0,3) * A.at(3,0) * A.at(1,2)));

    inv.at(1,3) = ((A.at(0,0) * A.at(1,2) * A.at(2,3)) - 
                   (A.at(0,0) * A.at(2,2) * A.at(1,3)) - 
                   (A.at(0,2) * A.at(1,0) * A.at(2,3)) + 
                   (A.at(0,2) * A.at(2,0) * A.at(1,3)) + 
                   (A.at(0,3) * A.at(1,0) * A.at(2,2)) - 
                   (A.at(0,3) * A.at(2,0) * A.at(1,2)));

    inv.at(2,0) = ((A.at(1,0) * A.at(2,1) * A.at(3,3)) - 
                   (A.at(1,0) * A.at(3,1) * A.at(2,3)) - 
                   (A.at(1,1) * A.at(2,0) * A.at(3,3)) + 
                   (A.at(1,1) * A.at(3,0) * A.at(2,3)) + 
                   (A.at(1,3) * A.at(2,0) * A.at(3,1)) - 
                   (A.at(1,3) * A.at(3,0) * A.at(2,1)));

    inv.at(2,1) = ((-A.at(0,0) * A.at(2,1) * A.at(3,3)) + 
                   (A.at(0,0) * A.at(3,1) * A.at(2,3)) + 
                   (A.at(0,1) * A.at(2,0) * A.at(3,3)) - 
                   (A.at(0,1) * A.at(3,0) * A.at(2,3)) - 
                   (A.at(0,3) * A.at(2,0) * A.at(3,1)) + 
                   (A.at(0,3) * A.at(3,0) * A.at(2,1)));

    inv.at(2,2) = ((A.at(0,0) * A.at(1,1) * A.at(3,3)) - 
                   (A.at(0,0) * A.at(3,1) * A.at(1,3)) - 
                   (A.at(0,1) * A.at(1,0) * A.at(3,3)) + 
                   (A.at(0,1) * A.at(3,0) * A.at(1,3)) + 
                   (A.at(0,3) * A.at(1,0) * A.at(3,1)) - 
                   (A.at(0,3) * A.at(3,0) * A.at(1,1)));

    inv.at(2,3) = ((-A.at(0,0) * A.at(1,1) * A.at(2,3)) + 
                   (A.at(0,0) * A.at(2,1) * A.at(1,3)) + 
                   (A.at(0,1) * A.at(1,0) * A.at(2,3)) - 
                   (A.at(0,1) * A.at(2,0) * A.at(1,3)) - 
                   (A.at(0,3) * A.at(1,0) * A.at(2,1)) + 
                   (A.at(0,3) * A.at(2,0) * A.at(1,1)));

    inv.at(3,0) = ((-A.at(1,0) * A.at(2,1) * A.at(3,2)) + 
                   (A.at(1,0) * A.at(3,1) * A.at(2,2)) + 
                   (A.at(1,1) * A.at(2,0) * A.at(3,2)) - 
                   (A.at(1,1) * A.at(3,0) * A.at(2,2)) - 
                   (A.at(1,2) * A.at(2,0) * A.at(3,1)) + 
                   (A.at(1,2) * A.at(3,0) * A.at(2,1)));

    inv.at(3,1) = ((A.at(0,0) * A.at(2,1) * A.at(3,2)) - 
                   (A.at(0,0) * A.at(3,1) * A.at(2,2)) - 
                   (A.at(0,1) * A.at(2,0) * A.at(3,2)) + 
                   (A.at(0,1) * A.at(3,0) * A.at(2,2)) + 
                   (A.at(0,2) * A.at(2,0) * A.at(3,1)) - 
                   (A.at(0,2) * A.at(3,0) * A.at(2,1)));

    inv.at(3,2) = ((-A.at(0,0) * A.at(1,1) * A.at(3,2)) + 
                   (A.at(0,0) * A.at(3,1) * A.at(1,2)) + 
                   (A.at(0,1) * A.at(1,0) * A.at(3,2)) - 
                   (A.at(0,1) * A.at(3,0) * A.at(1,2)) - 
                   (A.at(0,2) * A.at(1,0) * A.at(3,1)) + 
                   (A.at(0,2) * A.at(3,0) * A.at(1,1)));

    inv.at(3,3) = ((A.at(0,0) * A.at(1,1) * A.at(2,2)) - 
                   (A.at(0,0) * A.at(2,1) * A.at(1,2)) - 
                   (A.at(0,1) * A.at(1,0) * A.at(2,2)) + 
                   (A.at(0,1) * A.at(2,0) * A.at(1,2)) + 
                   (A.at(0,2) * A.at(1,0) * A.at(2,1)) - 
                   (A.at(0,2) * A.at(2,0) * A.at(1,1)));

    float det = ((A.at(0,0) * inv.at(0,0)) +
                 (A.at(1,0) * inv.at(0,1)) +
                 (A.at(2,0) * inv.at(0,2)) +
                 (A.at(3,0) * inv.at(0,3)));

    if (M3D_FABSF(det) < M3D_INVERSE_MATRIX_EPSILON) {
        if (isInvertible != nullptr)
            *isInvertible = false;
    }
    else {
        if (isInvertible != nullptr)
            *isInvertible = true;

        det = 1.0f / det;

        for (int c = 0; c < 4; ++c)
            for (int r = 0; r < 4; ++r)
                inv.at(r, c) *= det;
    }

    return inv;
}

M3D_DEF Mat4 operator + (Mat4 const &A, Mat4 const &B)
{
    Mat4 result = Mat4 {};

    result.at(0,0) = A.at(0,0) + B.at(0,0);
    result.at(1,0) = A.at(1,0) + B.at(1,0);
    result.at(2,0) = A.at(2,0) + B.at(2,0);
    result.at(3,0) = A.at(3,0) + B.at(3,0);
    result.at(0,1) = A.at(0,1) + B.at(0,1);
    result.at(1,1) = A.at(1,1) + B.at(1,1);
    result.at(2,1) = A.at(2,1) + B.at(2,1);
    result.at(3,1) = A.at(3,1) + B.at(3,1);
    result.at(0,2) = A.at(0,2) + B.at(0,2);
    result.at(1,2) = A.at(1,2) + B.at(1,2);
    result.at(2,2) = A.at(2,2) + B.at(2,2);
    result.at(3,2) = A.at(3,2) + B.at(3,2);
    result.at(0,3) = A.at(0,3) + B.at(0,3);
    result.at(1,3) = A.at(1,3) + B.at(1,3);
    result.at(2,3) = A.at(2,3) + B.at(2,3);
    result.at(3,3) = A.at(3,3) + B.at(3,3);

    return result;
}

M3D_DEF Mat4 operator * (Mat4 const  &A, Mat4 const &B)
{
    Mat4 result = Mat4 {};

    for (int c = 0; c < 4; ++c) {
        for (int r = 0; r < 4; ++r) {
            result.at(r, c) = ((A.at(r, 0) * B.at(0, c)) +
                               (A.at(r, 1) * B.at(1, c)) +
                               (A.at(r, 2) * B.at(2, c)) +
                               (A.at(r, 3) * B.at(3, c)));
        }
    }

    return result;
}

M3D_DEF Vec4 operator * (Mat4 const &A, Vec4 const &b) {
    Vec4 result = Vec4 {};

    for (int r = 0; r < 4; ++r) {
        result[r] = ((A.at(r, 0) * b.x) +
                     (A.at(r, 1) * b.y) +
                     (A.at(r, 2) * b.z) +
                     (A.at(r, 3) * b.w));
    }

    return result;
}
/**** END Mat4 definitions ****/


/**** BEGIN Vec2 definitions ****/
inline M3D_DEF Vec2 vec2(float x, float y)
{
    return Vec2 { x, y };
}

inline M3D_DEF Vec2 operator - (Vec2 a)
{
    return Vec2 { -a.x, -a.y };
}

inline M3D_DEF Vec2 operator - (Vec2 a, Vec2 b)
{
    return Vec2 { a.x - b.x, a.y - b.y };
}

inline M3D_DEF Vec2 operator + (Vec2 a, Vec2 b)
{
    return Vec2 { a.x + b.x, a.y + b.y };
}

inline M3D_DEF Vec2 operator * (float scale, Vec2 a)
{
    return Vec2 { scale * a.x, scale * a.y };
}

inline M3D_DEF Vec2 operator * (Vec2 a, float scale)
{
    return scale * a;
}

inline M3D_DEF Vec2 operator / (Vec2 a, float scale)
{
    return a * (1.0f / scale);
}

inline M3D_DEF Vec2& operator += (Vec2 &a, Vec2 b)
{
    a = a + b;
    return a;
}

inline M3D_DEF Vec2& operator *= (Vec2 &a, float scale)
{
    a.x *= scale;
    a.y *= scale;
    return a;
}

inline M3D_DEF Vec2 hadamard(Vec2 a, Vec2 b)
{
    return Vec2{ a.x*b.x, a.y*b.y };
}

inline M3D_DEF float dot(Vec2 a, Vec2 b)
{
    return a.x*b.x + a.y*b.y;
}

inline M3D_DEF float len_sq(Vec2 v)
{
    return dot(v, v);
}

inline M3D_DEF float length(Vec2 v)
{
    return M3D_SQRTF(len_sq(v));
}

inline M3D_DEF Vec2 normalize(Vec2 v)
{
    Vec2  r   = Vec2{};
    float len = length(v);

    r.x = v.x / len;
    r.y = v.y / len;

    return r;
}
/**** END Vec2 definitions ****/


/**** BEGIN Vec3 definitions ****/
inline M3D_DEF Vec3 vec3(float x, float y, float z)
{    return Vec3 { x, y, z };
}

inline M3D_DEF Vec3 vec3(Vec2 v2)
{
    return vec3(v2, 0);
}

inline M3D_DEF Vec3 vec3(Vec2 v2, float z)
{
    return Vec3 { v2.x, v2.y, z };
}

inline M3D_DEF Vec3 operator - (Vec3 a)
{
    return Vec3 { -a.x, -a.y, -a.z };
}

inline M3D_DEF Vec3 operator - (Vec3 a, Vec3 b)
{
    return Vec3 {a.x - b.x, a.y - b.y, a.z - b.z };
}

inline M3D_DEF Vec3 operator + (Vec3 a, Vec3 b)
{
    return Vec3 { a.x + b.x, a.y + b.y, a.z + b.z };
}

inline M3D_DEF Vec3 operator * (float scale, Vec3 a)
{
    return Vec3 { scale * a.x, scale * a.y, scale * a.z };
}

inline M3D_DEF Vec3 operator * (Vec3 a, float scale)
{
    return scale * a;
}

inline M3D_DEF Vec3 operator / (Vec3 a, float scale)
{
    return a * (1.0f / scale);
}

inline M3D_DEF Vec3& operator += (Vec3 &a, Vec3 b)
{
    a = a + b;
    return a;
}

inline M3D_DEF Vec3& operator *= (Vec3 &a, float scale)
{
    a.x *= scale;
    a.y *= scale;
    a.z *= scale;
    return a;
}

inline M3D_DEF Vec3 hadamard(Vec3 a, Vec3 b)
{
    return Vec3 { a.x*b.x, a.y*b.y, a.z*b.z };
}

inline M3D_DEF float dot(Vec3 a, Vec3 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

inline M3D_DEF Vec3 cross(Vec3 a, Vec3 b)
{
    return Vec3 {
        a.y*b.z - a.z*b.y,
        a.z*b.x - a.x*b.z,
        a.x*b.y - a.y*b.x,
    };
}

inline M3D_DEF float len_sq(Vec3 v)
{
    return dot(v, v);
}

inline M3D_DEF float length(Vec3 v)
{
    return M3D_SQRTF(len_sq(v));
}

inline M3D_DEF Vec3 normalize(Vec3 v)
{
    Vec3  r   = Vec3{};
    float len = length(v);

    r.x = v.x / len;
    r.y = v.y / len;
    r.z = v.z / len;

    return r;
}
/**** END Vec3 definitions ****/


/**** BEGIN Vec4 definitions ****/
inline M3D_DEF Vec4 vec4(float x, float y, float z, float w)
{
    return Vec4 { x, y, z, w };
}

inline M3D_DEF Vec4 vec4(Vec2 v2)
{
    return Vec4 { v2.x, v2.y, 0, 0 };
}

inline M3D_DEF Vec4 vec4(Vec2 v2, float z, float w)
{
    return Vec4 { v2.x, v2.y, z, w };
}

inline M3D_DEF Vec4 vec4(Vec3 v3)
{
    return Vec4 { v3.x, v3.y, v3.z, 0 };
}

inline M3D_DEF Vec4 vec4(Vec3 v3, float w)
{
    return Vec4 { v3.x, v3.y, v3.z, w };
}

inline M3D_DEF Vec4 operator - (Vec4 a)
{
    return Vec4 { -a.x, -a.y, -a.z, -a.w };
}

inline M3D_DEF Vec4 operator - (Vec4 a, Vec4 b)
{
    return Vec4 { a.x - b.x, a.y - b.y, a.z - b.z, a.w - b.w };
}

inline M3D_DEF Vec4 operator + (Vec4 a, Vec4 b)
{
    return Vec4 { a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w };
}

inline M3D_DEF Vec4 operator * (float scale, Vec4 a)
{
    return Vec4 { scale * a.x, scale * a.y, scale * a.z, scale * a.w };
}

inline M3D_DEF Vec4 operator * (Vec4 a, float scale)
{
    return Vec4 { scale * a.x, scale * a.y, scale * a.z, scale * a.w };
}

inline M3D_DEF Vec4 operator / (Vec4 a, float scale)
{
    return a * (1.0f / scale);
}

inline M3D_DEF Vec4& operator += (Vec4 &a, Vec4 b)
{
    a = a + b;
    return a;
}

inline M3D_DEF Vec4& operator *= (Vec4 &a, float scale)
{
    a.x *= scale;
    a.y *= scale;
    a.z *= scale;
    a.w *= scale;
    return a;
}

inline M3D_DEF Vec4 hadamard(Vec4 a, Vec4 b)
{
    return Vec4 { a.x*b.x, a.y*b.y, a.z*b.z, a.w*b.w };
}

inline M3D_DEF float dot(Vec4 a, Vec4 b)
{
    return a.x*b.x + a.y*b.y + a.z*b.z + a.w*b.w;
}

inline M3D_DEF float len_sq(Vec4 v)
{
    return dot(v, v);
}

inline M3D_DEF float length(Vec4 v)
{
    return M3D_SQRTF(len_sq(v));
}

inline M3D_DEF Vec4 normalize(Vec4 v)
{
    Vec4  r   = Vec4{};
    float len = length(v);

    r.x = v.x / len;
    r.y = v.y / len;
    r.z = v.z / len;
    r.w = v.w / len;

    return r;
}
/**** END Vec4 definitions ****/

#endif // M3D_IMPLEMENTATION
#undef M3D_IMPLEMENTATION
