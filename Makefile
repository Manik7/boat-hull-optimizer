all: release

release:
	$(MAKE) -j4 -C build/release/

debug:
	$(MAKE) -C build/debug/

visualization: clean_data
	./main
	gnuplot visualization.gp

profile: Main
	./main
	gprof -p ./main gmon.out
	
plot:
	gnuplot visualization.gp
	
setup:	setup_release setup_debug

setup_release:
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ ../..

setup_debug:
	cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ ../..

clean: clean_data clean_profiling
	$(MAKE) -C build/release/ clean
	$(MAKE) -C build/debug/ clean

clean_data:
	rm -f data/*.dat data/*.svg

clean_profiling:
	rm -f gmon.out

#%.o: %.cpp %.h
#	$(CXX) $(CXXFLAGS) -c $<

# $@ The file name of the target of the rule.
# $< The name of the first dependency.
# $^ The names of all the dependencies
