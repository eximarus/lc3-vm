default:
	CXX=/usr/bin/clang++ cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 && CXX=/usr/bin/clang++ cmake --build build

debug:
	CXX=/usr/bin/clang++ cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_BUILD_TYPE=Debug && CXX=/usr/bin/clang++ cmake --build build
