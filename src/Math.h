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
#include <variant>

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

	template<class T, size_t size> struct Vec;

	template<class T> using Vec2 = Vec<T, 2>;
	template<class T> using Vec3 = Vec<T, 3>;
	template<class T> using Vec4 = Vec<T, 4>;

	//template<class T> struct Vec2;
	//template<class T> struct Vec3;
	//template<class T> struct Vec4;

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
	/*
	template<class T> void Rotate(Vec2<T>& v, float rad);
	template<class T> void Rotate90CW(Vec2<T>& v);			// clockwise
	template<class T> void Rotate90CCW(Vec2<T>& v);			// counterclockwise
	*/
	

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

		// used for deducing what type the resulting structure will hold,
		// it prioritizes floating types and types with higher precision.
		// if returns true - T, else - C
		template<class T, class C>
		constexpr bool ElemType()
		{
			if constexpr (std::is_same<T, C>::value)
			{
				if constexpr (sizeof(T) > sizeof(C)) return true;
				else return false;
			}
			else
			{
				if constexpr (std::is_floating_point<T>::value && !(std::is_floating_point<C>::value)) return true;
				if constexpr (std::is_floating_point<C>::value && !(std::is_floating_point<C>::value)) return false;
				if constexpr (sizeof(T) > sizeof(C)) return true;
				else return false;
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
		Vec<double, size> Normalize() const { double mag = Mag(); return (mag == 0.0) ? (Vec<double, size>)(*this) : (Vec<double, size>)((*this) / mag); }

	public:
		template<class NT>
		operator Vec<NT, size>() const 
		{
			Vec<NT, size> out;
			helper::CopyCastArray(this->elems, out.elems);
			return out;
		}

		T& operator[](uint16_t i) { return elems[i]; }
		const T& operator[](uint16_t i) const { return elems[i]; }

		template<class C>
		friend Vec<C, size> operator+(const Vec<T, size>& lhs, const C& rhs)
		{ 
			Vec<C, size> out = (Vec<C, size>)lhs; 
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e += rhs; }); 
			return out; 
		}
		template<class C>
		friend Vec<C, size> operator-(const Vec<T, size>&lhs, const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)lhs;
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e -= rhs; });
			return out;
		}
		template<class C>
		friend Vec<C, size> operator*(const Vec<T, size>& lhs, const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)lhs;
			std::for_each(out.elems.begin(), out.elems.end(), [&](C& e) { e *= rhs; });
			return out;
		}
		template<class C>
		friend Vec<C, size> operator/(const Vec<T, size>& lhs, const C& rhs)
		{
			Vec<C, size> out = (Vec<C, size>)lhs;
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
		friend auto operator+(const Vec<T, size>& lhs, const Vec<C, size2>& rhs)
		{
			if constexpr (size2 > size)
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] += lhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] += lhs.elems[i];
					}
					return out;
				}
			}
			else
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] += rhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] += rhs.elems[i];
					}
					return out;
				}
			}
		}

		template<class C, size_t size2>
		friend auto operator-(const Vec<T, size>& lhs, const Vec<C, size2>& rhs)
		{
			if constexpr (size2 > size)
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] -= lhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] -= lhs.elems[i];
					}
					return out;
				}
			}
			else
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] -= rhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] -= rhs.elems[i];
					}
					return out;
				}
			}
		}

		template<class C, size_t size2>
		friend auto operator*(const Vec<T, size>& lhs, const Vec<C, size2>& rhs)
		{
			if constexpr (size2 > size)
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] *= lhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] *= lhs.elems[i];
					}
					return out;
				}
			}
			else
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] *= rhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] *= rhs.elems[i];
					}
					return out;
				}
			}
		}

		template<class C, size_t size2>
		friend auto operator/(const Vec<T, size>& lhs, const Vec<C, size2>& rhs)
		{
			if constexpr (size2 > size)
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] /= lhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size2 > out;
					helper::CopyCastArray(rhs.elems, out.elems);
					for (size_t i = 0; i < size; i++)
					{
						out.elems[i] /= lhs.elems[i];
					}
					return out;
				}
			}
			else
			{
				if constexpr (helper::ElemType<T, C>())
				{
					Vec<T, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] /= rhs.elems[i];
					}
					return out;
				}
				else
				{
					Vec<C, size> out;
					helper::CopyCastArray(lhs.elems, out.elems);
					for (size_t i = 0; i < size2; i++)
					{
						out.elems[i] /= rhs.elems[i];
					}
					return out;
				}
			}
		}

		friend std::ostream& operator<<(std::ostream& os, const Vec<T, size>& v)
		{
			for (size_t i = 0; i < v.elems.size(); ++i) os << v.elems[i] << ((i == (v.elems.size() - 1)) ? "" : ", ");
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
		Segment2(T ax, T ay, T bx, T by) : a(Vec2<T>({ ax, ay })), b(Vec2<T>({ bx, by })) {}
		Segment2(const Segment2& src) : a(src.a), b(src.b) {}

		double Len() const { return Distance(a, b); }
		double LenSq() const { return DistanceSq(a, b); }

		double CenterA() const { return Avg(a[0], a[1]); }
		double CenterB() const { return Avg(b[0], b[1]); }
		Vec2D Center() const { return Avg(a, b); }

		Vec2D Delta() const { return b - a; }
		double DeltaX() const { return b[0] - a[0]; }
		double DeltaY() const { return b[1] - a[1]; }
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
			os << "(" << s.a[0] << ", " << s.a[1] << ")" << " (" << s.b[0] << ", " << s.b[1] << ")";
			return os;
		}
	};
	

	// -----------------------------------------------------
	
	template<class T, size_t row, size_t col>
	struct Matrix2
	{
		Array2<T, row, col> elems;

	public:
		Matrix2() { for (auto& r : elems) r.fill((T)0); };
		Matrix2(const std::initializer_list<std::initializer_list<T>>& ls)
		{
			for (size_t i = 0; i < ls.size(); ++i)
			{
				auto r = *(ls.begin() + i);
				for (size_t j = 0; j < r.size(); j++)
				{
					elems[i][j] = *(r.begin() + j);
				}
			}
		}

		static Matrix2<T, row, col> Scale(T factor)
		{
			Matrix2<T, row, col> out;
			for (size_t r = 0; r < row; ++r)
			{
				out.elems[r][r] = (T)factor;
			}
			return out;
		}
		static Matrix2<T, row, col> Identity()
		{
			return Scale((T)1);
		}
		static Matrix2<T, 2, 2> FlipY2()
		{
			return { {1,0},{0,-1} };
		}
		static Matrix2<T, 2, 2> Rotation2(float ang)
		{
			return { {std::cos(ang),-std::sin(ang)},{std::sin(ang),std::count(ang)} };
		}

	public:
		template<class C>
		friend auto operator*(const Matrix2<T, row, col>& mat, const Vec<C, row>& vec)
		{
			auto out = (helper::ElemType<T, C>()) ? Vec<T, row>() : Vec<C, row>();
			for (size_t i = 0; i < row; i++)
			{
				out.elems[i] = Vec<T, col>(mat.elems[i]).Dot(vec);
			}
			return out;
		}
		
		friend std::ostream& operator<<(std::ostream& os, const Matrix2<T, row, col>& m)
		{
			for (size_t r = 0; r < row; ++r)
			{
				os << "[";
				for (size_t c = 0; c < col; ++c)
				{
					os << m.elems[r][c] << ((c == (col - 1)) ? "" : ", ");
				}
				os << "]\n";
			}
			return os;
		}
	};

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
		return Sqr(p2[0] - p1[0]) + Sqr(p2[1] - p1[1]);
	}

	// distance
	template<class T>
	double Distance(const Vec2<T>& p1, const Vec2<T>& p2){ return std::sqrt(DistanceSq(p1, p2)); }
	
	template<class T>
	double Distance(const Segment2<T>& s, const Vec2<T>& p)
	{
		const double a = s.a[1] - s.b[1];
		const double b = s.b[0] - s.a[0];
		const double c = s.a[0] * s.b[1] - s.b[0] * s.a[1];
		return std::abs(a * p[0] + b * p[1] + c) / std::sqrt(Sqr(a) + Sqr(b));
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
	/*
	template<class T>
	void Rotate(Vec2<T>& v, float rad) { v.Rotate(rad); }
	
	template<class T>
	void Rotate90CW(Vec2<T>& v) { v.Rotate90CW(); }

	template<class T>
	void Rotate90CCW(Vec2<T>& v) { v.Rotate90CCW(); }
	*/

	//template<class T>
	//double Angle(const Vec2<T>& v) { return std::asin(v.Mag()); }
}