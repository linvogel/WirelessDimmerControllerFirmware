#include "util/custom_array.hpp"

#include "logging.hpp"

#include <iostream>

using namespace dim;

int main()
{
	dim::log::init();
	
	custom_array<int> arr1;
	debug("step 1");
	arr1.resize(8);
	debug("step 2");
	arr1.resize(16);
	debug("step 3");
	custom_array<int> arr2(arr1);
	debug("step 4");
	
	custom_array<int> arr3;
	arr3 = custom_array<int>(arr2);
	
	info("Location 2: %02d %02d %02d", arr1[2], arr2[2], arr3[2]);
	arr2[2] = 42;
	info("Location 2: %02d %02d %02d", arr1[2], arr2[2], arr3[2]);
	arr1[2] = 0;
	info("Location 2: %02d %02d %02d", arr1[2], arr2[2], arr3[2]);
	
}