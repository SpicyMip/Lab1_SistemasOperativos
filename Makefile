all: no_opti opti

no_opti:
	gcc -o no_opti No_optimizado.c
	./no_opti
opti:
	gcc -o opti optimizado.c
	./opti
clean:
	rm -rf horizontal
	rm -rf vertical
	rm -f no_opti
	rm -f opti