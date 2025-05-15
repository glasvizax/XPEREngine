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

template <typename T, glm::precision P>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<2, T, P>& v)
{
	return os << "(" << v.x << ", " << v.y << ")";
}

template <typename T, glm::precision P>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<3, T, P>& v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

template <typename T, glm::precision P>
inline std::ostream& operator<<(std::ostream& os, const glm::vec<4, T, P>& v)
{
	return os << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")";
}