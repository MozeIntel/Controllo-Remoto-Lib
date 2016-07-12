#pragma once

#include <cstdint>
#include <limits>
#include <random>
#include <type_traits>

namespace crlib
{

/*
Helper class for simple RNG. The type T must be either 
a 16/32/64 bit integer or a float/double/long double.
8 bit integers are not supported.
*/
template<typename T = uint32_t>
class random
{
	static_assert(std::is_arithmetic<T>::value, "Type must arithmetic");
	typedef typename std::conditional<std::is_floating_point<T>::value, std::uniform_real_distribution<T>, std::uniform_int_distribution<T>>::type UniformDistribution;

public:

	//Create a RNG with [min(T), max(T)] limits (inclusive).
	random();
	//Create a RNG with [min(T), max] limits (inclusive).
	random(T max);
	//Create a RNG with [min, max] limits (inclusive).
	random(T min, T max);
	random(const random&) = delete;
	random(random&&) = delete;

	T get();

private:
	std::random_device m_rand_device;
	std::mt19937 m_rand;
	UniformDistribution m_distribution;
};



template<typename T>
inline random<T>::random() :
	random(std::numeric_limits<T>::min(), std::numeric_limits<T>::max())
{
}

template<typename T>
inline random<T>::random(T max) :
	random(std::numeric_limits<T>::min(), max)
{
}

template<typename T>
inline random<T>::random(T min, T max) :
	m_rand_device(),
	m_rand(m_rand_device()),
	m_distribution(min, max)
{
}

template<typename T>
inline T random<T>::get()
{
	return m_distribution(m_rand);
}

} //namespace