#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

struct HelperFunctions {
		
	static constexpr double deg(double rad) {
		return rad/3.14159265*180.0;
	}
	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
};

#endif //HELPER_FUNCTIONS_H