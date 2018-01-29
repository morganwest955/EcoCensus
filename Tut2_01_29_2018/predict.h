#pragma once

#ifdef PREDICT_EXPORTS  
#define PREDICT_API __declspec(dllexport)   
#else  
#define PREDICT_API __declspec(dllimport)   
#endif

namespace Predict {
	class Functions {
	public:
		static PREDICT_API int predictions(char* dir);
	};
}
