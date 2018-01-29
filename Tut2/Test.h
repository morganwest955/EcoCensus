#pragma once

#ifdef TEST_EXPORTS  
#define TEST_API __declspec(dllexport)   
#else  
#define TEST_API __declspec(dllimport)   
#endif

namespace Test {
	class Functions {
	public:
		static TEST_API double add(double a, double b);

		static TEST_API double addp(double a, double b);
	};
}
