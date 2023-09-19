# Virtualized

Integrantes:
Cleber Lucas Farias
Daniel Silvestre de França Souza
Gabriel Jatoba Campos Marques dos Santos
João Pedro Barbosa Marins
Joana D'arc Oliveira do Nascimento


Comando para compilar no Ubuntu:

gcc -Iraylib/include -Lraylib/lib main.c player.c recordes.c menu.c gameover.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o main 

ou 

gcc -Iraylib/include -Lraylib/lib main.c player.c recordes.c menu.c gameover.c -lraylib -lm -o main

ou 

gcc main.c player.c recordes.c menu.c gameover.c -lraylib -lm -o main