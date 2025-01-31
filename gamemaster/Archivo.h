#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <fstream>
#include "Defines.h"
#include "cListaT.h"
using namespace std;

class Archivo
{
	string nombre;
	ARCHIVO tipo;
	time_t timeStamp; //ult acceso

public:
	Archivo(string nombre = "",
		ARCHIVO tipo = ARCHIVO::DAT,
		time_t timeStamp = time(0)) {
		this->nombre = nombre;
		this->tipo = tipo;
		this->timeStamp = timeStamp;
	}
	~Archivo() {	}
	void Log();

	template<class T>
	void escribirArchivoDat(cListaT<T>* lista);
	template<class T>
	cListaT<T>* leerArchivoDat(int cuantos = -1);
	string leerArchivoTxt();
};

template<class T>
inline void Archivo::escribirArchivoDat(cListaT<T>* lista)
{
	ofstream myfile;
	myfile.open(nombre + ".dat");
	if (myfile.fail())
		throw new exception("FILE_ERROR");

	for(int i = 0; i < lista->getCA(); i++)
		myfile.write((char*)((*lista)[i]), sizeof(T));

	myfile.close();
	if(!myfile.good())
		throw new exception("FILE_ERROR");
	Log();

//	FILE* fp;
//	fp = fopen((nombre + ".dat").c_str(), "w+b");
//
//	if (fp == NULL)
//		throw new exception("FILE_ERROR");
//
//	for (int i = 0; i < lista->CA; i++)
//	{
//		fwrite((*lista)[i], sizeof(T), 1, fp); //vamos escribiendo de a un objeto en el archivo
//	}
//	
//	fclose(fp);
//	Log();
}

template<class T>
inline cListaT<T>* Archivo::leerArchivoDat(int cuantos)
{

	ifstream myfile;
	myfile.open(nombre + ".dat");
	if (myfile.fail())
		throw new exception("FILE_ERROR");

	cListaT<T>* lista = new cListaT<T>();
	int i = 0; T* objeto = NULL;

	if (cuantos == -1) {
		do {
			objeto = new T();
			lista->AgregarItem(objeto);

			myfile.read((char*)((*lista)[i]), sizeof(T));
			if (myfile.eof())
				break;
			i++;
		} while (true);
		lista->QuitarenPos(lista->CA - 1);
	}
	else {
		for (int i = 0; i < cuantos; i++) {
			objeto = new T();
			try {
				lista->AgregarItem(objeto);
			}
			catch (exception* ex) {
				delete ex;
				continue;
			}
			myfile.read((char*)((*lista)[i]), sizeof(T));
			//TODO: uno queda vacio y como se repite lanza una excepcion
		}
	}
	myfile.close();
	return lista;


	//FILE* fp;
	//fp = fopen((nombre + ".dat").c_str(), "r+b");
	//if (fp == NULL)
	//	return NULL;

	//cListaT<T>* lista = new cListaT<T>();

	//T* elemento = NULL;
	//while (!feof(fp)) { //hasta que no llegue al final del archivo
	//	elemento = new T(); //creamos memoria para guardar lo que levantemos del archivo
	//	fread(elemento, sizeof(T), 1, fp);
	//	try {
	//		lista->AgregarItem(elemento); //agrega los elementos recuperados del archivo a una lista
	//	}
	//	catch (exception* ex) {
	//	}
	//} 
	//lista->QuitarenPos(lista->getCA() - 1); //y sacamos el �litmo elemento porque el feof() marca fin del archivo luego de intentar levantar el �ltimo objeto
	//fclose(fp);
	//
	//return lista;
}

inline void Archivo::Log()
{
	this->timeStamp = time(0);
}

inline string Archivo::leerArchivoTxt()
{
	string cadena = "";
	string line = "";

	ifstream myfile;
	myfile.open(nombre + ".txt");
	if (myfile.fail())
		throw new exception("FILE_ERROR");

	while (!myfile.eof()) { //mientras no haya llegado al final del archivo
		getline(myfile, line); //vamos levantando de a lineas
		cadena += line + "\n"; //y las concatenamos en una cadena
	}
	myfile.close();
	return cadena;
}
