# BITTINESS = -m32
# CC = cc ${BITTINESS}
# CXX = c++ ${BITTINESS}

project2: gc_project2.cpp
	gcc -m32 -Wno-deprecated -framework OpenGL -framework GLUT gc_project2.cpp -o main

run: gc_project2
	./main

clean: gc_project2
	rm main
