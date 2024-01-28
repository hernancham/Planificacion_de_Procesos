#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

struct Proceso
{
  std::string id;
  int t_llegada;
  int prioridad;
  int duracion;
  int mbytes;
  int n_impresoras;
  int n_escaneres;
  int n_modems;
  int n_cds;
};

class FileProcessReader
{
public:
  std::vector<Proceso> data;

  FileProcessReader(const std::string &filename)
  {
    std::ifstream file(filename);
    if (file.is_open())
    {
      // Ignora la primera línea (encabezado)
      std::string line;
      std::getline(file, line);

      while (std::getline(file, line))
      {
        std::istringstream iss(line);
        Proceso item;
        std::string dat;
        getline(iss, dat, ',');
        item.id = dat;
        getline(iss, dat, ',');
        item.t_llegada = stoi(dat);
        getline(iss, dat, ',');
        item.prioridad = stoi(dat);
        getline(iss, dat, ',');
        item.duracion = stoi(dat);
        getline(iss, dat, ',');
        item.mbytes = stoi(dat);
        getline(iss, dat, ',');
        item.n_impresoras = stoi(dat);
        getline(iss, dat, ',');
        item.n_escaneres = stoi(dat);
        getline(iss, dat, ',');
        item.n_modems = stoi(dat);
        getline(iss, dat, ',');
        item.n_cds = stoi(dat);

        data.push_back(item);
      }

      file.close();
    }
    else
    {
      std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
    }
  }
};