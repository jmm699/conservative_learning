output: .objects/allocate.o .objects/cholesky_inverse.o .objects/cl_0.o .objects/main.o .objects/readdata.o
	gcc -Wall -g .objects/allocate.o .objects/cholesky_inverse.o .objects/cl_0.o .objects/main.o .objects/readdata.o -lm -o cl

gendata:
	gcc -Wall data_gen.c -o gendata

.objects/allocate.o: allocate.c globals.h | .objects
	gcc -Wall -g -c allocate.c globals.h
	mv allocate.o .objects

.objects/cholesky_inverse.o: cholesky_inverse.c | .objects
	gcc -Wall -g -c cholesky_inverse.c
	mv cholesky_inverse.o .objects

.objects/cl_0.o: cl_0.c | .objects
	gcc -Wall -g -c cl_0.c
	mv cl_0.o .objects

.objects/main.o: main.c globals.h | .objects
	gcc -Wall -g -c main.c globals.h
	mv main.o .objects

.objects/readdata.o: readdata.c globals.h | .objects
	gcc -Wall -g -c readdata.c globals.h
	mv readdata.o .objects

.objects:
	mkdir .objects

clean:
	rm *.gch
	rm -rf .objects
