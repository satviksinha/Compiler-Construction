gcc -c lexer.c
gcc -c parser.c
gcc -c stackAdt.c
gcc -c tree.c
gcc -o compilers_exe lexer.o parser.o stack_adt.o tree.o
./compilers_exe