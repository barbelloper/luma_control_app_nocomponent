#include <iostream>

auto add(auto x, auto y)
{
	return x+y;
}

int main(void)
{
	std::cout<<add(1.2,3)<<std::endl;
}