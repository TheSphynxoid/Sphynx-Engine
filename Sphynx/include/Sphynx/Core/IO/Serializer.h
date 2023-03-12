#pragma once

namespace Sphynx {

	struct IOBuffer {
		size_t Size;
		union {
			char* data;
			void* pointer;
		};
	};
	template<class T>
	class Serializer {
	public:
		static std::vector<IOBuffer>& Serialize(T* obj) = 0;
		static T* Deserialize(std::vector<IOBuffer> data) = 0;
		static bool IsComposite() = 0;
	};
	template<>
	class Serializer<int> {
	public:
		static std::vector<IOBuffer> Serialize(int* obj) {
			IOBuffer b = { sizeof(int),(char*)(obj) };
			return std::vector<IOBuffer>(1, b);
		};
		static int* Deserialize(std::vector<IOBuffer> data) {
			return ((int*)data[0].pointer);
		}
		static bool IsComposite() { return false; };

	};
	template<>
	class Serializer<double> {
	public:
		static std::vector<IOBuffer> Serialize(double* obj) {
			IOBuffer b = { sizeof(double),(char*)(obj) };
			return std::vector<IOBuffer>(1, b);
		};
		static double* Deserialize(std::vector<IOBuffer> data) {
			return ((double*)data[0].pointer);
		}
		static bool IsComposite() { return false; };
	};
	template<>
	class Serializer<float> {
	public:
		static std::vector<IOBuffer> Serialize(float* obj) {
			IOBuffer b = { sizeof(float), (char*)(obj) };
			return std::vector<IOBuffer>(1, b);
		};
		static float* Deserialize(std::vector<IOBuffer> data) {
			return ((float*)data[0].pointer);
		}
		static bool IsComposite() { return false; };
	};
}