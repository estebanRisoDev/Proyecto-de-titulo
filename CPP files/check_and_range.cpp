#include "../Headerfiles/k2tree/Knn.hpp"

bool cmp(pair<int, int> & a, pair<int, int> & b){
	if(a.second != b.second){
		return a.second < b.second;
	}
	return  a.first < b.first;
}


int main(int argc, char * argv[]){


    MREP* rep=loadRepresentation2(argv[1]);
    

    cout<<rep->btl->dato[0]<<endl;
    cout<<rep->btl->dato[1]<<endl;
    cout<<rep->div_level_table[0]<<endl;

    uint aceptado;
    uint counter=0;
    uint contador2=0;

    cout<<"voy a ir en acpetado"<<endl;
    for(uint i=0;i<rep->numberOfNodes;i++){
        for(uint j=0;j<rep->numberOfNodes;j++){
            aceptado=compact_CheckLinkQuery(rep,i,j);
            
            if(aceptado==1){
                cout<<"x="<<j<<endl;
                cout<<"y="<<i<<endl;
                contador2++;
            }
            counter++;
        }
    }

    cout<<"contador fue es: "<<counter<<endl;
    cout<<"contador2 fue es: "<<contador2<<endl;

 return 0;   
}