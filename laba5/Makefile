compile:
	gcc -c -Wall -Werror -fpic lib1.c
	gcc -c -Wall -Werror -fpic lib2.c
	gcc -shared -o liblib1.so lib1.o -lm -ldl
	gcc -shared -o liblib2.so lib2.o -lm -ldl
	gcc -L. -Wall -o main1.out main1.c -llib1 -ldl
	gcc -L. -Wall -o main2.out main1.c -llib2 -ldl
	gcc -L. -Wall -o main.out main2.c -llib2 -llib1 -ldl

clean:
	rm *.o
	rm *.so
	rm *.out