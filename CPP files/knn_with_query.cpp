
#include <fstream>
#include <time.h>
#include "../Headerfiles/k2tree/Knn.hpp"
#include <numeric>



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

    query_points.push_back(make_pair(atoi(acum[0].c_str()),atoi(acum[1].c_str())));

}


void escritura_resultados(fstream &fila,int numero_vecinos,string archivo,string query_set,priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> resultados,int nodes,int x, int y){
    
    string vecinos=to_string(numero_vecinos);

    if(fila.is_open()){
        fila<<"los resultados de la consulta de n° vecinos "+vecinos+" del dataset: "+archivo+" de los puntos: x="+to_string(x)+" y="+to_string(y)+"\n"; 
            while(!resultados.empty()){
            KNNElementQueue a =resultados.top();
            fila<<(nodes-a.getCuadrant().getS().getY())+1<<" "<<a.getCuadrant().getS().getX()<<"\n";
            resultados.pop();
        }

    }
    else{
        cout<<"error"<<endl;
    }

}


vector<pair <int,int> > lectura_query(string query){
    
    vector<pair<int,int>> query_points;
    fstream newfile;
    string direccion="../Pruebas/Query_Point/"+query+".txt";
    
    
    newfile.open(direccion,ios::in); 
    if (newfile.is_open()){ 
        string tp;
        while(getline(newfile, tp)){ 
            splitString(tp," ",query_points);
        }
        newfile.close();
   }

    return query_points;
}


int main(int argc, char * argv[]){

	MREP * rep = loadRepresentation2(argv[1]);
    //string bitarray=generate_bitarray(rep); 
    char* dataset=argv[1];
    string archive=dataset;

    uint k_vecinos=atoi(argv[2]);
    long double acum_tiempo_CPU=0;

    vector<long double>tiempo_CPU;
    vector<long>tiempo_WALLTIME;
    char *dir=argv[3];
    string query=dir;
    vector<pair<int,int>> query_points=lectura_query(query);
    string direccion="../Pruebas/Resultados/resultado_vecinos_"+to_string(k_vecinos)+"_archivo_"+archive+"_queryset_"+query+".txt";
    fstream newfile;
    newfile.open(direccion,ios::out|ios::binary|ios::trunc);

    while(!query_points.empty()){
        K2Tree k2=K2Tree(rep);
        KNN knn=KNN(&k2);
	    auto start = std::chrono::high_resolution_clock::now(); 
        clock_t c_start=clock();
        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> resultado=knn.findNNQ(k_vecinos,Point(query_points.back().first,query_points.back().second));
        clock_t c_end=clock();
        auto finish = std::chrono::high_resolution_clock::now();

        escritura_resultados(newfile,k_vecinos,dataset,query,resultado,k2.getNodes(),query_points.back().first, query_points.back().second);
        

        long double time_elapsed_ns=1000.0*(c_end-c_start)/CLOCKS_PER_SEC;
        time_elapsed_ns=time_elapsed_ns*1000000.0;

        tiempo_CPU.push_back(time_elapsed_ns);
        tiempo_WALLTIME.push_back(std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count());
        query_points.pop_back();
    }
       
        cout<< "tiempo CPU usado Promedio: "<< accumulate(tiempo_CPU.begin(),tiempo_CPU.end(),0)/tiempo_CPU.size()<<" ns"<<endl;
        cout<< "tiempo WALLTIME usado Promedio: "<< accumulate(tiempo_WALLTIME.begin(),tiempo_WALLTIME.end(),0)/tiempo_WALLTIME.size()<<" ns"<<endl;

	newfile<<"tiempo CPU usado Promedio: "<< accumulate(tiempo_CPU.begin(),tiempo_CPU.end(),0)/tiempo_CPU.size()<<" ns\n";
        newfile<< "tiempo WALLTIME usado Promedio: "<< accumulate(tiempo_WALLTIME.begin(),tiempo_WALLTIME.end(),0)/tiempo_WALLTIME.size()<<" ns\n";

        newfile.close();
        cout<<"Para ver los resultados porfavor revise la carpeta Pruebas/Resultados"<<endl;    


 return 0;
}

