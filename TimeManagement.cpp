#include "TimeManagement.h"

bool TimeManagement::operator==(const std::tm& lhs, const std::tm& rhs)
{
	return (lhs.tm_year == rhs.tm_year) &&
		(lhs.tm_mon == rhs.tm_mon) &&
		(lhs.tm_mday == rhs.tm_mday) &&
		(lhs.tm_hour == rhs.tm_hour) &&
		(lhs.tm_min == rhs.tm_min) &&
		(lhs.tm_sec == rhs.tm_sec) &&
		(lhs.tm_wday == rhs.tm_wday) &&
		(lhs.tm_yday == rhs.tm_yday) &&
		(lhs.tm_isdst == rhs.tm_isdst);

}

bool TimeManagement::isSameDay(const std::tm& tm1, const std::tm& tm2)
{
	return (tm1.tm_year == tm2.tm_year) &&
		(tm1.tm_mon == tm2.tm_mon) &&
		(tm1.tm_mday == tm2.tm_mday);
}

bool TimeManagement::isBefore(const std::tm& tm1, const std::tm& tm2)
{
	std::tm tm1Copy = tm1;
	std::tm tm2Copy = tm2;

	std::time_t time1 = std::mktime(&tm1Copy);
	std::time_t time2 = std::mktime(&tm2Copy);

	return time1 < time2;
}
