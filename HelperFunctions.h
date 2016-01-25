#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

/* TODO: Try making this a namespace, and then just including 
 * this header everywhere and using the namespace to call it*/
struct HelperFunctions {
		
	static constexpr double deg(double rad) const {
		return rad/3.14159265*180.0;
	}
	
	static constexpr double rad(double deg) const {
		return deg/180.0*3.14159265;
	}
};

#endif //HELPER_FUNCTIONS_H