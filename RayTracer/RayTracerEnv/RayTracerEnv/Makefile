CC = g++
#DEBUG = -g
CFLAGS = -Wall
LFLAGS = -Wall $(DEBUG)
INCLUDES = -I.
#_OBJECTS = GzAppInterface.o	GzDisplayApplication.o main.o GzDisplay.o GzRend.o	raterizeTriangle.o matrix.o shader.o texture.o
_OBJECTS = Camera.o	Color.o	Disp.o	Light.o	Material.o	Pixel.o	Plane.o	Ray.o		Sphere.o	Vector.o	main.o  Render.o Shader.o
PROG = TracetoInfinity
all = $(PROG)
OBJECTS = $(patsubst %,obj/%,$(_OBJECTS))

#%: %.c
#	gcc -o $@ $<
obj/%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

TracetoInfinity: $(OBJECTS)
	$(CC) $(LFLAGS) $^ -o $@

.PHONY: clean

clean:
	rm -f obj/*.o
	rm -f TracetoInfinity
	rm -f resources/pot_*.ppm
	rm -f resources/output.ppm
	rm -f resources/ANIM*
	rm -f resources/hw6_*.ppm
