main:
	g++ main.cpp -fopenmp -g -o exe

gif: erase main
	./exe
	ffmpeg -f image2 -i render/%3d.ppm newGifTest.gif -y && eog newGifTest.gif  

erase:
	rm -rf render/*