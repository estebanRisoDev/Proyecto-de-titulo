#include "../Headerfiles/BitArray/MatrixRep.hpp"
#include "../Headerfiles/k2tree/Knn.hpp"
#include <chrono>


bool cmp(pair<int, int> & a, pair<int, int> & b){
	if(a.second != b.second){
		return a.second < b.second;
	}
	return  a.first < b.first;
}




int main(int argc, char * argv[]){

     if(argc < 2){
		printf("Uso: %s <archivo.txt>\n", argv[0]);
		return -1;
	}
	
	/* Carga de puntos */
	
	string completo(argv[1]);
	completo+= ".txt";
	string dir="../Pruebas/Datasets/"+completo;

	ifstream fe(dir.c_str());
	if(!fe.is_open()){
		printf("Error al abrir el archivo %s.txt\n", argv[1]);
		return -1;
	}
	vector<pair<int,int>> points;
	int x, y;
	int cantNode = 0;
	int cantEdge = 0;
	pair<int,int> p;
	while (!fe.eof()) {
		fe >> x;
		fe >> y;
		if(x > cantNode){
			cantNode = x;
		}
		if(y > cantNode){
			cantNode = y;
		}
		p = make_pair(y, x);
		points.push_back(p);
	}
	fe.close();
	uint pot2 = 2;
	while(pot2 < cantNode){
		pot2 *= 2;
	}

	cout<<"pot2 es"<< pot2<<endl;

	cantNode = pot2;
	cantEdge = points.size();
	
	/* Transformacion a adlist */
	
	sort(points.begin(), points.end(), cmp);
	
	int * listNumber = new int[cantNode + cantEdge];
	int listIndex = 0;	
	int pointIndex = 0;
	for(int i = 1; i <= cantNode; i++){
		listNumber[listIndex] = -i;	listIndex++;
		if(pointIndex < cantEdge){
			while(points[pointIndex].second == i){
				listNumber[listIndex] = points[pointIndex].first;	listIndex++;
				pointIndex++;
			}
		}
	}
	
	/* Transformacion a ktree */
	bool dado;
	
	uint max_level = floor(log(cantNode)/log(K)); 
	if(floor(log(cantNode)/log(K))==(log(cantNode)/log(K))) {
		max_level=max_level-1;
		dado=true;
	}else{
		dado=false;
	}
	
	uint nodes_read=0;
	uint *xedges = new uint[cantEdge];
	uint *yedges = new uint[cantEdge];
	uint cedg = 0;
	int k;
	for(int i=0;i<cantNode+cantEdge;i++) {
		k = listNumber[i];
		if(k<0) {
			nodes_read++;
		}
		else {
			k--;
			xedges[cedg]=nodes_read-1;
			yedges[cedg]=k;
			cedg++;
		}
	}

	MREPCR * rep = compactCreateKTree(xedges, yedges, cantNode,cantEdge,max_level,dado);
	
    uint *datos=rep->btl->data;
	uint *rs=rep->btl->Rs;
    uint integers=rep->btl->integers;
	uint s=rep->btl->s;
	uint n=rep->btl_len;
    fstream f;
    char *filename=argv[1];

    string fila=filename;
    fila+=".dat";
	string direccion="../Pruebas/Compactados/DAT/"+fila;


    f.open(direccion, ios::out | ios::binary |ios::trunc);

    if(f.is_open()){
		f.write(reinterpret_cast<char *>(&rep->numberOfNodes),sizeof(uint));
		f.write(reinterpret_cast<char *>(&rep->numberOfEdges),sizeof(ulong));
		f.write(reinterpret_cast<char *>(&rep->maxLevel),sizeof(uint));
		f.write(reinterpret_cast<char *>(&rep->btl_len),sizeof(uint));
		f.write(reinterpret_cast<char *>(&rep->bt_len),sizeof(uint));
		f.write(reinterpret_cast<char *>(&rep->btl->factor),sizeof(uint));
		f.write(reinterpret_cast<char *>(&rep->level),sizeof(uint));
		f.write(reinterpret_cast<char *>(datos),sizeof(uint)*(integers+1));
		f.write(reinterpret_cast<char *>(rs),sizeof(uint)*(n/s+1));

        f.close();
    }
    else{cout<<"no se pudo abrir"<<endl;}

 return 0;
}
