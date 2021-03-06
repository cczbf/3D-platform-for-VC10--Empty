#ifndef VEC_H
#define VEC_H

/*
Szymon Rusinkiewicz
Princeton University

Vec.h
Class for a constant-length vector

Supports the following operations:
	vec v1;			// Initialized to (0,0,0)
	vec v2(1,2,3);		// Initializes the 3 components
	vec v3(v2);		// Copy constructor
	float farray[3];
	vec v4 = vec(farray);	// Explicit: "v4 = farray" won't work
	Vec<3,double> vd;	// The "vec" used above is Vec<3,float>
	point p1, p2, p3;	// Same as vec

	v3 = v1 + v2;		// Also -, *, /  (all componentwise)
	v3 = 3.5 * v1;		// Also vec * scalar, vec / scalar
	v3 = v1 DOT v2;		// Actually operator^
	v3 = v1 CROSS v2;	// Actually operator%

	float f = v1[0];	// Subscript
	float *fp = v1;		// Implicit conversion to float *

	f = len(v1);		// Length (also len2 == squared length)
	f = dist(p1, p2);	// Distance (also dist2 == squared distance)
	normalize(v1);		// Normalize (i.e., make it unit length)
				// normalize(vec(0,0,0)) => vec(1,0,0)

	cout << v1 << endl;	// iostream output in the form (1,2,3)
	cin >> v2;		// iostream input using the same syntax

Also defines the utility functions sqr, cube, sgn, and swap
*/

#include <cmath>
#include <iostream>
#include <algorithm>

//using std::min;
//using std::max;
//using std::swap;
//using std::sqrt;

//using namespace std;


#ifndef likely
#  if !defined(__GNUC__) || (__GNUC__ == 2 && __GNUC_MINOR__ < 96)
#    define likely(x) (x)
#    define unlikely(x) (x)
#  else
#    define likely(x)   (__builtin_expect((x), 1))
#    define unlikely(x) (__builtin_expect((x), 0))
#  endif
#endif


template <int D, class T = float>
class Vec 
{
private:
	T v[D];

public:
	// Constructor for no arguments.  Everything initialized to 0.
	Vec() { for (int i = 0; i < D; i++) v[i] = T(0); }

	// Constructor for 1-4 arguments.  Ugly, but the compiler *should*
	// optimize away the ifs and unroll the for...  This one's explicit.
	explicit Vec(T x, T y = T(0), T z = T(0), T w = T(0))
		{ v[0] = x;  if (D > 1) v[1] = y;
		  if (D > 2) v[2] = z;  if (D > 3) v[3] = w;
		  for (int i = 4; i < D; i++) v[i] = T(0); }

	// Constructor from anything that can be accessed using []
	// This one's pretty aggressive, so marked explicit
	template <class S> explicit Vec(const S &x)
		{ for (int i = 0; i < D; i++) v[i] = x[i]; }

	// No destructor or assignment operator needed

	// Array reference and conversion to pointer - no bounds checking
	const T &operator [] (int i) const
		{ return v[i]; }
	T &operator [] (int i)
		{ return v[i]; }
	operator const T * () const
		{ return v; }
	operator const T * ()
		{ return v; }
	operator T * ()
		{ return v; }

	// Member operators
	Vec<D,T> &operator += (const Vec<D,T> &x)
		{ for (int i = 0; i < D; i++) v[i] += x[i];  return *this; }
	Vec<D,T> &operator -= (const Vec<D,T> &x)
		{ for (int i = 0; i < D; i++) v[i] -= x[i];  return *this; }
	Vec<D,T> &operator *= (const Vec<D,T> &x)
		{ for (int i = 0; i < D; i++) v[i] *= x[i];  return *this; }
	Vec<D,T> &operator *= (const T &x)
		{ for (int i = 0; i < D; i++) v[i] *= x;     return *this; }
	Vec<D,T> &operator /= (const Vec<D,T> &x)
		{ for (int i = 0; i < D; i++) v[i] /= x[i];  return *this; }
	Vec<D,T> &operator /= (const T &x)
		{ for (int i = 0; i < D; i++) v[i] /= x;     return *this; }

	// Outside of class: + - * / % ^ << >>

	// Some partial compatibility with valarrays and vectors
	typedef T value_type;
	
	size_t size() const
		{ return D; }

	T sum() const
		{ T total = v[0];
		  for (int i = 1; i < D; i++) total += v[i];
		  return total; 
		}
	T product() const
		{ T total = v[0];
		  for (int i = 1; i < D; i++) total *= v[i];
		  return total; 
		}
	T min_() const
		{ T m = v[0];
		  for (int i = 0; i < D; i++)
			if (v[i] < m)  m = v[i];
		  return m;
		}
	T max_() const
		{ T m = v[0];
		  for (int i = 1; i < D; i++)
			if (v[i] > m)  m = v[i];
		  return m; 
		}
	T *begin() { return &(v[0]); }
	const T *begin() const { return &(v[0]); }
	T *end() { return begin() + D; }
	const T *end() const { return begin() + D; }
};

typedef Vec<3,float> vec;
typedef Vec<3,float> point;


// Nonmember operators that take two Vecs
template <int D, class T>
static inline const Vec<D,T> operator + (const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	return Vec<D,T>(v1) += v2;
}

template <int D, class T>
static inline const Vec<D,T> operator - (const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	return Vec<D,T>(v1) -= v2;
}

template <int D, class T>
static inline const Vec<D,T> operator * (const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	return Vec<D,T>(v1) *= v2;
}

template <int D, class T>
static inline const Vec<D,T> operator / (const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	return Vec<D,T>(v1) /= v2;
}

template <int D, class T>
static inline const T operator ^ (const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	T sum = T(0);
	for (int i = 0; i < D; i++)
		sum += v1[i] * v2[i];
	return sum;
}
#define DOT ^

template <class T>
static inline const Vec<3,T> operator % (const Vec<3,T> &v1, const Vec<3,T> &v2)
{
	return Vec<3,T>(v1[1]*v2[2] - v1[2]*v2[1],
			v1[2]*v2[0] - v1[0]*v2[2],
			v1[0]*v2[1] - v1[1]*v2[0]);
}
#define CROSS %


// Unary operators
template <int D, class T>
static inline const Vec<D,T> &operator + (const Vec<D,T> &v)
{
	return v;
}

template <int D, class T>
static inline const Vec<D,T> operator - (const Vec<D,T> &v)
{
	Vec<D,T> result(v);
	for (int i = 0; i < D; i++)
		result[i] = -result[i];
	return result;
}

template <int D, class T>
static inline bool operator ! (const Vec<D,T> &v)
{
	for (int i = 0; i < D; i++)
		if (v[i]) return false;
	return true;
}


// Vec/scalar operators
template <int D, class T>
static inline const Vec<D,T> operator * (const T &x, const Vec<D,T> &v)
{
	Vec<D,T> result(v);
	for (int i = 0; i < D; i++)
		result[i] = x * result[i];
	return result;
}

template <int D, class T>
static inline const Vec<D,T> operator * (const Vec<D,T> &v, const T &x)
{
	return Vec<D,T>(v) *= x;
}

template <int D, class T>
static inline const Vec<D,T> operator / (const T &x, const Vec<D,T> &v)
{
	Vec<D,T> result(v);
	for (int i = 0; i < D; i++)
		result[i] = x / result[i];
	return result;
}

template <int D, class T>
static inline const Vec<D,T> operator / (const Vec<D,T> &v, const T &x)
{
	return Vec<D,T>(v) /= x;
}


// iostream operators
template <int D, class T>
static inline std::ostream &operator << (std::ostream &os, const Vec<D,T> &v)

{
	os << "(";
	for (int i = 0; i < D-1; i++)
		os << v[i] << ", ";
	return os << v[D-1] << ")";
}

template <int D, class T>
static inline std::istream &operator >> (std::istream &is, Vec<D,T> &v)
{
	char c1 = 0, c2 = 0, c3 = 0;

	is >> c1;
	if (c1 == '(' || c1 == '[') {
		is >> v[0] >> std::ws >> c2;
		for (int i = 1; i < D; i++)
			if (c2 == ',')
				is >> v[i] >> std::ws >> c2;
	}

	if (c1 == '(' && c3 != ')')
		is.setstate(std::ios::failbit);
	else if (c1 == '[' && c3 != ']')
		is.setstate(std::ios::failbit);

	return is;
}


// Utility functions for square and cube, to go along with sqrt and cbrt
template <class T>
static inline T sqr(const T &x)
{
	return x*x;
}

template <class T>
static inline T cube(const T &x)
{
	return x*x*x;
}


// Sign of a scalar
template <class T>
static inline T sgn(const T &x)
{
	return (x < T(0)) ? T(-1) : T(1);
}


// Functions on Vecs
template <int D, class T>
static inline const T len2(const Vec<D,T> &v)
{
	return v DOT v;
}

template <int D, class T>
static inline const T len(const Vec<D,T> &v)
{
	return sqrt(len2(v));
}

template <int D, class T>
static inline const T abs(const Vec<D,T> &v)
{
	return len(v);
}

template <int D, class T>
static inline const T dist2(const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	T d2 = sqr(v2[0]-v1[0]);
	for (int i = 1; i < D; i++)
		d2 += sqr(v2[i]-v1[i]);
	return d2;
}

template <int D, class T>
static inline const T dist(const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	return sqrt(dist2(v1,v2));
}

template <int D, class T>
static inline void normalize(Vec<D,T> &v)
{
	T l = len(v);
	if (unlikely(l <= T(0))) {
		v[0] = T(1);
		for (int i = 1; i < D; i++)
			v[i] = T(0);
		return;
	}

	l = T(1) / l;
	for (int i = 0; i < D; i++)
		v[i] *= l;
}

template <int D, class T>
static inline void swap(const Vec<D,T> &v1, const Vec<D,T> &v2)
{
	for (int i = 0; i < D; i++)
		swap(v1[i], v2[i]);
}

#endif
