#include <iostream>
#include <chrono>
#include "thread_pool.hpp"
#include "lector_data_task.hpp"

// Coleres de texto
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define WHITE "\033[37m"
#define BLACK "\033[30m"
#define SKYBLUE "\033[96m"
#define PURPLE "\033[95m"
#define GRAY "\033[90m"
#define SKYGREEN "\033[92m"

using namespace std;

int main()
{
  // Crea un pool de 1 hilo (Uniprocesador)
  ThreadPool pool;
  // Crea un lector de datos del Proceso
  FileProcessReader lector("listaplanif.txt");

  // Ordena los procesos por tiempo de llegada
  sort(lector.data.begin(), lector.data.end(), [](const Proceso &a, const Proceso &b)
       { return a.t_llegada < b.t_llegada; });

  // Inicio de la Simulacion
  cout << SKYGREEN << "Sistema de Planificacion FCFS Monoprocesador" << RESET << endl;
  int t_aux = 0;
  // Ejecuta los procesos encolando las tareas en el pool
  for (auto &dato : lector.data)
  {
    this_thread::sleep_for(chrono::seconds(dato.t_llegada - t_aux));
    t_aux = dato.t_llegada;
    pool.enqueue([=]()
                 { 
                  cout << YELLOW << "Proceso " << dato.id << " en ejecucion" << RESET;
                  cout << RED <<" ("<<dato.t_llegada<<"s)"<<RESET<< endl;
                  this_thread::sleep_for(chrono::seconds(dato.duracion));
                  
                  cout << MAGENTA << "Proceso " << dato.id << " terminado" << RESET;
                  cout << CYAN <<" ("<<dato.duracion<<"s)"<<RESET<< endl; });
  }
  // Fin de la Simulacion
  pool.enqueue([=]()
               { cout << SKYGREEN << "Fin de la simulacion" << RESET << endl; });
  return 0;
}