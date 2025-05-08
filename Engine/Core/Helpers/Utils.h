#pragma once

template <typename T> 
inline bool isInRange(T a, T b, T value)
{
	if (b > a)
	{
		return value >= a && value <= b;
	} 
	else
	{
		return value >= b && value <= a;
	}
}
