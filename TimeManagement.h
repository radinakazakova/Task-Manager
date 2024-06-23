#pragma once
#include <ctime>

namespace TimeManagement
{
	bool operator==(const std::tm& lhs, const std::tm& rhs);
	bool isSameDay(const std::tm& tm1, const std::tm& tm2); 
	bool isBefore(const std::tm& tm1, const std::tm& tm2); 

}