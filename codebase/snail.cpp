// TODO: int2,3,4 should have functions SnailIVec

// { snail.h }                           .----.   @   @                         
//                                      / .-"-.`.  \v/                          
//                                      | | '\ \ \_/ )                          
//  a smol and slow linalg library    ,-\ `-.' /.'  /                           
//  ---------------------------------'---`----'----'            james-bern 2022 
//                                                                              
// WARNING_____________________________________________________________________ 
//                                                                              
// i expect strict-aliasing optimizations will break this                       
//                                                                              
// OVERVIEW____________________________________________________________________ 
//                                                                              
// vec2, vec3, and vec4 are   2-,   3-, and   4-vectors  respectively           
// mat2, mat3, and mat4 are 2x2-, 3x3-, and 4x4-matrices respectively           
//                                                                              
// "CONSTRUCTORS"______________________________________________________________ 
//                                                                              
// vectors and matrices are plain old data                                      
//                                                                              
// this code is valid C/C++                                                     
// vec3 v = { 1.0, 2.0, 3.0 };                                                  
// unforunately, this code is NOT                                               
// vec3 v = 2.0 * { 1.0, 2.0, 3.0 }; // will NOT compile                        
//                                                                              
// the typical "constructors" are                                               
// V2(real x, real y);                                                          
// V3(real x, real y, real z);                                                  
// V4(real x, real y, real z, real w);                                          
//                                                                              
// they enable code like this                                                   
// vec3 v = 2.0 * V3(1.0, 2.0, 3.0);                                            
//                                                                              
// reasonable rule of thumb: just use V2, V3, V4 every time you make a vector   
// (i.e. never use the curly brace plain old data "constructor" syntax)         
// vec3 v = V3(1.0, 2.0, 3.0);                                                  
// vec3 v = 2.0 * V3(1.0, 2.0, 3.0);                                            
//                                                                              
// there are also less commonly used constructors!                              
//                                                                              
// V4(vec2 xy, real z); // loft a point from 2D -> 3D                           
// V4(vec3 xyz, real w); // loft a color from RGB -> RGBA                       
//                                                                              
// these are nice when porting from GLSL                                        
// V2(real x); // V2(x, x)                                                      
// V3(real x); // V3(x, x, x)                                                   
// V4(real x); // V4(x, x, x, x)                                                
//                                                                              
// these are nice is you're using V2 and friends everywhere                     
// M2(real xx, real xy, real yx, real yy);                                      
// M3(...);                                                                     
// M4(...);                                                                     
//                                                                              
// ACCESSORS___________________________________________________________________ 
//                                                                              
// the data of vector v can be accessed in multiple ways                        
// (but it is still the same plain old data)                                    
// -   v.x,  v.y,  v.y,  v.w                                                    
// -  v[0], v[1], v[2], v[3]                                                    
//                                                                              
// the data of TxT matrix M can be accessed with convenience function M(r, c)   
// - or, if you prefer, M.data[T * r + c]                                       
//                                                                              
// with cheat codes enabled (#define SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD) 
// you can also access vectors like this                                        
// -   v.r,  v.g,  v.b,  v.a                                                    
// -  v.xy                                                                      
// - v.xyz                                                                      
// - v.data[0], v.data[1], ...                                                  
//                                                                              
// FUNCTIONS___________________________________________________________________ 
//                                                                              
// inverse(M) returns the inverse of M, determinant(M) returns determinant...   
// e.g., solving a linear system A x = b                                        
//       mat3 A = M3(1.0, 1.0, -1.0, 2.0, 1.0, 3.0, 4.0, -1.0, 2.0);            
//       vec3 b = V3(5.0, 2.0, -1.0);                                           
//       vec3 x = inverse(A) * b;                                               
//       pprint(x);                                                             
//                                                                              
// ARITHMETIC OPERATOR OVERLOADING_____________________________________________ 
//                                                                              
// the typical arithmetic operators are overloaded                              
//                                                                              
// e.g., some random math                                                       
//       mat3 A = M3(1.0, 1.0, -1.0, 2.0, 1.0, 3.0, 4.0, -1.0, 2.0);            
//       vec3 b = V3(5.0, 2.0, -1.0);                                           
//       b += -(A * b) / 2;                                                     
//                                                                              
// NOTE! snail.cpp has no explicit notion of column vectors vs. row vectors     
//       the distinction is made only when necessary (and according to context) 
//                                                                              
// if M is a matrix and v is a vector then                                      
// - M * v --> $  M v$ (matrix times a column vector)                           
// - v * M --> $v^T M$ (row vector time a matrix)                               
//                                                                              
// NOTE! the * operator is NOT overloaded for vector-vector multpilication      
//       instead explicitly ask for the type of product you want                
// -          dot(a, b) --> $a^T b$ (inner product)                             
// -        outer(a, b) --> $a b^T$ (outer product)                             
// - cwiseProduct(a, b) -->         (cwise product)                             

#ifndef SNAIL_CPP
#define SNAIL_CPP

// fornow: these here too  ////////////////////////////////////////////////////

union int2 {
    struct { int i, j; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    int data[2];
    #endif
    int &operator [](int index) { return ((int *)(this))[index]; }
};

bool operator == (const int2 &a, const int2 &b) {
    return ((a.i == b.i) && (a.j == b.j));
}

union int3 {
    struct { int i, j, k; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    int data[3];
    #endif
    int &operator [](int index) { return ((int *)(this))[index]; }
};

bool operator == (const int3 &a, const int3 &b) {
    return ((a.i == b.i) && (a.j == b.j) && (a.k == b.k));
}

union int4 {
    struct { int i, j, k, l; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    int data[4];
    #endif
    int &operator [](int index) { return ((int *)(this))[index]; }
};

// dependencies ////////////////////////////////////////////////////////////////

#define _CRT_SECURE_NO_WARNINGS
#include <cstdio>
#include <cmath>
#include <cstring>

// internal macros /////////////////////////////////////////////////////////////

#define SNAIL_ASSERT(b) do { if (!(b)) { \
    printf("ASSERT Line %d in %s\n", __LINE__, __FILE__); \
    printf("press Enter to crash"); getchar(); \
    *((volatile int *) 0) = 0; \
} } while (0)

#define SNAIL_ABS(a) ((a) < 0 ? -(a) : (a))
#define SNAIL_MIN(a, b) ((a) < (b) ? (a) : (b))
#define SNAIL_MAX(a, b) ((a) > (b) ? (a) : (b))
#define SNAIL_CLAMP(t, a, b) SNAIL_MIN(SNAIL_MAX(t, a), b)

#define SNAIL_FOR_(i, N) for (int i = 0; i < N; ++i)

// vectors and matrices ////////////////////////////////////////////////////////

template <int T> union SnailVector {
    real data[T];
    real &operator [](int index) { return ((real *)(this))[index]; }
};

template <int T> union SnailMatrix {
    real data[T * T];
    real &operator ()(int r, int c) { return data[T * r + c]; }
    const real &operator ()(int r, int c) const { return data[T * r + c]; }
};

// sugary accessors ////////////////////////////////////////////////////////////

template <> union SnailVector<2> {
    struct { real x, y; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    real data[2];
    #endif
    real &operator [](int index) { return ((real *)(this))[index]; }
};
template <> union SnailVector<3> {
    struct { real x, y, z; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    struct { real r, g, b; };
    struct { SnailVector<2> xy; real _; };
    real data[3];
    #endif
    real &operator [](int index) { return ((real *)(this))[index]; }
};
template <> union SnailVector<4> {
    struct { real x, y, z, w; };
    #ifdef SNAIL_I_SOLEMNLY_SWEAR_I_AM_UP_TO_NO_GOOD
    struct { real r, g, b, a; };
    struct { SnailVector<3> xyz; real _; };
    real data[4];
    #endif
    real &operator [](int index) { return ((real *)(this))[index]; }
};

// "constructors" //////////////////////////////////////////////////////////////

SnailVector<2> V2(real x, real y) { return { x, y }; }
SnailVector<3> V3(real x, real y, real z) { return { x, y, z }; }
SnailVector<4> V4(real x, real y, real z, real w) { return { x, y, z, w }; }
SnailVector<3> V3(SnailVector<2> xy, real z) { return { xy.x, xy.y, z }; }
SnailVector<4> V4(SnailVector<3> xyz, real w) { return { xyz.x, xyz.y, xyz.z, w }; }
SnailVector<2> V2(real x) { return { x, x }; }
SnailVector<3> V3(real x) { return { x, x, x }; }
SnailVector<4> V4(real x) { return { x, x, x, x }; }

SnailMatrix<2> M2(real a0, real a1, real a2, real a3) { SnailMatrix<2> ret = { a0, a1, a2, a3 }; return ret; }
SnailMatrix<3> M3(real a0, real a1, real a2, real a3, real a4, real a5, real a6, real a7, real a8) { SnailMatrix<3> ret = { a0, a1, a2, a3, a4, a5, a6, a7, a8 }; return ret; }
SnailMatrix<4> M4(real a0, real a1, real a2, real a3, real a4, real a5, real a6, real a7, real a8, real a9, real a10, real a11, real a12, real a13, real a14, real a15) { SnailMatrix<4> ret = { a0, a1, a2, a3, a4, a5, a6, a7, a8, a9, a10, a11, a12, a13, a14, a15 }; return ret; }

SnailMatrix<2> hstack(SnailVector<2> col0, SnailVector<2> col1) { return { col0.x, col1.x, col0.y, col1.y }; }
SnailMatrix<3> hstack(SnailVector<3> col0, SnailVector<3> col1, SnailVector<3> col2) { return { col0.x, col1.x, col2.x, col0.y, col1.y, col2.y, col0.z, col1.z, col2.z }; }
SnailMatrix<4> hstack(SnailVector<4> col0, SnailVector<4> col1, SnailVector<4> col2, SnailVector<4> col3) { return { col0.x, col1.x, col2.x, col3.x, col0.y, col1.y, col2.y, col3.y, col0.z, col1.z, col2.z, col3.z, col0.w, col1.w, col2.w, col3.w }; }

// short names /////////////////////////////////////////////////////////////////

// typedef SnailVector<2> vec2;
// typedef SnailVector<3> vec3;
// typedef SnailVector<4> vec4;
// typedef SnailMatrix<2> mat2;
// typedef SnailMatrix<3> mat3;
// typedef SnailMatrix<4> mat4;
typedef SnailVector<2> vec2;
typedef SnailVector<3> vec3;
typedef SnailVector<4> vec4;
typedef SnailMatrix<2> mat2;
typedef SnailMatrix<3> mat3;
typedef SnailMatrix<4> mat4;

// arithmetic operators ////////////////////////////////////////////////////////

// vectors
template <int T> SnailVector<T>  operator +  (SnailVector<T> A, SnailVector<T> B) {
    SnailVector<T> result;
    SNAIL_FOR_(i, T) {
        result[i] = A[i] + B[i];
    }
    return result;
}
template <int T> SnailVector<T> &operator += (SnailVector<T> &A, SnailVector<T> B) {
    A = A + B;
    return A;
}

template <int T> SnailVector<T>  operator -  (SnailVector<T> A, SnailVector<T> B) {
    SnailVector<T> result;
    SNAIL_FOR_(i, T) {
        result[i] = A[i] - B[i];
    }
    return result;
}
template <int T> SnailVector<T> &operator -= (SnailVector<T> &A, SnailVector<T> B) {
    A = A - B;
    return A;
}

template <int T> SnailVector<T>  operator *  (real scalar, SnailVector<T> v) {
    SnailVector<T> result;
    SNAIL_FOR_(i, T) {
        result[i]  = scalar * v[i];
    }
    return result;
}
template <int T> SnailVector<T>  operator *  (SnailVector<T> v, real scalar) {
    SnailVector<T> result = scalar * v;
    return result;
}
template <int T> SnailVector<T> &operator *= (SnailVector<T> &v, real scalar) {
    v = scalar * v;
    return v;
}
template <int T> SnailVector<T>  operator -  (SnailVector<T> v) {
    return -1 * v;
}

template <int T> SnailVector<T>  operator /  (SnailVector<T> v, real scalar) {
    SnailVector<T> result;
    SNAIL_FOR_(i, T) {
        result[i]  = v[i] / scalar;
    }
    return result;
}
template <int T> SnailVector<T> &operator /= (SnailVector<T> &v, real scalar) {
    v = v / scalar;
    return v;
}

// matrices
template <int T> SnailMatrix<T>  operator +  (SnailMatrix<T> A, SnailMatrix<T> B) {
    SnailMatrix<T> ret = {};
    SNAIL_FOR_(k, T * T) {
        ret.data[k] = A.data[k] + B.data[k];
    }
    return ret;
}
template <int T> SnailMatrix<T> &operator += (SnailMatrix<T> &A, SnailMatrix<T> B) {
    A = A + B;
    return A;
}

template <int T> SnailMatrix<T>  operator -  (SnailMatrix<T> A, SnailMatrix<T> B) {
    SnailMatrix<T> ret = {};
    SNAIL_FOR_(i, T * T) {
        ret.data[i] = A.data[i] - B.data[i];
    }
    return ret;
}
template <int T> SnailMatrix<T> &operator -= (SnailMatrix<T> &A, SnailMatrix<T> B) {
    A = A + B;
    return A;
}

template <int T> SnailMatrix<T>  operator *  (SnailMatrix<T> A, SnailMatrix<T> B) {
    SnailMatrix<T> ret = {};
    SNAIL_FOR_(r, T) {
        SNAIL_FOR_(c, T) {
            SNAIL_FOR_(i, T) {
                ret(r, c) += A(r, i) * B(i, c);
            }
        }
    }
    return ret;
}
template <int T> SnailMatrix<T> &operator *= (SnailMatrix<T> &A, SnailMatrix<T> B) {
    A = A * B;
    return A;
}
template <int T> SnailVector<T>  operator *  (SnailMatrix<T> A, SnailVector<T> b) { // A b
    SnailVector<T> ret = {};
    SNAIL_FOR_(r, T) {
        SNAIL_FOR_(c, T) {
            ret[r] += A(r, c) * b[c];
        }
    }
    return ret;
}
template <int T> SnailVector<T>  operator *  (SnailVector<T> b, SnailMatrix<T> A) { // b^T A
    SnailVector<T> ret = {};
    SNAIL_FOR_(r, T) {
        SNAIL_FOR_(c, T) {
            ret[r] += A(c, r) * b[c];
        }
    }
    return ret;
}
template <int T> SnailMatrix<T>  operator *  (real scalar, SnailMatrix<T> M) {
    SnailMatrix<T> result = {};
    SNAIL_FOR_(k, T * T) {
        result.data[k] = scalar * M.data[k];
    }
    return result;
}
template <int T> SnailMatrix<T>  operator *  (SnailMatrix<T> M, real scalar) {
    return scalar * M;
}
template <int T> SnailMatrix<T> &operator *= (SnailMatrix<T> &M, real scalar) {
    M = scalar * M;
    return M;
}
template <int T> SnailMatrix<T>  operator -  (SnailMatrix<T> M) {
    return -1 * M;
}

template <int T> SnailMatrix<T>  operator /  (SnailMatrix<T> M, real scalar) {
    return (1 / scalar) * M;
}
template <int T> SnailMatrix<T> &operator /= (SnailMatrix<T> &M, real scalar) {
    M = M / scalar;
    return M;
}

// important vector functions //////////////////////////////////////////////////

/**
 * u dot v TODO latex 
 */
template <int T> real dot(SnailVector<T> A, SnailVector<T> B) {
    real result = 0;
    for (int i = 0; i < T; ++i) {
        result += A[i] * B[i];
    }
    return result;
}
template <int T> SnailMatrix<T> outer(SnailVector<T> u, SnailVector<T> v) {
    SnailMatrix<T> ret = {};
    SNAIL_FOR_(r, T) {
        SNAIL_FOR_(c, T) {
            ret(r, c) = u[r] * v[c];
        }
    }
    return ret;
}

real cross(SnailVector<2> A, SnailVector<2> B) {
    return A.x * B.y - A.y * B.x;
}
SnailVector<3> cross(SnailVector<3> A, SnailVector<3> B) {
    return { A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x };
}

template <int T> real squaredNorm(SnailVector<T> v) {
    return dot(v, v);
}
template <int T> real norm(SnailVector<T> v) {
    return sqrt(squaredNorm(v));
}
template <int T> real sum(SnailVector<T> v) {
    real result = 0.0;
    SNAIL_FOR_(i, T) result += v[i];
    return result;
}
template <int T> SnailVector<T> normalized(SnailVector<T> v) {
    real norm_v = norm(v);
    // SNAIL_ASSERT(fabs(norm_v) > 1e-7);
    return (1 / norm_v) * v;
}

// ALIASES
// template <int T> real length(SnailVector<T> v) { return norm(v); }
// template <int T> real squared_length(SnailVector<T> v) { return squaredNorm(v); }

// important matrix functions //////////////////////////////////////////////////

template <int T> SnailMatrix<T> transpose(SnailMatrix<T> M) {
    SnailMatrix<T> ret = {};
    SNAIL_FOR_(r, T) {
        SNAIL_FOR_(c, T) {
            ret(r, c) = M(c, r);
        }
    }
    return ret;
}

real determinant(SnailMatrix<2> M) {
    return M(0, 0) * M(1, 1) - M(0, 1) * M(1, 0);
}
real determinant(SnailMatrix<3> M) {
    return M(0, 0) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2))
        - M(0, 1) * (M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0))
        + M(0, 2) * (M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0));
}
real determinant(SnailMatrix<4> M) {
    real A2323 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2);
    real A1323 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1);
    real A1223 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1);
    real A0323 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0);
    real A0223 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0);
    real A0123 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0);
    return M(0, 0) * ( M(1, 1) * A2323 - M(1, 2) * A1323 + M(1, 3) * A1223 ) 
        - M(0, 1) * ( M(1, 0) * A2323 - M(1, 2) * A0323 + M(1, 3) * A0223 ) 
        + M(0, 2) * ( M(1, 0) * A1323 - M(1, 1) * A0323 + M(1, 3) * A0123 ) 
        - M(0, 3) * ( M(1, 0) * A1223 - M(1, 1) * A0223 + M(1, 2) * A0123 ) ;
}

SnailMatrix<2> inverse(SnailMatrix<2> M) {
    real invdet = 1 / determinant(M);
    return { invdet * M(1, 1), 
        invdet * -M(0, 1), 
        invdet * -M(1, 0), 
        invdet * M(0, 0) };
}
SnailMatrix<3> inverse(SnailMatrix<3> M) {
    real invdet = 1 / determinant(M);
    return { invdet * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)),
        invdet * (M(0, 2) * M(2, 1) - M(0, 1) * M(2, 2)),
        invdet * (M(0, 1) * M(1, 2) - M(0, 2) * M(1, 1)),
        invdet * (M(1, 2) * M(2, 0) - M(1, 0) * M(2, 2)),
        invdet * (M(0, 0) * M(2, 2) - M(0, 2) * M(2, 0)),
        invdet * (M(1, 0) * M(0, 2) - M(0, 0) * M(1, 2)),
        invdet * (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1)),
        invdet * (M(2, 0) * M(0, 1) - M(0, 0) * M(2, 1)),
        invdet * (M(0, 0) * M(1, 1) - M(1, 0) * M(0, 1)) };
}
SnailMatrix<4> inverse(SnailMatrix<4> M) {
    real invdet = 1 / determinant(M);
    real A2323 = M(2, 2) * M(3, 3) - M(2, 3) * M(3, 2) ;
    real A1323 = M(2, 1) * M(3, 3) - M(2, 3) * M(3, 1) ;
    real A1223 = M(2, 1) * M(3, 2) - M(2, 2) * M(3, 1) ;
    real A0323 = M(2, 0) * M(3, 3) - M(2, 3) * M(3, 0) ;
    real A0223 = M(2, 0) * M(3, 2) - M(2, 2) * M(3, 0) ;
    real A0123 = M(2, 0) * M(3, 1) - M(2, 1) * M(3, 0) ;
    real A2313 = M(1, 2) * M(3, 3) - M(1, 3) * M(3, 2) ;
    real A1313 = M(1, 1) * M(3, 3) - M(1, 3) * M(3, 1) ;
    real A1213 = M(1, 1) * M(3, 2) - M(1, 2) * M(3, 1) ;
    real A2312 = M(1, 2) * M(2, 3) - M(1, 3) * M(2, 2) ;
    real A1312 = M(1, 1) * M(2, 3) - M(1, 3) * M(2, 1) ;
    real A1212 = M(1, 1) * M(2, 2) - M(1, 2) * M(2, 1) ;
    real A0313 = M(1, 0) * M(3, 3) - M(1, 3) * M(3, 0) ;
    real A0213 = M(1, 0) * M(3, 2) - M(1, 2) * M(3, 0) ;
    real A0312 = M(1, 0) * M(2, 3) - M(1, 3) * M(2, 0) ;
    real A0212 = M(1, 0) * M(2, 2) - M(1, 2) * M(2, 0) ;
    real A0113 = M(1, 0) * M(3, 1) - M(1, 1) * M(3, 0) ;
    real A0112 = M(1, 0) * M(2, 1) - M(1, 1) * M(2, 0) ;
    return { invdet * ( M(1, 1) * A2323 - M(1, 2) * A1323 + M(1, 3) * A1223 ),
        invdet * - ( M(0, 1) * A2323 - M(0, 2) * A1323 + M(0, 3) * A1223 ),
        invdet *   ( M(0, 1) * A2313 - M(0, 2) * A1313 + M(0, 3) * A1213 ),
        invdet * - ( M(0, 1) * A2312 - M(0, 2) * A1312 + M(0, 3) * A1212 ),
        invdet * - ( M(1, 0) * A2323 - M(1, 2) * A0323 + M(1, 3) * A0223 ),
        invdet *   ( M(0, 0) * A2323 - M(0, 2) * A0323 + M(0, 3) * A0223 ),
        invdet * - ( M(0, 0) * A2313 - M(0, 2) * A0313 + M(0, 3) * A0213 ),
        invdet *   ( M(0, 0) * A2312 - M(0, 2) * A0312 + M(0, 3) * A0212 ),
        invdet *   ( M(1, 0) * A1323 - M(1, 1) * A0323 + M(1, 3) * A0123 ),
        invdet * - ( M(0, 0) * A1323 - M(0, 1) * A0323 + M(0, 3) * A0123 ),
        invdet *   ( M(0, 0) * A1313 - M(0, 1) * A0313 + M(0, 3) * A0113 ),
        invdet * - ( M(0, 0) * A1312 - M(0, 1) * A0312 + M(0, 3) * A0112 ),
        invdet * - ( M(1, 0) * A1223 - M(1, 1) * A0223 + M(1, 2) * A0123 ),
        invdet *   ( M(0, 0) * A1223 - M(0, 1) * A0223 + M(0, 2) * A0123 ),
        invdet * - ( M(0, 0) * A1213 - M(0, 1) * A0213 + M(0, 2) * A0113 ),
        invdet *   ( M(0, 0) * A1212 - M(0, 1) * A0212 + M(0, 2) * A0112 ) };
}

// using 4x4 transforms ////////////////////////////////////////////////////////

template <int T> SnailVector<T> transformPoint(const SnailMatrix<4> &M, SnailVector<T> p) {
    SnailVector<4> p_hom = {};
    memcpy(&p_hom, &p, T * sizeof(real));
    p_hom.w = 1;
    SnailVector<4> ret_hom = M * p_hom;
    ret_hom /= ret_hom.w;
    SnailVector<T> ret = {};
    memcpy(&ret, &ret_hom, T * sizeof(real));
    return ret;
}
template <int T> SnailVector<T> transformVector(const SnailMatrix<4> &M, SnailVector<T> v) {
    SnailVector<4> v_hom = {};
    memcpy(&v_hom, &v, T * sizeof(real));
    SnailVector<4> ret_hom = M * v_hom;
    SnailVector<T> ret = {};
    memcpy(&ret, &ret_hom, T * sizeof(real));
    return ret;
}
template <int T> SnailVector<T> transformNormal(const SnailMatrix<4> &M, SnailVector<T> n) {
    SnailVector<4> n_hom = {};
    memcpy(&n_hom, &n, T * sizeof(real));
    SnailVector<4> ret_hom = inverse(transpose(M)) * n_hom;
    SnailVector<T> ret = {};
    memcpy(&ret, &ret_hom, T * sizeof(real));
    return ret;
}

// 4x4 transform cookbook //////////////////////////////////////////////////////

template <int T> SnailMatrix<T> identityMatrix() {
    SnailMatrix<T> ret = {};
    for (int i = 0; i < T; ++i) {
        ret(i, i) = 1;
    }
    return ret;
}
const SnailMatrix<4> _Identity4x4 = identityMatrix<4>();

SnailMatrix<4> M4_Identity() {
    return _Identity4x4;
}

SnailMatrix<4> M4_Translation(real x, real y, real z = 0) {
    SnailMatrix<4> ret = _Identity4x4;
    ret(0, 3) = x;
    ret(1, 3) = y;
    ret(2, 3) = z;
    return ret;
}
SnailMatrix<4> M4_Translation(SnailVector<2> xy) {
    return M4_Translation(xy.x, xy.y);
}
SnailMatrix<4> M4_Translation(SnailVector<3> xyz) {
    return M4_Translation(xyz.x, xyz.y, xyz.z);
}
SnailMatrix<4> M4_Scaling(real x, real y, real z = 1) {
    SnailMatrix<4> ret = {};
    ret(0, 0) = x;
    ret(1, 1) = y;
    ret(2, 2) = z;
    ret(3, 3) = 1;
    return ret;
}
SnailMatrix<4> M4_Scaling(real s) {
    return M4_Scaling(s, s, s);
}
SnailMatrix<4> M4_Scaling(SnailVector<2> xy) {
    return M4_Scaling(xy.x, xy.y);
}
SnailMatrix<4> M4_Scaling(SnailVector<3> xyz) {
    return M4_Scaling(xyz.x, xyz.y, xyz.z);
}
SnailMatrix<4> M4_RotationAboutXAxis(real t) {
    SnailMatrix<4> ret = _Identity4x4;
    ret(1, 1) = cos(t); ret(1, 2) = -sin(t);
    ret(2, 1) = sin(t); ret(2, 2) =  cos(t);
    return ret;
}
SnailMatrix<4> M4_RotationAboutYAxis(real t) {
    SnailMatrix<4> ret = _Identity4x4;
    ret(0, 0) =  cos(t); ret(0, 2) = sin(t);
    ret(2, 0) = -sin(t); ret(2, 2) = cos(t);
    return ret;
}
SnailMatrix<4> M4_RotationAboutZAxis(real t) {
    SnailMatrix<4> ret = _Identity4x4;
    ret(0, 0) = cos(t); ret(0, 1) = -sin(t);
    ret(1, 0) = sin(t); ret(1, 1) =  cos(t);
    return ret;
}

SnailMatrix<4> M4_RotationAxisAngle(SnailVector<3> axis, real angle) {
    real x = axis.x;
    real y = axis.y;
    real z = axis.z;
    real x2 = x * x;
    real y2 = y * y;
    real z2 = z * z;
    real xy = x * y;
    real xz = x * z;
    real yz = y * z;
    real c = cos(angle);
    real s = sin(angle);
    real d = 1-c;
    return { c+x2*d, xy*d-z*s, xz*d+y*s, 0,
        xy*d+z*s, c+y2*d, yz*d-x*s, 0,
        xz*d-y*s, yz*d+x*s, c+z2*d, 0,
        0, 0, 0, 1 };
}

mat4 M4_RotationFrom(vec3 a, vec3 b) {
    // NOTE: twist dof is whatever
    // https://math.stackexchange.com/questions/180418/calculate-rotation-matrix-to-align-vector-a-to-vector-b-in-3d

    // FORNOW
    a = normalized(a);
    b = normalized(b);

    vec3 v = cross(a, b);
    real c = dot(a, b);
    if (SNAIL_ABS(c + 1.0) < 1e-5) return identityMatrix<4>();
    mat3 v_x = { 0.0, -v.z, v.y, v.z, 0.0, -v.x, -v.y, v.x, 0.0 };
    mat3 R = identityMatrix<3>() + v_x + v_x * v_x / (1 + c);
    return {
        R.data[0], R.data[1], R.data[2], 0.0,
        R.data[3], R.data[4], R.data[5], 0.0,
        R.data[6], R.data[7], R.data[8], 0.0,
              0.0,       0.0,       0.0, 1.0 };
}

// optimization stuff //////////////////////////////////////////////////////////

template <int T> SnailMatrix<T> firstDerivativeofUnitVector(SnailVector<T> v) {
    SnailVector<T> tmp = normalized(v);
    return (1 / norm(v)) * (identityMatrix<T>() - outer(tmp, tmp));
}
#define firstDerivativeOfNorm normalized
#define secondDerivativeOfNorm firstDerivativeofUnitVector

template <int T> real squaredNorm(SnailMatrix<T> M) {
    real ret = 0;
    for (int i = 0; i < T * T; ++i) {
        ret += M.data[i] * M.data[i];
    }
    return ret;
}

// misc functions //////////////////////////////////////////////////////////////

template <int T> SnailVector<T> cwiseAbs(SnailVector<T> A) {
    for (int i = 0; i < T; ++i) A[i] = abs(A[i]);
    return A;
}
template <int T> SnailVector<T> cwiseMin(SnailVector<T> A, SnailVector<T> B) {
    SnailVector<T> ret = {};
    for (int i = 0; i < T; ++i) ret[i] = (A[i] < B[i]) ? A[i] : B[i];
    return ret;
}
template <int T> SnailVector<T> cwiseMax(SnailVector<T> A, SnailVector<T> B) {
    SnailVector<T> ret = {};
    for (int i = 0; i < T; ++i) ret[i] = (A[i] > B[i]) ? A[i] : B[i];
    return ret;
}
template <int T> SnailVector<T> cwiseProduct(SnailVector<T> a, SnailVector<T> b) {
    SnailVector<T> ret = {};
    for (int i = 0; i < T; ++i) ret[i] = a[i] * b[i];
    return ret;
}
SnailVector<2> e_theta(real theta) {
    return { cos(theta), sin(theta) };
}
double atan2(SnailVector<2> a) {
    return atan2(a.y, a.x);
}
SnailVector<2> rotated(SnailVector<2> a, real theta) {
    return { cos(theta) * a.x - sin(theta) * a.y, sin(theta) * a.x + cos(theta) * a.y };
}
SnailMatrix<2> R_theta_2x2(real theta) {
    return { cos(theta), -sin(theta), sin(theta), cos(theta) };
}
SnailVector<2> perpendicularTo(SnailVector<2> v) {
    return { v.y, -v.x };
}
SnailMatrix<4> xyzo2mat4(vec3 x, vec3 y, vec3 z, vec3 o) {
    return {
        x[0], y[0], z[0], o[0],
        x[1], y[1], z[1], o[1],
        x[2], y[2], z[2], o[2],
        0, 0, 0, 1
    };
}
#define M4_xyzo xyzo2mat4
template <int T> SnailVector<T> magClamped(SnailVector<T> a, real c) {
    double norm_a = norm(a);
    if (SNAIL_ABS(norm_a) < c) { return a; }
    return a / norm_a * SNAIL_CLAMP(norm_a, -c, c);
}

// utility /////////////////////////////////////////////////////////////////////

template <int T> void pprint(SnailVector<T> v) {
    printf("V%d(", T);
    SNAIL_FOR_(i, T) {
        printf("%lf", v[i]);
        if (i != T - 1) printf(", ");
    }
    printf(")\n");
}
template <int T> void pprint(SnailMatrix<T> M) {
    SNAIL_FOR_(r, T) {
        printf("| ");
        SNAIL_FOR_(c, T) {
            printf("%lf", M(r, c));
            if (c != T - 1) printf(", ");
        }
        printf(" |\n");
    }
}

#undef SNAIL_ASSERT
#undef SNAIL_ABS
#undef SNAIL_MIN
#undef SNAIL_MAX
#undef SNAIL_CLAMP
#undef SNAIL_FOR_
#endif


