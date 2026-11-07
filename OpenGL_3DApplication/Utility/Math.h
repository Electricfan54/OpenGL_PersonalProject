#pragma once
#include <iostream>
#include <math.h>

namespace Math
{

	static void Clamp(float& val, const float& min, const float& max)
	{
		val = val > min ? val : min;
		val = val < max ? val : max;
	}

	struct float2
	{
		float x, y;

		float2 operator/(const float& other) const
		{
			return { x / other, y / other };
		}

		void operator/=(const float& other)
		{
			x = x / other;
			y = y / other;
		}

		float2 operator*(const float& other) const
		{
			return { x * other, y * other };
		}

		void operator*=(const float& other)
		{
			x = x * other;
			y = y * other;
		}


		float operator*(const float2& other) const
		{
			return x * other.x + y * other.y;
		}

		float2 Normalized()
		{
			float norm = x * x + y * y;
			norm = sqrtf(norm);
			return *this / norm;
		}
	};

	struct double2
	{
		double x, y;
	};

	struct vector3
	{
		float x, y, z;

		vector3()
			:x(0), y(0), z(0)
		{
		}

		vector3(const vector3& other)
			:x(other.x), y(other.y), z(other.z)
		{
		}

		vector3(float x, float y, float z)
			:x(x), y(y), z(z)
		{}

		vector3 operator*(const float& scalar) const
		{
			return vector3(x * scalar, y * scalar, z * scalar);
		}

		float operator*(const vector3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}

		vector3 operator/(const float& scalar) const
		{
			return vector3(x / scalar, y / scalar, z / scalar);
		}

		vector3 operator-(const vector3& other) const
		{
			return vector3{ x - other.x,y - other.y,z - other.z };
		}

		vector3 operator+(const vector3& other) const
		{
			return vector3{ x + other.x,y + other.y,z + other.z };
		}

		void operator+=(const vector3& other)
		{
			x += other.x;
			y += other.y;
			z += other.z;
			//*this = { x + other.x,y + other.y,z + other.z };
		}

		void operator-=(const vector3& other)
		{
			*this = { x - other.x,y - other.y,z - other.z };
		}

		vector3& operator=(vector3 other)
		{
			this->x = other.x;
			this->y = other.y;
			this->z = other.z;
			return *this;
		}

		bool operator==(const vector3& other) const
		{
			if (x == other.x && y == other.y && z == other.z)
				return true;
			return false;
		}

		bool operator()(const vector3& a, const vector3& b) const
		{
			return a == b;
		}

		vector3 Normalized() const
		{
			float norm = x * x + y * y + z * z;
			norm = sqrtf(norm);
			return *this / norm;
		}

		static vector3 Cross(const vector3& v1, const vector3& v2)
		{
			return { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };
		}

		float Magnitude() const
		{
			return sqrtf(x * x + y * y + z * z);
		}

		float MagnitudeSquared() const
		{
			return x * x + y * y + z * z;
		}

		static Math::vector3 Forward()
		{
			return Math::vector3(0, 0, 1);
		}

		static Math::vector3 Up()
		{
			return Math::vector3(0, 1, 0);
		}

		static Math::vector3 Right()
		{
			return Math::vector3(1, 0, 0);
		}

	};

	struct int2
	{
		int x, y;

		int2 operator/(const int& other) const
		{
			return { x / other, y / other };
		}

		int2 operator*(const int& other) const
		{
			return { x * other, y * other };
		}


		int operator*(const int2& other) const
		{
			return x * other.x + y * other.y;
		}
	};
	
	struct int3
	{
		int x, y, z;

		int3 operator/(const int& other) const
		{
			return { x / other, y / other, z / other };
		}

		int3 operator*(const int& other) const
		{
			return { x * other, y * other, z * other };
		}


		int operator*(const int3& other) const
		{
			return x * other.x + y * other.y + z * other.z;
		}
	};


	struct mat4f
	{
		float matrix[16];

		mat4f()
		{
			for (size_t i = 0; i < 16; i++)
			{
				matrix[i] = 0;
			}
		}

		float& operator[](const int& i)
		{
			return matrix[i];
		}

		const float& operator[](const int& i)const
		{
			return matrix[i];
		}

		float& at(int row, int col)
		{
			return matrix[row * 4 + col];
		}

		const float& at(int row, int col)const
		{
			return matrix[row * 4 + col];
		}


		mat4f operator*(const mat4f& other)const
		{
			mat4f newMatrix;

			for (size_t i = 0; i < 16; i++)
			{
				for (size_t x = 0; x < 4; x++)
				{
					newMatrix[i] += at(i/4,x) * other.at(x,i%4);
				}
			}
			return newMatrix;
		}

		mat4f operator*(const float& scalar)const
		{
			mat4f newMatrix;

			for (size_t i = 0; i < 16; i++)
			{
				newMatrix[i] = matrix[i] * scalar;
			}
			return newMatrix;
		}

		mat4f Transposed()const
		{
			mat4f newMatrix;
			int x = 0;
			for (size_t i = 0; i < 16; i++)
			{
				x = i / 4;
				newMatrix.at(i%4,x) = matrix[i];
			}
			return newMatrix;
		}

		mat4f Inversed()const
		{
			mat4f newMatrix = *this;

			float a = at(0, 0);
			float b = at(0, 1);
			float c = at(0, 2);
			float d = at(1, 0);
			float e = at(1, 1);
			float f = at(1, 2);
			float g = at(2, 0);
			float h = at(2, 1);
			float i = at(2, 2);

			float det = a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
			det = 1.0f / det;

			newMatrix.at(0, 0) = at(1, 1) * at(2, 2) - at(1, 2) * at(2, 1);
			newMatrix.at(1, 0) = -(d*i -f*g);
			newMatrix.at(2, 0) = d*h-e*g;

			newMatrix.at(0, 1) = -(b*i-c*h);
			newMatrix.at(1, 1) = a*i-c*g;
			newMatrix.at(2, 1) = -(a*h-b*g);

			newMatrix.at(0, 2) = b*f-c*e;
			newMatrix.at(1, 2) = -(a*f-c*d);
			newMatrix.at(2, 2) = a * e - b * d;

			for (int y = 0; y < 3; y++)
			{
				for (int x = 0; x < 3; x++)
				{
					newMatrix.at(y, x) = newMatrix.at(y, x) * det;
				}
			}

			return newMatrix;
		}

	};

	struct vector4
	{
		float x, y, z, w;

		vector4()
			: x(0), y(0), z(0), w(0)
		{
		}

		vector4(const float& x, const float& y, const float& z, const float& w)
			: x(x), y(y), z(z), w(w)
		{
		}

		vector4(const vector3& vec)
			: x(vec.x), y(vec.y), z(vec.z), w(1)
		{
		}

		vector4 operator*(const mat4f& matrix) const
		{
			vector4 newVec;

			newVec.x = matrix.at(0, 0) * x + matrix.at(0, 1) * y + matrix.at(0, 2) * z + matrix.at(0, 3) * w;
			newVec.y = matrix.at(1, 0) * x + matrix.at(1, 1) * y + matrix.at(1, 2) * z + matrix.at(1, 3) * w;
			newVec.z = matrix.at(2, 0) * x + matrix.at(2, 1) * y + matrix.at(2, 2) * z + matrix.at(2, 3) * w;
			newVec.w = matrix.at(3, 0) * x + matrix.at(3, 1) * y + matrix.at(3, 2) * z + matrix.at(3, 3) * w;

			return newVec;
		}

	};


	struct quaternion
	{
		float w,x, y, z;

		quaternion()
			: x(0), y(0), z(0), w(1)
		{
		}

		quaternion(float _w, float _x, float _y, float _z)
			: x(_x), y(_y), z(_z), w(_w)
		{
		}

		quaternion(const vector3& vec)
			: x(vec.x), y(vec.y), z(vec.z), w(0)
		{
		}

		quaternion operator*(const quaternion& other) const
		{
			vector3 temp = {x,y,z};
			vector3 temp2 = {other.x,other.y,other.z};
			vector3 temp3 = temp2*w + temp*other.w + vector3::Cross(temp,temp2);

			return quaternion(w * other.w - temp * temp2, temp3.x, temp3.y, temp3.z);
		}

		quaternion operator+(const quaternion& other) const
		{
			return quaternion(w + other.w, x + other.x, y + other.y, z + other.z);
		}

		quaternion operator/(const float& scalar) const
		{
			return { w / scalar,x / scalar,y / scalar,z / scalar };
		}

		float Norm()const
		{
			return w * w + x * x + y * y + z * z;
		}

		quaternion Conjugate()const
		{
			return { w,-x,-y,-z };
		}

		quaternion Inverse()const
		{
			return Conjugate() / Norm();
		}

	};

	static mat4f IdentityMatrix()
	{
		mat4f matrix;
		matrix.at(0, 0) = 1;
		matrix.at(1, 1) = 1;
		matrix.at(2, 2) = 1;
		matrix.at(3, 3) = 1;
		return matrix;
	}

	static mat4f TranslationMatrix(float x, float y, float z)
	{
		mat4f matrix = IdentityMatrix();
		matrix.at(0, 3) = x;
		matrix.at(1, 3) = y;
		matrix.at(2, 3) = z;
		return matrix;
	}

	static mat4f TranslationMatrix(const vector3& pos)
	{
		mat4f matrix = IdentityMatrix();
		matrix.at(0, 3) = pos.x;
		matrix.at(1, 3) = pos.y;
		matrix.at(2, 3) = pos.z;
		return matrix;
	}

	static mat4f ProjectionMatrix(float far, float near, float fov, float aspect)
	{
		mat4f matrix;
		matrix.at(0, 0) = 1/(aspect* tanf(fov/2));
		matrix.at(1, 1) = 1/ tanf(fov/2);
		matrix.at(2, 2) = -(far+near)/(far - near);
		matrix.at(2, 3) = -(2*far*near)/(far-near);
		matrix.at(3, 2) = -1;
		return matrix;
	}

	static mat4f RotationMatrix(quaternion rotation)
	{
		mat4f rotMatrix;

		float x = rotation.x;
		float y = rotation.y;
		float z = rotation.z;
		float w = rotation.w;

		float m = x * x + y * y + z * z + w * w;
		m = sqrtf(m);

		float M = 2 * m;

		rotMatrix.at(0, 0) = 1 - M * (y * y + z * z);
		rotMatrix.at(0, 1) = M * (x * y - z * w);
		rotMatrix.at(0, 2) = M * (x * z + y * w);

		rotMatrix.at(1, 0) = M * (x * y + z * w);
		rotMatrix.at(1, 1) = 1 - M * (x * x + z * z);
		rotMatrix.at(1, 2) = M * (y * z - x * w);

		rotMatrix.at(2, 0) = M * (x * z - y * w);
		rotMatrix.at(2, 1) = M * (y * z + x * w);
		rotMatrix.at(2, 2) = 1 - M * (x * x + y * y);

		rotMatrix.at(3, 3) = 1;

		return rotMatrix;
	}

	static mat4f ScaleMatrix(const float& x, const float& y, const float& z)
	{
		mat4f scaleMatrix = IdentityMatrix();
		scaleMatrix.at(0, 0) = x;
		scaleMatrix.at(1, 1) = y;
		scaleMatrix.at(2, 2) = z;
		return scaleMatrix;
	}

	static mat4f ScaleMatrix(const vector3& scale)
	{
		mat4f scaleMatrix = IdentityMatrix();
		scaleMatrix.at(0, 0) = scale.x;
		scaleMatrix.at(1, 1) = scale.y;
		scaleMatrix.at(2, 2) = scale.z;
		return scaleMatrix;
	}

};

// Overloading the std::hash function to allow Math::vector3 to be used as a map key
template<>
struct std::hash<Math::vector3>
{
	std::size_t operator()(const Math::vector3& other) const noexcept
	{
		size_t hash = std::hash<float>{}(other.x);
		hash += std::hash<float>{}(hash + other.y);
		hash += std::hash<float>{}(hash + other.z);

		return hash;
	}
};