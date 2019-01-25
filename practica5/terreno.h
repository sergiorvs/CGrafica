#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <GL/glut.h>
#include <string>
#include <string.h>
#include <cstring>
#include <fstream>

#include <vector>

#include "TextureManager.cpp"

using namespace std;


//funcion para tokenizar un string
vector<string> split(string str,string sep){
    char* cstr=const_cast<char*>(str.c_str());
    char* current;
    vector<string> arr;
    current=strtok(cstr,sep.c_str());
    while(current!=NULL){
        arr.push_back(current);
        current=strtok(NULL,sep.c_str());
    }
    return arr;
}


struct PuntoTerreno// Definicion de un punto del terreo
{
	GLfloat s, t; // Coordenadas de texturas
	GLfloat nx, ny, nz; // Coordenadas de la normal
	GLfloat x, y, z; // posición del vértice

	GLvoid printCords(){
		cout<< "(" << x << ", " << y << ", " << z << ")" <<endl;
	}

	GLvoid printNormals(){
		cout<< "(" << nx << ", " << ny << ", " << nz << ")" <<endl;
	}

	GLfloat* getCords(){
		GLfloat *cord = new GLfloat[3];
		cord[0] = x; cord[1] = y; cord[2] = z;
		return cord;
	}

	GLfloat magnitude(){
		return sqrt(x*x + y*y + z*z);
	}

	PuntoTerreno operator /(GLfloat k){
		PuntoTerreno result;
		result.x = x/k;
		result.y = y/k;
		result.z = z/k;
		return result;
	}

	PuntoTerreno operator *(PuntoTerreno v){
		PuntoTerreno result;
		result.x = y*v.z - z*v.y;
		result.y = -1*(x*v.z - z*v.x);
		result.z = x*v.y - y*v.x;
		return result;
	}

	PuntoTerreno operator -(PuntoTerreno b){
		PuntoTerreno result;
		result.x = x - b.x;
		result.y = y - b.y;
		result.z = z - b.z;
		return result;
	}

	PuntoTerreno operator +(PuntoTerreno b){
		PuntoTerreno result;
		result.x = x + b.x;
		result.y = y + b.y;
		result.z = z + b.z;
		return result;
	}
};



PuntoTerreno calculateNormal(PuntoTerreno a, PuntoTerreno b, PuntoTerreno c){
	PuntoTerreno ab = b - a;
	PuntoTerreno ac = c - a; 
	PuntoTerreno normal = ab * ac;
	normal = normal/normal.magnitude();

	return normal;
}


class Terreno
{
public :
	Terreno(){}
	bool load(string filename); // carga de un archivo de modelo digital de terreno
	void display(); // Visualizacion del terreno
	void computeNormals(); // calcula las normales de cada vertice
	int nb_pt_x, nb_pt_z; // quantidad de punto en X y Z
	float dist_x, dist_z; // distancia entre dos puntos segun X y Z
	vector <PuntoTerreno> lista_puntos; // Tabla que contiene los puntos del terreno
	vector <GLuint> lista_indices ; // Tabla que contiene los indices
};



bool Terreno::load(string filename){
	vector <string> line;
	ifstream myfile(filename);
	if(!myfile.is_open()){
		cout<<"no se pudo abrir el archivo"<<endl;
		return 0;
	}

	GLfloat z = 0;
	GLfloat x = 0;

	GLfloat s = (x/50) / 101;
	GLfloat t = (z/50) / 101;

	string lines;
	getline(myfile, lines);
	PuntoTerreno point;
	while(getline(myfile, lines)){
		x = 0;
		line.clear();
		line = split(lines.erase(lines.size()-1), " ");
		for(int i=0; i<line.size(); i++)
		{
			point.x = x;
			point.y = stof(line[i]);
			x += 50;
			point.z = z;

			point.s = s;
			s = (float)(i+1)/101;
			point.t = t;

			lista_puntos.push_back(point);
		}
		z += 50;
		int auxT = z/50;
		t = (float)(auxT)/(float)101;
	}

	int salto = line.size();

	for(int i=0; i<lista_puntos.size()-101; i++){
		//if( i >= 10100 ) break;
		if( (i+1)%101 != 0){
			lista_indices.push_back(i+1);
			lista_indices.push_back(i);
			lista_indices.push_back(i+salto);

			lista_indices.push_back(i+1);
			lista_indices.push_back(i+salto);
			lista_indices.push_back(i+salto+1);
		}
	}

	computeNormals();

	return 1;
}




void Terreno::display()
{
	PuntoTerreno *vertexs = lista_puntos.data();
	GLuint *indexs = lista_indices.data();
	// create one display list
	//glBindTexture(GL_TEXTURE_2D, terrain);
	glInterleavedArrays(GL_T2F_N3F_V3F, sizeof(PuntoTerreno),&vertexs[0].s);
	// compile the display list, store a triangle in it
	glDrawElements(GL_TRIANGLES, lista_indices.size(), GL_UNSIGNED_INT,(void*)indexs);
	// draw the display list
}


void Terreno::computeNormals(){
	int cont = 0;
	for(int i=0; i<lista_puntos.size(); i++){
		PuntoTerreno normal;
		normal.x = 0; normal.y = 0; normal.z = 0;
		for(int j=0; j<lista_indices.size(); j+=3){
			if(i == lista_indices[j] || i == lista_indices[j+1] || i == lista_indices[j+2]){
				cont++;
				PuntoTerreno a = lista_puntos[lista_indices[j]];
				PuntoTerreno b = lista_puntos[lista_indices[j+1]];
				PuntoTerreno c = lista_puntos[lista_indices[j+2]];
				normal = normal + calculateNormal(a,b,c);
			}
		}
		lista_puntos[i].nx = normal.x/cont;
		lista_puntos[i].ny = normal.y/cont;
		lista_puntos[i].nz = normal.z/cont;
	}
}