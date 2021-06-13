release:
	clear && cd .. && echo "\n=================================\nConfiguration\n=================================\n" && cmake -B build -DCMAKE_BUILD_TYPE=Release &&  echo "\n=================================\nBuild\n=================================\n" && cmake --build build && echo "\n=================================\nServer Starting\n=================================\n" && ./build/bme280Server

debug:
	clear && cd .. && echo "\n=================================\nConfiguration\n=================================\n" && cmake -B build -DCMAKE_BUILD_TYPE=Debug &&  echo "\n=================================\nBuild\n=================================\n" && cmake --build build && echo "\n=================================\nServer Starting\n=================================\n" && ./build/bme280Server
