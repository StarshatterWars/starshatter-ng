#include <gtest/gtest.h>

#include <iostream>

#include <InfoEx.h>


TEST(InfoEx, DirectX)
{
	std::cout << static_cast<int>(InfoEx::DirectX()) << std::endl;
}
