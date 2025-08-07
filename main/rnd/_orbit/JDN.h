#pragma once

#include <ctime>
#include <rnd/_orbit/Defines.h>
#include <rnd/_orbit/Types.h>
#include <rnd/_orbit/Constants.h>

ORBIT_BEGIN

/// JDN
///
/// 
struct JDN
{
	~JDN() = delete;

	static scalar_t from(Date const &date, Time const &time = Time())
	{
		int32_t  year = (2 < date.month) ? date. year : date. year -  1;
		int32_t month = (2 < date.month) ? date.month : date.month + 12;
		int32_t   day = date.day;

		int32_t A = year / 100;
		int32_t B = 2 - A + (A / 4);

		scalar_t frac = (3600.0 * time.hour + 60.0 * time.minute + time.second) / constants::seconds_per_day;

		return int32_t(365.25 * (year + 4716)) + int32_t(30.6001 * (month + 1)) + day + B - 1524.5 + frac;
	}

	static scalar_t now()
	{
		std::time_t localTime = std::time(nullptr);
		std::tm utcTime;

		::gmtime_s(&utcTime, &localTime);

		Date date { utcTime.tm_year + 1900, utcTime.tm_mon + 1, utcTime.tm_mday };
		Time time { utcTime.tm_hour, utcTime.tm_min, scalar_t(utcTime.tm_sec) };

		return from(date, time);
	}

	static scalar_t toCentury2000(scalar_t jdn)
	{
		return (jdn - constants::J2000) / constants::days_per_century;
	}

};

ORBIT_END
