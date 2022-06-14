TRGDIR=./
CPPFLAGS= -c -g -std=c++17 -I inc
LIBS= -lglut -lGLU -lGL -lm
SRC=./src/
INC=./inc/
OBJ=./obj/

__start__: a.out
	./a.out

a.out: ${OBJ}main.o ${OBJ}dron.o ${LIBS} ${OBJ}tereny.o ${OBJ}Brylka.o ${OBJ}ostroslup.o
	g++ -o a.out ${OBJ}main.o ${OBJ}dron.o ${LIBS} ${OBJ}tereny.o ${OBJ}Brylka.o ${OBJ}ostroslup.o -lpthread

${OBJ}main.o: ${SRC}main.cpp inc/dron.hh inc/OpenGL_API.hh inc/Wektor.hh inc/tereny.hh
	g++ ${CPPFLAGS} -o ${OBJ}main.o ${SRC}main.cpp ${LIBS}

${OBJ}dron.o: src/dron.cpp inc/dron.hh inc/Prostokat.hh inc/Wektor.hh inc/dysk.hh
	g++ ${CPPFLAGS} -o ${OBJ}dron.o src/dron.cpp ${LIBS}

${OBJ}tereny.o: src/tereny.cpp inc/tereny.hh inc/ostroslup.hh inc/Prostokat.hh inc/Draw3D_api_interface.hh inc/dron.hh inc/Brylka.hh
	g++ ${CPPFLAGS} -o ${OBJ}tereny.o src/tereny.cpp ${LIBS}

${OBJ}Brylka.o: src/Brylka.cpp inc/Brylka.hh inc/Draw3D_api_interface.hh inc/OpenGL_API.hh
	g++ ${CPPFLAGS} -o ${OBJ}Brylka.o src/Brylka.cpp ${LIBS}

${OBJ}ostroslup.o: src/ostroslup.cpp inc/ostroslup.hh inc/Brylka.hh inc/Draw3D_api_interface.hh inc/Wektor.hh
	g++ ${CPPFLAGS} -o ${OBJ}ostroslup.o src/ostroslup.cpp ${LIBS}

${OBJ}OpenGL_API.o: src/OpenGL_API.cpp inc/OpenGL_API.hh
	g++ ${CPPFLAGS} -o ${OBJ}OpenGL_API.o src/OpenGL_API.cpp ${LIBS}

inc/OpenGL_API.hh: inc/Draw3D_api_interface.hh
	touch inc/OpenGL_API.hh

clean:
	rm -rf ${OBJ}
	mkdir ${OBJ}