all : 
	g++ *.cpp -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -o pool
	./pool
