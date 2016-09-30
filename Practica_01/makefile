CXX = g++

all: fumadores productor-consumidor

fumadores:
	$(CXX) -g -c fumadores.cpp
	$(CXX) -o fumadores fumadores.o -lrt -lpthread

productor-consumidor:
	$(CXX) -g -c productor-consumidor.cpp
	$(CXX) -o productor-consumidor productor-consumidor.o -lrt -lpthread

clean:
	@echo Limpiando...
	-rm fumadores productor-consumidor ./*.o
