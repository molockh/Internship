copyfile: src/main.c lib/libcopyfile.a
	gcc src/main.c -lcopyfile -o copyfile -I include -L lib

lib/libcopyfile.a: obj/copyfile.o
	mkdir -p lib
	ar cr lib/libcopyfile.a obj/copyfile.o

obj/copyfile.o: src/copyfile.c
	mkdir -p obj
	gcc -c src/copyfile.c -o obj/copyfile.o
