all: release debug

release:
	$(MAKE) -C build/release/

debug:
	$(MAKE) -C build/debug/

visualization:
	gnuplot visualization.gp

plot:
	cd data/ && gnuplot *.gp

update:
	cd data/ && gnuplot best_solution.gp

profile: debug
	cd build/debug/ && ./hull_optimizer
	cd build/debug/ && gprof -p ./hull_optimizer gmon.out
	
setup:
	rm -rf build/release/*
	rm -rf build/debug/*
	cd build/release && cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ ../..
	cd build/debug && cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=g++ ../..

clean: clean_data clean_profiling
	$(MAKE) -C build/release/ clean
	$(MAKE) -C build/debug/ clean

clean_data:
	rm -f data/*.dat data/*.svg data/*.gp data/GA_*.txt data/GA_*.pc

clean_profiling:
	rm -f gmon.out

#%.o: %.cpp %.h
#	$(CXX) $(CXXFLAGS) -c $<

# $@ The file name of the target of the rule.
# $< The name of the first dependency.
# $^ The names of all the dependencies
