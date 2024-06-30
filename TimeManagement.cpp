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

bool TimeManagement::isSameDay(const std::tm& date1, const std::tm& date2)
{
	return date1.tm_year == date2.tm_year &&
		   date1.tm_mon == date2.tm_mon &&
		   date1.tm_mday == date2.tm_mday;
}

bool TimeManagement::isBefore(const std::tm& tm1, const std::tm& tm2)
{
	std::tm tm1Copy = tm1;
	std::tm tm2Copy = tm2;

	std::time_t time1 = std::mktime(&tm1Copy);
	std::time_t time2 = std::mktime(&tm2Copy);

	return time1 < time2;
}
