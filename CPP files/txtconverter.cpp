#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <algorithm>


using namespace std;

void splitString(string str, string delimiter,vector<pair<int,int>> &query_points){
    int start = 0;
    int end = str.find(delimiter);
    vector<string> acum;
    while (end != -1) {
        acum.push_back(str.substr(start, end - start)); 
        start = end + delimiter.size();
        end = str.find(delimiter, start);
    }
    acum.push_back(str.substr(start, end - start));

    query_points.push_back(make_pair(atoi(acum[1].c_str()),atoi(acum[2].c_str())));

}


vector<pair<int,int>> Lectura(string archivo){

    string direccion="../Pruebas/Datasets/Fernando_Datasets/"+archivo+".txt";
    vector<pair<int,int>> points;
    fstream f;
    bool acceso=false;

    f.open(direccion,ios::in);

    if(f.is_open()){
      string tp;
       while(getline(f, tp)){ 
            if(acceso==true){
                splitString(tp," ",points);
            }
            acceso=true;
        }
        f.close(); 

    }else{
        cout<<"no se pudo abrir tal archivo"<<endl;
    }

    reverse(points.begin(),points.end());

    return points;
}

void Escritura(vector<pair<int,int>> puntos, string archivo){

    fstream f;
    string direccion="../Pruebas/Datasets/"+archivo+".txt";

    f.open(direccion,ios::out|ios::binary|ios::trunc);
    if(f.is_open()){
        while(!puntos.empty()){
            f<<puntos.back().first <<" "<<puntos.back().second<<"\n";
            puntos.pop_back();
        }
    }else{
        cout<<"error al abrir el archivo"<<endl;
    }


}



int main(int argc, char * argv[]){
    
    char *dataset=argv[1];
    string archivo=dataset;

    vector<pair<int,int>> puntos=Lectura(archivo);
    Escritura(puntos,archivo);

    cout<<"fin del programa"<<endl;
    return 0;
}