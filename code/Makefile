CXX = gcc
CXXFLAGS = -Wall -Wextra
MAIN_SRC = allocate.c cholesky_inverse.c cl_0.c readdata.c sgd.c cl_alg.c main.c util.c
MAIN_OBJ = $(MAIN_SRC:%.c=%.o)

output: $(MAIN_OBJ)
	$(CXX) $(CXXFLAGS) $(MAIN_OBJ) -lm -o cl

gendata: data_gen.c
	gcc -Wall data_gen.c -o gendata

teacher: teacher.o cl_0.o
	gcc -Wall teacher.o cl_0.o -o teacher

teacher.o: teacher.c globals.h
	gcc -Wall -g -c teacher.c

%.o: %.c globals.h
	$(CXX) $(CXXFLAGS) -c $<

.PHONY: clean
clean:
	-rm *.o
