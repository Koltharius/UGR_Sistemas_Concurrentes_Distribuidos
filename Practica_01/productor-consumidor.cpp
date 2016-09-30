#include <cstdlib>
#include <iostream>
#include <pthread.h>
#include <semaphore.h>

using namespace std;

// Declaramos los semaforos para el productor y el consumidor
sem_t sem_productor, sem_consumidor;

// Tamaño del vector que usaremos como pila fifo y nº de items que produciremos
const int tam_vector = 10;
const int num_items = 100;

// variables asociadas a la pila
int buffer[tam_vector];
int primera_libre = 0;
int primera_ocupada = 0;

// Funcion para producir datos
int producir_dato() {
  static int contador = 1;
  return contador++;
}

// Funcion para consumir datos
void consumir_dato(int dato) { cout << "Dato recibido: " << dato << endl; }

// Funcion que realiza el productor para ir produciendo datos y almacenarlos en
// la pila que creamos anteriormente
void *productor(void *) {
  int datos;

  for (int i = 0; i < num_items; i++) {
    sem_wait(&(sem_productor));
    datos = producir_dato();
    buffer[primera_libre] = datos;
    primera_libre = (primera_libre + 1) % tam_vector;
    sem_post(&(sem_consumidor));
  }
}

// Funcion que consume los datos almacenados en la pila y los muestra por
// pantalla
void *consumidor(void *) {
  int datos;

  for (int i = 0; i < num_items; i++) {
    sem_wait(&(sem_consumidor));
    datos = buffer[primera_ocupada];
    consumir_dato(datos);
    primera_ocupada = (primera_ocupada + 1) % tam_vector;
    sem_post(&(sem_productor));
  }
  primera_ocupada = 0;
}

/////////////////////////////////////////////////////

int main(int argc, char *argv[]) {
  sem_init(&(sem_productor), 0, tam_vector);
  sem_init(&(sem_consumidor), 0, 0);

  // Creamos las hebras para cada proceso
  pthread_t hebra_productora, hebra_consumidora;

  // Asignamos la hebra a un proceso
  pthread_create(&(hebra_productora), NULL, productor, NULL);
  pthread_create(&(hebra_consumidora), NULL, consumidor, NULL);

  // Esperamos a que ambas hebras terminen
  pthread_join(hebra_productora, NULL);
  pthread_join(hebra_consumidora, NULL);

  pthread_exit(NULL);

  // Destruimos los semáforos
  sem_destroy(&(sem_productor));
  sem_destroy(&(sem_consumidor));

  return 0;
}
