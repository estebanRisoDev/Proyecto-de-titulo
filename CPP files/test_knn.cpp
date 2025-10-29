
#include <fstream>
#include <time.h>
#include "../Headerfiles/k2tree/Knn.hpp"


string generate_bitarray(MREP* rep){

  string arr;  

    for(int i=0;i<rep->btl_len;i++){
        if(isBitSet2(rep->btl,i)!=0){
            arr+="1";
        }else{
            arr+="0";
        }
    } 

    return arr;
}


int main(int argc, char * argv[]){

    if(argc < 4){
		printf("Uso: %s <Archivo.dat> <numero_de_vecinos> < coord_x > < coord_y >\n", argv[0]);
		return -1;
	}

	MREP * rep = loadRepresentation2(argv[1]);
    string bitarray=generate_bitarray(rep);
    cout<<"bitarray:"<<bitarray<<endl<<endl;
    uint size=rep->btl->dato.size();
    uint forcounter=1;

    for(uint i=size;i>0;i--){
 	cout<<"integer n°"<<forcounter<<":"<<rep->btl->dato[i-1]<<endl;
	forcounter++;
    }

    K2Tree k2=K2Tree(rep);
    KNN knn=KNN(&k2);

    uint k_vecinos=atoi(argv[2]);
    uint coord_x=atoi(argv[3]);
    uint coord_y=atoi(argv[4]);

    // auto es un tipo de dato que se asigna solo, es para evitar colocar la extension tan larga de std::chrono... 
	auto start = std::chrono::high_resolution_clock::now(); 
    clock_t c_start=clock();
    priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> resultado=knn.findNNQ(k_vecinos,Point(coord_x,coord_y));
    clock_t c_end=clock();
    auto finish = std::chrono::high_resolution_clock::now();
    priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> resultado2=resultado;

    while(!resultado2.empty()){
            KNNElementQueue a =resultado2.top();
            cout<<a.getDistance()<<" ";
            resultado2.pop();
    }
    cout<<endl;
    cout<<endl;

    while(!resultado.empty()){
        KNNElementQueue a =resultado.top();
        cout <<"X:"<<(k2.getNodes()-a.getCuadrant().getS().getY())+1<< " ";
	cout <<"Y:"<<a.getCuadrant().getS().getX()<<" ";
        cout<<endl;
        resultado.pop();
    }

	cout<<"cantidad nodos"<<k2.getNodes()<<endl;
  
    long double time_elapsed_ns=1000.0*(c_end-c_start)/CLOCKS_PER_SEC;
    time_elapsed_ns=time_elapsed_ns*1000000.0;
    cout<< "tiempo usado CPU: "<< time_elapsed_ns<<" ns"<<endl;

    
    cout <<"Tiempo usado WallTime: "<<std::chrono::duration_cast<std::chrono::nanoseconds>(finish-start).count() << "ns\n";
	cout<<"fin del programa"<<endl;    


 return 0;
}

