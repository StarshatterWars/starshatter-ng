#include <gtest/gtest.h>

#include <iostream>

#include <InfoEx.h>


TEST(InfoEx, ShortDescription)
{
	std::cout << InfoEx::ShortDescription() << std::endl;
}


TEST(InfoEx, LongDescription)
{
	std::cout << InfoEx::LongDescription() << std::endl;
}
