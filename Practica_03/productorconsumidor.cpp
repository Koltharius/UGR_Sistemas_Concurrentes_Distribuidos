#include "mpi.h"
#include <cmath>
#include <iostream>

#define Productor 0
#define Consumidor 1
#define Buffer 5
#define TAM 5
#define ITERS 20

using namespace std;

void productor(int id) {
  for (unsigned int i = 0; i < 4; i++) {
    cout << "Productor " << id << " produce valor " << i << endl;
    MPI_Ssend(&i, 1, MPI_INT, Buffer, Productor, MPI_COMM_WORLD);
  }
}

void consumidor(int id) {
  int value, peticion = 1;
  float raiz;
  MPI_Status status;

  for (unsigned int i = 0; i < 5; i++) {
    MPI_Ssend(&peticion, 1, MPI_INT, Buffer, Consumidor, MPI_COMM_WORLD);
    MPI_Recv(&value, 1, MPI_INT, Buffer, 0, MPI_COMM_WORLD, &status);
    cout << "Consumidor " << id << " recibe valor " << value << " de Buffer"
         << endl;
    raiz = sqrt(value);
    cout << raiz << endl;
  }
}

void buffer() {
  int value[TAM], peticion, pos = 0, rama;
  MPI_Status status;

  for (unsigned int i = 0; i < 40; i++) {
    if (pos == 0) { // el consumidor no puede consumir
      rama = 0;
    } else if (pos == TAM) { // el productor no puede producir
      rama = 1;
    } else { // se puede consumir o producir
      MPI_Probe(MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
      if (status.MPI_TAG == Productor) {
        rama = 0;
      } else {
        rama = 1;
      }
    }

    switch (rama) {
    case 0:
      MPI_Recv(&value[pos], 1, MPI_INT, MPI_ANY_SOURCE, Productor,
               MPI_COMM_WORLD, &status);
      cout << "Buffer recibe " << value[pos] << " de productor "
           << status.MPI_SOURCE << endl;
      pos++;
      break;
    case 1:
      MPI_Recv(&peticion, 1, MPI_INT, MPI_ANY_SOURCE, Consumidor,
               MPI_COMM_WORLD, &status);
      MPI_Ssend(&value[pos - 1], 1, MPI_INT, status.MPI_SOURCE, 0,
                MPI_COMM_WORLD);
      cout << "Buffer envia " << value[pos - 1] << " a conusmidor "
           << status.MPI_SOURCE << endl;
      pos--;
      break;
    }
  }
}

int main(int argc, char *argv[]) {
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 10) {
    if (rank == 0) {
      cout << "mpirun -np 10 " << argv[0] << endl;
    }
  } else {
    if (rank < Buffer) {
      productor(rank);
    } else if (rank == Buffer) {
      buffer();
    } else {
      consumidor(rank);
    }
  }

  MPI_Finalize();
  return 0;
}
