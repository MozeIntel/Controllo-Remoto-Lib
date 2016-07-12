#pragma once

#include <cstdint>
#include <chrono>
#include <type_traits>

namespace crlib
{

enum class MONTH : uint16_t
{
	JANUARY,
	FEBRUARY,
	MARCH,
	APRIL,
	MAY,
	JUNE,
	JULY,
	AUGUST,
	SEPTEMBER,
	OCTOBER,
	NOVEMBER,
	DECEMBER,
	INVALID
};

enum class DAY : uint16_t
{
	SUNDAY,
	MONDAY,
	TUESDAY,
	WEDNESDAY,
	THURSDAY,
	FRIDAY,
	SATURDAY,
	INVALID
};


struct time
{
	time();

	uint16_t year;
	MONTH month;
	DAY day_of_week;
	uint16_t day;
	uint16_t hour;
	uint16_t minute;
	uint16_t second;
	uint16_t millis;
};


typedef std::chrono::nanoseconds nanos;
typedef std::chrono::microseconds micros;
typedef std::chrono::milliseconds millis;
typedef std::chrono::seconds secs;

template<typename T> inline constexpr bool is_duration() { return false; }
template<> inline constexpr bool is_duration<nanos>() { return true; }
template<> inline constexpr bool is_duration<micros>() { return true; }
template<> inline constexpr bool is_duration<millis>() { return true; }
template<> inline constexpr bool is_duration<secs>() { return true; }


template<typename T = millis>
uint64_t time_stamp()
{
	static_assert(is_duration<T>(), "Type must be std::chrono::duration");
	return std::chrono::duration_cast<T>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

template<typename T, typename V = secs>
uint64_t duration_cast(uint64_t value)
{
	return std::chrono::duration_cast<T>(V(value)).count();
}


} //namespace