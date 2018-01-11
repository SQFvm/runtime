#include "full.h"


sqf::arraydata::arraydata()
{
}
template<size_t SIZE> sqf::arraydata::arraydata(std::array<value_s, SIZE>)
{
}

template<size_t SIZE> sqf::arraydata::operator std::array<value_s, SIZE>() const
{

}

std::string sqf::arraydata::to_string(void) const
{
	return std::string();
}


sqf::value_s & sqf::arraydata::operator[](int i)
{
	return mvalue.at(i);
}

sqf::value_s sqf::arraydata::operator[](int i) const
{
	return mvalue[i];
}
