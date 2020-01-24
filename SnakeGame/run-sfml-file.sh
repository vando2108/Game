name="${PWD##*/}"
g++ -c main.cpp 
g++ main.o -o "$name".exe -lsfml-graphics -lsfml-window -lsfml-system 
./"$name".exe