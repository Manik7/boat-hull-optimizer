#ifndef HELPER_FUNCTIONS_H
#define HELPER_FUNCTIONS_H

/* TODO: Try making this a namespace, and then just including 
 * this header everywhere and using the namespace to call it*/
namespace helper {
		
	static constexpr double deg(double rad) {
		return rad/3.14159265*180.0;
	}
	
	static constexpr double rad(double deg) {
		return deg/180.0*3.14159265;
	}
}

#endif //HELPER_FUNCTIONS_H