# Step 2: Build the library, example applications, and tools
.PHONY: all build_lib build_example compile_client

all: build_lib build_example compile_client

build_lib:
	@echo "Building the library, example applications, and tools..."
	mkdir -p build
	cd build && cmake .. && cmake --build .

# Step 3: Build the example application
build_example: build_lib
	@echo "Building the example application..."
	cd build/examples/c/BGT60LTR11AIP/raw_data && make

# Compile client.c with gcc
compile_client:
	@echo "Compiling client.c..."
	gcc -o client client.c -lwebsockets

clean:
	@echo "Cleaning up build directories and files..."
	rm -rf build client
