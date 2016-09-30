#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>

using namespace std;

// Declaramos los semaforos para los tres fumadores y para el estanquero

sem_t Sfumador_1, Sfumador_2, Sfumador_3, Sestanquero;

// fumador_1 tiene papel y tabaco, el fumador_2 tiene papel y cerillas
// y el fumador_3 tiene tabaco y cerillas, por lo que estan esperando
// a que el estanquero deposite el elemento que les falta para empezar a fumar

int producto;

// Accion de fumar

void fumar() {
  const unsigned miliseg = 100U + (rand() % 1900U);
  usleep(1000U * miliseg);
}

// Despierta al fumador 1 el cual coje el producto, llama a la accion fumar y a
// continuacion llama al estanquero

void *fumador_1(void *) {
  while (true) {
    sem_wait(&Sfumador_1);
    cout << "Soy el fumador uno y recojo el papel." << endl;
    sem_post(&Sestanquero);
    cout << "Soy el fumador uno y comienzo a fumar" << endl;
    fumar();
    cout << "Soy el fumador uno y termino de fumar" << endl;
  }
}

// Despierta al fumador 2 el cual coje el producto, llama a la accion fumar y a
// continuacion llama al estanquero

void *fumador_2(void *) {
  while (true) {
    sem_wait(&Sfumador_2);
    cout << "Soy el fumador dos y recojo el tabaco." << endl;
    cout << "Soy el fumador dos y comienzo a fumar" << endl;
    fumar();
    cout << "Soy el fumador dos y termino de fumar" << endl;
    sem_post(&Sestanquero);
  }
}

// Despierta al fumador 3 el cual coje el producto, llama a la accion fumar y a
// continuacion llama al estanquero

void *fumador_3(void *) {
  while (true) {
    sem_wait(&(Sfumador_3));
    cout << "Soy el fumador tres y recojo las cerillas." << endl;
    cout << "Soy el fumador tres y comienzo a fumar" << endl;
    fumar();
    cout << "Soy el fumador tres y termino de fumar" << endl;
    sem_post(&(Sestanquero));
  }
}

// Despierta al estanquero el cual produce un objeto random y despierta al
// fumador de turno
void *estanquero(void *) {
  while (true) {
    sem_wait(&(Sestanquero));

    producto = rand() % 3;

    if (producto == 1) {
      cout << "Papel producido." << endl;
      sem_post(&(Sfumador_1));
    }

    else {
      if (producto == 2) {
        cout << "Tabaco producido." << endl;
        sem_post(&(Sfumador_2));
      }

      else {
        cout << "Papel producido." << endl;
        sem_post(&(Sfumador_3));
      }
    }
  }
}

/////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  srand(time(NULL));

  sem_init(&(Sestanquero), 0, 1);
  sem_init(&(Sfumador_1), 0, 0);
  sem_init(&(Sfumador_2), 0, 0);
  sem_init(&(Sfumador_3), 0, 0);

  pthread_t hebra_fum1, hebra_fum2, hebra_fum3, hebra_est;

  pthread_create(&(hebra_est), NULL, estanquero, NULL);
  pthread_create(&(hebra_fum1), NULL, fumador_1, NULL);
  pthread_create(&(hebra_fum2), NULL, fumador_2, NULL);
  pthread_create(&(hebra_fum3), NULL, fumador_3, NULL);

  pthread_join(hebra_est, NULL);
  pthread_join(hebra_fum1, NULL);
  pthread_join(hebra_fum2, NULL);
  pthread_join(hebra_fum3, NULL);

  pthread_exit(NULL);

  sem_destroy(&(Sfumador_1));
  sem_destroy(&(Sfumador_2));
  sem_destroy(&(Sfumador_3));
  sem_destroy(&(Sestanquero));

  return 0;
}
