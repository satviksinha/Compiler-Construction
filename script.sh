gcc -c lexer.c
gcc -c parser.c
gcc -c stack_adt.c
gcc -o compilers_exe lexer.o parser.o stack_adt.o
./compilers_exe