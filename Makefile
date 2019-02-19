CPP = g++
CPPFLAGS = -g -O3 -Wall -Wextra -pedantic -std=c++14 `sdl2-config --cflags`
PKGFLAGS = `sdl2-config --libs`
DEPS = colors.h particles.h buttons.h
OBJ = colors.o particles.o buttons.o main.o

ChargeGraphs: $(OBJ)
	$(CPP) -o $@ $^ $(CPPFLAGS) $(PKGFLAGS)

%.o: %.cpp $(DEPS)
	$(CPP) -c -o $@ $< $(CPPFLAGS)

.PHONY: clean
clean:
	rm *.o ChargeGraphs