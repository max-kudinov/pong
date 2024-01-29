main: main.o state.o logic.o render.o
	@clang main.o state.o logic.o render.o -o main -Wall -lSDL2 -lSDL2_ttf

main.o: main.c
	@clang -c main.c

state.o: state.c
	@clang -c state.c

logic.o: logic.c
	@clang -c logic.c

render.o: render.c
	@clang -c render.c

clean:
	rm -f main main.o state.o logic.o render.o
