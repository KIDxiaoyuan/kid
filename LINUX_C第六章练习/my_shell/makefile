cc = gcc
OBJ = a.out
all:
	$(cc) -c ./*.c 
	$(cc) -o $(OBJ) ./*.o /lib64/libreadline.so.6 -I -lreadlin -lncurses -g
	rm *.o 
clean:
	rm *.o
