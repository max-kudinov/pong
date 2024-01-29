main: main.o state.o logic.o render.o
	@clang main.o state.o input.o logic.o render.o -o main -Wall -lSDL2 -lSDL2_ttf

main.o: main.c
	@clang -c main.c

state.o: state.c state.h
	@clang -c state.c

input.o: input.c input.h
	@clang -c input.c

logic.o: logic.c logic.h
	@clang -c logic.c

render.o: render.c render.h
	@clang -c render.c

clean:
	rm -f main main.o state.o input.o logic.o render.o
