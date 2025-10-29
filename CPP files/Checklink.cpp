#include "../Headerfiles/k2tree/Knn.hpp"

bool cmp(pair<int, int> & a, pair<int, int> & b){
	if(a.second != b.second){
		return a.second < b.second;
	}
	return  a.first < b.first;
}


int main(int argc, char * argv[]){


    
    if(argc < 3){
		printf("Uso: %s <Archivo.dat> <coord_x> < coord_y >\n", argv[0]);
		return -1;
	}

    MREP* rep=loadRepresentation2(argv[1]);
    rep->div_level_table = (uint *)malloc(sizeof(uint)*(rep->maxLevel+1));
    
    for(uint i=0;i<=rep->maxLevel;i++)
		rep->div_level_table[i]=exp_pow(K,rep->maxLevel-i);

    int x=atoi(argv[2])-1;
    int y=atoi(argv[3])-1;

    if(x<=-1 || y<=-1){
        cout<<"fallo no puede ser 0 o negativo";
    }
    else{
        if(compact_CheckLinkQuery(rep,x,y)==1){
        cout<<"existe ese numero"<<endl;
        }
        else{
            cout<<"no existe esa posicion"<<endl;
            cout<<"esa posición es x="<<x+1<<" y="<<y+1<<endl;
        }
    }


   

 return 0;   
}
