#include "mpi.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

using namespace std;

#define FILOSOFO 5

void Filosofo(int id, int nprocesos) {
  int izq = (id + 1) % nprocesos, der = (id + nprocesos - 1) % nprocesos;

  int solicitud;
  MPI_Status status;

  while (true) {
    cout << "Filosofo " << id << " pensando" << endl;
    sleep(rand() % 3 + 1);

    // solicita sentarse
    cout << "Filosofo " << id << " solicita sentarse" << endl;
    solicitud = 1;
    MPI_Ssend(&solicitud, 1, MPI_INT, 10, FILOSOFO, MPI_COMM_WORLD);

    // recibe la solicitud del camarero
    MPI_Recv(&solicitud, 1, MPI_INT, 10, id, MPI_COMM_WORLD, &status);

    if (solicitud == 0) {
      // solicita tenedor izquierdo
      cout << "Filosofo " << id << " solicita tenedor izq " << izq << endl;
      MPI_Ssend(&id, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);

      // solicita tenedor derecho
      cout << "Filosofo " << id << " solicita tenedor der " << der << endl;
      MPI_Ssend(&id, 1, MPI_INT, der, der, MPI_COMM_WORLD);

      //  comiendo
      cout << "Filosofo " << id << " comiendo" << endl;
      sleep(rand() % 3 + 1);

      // suelta el tenedor izquierdo
      cout << "Filosofo " << id << " suelta el tenedor izq " << izq << endl;
      MPI_Ssend(&id, 1, MPI_INT, izq, izq, MPI_COMM_WORLD);

      // suelta el tenedor derecho
      cout << "Filosofo " << id << " suelta el tenedor der " << der << endl;
      MPI_Ssend(&id, 1, MPI_INT, der, der, MPI_COMM_WORLD);

      solicitud = 2;
      cout << "Filosofo " << id << " solicita levantarse" << endl;
      MPI_Ssend(&solicitud, 1, MPI_INT, 10, FILOSOFO, MPI_COMM_WORLD);
    }
  }
}

void Tenedor(int id, int nprocesos) {
  int buf, Filo;
  MPI_Status status;

  while (true) {
    // Recibe la peticion del filosofo
    MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, id, MPI_COMM_WORLD, &status);
    Filo = status.MPI_SOURCE;
    cout << "Tenedor " << id << " recibe peticion de " << Filo << endl;

    // Espera a que el filosofo suelte el tenedor
    MPI_Recv(&buf, 1, MPI_INT, Filo, id, MPI_COMM_WORLD, &status);
    cout << "Tenedor " << id << " recibe liberacion de " << Filo << endl;
  }
}

void Camarero() {
  int num_filosofos_sentados = 0;
  int Filo, buf;
  int solicitud_aceptada = 0;
  MPI_Status status;

  while (true) {
    while (num_filosofos_sentados <= 4) {
      MPI_Recv(&buf, 1, MPI_INT, MPI_ANY_SOURCE, FILOSOFO, MPI_COMM_WORLD,
               &status);

      Filo = status.MPI_SOURCE;
      if (buf == 1) {
        // el filosofo solicita sentarse
        cout << "Camarero recibe la solicitud para sentarse de " << Filo
             << endl;

        // se le confirma al filosofo que puede sentarse
        MPI_Ssend(&solicitud_aceptada, 1, MPI_INT, Filo, Filo, MPI_COMM_WORLD);
        cout << "El camarero sienta a " << Filo << endl;
        num_filosofos_sentados++;
      }

      else {
        // el filosofo solicita levantarse
        cout << "El filosofo " << Filo << " se levanta" << endl;
        num_filosofos_sentados--;
      }
    }
  }
}

int main(int argc, char **argv) {
  int rank, size;
  srand(time(0));
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size != 11) {
    if (rank == 0) {
      cout << "El numero de procesos debe ser 11" << endl;
    }
    MPI_Finalize();
    return 0;

    if (rank % 2 == 0 && rank != 10) {
      Filosofo(rank, size); // los pares son filosofos
    }

    else if (rank == 10) {
      Camarero(); // el 10 es el camarero
    }

    else {
      Tenedor(rank, size); // los impares son tenedores
    }

    MPI_Finalize();
    return 0;
  }
}
