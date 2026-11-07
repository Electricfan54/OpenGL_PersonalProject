#pragma once
#include "../Utility/Math.h"

class material
{
protected:
	float ambientStrength;
	float specularStrength;
	int specularPow;
	Math::vector4 color;
public:
	material();

	//Getters
	const float& SpecularStrength() const
	{
		return specularStrength;
	}

	const int& SpecularPow() const
	{
		return specularPow;
	}

	const float& AmbientStrength() const
	{
		return ambientStrength;
	}

	const Math::vector4& Color() const
	{
		return color;
	}

	//Setters
	void SpecularStrength(const float& value)
	{
		specularStrength = value;
	}

	void SpecularPow(const int& value)
	{
		specularPow = value;
	}

	void AmbientStrength(const float& value)
	{
		ambientStrength = value;
	}

	void Color(const Math::vector4 _Color)
	{
		color = _Color;
	}
};

