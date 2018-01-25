#pragma once

#ifdef ECPY_EXPORTS  
#define ECPY_API __declspec(dllexport)   
#else  
#define ECPY_API __declspec(dllimport)   
#endif

/*
	You'll need to include this header in the project you want to use the libraries with
	as well as pointing it to the library folder
	so that it knows the function signatures
*/

namespace ECPy {
	class Functions {
	public:
		static ECPY_API int predictions(char* dir);

		static ECPY_API int image_partition(char* dir);

		static ECPY_API int train(char* dir, int iters);

	};
}
