#pragma once
#include <cmath>
#include <cassert>
#include <iostream>

template<int n,typename t> struct vec {
    vec() = default;
    t& operator[](const int i) { assert(i >= 0 && i < n); return data[i]; }
    t   operator[](const int i) const { assert(i >= 0 && i < n); return data[i]; }
    t norm2() const { return (*this) * (*this); }
    t norm()  const { return std::sqrt(norm2()); }
    t data[n] = { 0 };
};

// vector dot product 返回double
template<int n, typename t1,typename t2> double operator*(const vec<n,t1>& lhs, const vec<n,t2>& rhs) {
    double ret = 0;
    for (int i = n; i--; ret += lhs[i] * rhs[i]);
    return ret;
}

//vector plus 返回基准为前一个vector数据类型 t1
template<int n, typename t1,typename t2> vec<n,t1> operator+(const vec<n,t1>& lhs, const vec<n,t2>& rhs) {
    vec<n,t1> ret = lhs;
    for (int i = n; i--; ret[i] += rhs[i]);
    return ret;
}
//vector minus 返回基准为前一个vector数据类型 t1
template<int n, typename t1, typename t2> vec<n,t1> operator-(const vec<n,t1>& lhs, const vec<n,t2>& rhs) {
    vec<n,t1> ret = lhs;
    for (int i = n; i--; ret[i] -= rhs[i]);
    return ret;
}

//数乘 返回基准为t1 
template<int n, typename t1,typename t2> vec<n,t1> operator*(const t2& rhs, const vec<n,t1>& lhs) {
    vec<n,t1> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}
//数乘 返回基准为t1
template<int n, typename t1, typename t2> vec<n,t1> operator*(const vec<n,t1>& lhs, const t2& rhs) {
    vec<n,t1> ret = lhs;
    for (int i = n; i--; ret[i] *= rhs);
    return ret;
}
//vector divide 返回基准为前一个vector数据类型
template<int n, typename t1,typename t2> vec<n,t1> operator/(const vec<n,t1>& lhs, const t2& rhs) {
    vec<n,t1> ret = lhs;
    for (int i = n; i--; ret[i] /= rhs);
    return ret;
}


//embed和proj暂定为同一数据类型
template<int n1, int n2, typename t> vec<n1,t> embed(const vec<n2,t>& v, t fill = 1) {
    vec<n1,t> ret;
    for (int i = n1; i--; ret[i] = (i < n2 ? v[i] : fill));
    return ret;
}
template<typename t> vec<4, t> embed(const vec<3, t>& v, t fill = 1) {
    vec<4, t> ret;
    for (int i = 4; i--; ret[i] = (i < 3 ? v[i] : fill));
    return ret;
}
template<int n1, int n2, typename t> vec<n1,t> proj(const vec<n2,t>& v) {
    vec<n1,t> ret;
    for (int i = n1; i--; ret[i] = v[i]);
    return ret;
}
template<typename t>vec<3, t> proj(const vec<4, t>& v)
{
    vec<3, t>ret;
    for (int i = 3; i--; ret[i] = v[i]);
    return ret;
}


template<int n, typename t> std::ostream& operator<<(std::ostream& out, const vec<n,t>& v) {
    out << "(";
    for (int i = 0; i < n; i++) out << v[i] << " ";
    out << ")";
    return out;
}

/////////////////////////////////////////////////////////////////////////////////


//norm表示长度，norm2表示长度的平方，而normalize表示单位化该向量
template<typename t> struct vec<2,t> {
    vec() = default;
    vec(t X, t Y) : x(X), y(Y) {}

    template<typename type>
    vec(const vec<2, type>& v) :x(v.x), y(v.y) {}

    t& operator[](const int i) { assert(i >= 0 && i < 2); return i == 0 ? x : y; }
    t  operator[](const int i) const { assert(i >= 0 && i < 2); return i == 0 ? x : y; }
    t norm2() const { return (*this) * (*this); }
    t norm()  const { return std::sqrt(norm2()); }
    vec& normalize() { *this = (*this) / norm(); return *this; }

    t x{}, y{};
};

/////////////////////////////////////////////////////////////////////////////////

template<typename t> struct vec<3,t> {
    vec() = default;
    vec(t X, t Y, t Z) : x(X), y(Y), z(Z) {}


    template<typename type>
    vec(const vec<3, type>& v) :x(v.x), y(v.y) ,z(v.z){}

    t& operator[](const int i) { assert(i >= 0 && i < 3); return i == 0 ? x : (1 == i ? y : z); }
    t  operator[](const int i) const { assert(i >= 0 && i < 3); return i == 0 ? x : (1 == i ? y : z); }
    t norm2() const { return (*this) * (*this); }
    t norm()  const { return std::sqrt(norm2()); }
    vec& normalize() { *this = (*this) / norm(); return *this; }
    t x{}, y{}, z{};
};

/////////////////////////////////////////////////////////////////////////////////


template<typename t> struct vec<4,t> {
    vec() = default;
    vec(t X, t Y, t Z,t W) : x(X), y(Y), z(Z),w(W) {}

    template<typename type>
    vec(const vec<4, type>& v) :x(v.x), y(v.y),z(v.z),w(v.w) {}

    t& operator[](const int i) { assert(i >= 0 && i < 4);  return i == 0 ? x : (1 == i ? y :(2==i ? z:w)); }
    t  operator[](const int i) const { assert(i >= 0 && i <4); return i == 0 ? x : (1 == i ? y : (2 == i ? z : w));}
    t norm2() const { return (*this) * (*this); }
    t norm()  const { return std::sqrt(norm2()); }
    vec& normalize() { *this = (*this) / norm(); return *this; }

    t x{}, y{}, z{}, w{};
};

/////////////////////////////////////////////////////////////////////////////////

template<int n> struct dt;

template<int nrows, int ncols> struct mat {
  
    vec<ncols,double> rows[nrows] = { {} };

    mat() = default;
    vec<ncols, double>& operator[] (const int idx) { assert(idx >= 0 && idx < nrows); return rows[idx]; }
    const vec<ncols,double>& operator[] (const int idx) const { assert(idx >= 0 && idx < nrows); return rows[idx]; }

    vec<nrows,double> col(const int idx) const {
        assert(idx >= 0 && idx < ncols);
        vec<nrows,double> ret;
        for (int i = nrows; i--; ret[i] = rows[i][idx]);
        return ret;
    }

    void set_col(const int idx, const vec<nrows,double>& v) {
        assert(idx >= 0 && idx < ncols);
        for (int i = nrows; i--; rows[i][idx] = v[i]);
    }

    static mat<nrows, ncols> identity() {
        mat<nrows, ncols> ret;
        for (int i = nrows; i--; )
            for (int j = ncols; j--; ret[i][j] = (i == j));
       
        return ret;
    }

    double det() const {
        return dt<ncols>::det(*this);
    }

    mat<nrows - 1, ncols - 1> get_minor(const int row, const int col) const {
        mat<nrows - 1, ncols - 1> ret;
        for (int i = nrows - 1; i--; )
            for (int j = ncols - 1; j--; ret[i][j] = rows[i < row ? i : i + 1][j < col ? j : j + 1]);
        return ret;
    }

    double cofactor(const int row, const int col) const {
        return get_minor(row, col).det() * ((row + col) % 2 ? -1 : 1);
    }

    mat<nrows, ncols> adjugate() const {
        mat<nrows, ncols> ret;
        for (int i = nrows; i--; )
            for (int j = ncols; j--; ret[i][j] = cofactor(i, j));
        return ret;
    }

    mat<nrows, ncols> invert_transpose() const {
        mat<nrows, ncols> ret = adjugate();
        return ret / (ret[0] * rows[0]);
    }

    mat<nrows, ncols> invert() const {
        return invert_transpose().transpose();
    }

    mat<ncols, nrows> transpose() const {
        mat<ncols, nrows> ret;
        for (int i = ncols; i--; ret[i] = this->col(i));
        return ret;
    }
};

/////////////////////////////////////////////////////////////////////////////////

template<int nrows, int ncols> vec<nrows,double> operator*(const mat<nrows, ncols>& lhs, const vec<ncols,double>& rhs) {
    vec<nrows,double> ret;
    for (int i = nrows; i--; ret[i] = lhs[i] * rhs);
    return ret;
}

template<int R1, int C1, int C2>mat<R1, C2> operator*(const mat<R1, C1>& lhs, const mat<C1, C2>& rhs) {
    mat<R1, C2> result;
    for (int i = R1; i--; )
        for (int j = C2; j--; result[i][j] = lhs[i] * rhs.col(j));
    return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator*(const mat<nrows, ncols>& lhs, const double& val) {
    mat<nrows, ncols> result;
    for (int i = nrows; i--; result[i] = lhs[i] * val);
    return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator/(const mat<nrows, ncols>& lhs, const double& val) {
    mat<nrows, ncols> result;
    for (int i = nrows; i--; result[i] = lhs[i] / val);
    return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator+(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
    mat<nrows, ncols> result;
    for (int i = nrows; i--; )
        for (int j = ncols; j--; result[i][j] = lhs[i][j] + rhs[i][j]);
    return result;
}

template<int nrows, int ncols>mat<nrows, ncols> operator-(const mat<nrows, ncols>& lhs, const mat<nrows, ncols>& rhs) {
    mat<nrows, ncols> result;
    for (int i = nrows; i--; )
        for (int j = ncols; j--; result[i][j] = lhs[i][j] - rhs[i][j]);
    return result;
}

template<int nrows, int ncols> std::ostream& operator<<(std::ostream& out, const mat<nrows, ncols>& m) {
    for (int i = 0; i < nrows; i++) out << m[i] << std::endl;
    return out;
}

/////////////////////////////////////////////////////////////////////////////////

template<int n> struct dt {
    static double det(const mat<n, n>& src) {
        double ret = 0;
        for (int i = n; i--; ret += src[0][i] * src.cofactor(0, i));
        return ret;
    }
};

template<> struct dt<1> {
    static double det(const mat<1, 1>& src) {
        return src[0][0];
    }
};

/////////////////////////////////////////////////////////////////////////////////

typedef vec<2,double> vec2;
typedef vec<3,double> vec3;
typedef vec<4,double> vec4;
typedef vec<4, int>vec4i;
typedef vec<2, int>vec2i;
typedef vec<3, int>vec3i;
typedef vec<4, float>vec4f;
typedef vec<2, float>vec2f;
typedef vec<3, float>vec3f;
typedef mat<4, 4>mat4;




inline vec3 cross(const vec3& v1, const vec3& v2)
{
    return vec<3,double>{v1.y* v2.z - v1.z * v2.y, v1.z* v2.x - v1.x * v2.z, v1.x* v2.y - v1.y * v2.x};
}
/*view matrix and projection matrix*/
mat4 lookat(vec3 pos, vec3 target, vec3 up);

mat4 proj_matrix(float fovy, float aspect, float near, float far);


mat4 viewport(int x, int y, int w, int h);
////////////////////////////////////////////////
inline float float_clamp(float f, float min, float max) {
    return f < min ? min : (f > max ? max : f);
}