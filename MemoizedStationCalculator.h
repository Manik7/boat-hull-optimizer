#ifndef MEMOIZED_STATION_CALCULATOR_H
#define MEMOIZED_STATION_CALCULATOR_H

#include <cassert>
#include <unordered_map>
#include <utility>
// #include <boost/functional/hash.hpp>

#include "StationCalculator.h"

template<typename CoordType, typename HullParameters>
class MemoizedStationCalculator : public StationCalculator<CoordType, HullParameters> {	
// 	struct InputParameters {
// 		CoordType beam_x, chine_x, chine_y, keel_y;
// 		InputParameters(CoordType beam_x, CoordType chine_x, CoordType chine_y, CoordType keel_y) : beam_x(beam_x), chine_x(chine_x), chine_y(chine_y), keel_y(keel_y) {}
// 	};
	
	/*TODO: whereby InputParameters is some sort of struct containing (beam_x, chine_x, chine_y, keel_y) 
	 * for the calculator and the int is a counter storing the iteration when the key was last retrieved*/
// 	using Key = InputParameters; 
	using Key = int; 
	using T = std::pair<StationProperties, int>;
	mutable std::unordered_map<Key, T> map; //TODO: fix the size of the map to some constant size
	
	static constexpr int target_map_size = 1000;
	static constexpr int insertions_until_cleanup = 100; //the number of insertions which can be performed before cleanup is invoked
	static constexpr int maximum_allowed_age = 3;
	mutable int insertion_counter = 0; // increment this value every time a new station is added to the map
	mutable int iteration_no = 0; //the current iteration i.e. time step
	
public:
	
	MemoizedStationCalculator(HullParameters& hullparam) : StationCalculator<CoordType, HullParameters>(hullparam) {}
	
	void increment_iteration_no() {++iteration_no;}
	
	/*if element in map, return StationProperties and set its last access time to the current iteration. 
	 *If element not in map, calculate it, and insert it into the map (with current iteration as last 
	 * access time)*/
	StationProperties calculate_station_properties(int beam_x, int chine_x, int chine_y, int keel_y) const
	{
		/* NOTE: map[] creates a new entry with that key (and a default-constructed 'value') 
		* if the element is not in the map. Question: is it possible to get the 
		* default-construction to calculate the values directly, so you don't have to 
		* overwrite it immediately? */
		assert(beam_x < 1000 && chine_x < 1000 && chine_y < 1000 && keel_y < 1000);
		int key(((beam_x * 1000 + chine_x) * 1000 + chine_y) * 1000 + keel_y);
// 		T& value(map[key]);
		auto value = map.find(key);
		
// 		value->second.second = iteration_no; //whether default-constructed or already in there, update the access information
		
		if(value != map.end()) {
			return value->second.first;
			value->second.second = iteration_no;
		} else {
			//compute it and write that shiznit in there
			
			T value_pair(StationCalculator<CoordType, HullParameters>::calculate_station_properties(beam_x, chine_x, chine_y, keel_y), iteration_no);
			std::pair<int, T> entry(key,value_pair);
			
// 			value->second = std::pair<StationProperties, int>(StationCalculator<CoordType, HullParameters>::calculate_station_properties(beam_x, chine_x, chine_y, keel_y), iteration_no);
// 			value->second.first = StationCalculator<CoordType, HullParameters>::calculate_station_properties(beam_x, chine_x, chine_y, keel_y);
// 			value->second.second = iteration_no;
			if(map_is_populated()) ++insertion_counter;
			if(map_is_full()) cleanup();
			
// 			return value->second.first; //I hope the reference isn't invalidated by writing here
			return value_pair.first;
		}	
	}
	
private:
	inline bool map_is_populated() const {
		if (map.size() + insertions_until_cleanup >= target_map_size) return true;
		else return false;
	}
	
	inline bool map_is_full() const {
		if (insertion_counter >= insertions_until_cleanup && map.size()) {
			return true;
		} else {
			return false;
		}
	}
	
	/*iterate through the map and remove n elements, where n=insertions_until_cleanup, where each 
	 * removed element is to be older than maximum_allowed_age. What if this latter constraint is 
	 * impossible to meet? This could happen if all the stations in the map are really new for
	 * instance. Remember to exempt the map from cleanup during its initial populating of the map.*/
	void cleanup() const
	{
		//auto should be: unordered_map<Key,T>::iterator
		for (auto it = map.begin(); insertion_counter > 0 && it != map.end(); ++it) {
			/* it.second is the 'value'. In my case it's a pair, and value->second is the 
			* last access time. If the element is too old, erase it. */
			if(it->second.second + maximum_allowed_age < iteration_no) {
				map.erase(it);
				--insertion_counter;
			}
		}
	}
	
};

#endif //MEMOIZED_STATION_CALCULATOR_H