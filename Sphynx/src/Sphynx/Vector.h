#pragma once

namespace Sphynx {
	template<typename T = float>
	struct Vector4{
		T x, y, z, w;
		//Must be nullable
		Vector4(T _x, T _y, T _z, T _w) :x(_x), y(_y), z(_z), w(_w) {};
	};
	template<>
	struct Vector4<int>{
		union
		{
			struct { int x, y, z, w; };
			struct { int r, g, b, a; };
		};
		Vector4(int _x, int _y, int _z, int _w) :x(_x), y(_y), z(_z), w(_w) {};
	};
	template<>
	struct Vector4<float>{
		union
		{
			struct { float x, y, z, w; };
			struct { float r, g, b, a; };
		};
		Vector4(float _x, float _y, float _z, float _w) :x(_x), y(_y), z(_z), w(_w) {};
	};
	template<>
	struct Vector4<double>{
		union
		{
			struct { double x, y, z, w; };
			struct { double r, g, b, a; };
		};
		Vector4(double _x, double _y, double _z, double _w) :x(_x), y(_y), z(_z), w(_w) {};

	};
	template<typename T = float>
	struct Vector3 {
		T x, y, z;
		Vector3(T _x, T _y, T _z) :x(_x), y(_y), z(_z) {};
		operator Vector4<T>() {
			return Vector4<T>(x, y, z, NULL);
		}
	};
	template<>
	struct Vector3<float> {
		union {
			struct { float x, y, z; };
			struct { float r, g, b; };
		};
		Vector3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};
	template<>
	struct Vector3<int> {
		union {
			struct { int x, y, z; };
			struct { int r, b, g; };
		};
		Vector3(int _x, int _y, int _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};
	template<>
	struct Vector3<double> {
		union {
			struct { double x, y, z; };
			struct { double r, b, g; };
		};
		Vector3(double _x, double _y, double _z) {
			x = _x;
			y = _y;
			z = _z;
		}
	};
	typedef Vector3<> Vec3;
	template<typename T = float>
	struct Vector2 {
		T x, y;
		operator Vector3<T>() {
			return Vector3<T>(x, y, NULL);
		}
		operator Vector4<T>() {
			return Vector4<T>(x, y, NULL, NULL);
		}
	};
	template<>
	struct Vector2<int> {
		int x, y;
	};
	template<>
	struct Vector2<float> {
		float x, y;
		Vector2(float _x, float _y) { x = _x; y = _y; };
	};
	typedef Vector2<float> Vec2;
}