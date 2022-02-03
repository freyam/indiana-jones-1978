.ONESHELL:

default:
	if [ ! -d "build" ]; then
		mkdir build && cd build && cmake .. && make && ./IJ_1978
	else
		cd build && make && ./IJ_1978
	fi

clean:
	rm -rf build