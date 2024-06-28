/*
* Some useful math structures and operations.
* Structures include a variety of methods, like
* calculating average, dot product, magnitude,
* and most of arithmetic operations
* (excluding those which make no sense)
*
* Templated structures:
* - 2D, 3D, 4D vectors
* - line segments
*
* There are some aliases like position, point, color,
* etc. already defined in the format <name><dimensions><data type>
*/

#pragma once

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <any>
#include <type_traits>
#include <cmath>
#include <string>
#include <format>
#include <numbers>
#include <initializer_list>
#include <numeric>
#include <array>
#include <exception>
#include <stdexcept>
#include <type_traits>

/*****************************************************/
// List of available structures and functions 
/*****************************************************/

namespace math
{
	// Constants ----------------------------------------

	constexpr double PI = 3.1415926535897932;
	constexpr double PI2 = PI * 2;	// PI * 2

	constexpr double PI_2 = PI / 2; // PI / 2
	constexpr double PI_3 = PI / 3; // PI / 3
	constexpr double PI_4 = PI / 4; // PI / 4
	constexpr double PI_6 = PI / 6; // PI / 6

	// Structures ---------------------------------------

	template<class T> struct Vec2;
	template<class T> struct Vec3;
	template<class T> struct Vec4;

	template<class T> struct Segment2;

	template<class T, size_t row, size_t col> struct Matrix2;

	template<class T, size_t row, size_t col>
	using Array2 = std::array<std::array<T, col>, row>;	// helper for creating 2d arrays

	// Functions ----------------------------------------

	// square
	template<class T> T Sqr(T a);

	// average
	template<class T> double Avg(T a, T b);
	template<class T> double Avg(T a, T b, T c);
	template<class T> double Avg(std::initializer_list<T> ls);
	template<class T> Vec2<double> Avg(const Vec2<T>& v1, const Vec2<T>& v2);

	// distance sq
	template<class T> double DistanceSq(const Vec2<T>& p1, const Vec2<T>& p2);

	// distance
	template<class T> double Distance(const Vec2<T>& p1, const Vec2<T>& p2); 
	template<class T> double Distance(const Segment2<T>& s, const Vec2<T>& p);

	// dot product
	template<class T> double Dot(const Vec2<T>& v1, const Vec2<T>& v2);
	template<class T> double Dot(const Vec3<T>& v1, const Vec3<T>& v2);
	template<class T> double Dot(const Vec4<T>& v1, const Vec4<T>& v2);

	// rotation
	double DegToRad(double d);
	double RadToDeg(double r);

	template<class T> void Rotate(Vec2<T>& v, float rad);
	template<class T> void Rotate90CW(Vec2<T>& v);			// clockwise
	template<class T> void Rotate90CCW(Vec2<T>& v);			// counterclockwise

// Aliases -------------------------------------------

	// vec 2d
	using Vec2I = Vec2<int>;
	using Vec2F = Vec2<float>;
	using Vec2D = Vec2<double>;

	using Point2I = Vec2<int>;
	using Point2F = Vec2<float>;
	using Point2D = Vec2<double>;

	using Pos2I = Vec2<int>;
	using Pos2F = Vec2<float>;
	using Pos2D = Vec2<double>;

	// vec 3d
	using Vec3I = Vec3<int>;
	using Vec3F = Vec3<float>;
	using Vec3D = Vec3<double>;

	using Point3I = Vec3<int>;
	using Point3F = Vec3<float>;
	using Point3D = Vec3<double>;

	using Pos3I = Vec3<int>;
	using Pos3F = Vec3<float>;
	using Pos3D = Vec3<double>;

	// vec 4d
	using Vec4I = Vec4<int>;
	using Vec4F = Vec4<float>;
	using Vec4D = Vec4<double>;

	using Point4I = Vec4<int>;
	using Point4F = Vec4<float>;
	using Point4D = Vec4<double>;

	using Pos4I = Vec4<int>;
	using Pos4F = Vec4<float>;
	using Pos4D = Vec4<double>;

	// color
	using Color3B = Vec3<uint8_t>;
	using Color3F = Vec3<float>;

	using Color4B = Vec4<uint8_t>;
	using Color4F = Vec4<float>;

	// line 2d
	using Segment2I = Segment2<int>;
	using Segment2F = Segment2<float>;
	using Segment2D = Segment2<double>;

	using Line2I = Segment2<int>;
	using Line2F = Segment2<float>;
	using Line2D = Segment2<double>;

	using Edge2I = Segment2<int>;
	using Edge2F = Segment2<float>;
	using Edge2D = Segment2<double>;
}


/*****************************************************/
// Implementation 
/*****************************************************/

namespace math
{
	namespace helper
	{
		template<class T, class NT, size_t size>
		void CopyCastArray(const std::array<T, size>& src, std::array<NT, size>& dest)
		{
			for (size_t i = 0; i < size; ++i)
			{
				dest[i] = static_cast<NT>(src[i]);
			}
		}
	}

	// Structures ------------------------------------------

	template<class T, size_t size>
	struct Vec
	{
		std::array<T, size> elems;

	public:
		Vec() = default;
		Vec(T v) { elems.fill(v); }
		Vec(const std::array<T, size> src) : elems(src) { }

		T Sum() const { return std::accumulate(elems.begin(), elems.end(), 0); }
		T Mul() const { return std::accumulate(elems.begin(), elems.end(), 1, std::multiplies<T>()); }
		T Avg() const { return Sum() / elems.size(); }

		T Min() const { return std::min_element(elems.begin(), elems.end()); }
		T Max() const { return std::max_element(elems.begin(), elems.end()); }
		double MagSq() const
		{
			double mag = 0.0;
			for (auto e : elems) { mag += (e * e); }
			return mag;
		}
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec<T, size>& v) const
		{
			double dot = 0.0;
			for (size_t i = 0; i < elems.size(); ++i) { dot += (elems[i] * v.elems[i]); }
			return dot;
		}
		//Vec<double, size> Normalize() const { double mag = Mag(); return ? *this : *this / mag; }

	public:
		template<class NT>
		operator Vec<NT, size>() const 
		{
			Vec<NT, size> out;
			helper::CopyCastArray(this->elems, out.elems);
			return out;
		}

		template<class C>
		Vec<C, size> operator+(const C& rhs) 
		{ 
			Vec<C, size> out = (Vec<C, size>)*this; 
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e += rhs; }); 
			return out; 
		}
		template<class C>
		Vec<C, size> operator-(const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)*this;
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e -= rhs; });
			return out;
		}
		template<class C>
		Vec<C, size> operator*(const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)*this;
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e *= rhs; });
			return out;
		}
		template<class C>
		Vec<C, size> operator/(const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)*this;
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e /= rhs; });
			return out;
		}

		Vec<T, size> operator-()
		{
			Vec<T, size> out = *this;
			std::for_each(out.elems.begin(), out.elems.end(), [](T& e) { e = -e; });
			return out;
		}

		template<class C, size_t size2>
		auto operator+(const Vec<C, size2>& rhs)
		{
			if constexpr (size2 > size)
			{
				Vec<C, size2> out;
				helper::CopyCastArray(rhs.elems, out.elems);
				for (size_t i = 0; i < size; i++)
				{
					out.elems[i] += this->elems[i];
				}
				return out;
			}
			else
			{
				Vec<C, size> out;
				helper::CopyCastArray(this->elems, out.elems);
				for (size_t i = 0; i < size2; i++)
				{
					out.elems[i] += rhs.elems[i];
				}
				return out;
			}
		}


		friend std::ostream& operator<<(std::ostream& os, const Vec<T, size>& v)
		{
			for (size_t i = 0; i < v.elems.size(); ++i) os << v.elems[i] << ((i == (v.elems.size() - 1)) ? "" : ", ");
			return os;
		}
			/*


		template<class C>
		friend Vec2<C> operator+(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x + rhs.x, lhs.y + rhs.y); }
		template<class C>
		friend Vec2<C> operator-(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x - rhs.x, lhs.y - rhs.y); }
		template<class C>
		friend Vec2<C> operator*(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x * rhs.x, lhs.y * rhs.y); }
		template<class C>
		friend Vec2<C> operator/(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x / rhs.x, lhs.y / rhs.y); }

		friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
		{
			os << v.x << ", " << v.y;
			return os;
		}
	};
*/
	};

	template<class T>
	struct Vec2
	{
		T x, y;

	public:
		Vec2() = default;
		Vec2(T v) : x(v), y(v) {}
		Vec2(T x, T y) : x(x), y(y) {}
		Vec2(const Vec2<T>& src) : x(src.x), y(src.y) {}

		T Sum() const { return x + y; }
		T Sub() const { return x - y; }
		T Mul() const { return x * y; }
		T Div() const { return x / y; }
		T Avg() const { return (x + y) / 2; }

		T Min() const { return std::min(x, y); }
		T Max() const { return std::max(x, y); }
		double MagSq() const { return x * x + y * y; }
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec2& v) const { return x * v.x + y * v.y; }

		void Zero() { x = y = 0; }

		Vec2D Normalize() const { double mag = Mag(); return (mag == 0) ? Vec2<double>(x, y) : *this / mag; }
		void Rotate(float rad) 
		{
			float cs = std::cos(rad);
			float sn = std::sin(rad);
			float nx = x * cs - y * sn;
			float ny = x * sn + y * cs;
			x = nx; 
			y = ny;
		}
		void Rotate90CW() { float nx = y, ny = -x; x = nx; y = ny; }
		void Rotate90CCW() { float nx = -y, ny = x; x = nx; y = ny; }

		std::string ToStr() const { return std::format("{}, {}", x, y); }

	public:
		template<class NT>
		operator Vec2<NT>() const { return Vec2<NT>{(NT)x, (NT)y}; }

		Vec2& operator+=(T v) { x += v; y += v; return *this; }
		Vec2& operator-=(T v) { x -= v; y -= v; return *this; }
		Vec2& operator*=(T v) { x *= v; y *= v; return *this; }
		Vec2& operator/=(T v) { x /= v; y /= v; return *this; }

		Vec2& operator+=(const Vec2& v) { x += v.x; y += v.y; return *this; }
		Vec2& operator-=(const Vec2& v) { x -= v.x; y -= v.y; return *this; }
		Vec2& operator*=(const Vec2& v) { x *= v.x; y *= v.y; return *this; }
		Vec2& operator/=(const Vec2& v) { x /= v.x; y /= v.y; return *this; }

		bool operator==(const Vec2& v) { return x == v.x && y == v.y; }
		bool operator!=(const Vec2& v) { return x != v.x || y != v.y; }
		bool operator>(const Vec2& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec2& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec2& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec2& v) { return MagSq() <= v.MagSq(); }
		
		T& operator[](size_t i)
		{
			switch (i)
			{
				case 0: return x;
				case 1: return y;
				default: throw std::out_of_range("Index out of range. Allowed indices for Vec2: 0, 1.");
			}
		}

		template<class C>
		friend Vec2<C> operator+(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x + rhs, lhs.y + rhs); }
		template<class C>
		friend Vec2<C> operator-(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x - rhs, lhs.y - rhs); }
		template<class C>
		friend Vec2<C> operator*(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x * rhs, lhs.y * rhs); }
		template<class C>
		friend Vec2<C> operator/(const Vec2<T>& lhs, const C& rhs) { return Vec2<C>(lhs.x / rhs, lhs.y / rhs); }

		friend Vec2<T> operator-(const Vec2<T> vec) { return Vec2<T>(-vec.x, -vec.y); }

		template<class C>
		friend Vec2<C> operator+(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x + rhs.x, lhs.y + rhs.y); }
		template<class C>
		friend Vec2<C> operator-(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x - rhs.x, lhs.y - rhs.y); }
		template<class C>
		friend Vec2<C> operator*(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x * rhs.x, lhs.y * rhs.y); }
		template<class C>
		friend Vec2<C> operator/(const Vec2<T>& lhs, const Vec2<C>& rhs) { return Vec2<C>(lhs.x / rhs.x, lhs.y / rhs.y); }

		friend std::ostream& operator<<(std::ostream& os, const Vec2<T>& v)
		{
			os << v.x << ", " << v.y;
			return os;
		}
	};

	template<class T>
	struct Vec3
	{
		T x, y, z;

	public:
		Vec3() = default;
		Vec3(T v) : x(v), y(v), z(v) {}
		Vec3(T x, T y, T z) : x(x), y(y), z(z) {}
		Vec3(const Vec3<T>& src) : x(src.x), y(src.y), z(src.z) {}

		T Sum() const { return x + y + z; }
		T Mul() const { return x * y * z; }
		T Avg() const { return (x + y + z) / 3; }

		T Min() const { return std::min({ x,y,z }); }
		T Max() const { return std::max({ x,y,z }); }
		double MagSq() const { return x * x + y * y + z * z; }
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }

		void Zero() { x = y = z = 0; }

		Vec3<double> Normalize() const { double mag = Mag(); return (mag == 0) ? Vec3<double>(x, y, z) : *this / mag; }

		std::string ToStr() const { return std::format("{}, {}, {}", x, y, z); }

	public:
		template<class NT>
		operator Vec3<NT>() const { return Vec3<NT>{(NT)x, (NT)y}; }

		Vec3& operator+=(T v) { x += v; y += v; z += v; return *this; }
		Vec3& operator-=(T v) { x -= v; y -= v; z -= v; return *this; }
		Vec3& operator*=(T v) { x *= v; y *= v; z *= v; return *this; }
		Vec3& operator/=(T v) { x /= v; y /= v; z /= v; return *this; }

		Vec3& operator+=(const Vec3& v) { x += v.x; y += v.y; z += v.z; return *this; }
		Vec3& operator-=(const Vec3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
		Vec3& operator*=(const Vec3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }
		Vec3& operator/=(const Vec3& v) { x /= v.x; y /= v.y; z /= v.z; return *this; }

		bool operator==(const Vec3& v) { return x == v.x && y == v.y && z == v.z; }
		bool operator!=(const Vec3& v) { return x != v.x || y != v.y || z != v.z; }
		bool operator>(const Vec3& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec3& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec3& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec3& v) { return MagSq() <= v.MagSq(); }

		T& operator[](size_t i)
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			default: throw std::out_of_range("Index out of range. Allowed indices for Vec3: 0, 1, 2.");
			}
		}

		template<class C>
		friend Vec3<C> operator+(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs); }
		template<class C>
		friend Vec3<C> operator-(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs); }
		template<class C>
		friend Vec3<C> operator*(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); }
		template<class C>
		friend Vec3<C> operator/(const Vec3<T>& lhs, const C& rhs) { return Vec3<C>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); }

		template<class C>
		friend Vec3<C> operator+(const Vec3<T>& lhs, const Vec3<C>& rhs) { return Vec3<C>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
		template<class C>
		friend Vec3<C> operator-(const Vec3<T>& lhs, const Vec3<C>& rhs) { return Vec3<C>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
		template<class C>
		friend Vec3<C> operator*(const Vec3<T>& lhs, const Vec3<C>& rhs) { return Vec3<C>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
		template<class C>
		friend Vec3<C> operator/(const Vec3<T>& lhs, const Vec3<C>& rhs) { return Vec3<C>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); }

		friend Vec3<T> operator-(const Vec3<T> vec) { return Vec3<T>(-vec.x, -vec.y, -vec.z); }

		friend std::ostream& operator<<(std::ostream& os, const Vec3<T>& v)
		{
			os << v.x << ", " << v.y << ", " << v.z;
			return os;
		}
	};

	template<class T>
	struct Vec4
	{
		T x, y, z, w;

	public:
		Vec4() = default;
		Vec4(T v) : x(v), y(v), z(v), w(v) {}
		Vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
		Vec4(const Vec4<T>& src) : x(src.x), y(src.y), z(src.z), w(src.w) {}

		T Sum() const { return x + y + z + w; }
		T Mul() const { return x * y * z * w; }
		T Avg() const { return (x + y + z + w) / 4; }

		T Min() const { return std::min({ x,y,z,w }); }
		T Max() const { return std::max({ x,y,z,w }); }
		double MagSq() const { return x * x + y * y + z * z + w * w; }
		double Mag() const { return std::sqrt(MagSq()); }

		double Dot(const Vec4& v) const { return x * v.x + y * v.y + z * v.z + w * v.w; }

		void Zero() { x = y = z = w = 0; }

		Vec4<double> Normalize() const { double mag = Mag(); return (mag == 0) ? Vec4<double>(x, y, z, w) : *this / mag; }

		std::string ToStr() const { return std::format("{}, {}, {}, {}", x, y, z, w); }

	public:
		template<class NT>
		operator Vec4<NT>() const { return Vec4<NT>{(NT)x, (NT)y}; }

		Vec4& operator+=(T v) { x += v; y += v; z += v; w += v; return *this; }
		Vec4& operator-=(T v) { x -= v; y -= v; z -= v; w -= v; return *this; }
		Vec4& operator*=(T v) { x *= v; y *= v; z *= v; w *= v; return *this; }
		Vec4& operator/=(T v) { x /= v; y /= v; z /= v; w /= v; return *this; }

		Vec4& operator+=(const Vec4& v) { x += v.x; y += v.y; z += v.z; w += v.w; return *this; }
		Vec4& operator-=(const Vec4& v) { x -= v.x; y -= v.y; z -= v.z; w -= v.w; return *this; }
		Vec4& operator*=(const Vec4& v) { x *= v.x; y *= v.y; z *= v.z; w *= v.w; return *this; }
		Vec4& operator/=(const Vec4& v) { x /= v.x; y /= v.y; z /= v.z; w /= v.w; return *this; }

		bool operator==(const Vec4& v) { return x == v.x && y == v.y && z == v.z && w == v.w; }
		bool operator!=(const Vec4& v) { return x != v.x || y != v.y || z != v.z || w != v.w; }
		bool operator>(const Vec4& v) { return MagSq() > v.MagSq(); }
		bool operator>=(const Vec4& v) { return MagSq() >= v.MagSq(); }
		bool operator<(const Vec4& v) { return MagSq() < v.MagSq(); }
		bool operator<=(const Vec4& v) { return MagSq() <= v.MagSq(); }

		T& operator[](size_t i)
		{
			switch (i)
			{
			case 0: return x;
			case 1: return y;
			case 2: return z;
			case 3: return w;
			default: throw std::out_of_range("Index out of range. Allowed indices for Vec4: 0, 1, 2, 3.");
			}
		}

		template<class C>
		friend Vec4<C> operator+(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs); }
		template<class C>
		friend Vec4<C> operator-(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs); }
		template<class C>
		friend Vec4<C> operator*(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); }
		template<class C>
		friend Vec4<C> operator/(const Vec4<T>& lhs, const C& rhs) { return Vec4<C>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); }

		template<class C>
		friend Vec4<C> operator+(const Vec4<T>& lhs, const Vec4<C>& rhs) { return Vec4<C>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
		template<class C>
		friend Vec4<C> operator-(const Vec4<T>& lhs, const Vec4<C>& rhs) { return Vec4<C>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
		template<class C>
		friend Vec4<C> operator*(const Vec4<T>& lhs, const Vec4<C>& rhs) { return Vec4<C>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
		template<class C>
		friend Vec4<C> operator/(const Vec4<T>& lhs, const Vec4<C>& rhs) { return Vec4<C>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); }

		friend Vec4<T> operator-(const Vec4<T> vec) { return Vec4<T>(-vec.x, -vec.y, -vec.z, -vec.w); }

		friend std::ostream& operator<<(std::ostream& os, const Vec4<T>& v)
		{
			os << v.x << ", " << v.y << ", " << v.z << ", " << v.w;
			return os;
		}
	};

	// -----------------------------------------------------

	template<class T>
	struct Segment2
	{
		Vec2<T> a, b;

	public:
		Segment2() = default;
		Segment2(const Vec2<T>& a, const Vec2<T>& b) : a(a), b(b) {}
		Segment2(T ax, T ay, T bx, T by) : a(Vec2<T>(ax, ay)), b(Vec2<T>(bx, by)) {}
		Segment2(const Segment2& src) : a(src.a), b(src.b) {}

		double Len() const { return Distance(a, b); }
		double LenSq() const { return DistanceSq(a, b); }

		double CenterA() const { return Avg(a.x, a.y); }
		double CenterB() const { return Avg(b.x, b.y); }
		Vec2D Center() const { return Avg(a, b); }

		Vec2D Delta() const { return b - a; }
		double DeltaX() const { return b.x - a.x; }
		double DeltaY() const { return b.y - a.y; }
		Vec2D Dir() const { return -(Delta().Normalize()); }

	public:
		template<class NT>
		operator Segment2<NT>() const { return Segment2<NT>{(Vec2<NT>)a, (Vec2<NT>)b}; }

		Segment2& operator+=(T v) { a += v; b += v; return *this; }
		Segment2& operator-=(T v) { a -= v; b -= v; return *this; }
		Segment2& operator*=(T v) { a *= v; b *= v; return*this; }
		Segment2& operator/=(T v) { a /= v; b /= v; return *this; }

		Segment2& operator+=(const Segment2& s) { a += s.a; b += s.b; return *this; }
		Segment2& operator-=(const Segment2& s) { a -= s.a; b -= s.b; return *this; }
		Segment2& operator*=(const Segment2& s) { a *= s.a; b *= s.b; return *this; }
		Segment2& operator/=(const Segment2& s) { a /= s.a; b /= s.b; return *this; }

		bool operator==(const Segment2& s) { return a == s.a && b == s.b; }
		bool operator!=(const Segment2& s) { return a != s.a || b != s.b; }

		friend std::ostream& operator<<(std::ostream& os, const Segment2<T>& s)
		{
			os << "(" << s.a.x << ", " << s.a.y << ")" << " (" << s.b.x << ", " << s.b.y << ")";
			return os;
		}
	};

	// -----------------------------------------------------
	/*
	template<class T, size_t row, size_t col>
	struct Matrix2
	{
		Array2<T, row, col> elems;

	public:
		Vec2<T> operator*(const Vec2<T>& vec) const
		{
			Vec2<T> out;
			for (size_t r = 0; r < row; ++r)
			{
				for (size_t c = 0; c < col; ++c)
				{

				}
			}
		}
		
	};*/

	// Functions ----------------------------------------------

	// square
	template<class T>
	T Sqr(T a) { return a * a; }

	// average
	template<class T>
	double Avg(T a, T b) { return (a + b) / 2; }

	template<class T>
	double Avg(T a, T b, T c) { return (a + b + c) / 3; }

	template<class T>
	double Avg(std::initializer_list<T> ls) { return (std::accumulate(ls.begin(), ls.end(), 0)) / ls.size(); }

	template<class T>
	Vec2D Avg(const Vec2<T>& v1, const Vec2<T>& v2) { return (v1 + v2) * 0.5; }

	// distance sq
	template<class T>
	double DistanceSq(const Vec2<T>& p1, const Vec2<T>& p2)
	{
		return Sqr(p2.x - p1.x) + Sqr(p2.y - p1.y);
	}

	// distance
	template<class T>
	double Distance(const Vec2<T>& p1, const Vec2<T>& p2){ return std::sqrt(DistanceSq(p1, p2)); }
	
	template<class T>
	double Distance(const Segment2<T>& s, const Vec2<T>& p)
	{
		const double a = s.a.y - s.b.y;
		const double b = s.b.x - s.a.x;
		const double c = s.a.x * s.b.y - s.b.x * s.a.y;
		return std::abs(a * p.x + b * p.y + c) / std::sqrt(Sqr(a) + Sqr(b));
	}

	// dot product
	template<class T>
	double Dot(const Vec2<T>& v1, const Vec2<T>& v2) { return v1.Dot(v2); }

	template<class T>
	double Dot(const Vec3<T>& v1, const Vec3<T>& v2) { return v1.Dot(v2); }

	template<class T>
	double Dot(const Vec4<T>& v1, const Vec4<T>& v2) { return v1.Dot(v2); }

	// rotation
	double DegToRad(double d) { return d * (PI / 180); }
	double RadToDeg(double r) { return r * (180 / PI); }

	template<class T>
	void Rotate(Vec2<T>& v, float rad) { v.Rotate(rad); }
	
	template<class T>
	void Rotate90CW(Vec2<T>& v) { v.Rotate90CW(); }

	template<class T>
	void Rotate90CCW(Vec2<T>& v) { v.Rotate90CCW(); }

	//template<class T>
	//double Angle(const Vec2<T>& v) { return std::asin(v.Mag()); }
}