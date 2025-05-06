#pragma once

template <size_t N, typename T>
inline void uniformVecArrayFunc(GLint location, GLsizei count, const glm::vec<N, T>* const data)
{
	if constexpr (N == 2)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform2fv(location, count, data);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform2dv(location, count, data);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform2iv(location, count, data);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform2uiv(location, count, data);
		}
	}

	if constexpr (N == 3)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform3fv(location, count, data);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform3dv(location, count, data);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform3iv(location, count, data);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform3uiv(location, count, data);
		}
	}

	if constexpr (N == 4)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform4fv(location, count, data);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform4dv(location, count, data);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform4iv(location, count, data);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform4uiv(location, count, data);
		}
	}
}

template <size_t N, typename T>
inline void uniformVecFunc(GLint location, glm::vec<N, T> vec)
{
	if constexpr (N == 2)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform2f(location, vec.x, vec.y);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform2d(location, vec.x, vec.y);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform2i(location, vec.x, vec.y);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform2ui(location, vec.x, vec.y);
		}
	}

	if constexpr (N == 3)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform3f(location, vec.x, vec.y, vec.z);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform3d(location, vec.x, vec.y, vec.z);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform3i(location, vec.x, vec.y, vec.z);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform3ui(location, vec.x, vec.y, vec.z);
		}
	}

	if constexpr (N == 4)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniform4f(location, vec.x, vec.y, vec.z, vec.w);
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniform4d(location, vec.x, vec.y, vec.z, vec.w);
		}

		if constexpr (std::is_same_v<T, int>)
		{
			glUniform4i(location, vec.x, vec.y, vec.z, vec.w);
		}

		if constexpr (std::is_same_v<T, uint>)
		{
			glUniform4ui(location, vec.x, vec.y, vec.z, vec.w);
		}
	}
}

template <typename T>
inline void uniformValueFunc(GLint location, T val)
{
	if constexpr (std::is_same_v<T, float>)
	{
		glUniform1f(location, val);
	}

	if constexpr (std::is_same_v<T, double>)
	{
		glUniform1d(location, val);
	}

	if constexpr (std::is_same_v<T, int>)
	{
		glUniform1i(location, val);
	}

	if constexpr (std::is_same_v<T, uint>)
	{
		glUniform1ui(location, val);
	}
}

template <size_t N, typename T>
inline void uniformMatArrayFunc(GLint location, const glm::mat<N,N,T>* const mat, GLsizei count)
{
	if constexpr (N == 2)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniformMatrix2fv(location, count, GL_FALSE, rcast<float>(mat));
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniformMatrix2dv(location, count, GL_FALSE, rcast<double>(mat));
		}
	}

	if constexpr (N == 3)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniformMatrix3fv(location, count, GL_FALSE, rcast<float>(mat));
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniformMatrix3dv(location, count, GL_FALSE, rcast<double>(mat));
		}
	}

	if constexpr (N == 4)
	{
		if constexpr (std::is_same_v<T, float>)
		{
			glUniformMatrix4fv(location, count, GL_FALSE, rcast<float>(mat));
		}

		if constexpr (std::is_same_v<T, double>)
		{
			glUniformMatrix4dv(location, count, GL_FALSE, rcast<double>(mat));
		}
	}
}