HEADERS := $(wildcard include/*.hpp)

debug: src/*.cpp $(HEADERS) 
	time g++ -std=c++2a src/*.cpp -o bin/conway.exe -g  -O1 -Wall -Wno-narrowing -Wno-unused-variable -Wno-enum-compare -I include/ -L /lib/ -lraylib -lrayguidll -lopengl32 -lgdi32 -lwinmm

	./bin/conway
build: src/*.cpp $(HEADERS)
	time g++ -std=c++2a src/*.cpp -o bin/conway.exe -g -O1 -Wall -Wno-narrowing -Wno-unused-variable -Wno-enum-compare -I include/ -L /lib/ -lraylib -lrayguidll -lopengl32 -lgdi32 -lwinmm
run: 
	./bin/conway 
