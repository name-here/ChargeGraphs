CPP = g++
CPPFLAGS = -g -O3 -Wall -Wextra -pedantic -std=c++11 `sdl2-config --cflags`
PKGFLAGS = `sdl2-config --libs`
DEPS = buttons.h
OBJ = buttons.o main.o

ChargeGraphs: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(PKGFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm *.o ChargeGraphs