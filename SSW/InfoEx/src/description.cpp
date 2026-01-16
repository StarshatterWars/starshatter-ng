#include <InfoEx.h>

#include <infoware/infoware.hpp>

#include <Text.h>


namespace
{


static constexpr const char* prefixes[] {"", "k", "M", "G", "T", "P", "E", "Z", "Y", "R", "Q"};


Text
HumanReadable(long double value, const char* unit)
{
	int magnitude = 0;
	while (value > 1000) {
		value /= 1000;
		magnitude++;
	}
	return Text::format("%.2Lf %s%s", value, prefixes[magnitude], unit);
}


}  // namespace


namespace InfoEx
{


Text
ShortDescription()
{
	return iware::system::OS_info().full_name.data();
}


Text
LongDescription()
{
	return Text::format(
		"OS: %s\nPhysical memory: %s\nCPU: %s\nCPU frequency: %s",
		iware::system::OS_info().full_name.data(),
		HumanReadable(iware::system::memory().physical_total, "B").data(),
		iware::cpu::model_name().data(),
		HumanReadable(iware::cpu::frequency(), "Hz").data());
}


}  // namespace InfoEx
