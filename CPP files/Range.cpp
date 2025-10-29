#include "../Headerfiles/k2tree/Knn.hpp"

bool cmp(pair<int, int> & a, pair<int, int> & b){
	if(a.second != b.second){
		return a.second < b.second;
	}
	return  a.first < b.first;
}


int main(int argc, char * argv[]){

    
     if(argc < 5){
		printf("Uso: %s <archivo.DAT> <rango x1> <rango x2> <rango y1> <rango y2>\n", argv[0]);
		return -1;
	}

    MREP* rep=loadRepresentation2(argv[1]);
    rep->div_level_table = (uint *)malloc(sizeof(uint)*(rep->maxLevel+1));
    
    for(uint i=0;i<=rep->maxLevel;i++)
		rep->div_level_table[i]=exp_pow(K,rep->maxLevel-i);

    int x1=atoi(argv[2]); 
    int x2=atoi(argv[3]);
    int y1=atoi(argv[4]);
    int y2=atoi(argv[5]);

    if(x1<=0 || x2<=0 || y1<=0 || y2<=0){
        cout<<"error no puede ser 0 o negativo"<<endl;
    }else{
        x1=x1-1;
        x2=x2-1;
        y1=y1-1;
        y2=y2-1;
        if(x2 < x1 || y2<y1 ){cout<<"error"<<endl;}
        else{ 
            compactCheckRangeQuery2(rep,y1,y2,x1,x2);
        }

    }

 return 0;   
}