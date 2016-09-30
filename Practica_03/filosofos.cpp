#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

void Filosofo(int id, int nprocesos) {
  int izq, der;

  if (id != 0) {
    izq = (id + 1) % nprocesos, der = (id + nprocesos - 1) % nprocesos;
  } else {
    izq = (id + 1) % nprocesos, der = (id + nprocesos - 1) % nprocesos;
  }

  MPI_Status status;
  while (true) {
    // Solicita el tenedor izquierdo
    cout << "Filosofo " << id << " coge el tenedor izquierdo " << izq << endl;
    MPI_Ssend(&id, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);

    // Solicita el tenedor derecho
    cout << "Filosofo " << id << " coge el tenedor derecho " << der << endl;
    MPI_Ssend(&id, 1, MPI_INT, der, der, MPI_COMM_WORLD);

    cout << "Filosofo " << id << " COMIENDO" << endl;
    sleep((rand() % 3) + 1); // comiendo

    // suelta el tenedor izquierdo
    cout << "Filosofo " << id << " suelta tenedor izquierdo " << izq << endl;
    MPI_Ssend(&id, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);

    // suelta el tenedor derecho
    cout << "Filosofo " << id << " suelta tenedor derecho " << der << endl;
    MPI_Ssend(&id, 1, MPI_INT, der, der, MPI_COMM_WORLD);

    cout << "Filosofo " << id << " PENSANDO" << endl;
    sleep((rand() % 3) + 1); // pensando
  }
}

void Tenedor(int id, int nprocesos) {
  int buf, Filo;
  MPI_Status status;

  while (true) {
    // Espera la peticion de un filosofo vecino
    MPI_Probe(MPI_ANY_SOURCE, id, MPI_COMM_WORLD, &status);
    Filo = status.MPI_SOURCE;

    // Recibe la peticion del filosofo
    MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, id, MPI_COMM_WORLD, &status);
    cout << "Tenedor " << id << " recibe peticion de " << Filo << endl;

    // Esperar a que el filosofo suelte el tenedor
    MPI_Recv(&buf, 1, MPI_INT, Filo, id, MPI_COMM_WORLD, &status);
    cout << "Tenedor " << id << " recibe liberacion de " << Filo << endl;
  }
}

int main(int argc, char **argv) {
  int rank, size;
  srand(time(0));

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 10) {
    if (rank == 0) {
      cout << "El numero de procesos debe ser 10" << endl;
    }

    MPI_Finalize();
    return 0;
  }

  if (rank % 2 == 0) {
    Filosofo(rank, size);
  }

  else {
    Tenedor(rank, size);
  }

  MPI_Finalize();
  return 0;
}
