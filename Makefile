.PHONY: release debug clean rebuild

configure:
	cmake -S . -B build -DCMAKE_CXX_COMPILER=g++

release:
	cmake --build build --target mapgen

debug:
	cmake --build build --target mapgen_debug

clean:
	cmake --build build --target clean

rebuild: clean release