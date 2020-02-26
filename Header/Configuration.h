#include "fakeDetect.h"

class Configuration
{
	public:
					Configuration	();
		virtual 	~Configuration	();

		int 		debug;
    	int 		blur_uv_ksize;
    	int 		blur_ir_ksize;
    	float 		frame_percent;
		std::string confPath;
};