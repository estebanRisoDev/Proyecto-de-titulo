#include "BitArrayRS.hpp"
#include <fstream>
#ifndef MATRIXREP_HPP
#define MATRIXREP_HPP
#define MAX_INFO 1024*1024+10


#include <cstring>


typedef struct matrixRep{
    BITRS * btl;
    uint btl_len;               //Numero de bytes de T:L 4
    uint bt_len;                //Numero de bytes de T   4
    int maxLevel;           
    uint numberOfNodes;     // 4
    ulong numberOfEdges;    // 8 
    uint *div_level_table;  // 4    4+4+4+4+8+4+4+4+4+4+4+4+4  
    uint * element;  //4 
	int level;
	uint *info;
	uint * info2[2];
}MREP;


typedef struct matrixRep_cr{
    BITRSCR * btl;
    uint btl_len;               //Numero de bytes de T:L 4
    uint bt_len;                //Numero de bytes de T   4
    int maxLevel;           
    uint numberOfNodes;     // 4
    ulong numberOfEdges;    // 8 
    uint * div_level_table;  // 4    4+4+4+4+8+4+4+4+4+4+4+4+4  
    uint * info; // 4				
    uint * info2[2]; // 4 
    uint * element;  //4 
    uint * basex; // 4 
    uint * basey; // 4 
    int iniq; // 4 
    int finq; // 4
	int level;
}MREPCR;

typedef struct edgeinfo{
    uint xval;
    uint yval;
    uint kval;
}tedge;

typedef struct QUEUEOFFCONS{
    uint offsetL;
    uint offsetR;
    struct QUEUEOFFCONS *link;

}QUEUEOFFCONS;



#define K 2

MREP * compactCreateKTree(uint * xedges, uint *yedges, uint numberOfNodes,ulong numberOfEdges, uint maxl);


uint * compactAdjacencyList(MREP * rep, int x);
uint * compact2AdjacencyList(MREP * rep, int x);
uint * compactInverseList(MREP * rep, int y);
uint ** compactRangeQuery(MREP * rep, uint p1, uint p2, uint q1, uint q2);
uint compactCheckLinkQuery(MREP * rep, uint p, uint q);
uint compact2CheckLinkQuery(MREP * rep, uint p, uint q);
uint compactCheckRangeQuery(MREP * rep, uint p1, uint p2, uint q1, uint q2);

uint compact_CheckLinkQuery(MREP * rep, uint p, uint q);
uint recursive_CheckLinkQuery(MREP * rep, uint p, uint q, uint node, uint level);


int * compactFullDecompression(MREP * rep);

MREP * loadRepresentation(uint numberOfNodes, ulong numberOfEdges, uint maxLevel, uint  btl_len, uint  bt_len, uint factor , uint level );
void destroyRepresentation(MREP * rep);
vector <uint> getLevelTable(fstream &f,uint &size);
vector<uint> getDatoVector(fstream &f, uint integers);
vector<uint> getRsVector(fstream &f, uint size);



	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

QUEUEOFFCONS * finalQUEUEOFFCONS;


vector<uint> getDatoVector(fstream &f, uint integers){

	
	uint dato;
	vector<uint> vector;

	for(uint i=0;i<integers+1;i++){
		f.read(reinterpret_cast<char *>(&dato),sizeof(uint));
		vector.push_back(dato);
	}
   
	return vector;
}


vector<uint> getRsVector( fstream &f , uint size){
	
	uint dato;
	vector<uint> vector;
 
	cout<<"getRsVector"<<endl;
	
	for(int i=0;i<size;i++){
		f.read(reinterpret_cast<char *>(&dato),sizeof(uint));
		vector.push_back(dato);
	}

	f.close();

	return vector;
}



void AddItem2 (MREPCR *rep, int elem, int cantx,int canty) {
	if(rep->iniq!=-1){
	rep->finq++;
		rep -> element[rep->finq] = elem;
		rep -> basex[rep->finq] = cantx;
		rep -> basey[rep->finq] = canty;
		
	}
	else{
		rep->iniq=0;
		rep->finq=0;
		rep -> element[rep->iniq] = elem;
		rep -> basex[rep->iniq] = cantx;
		rep -> basey[rep->iniq] = canty;
	}
}

void RemoveItem2 (MREPCR * rep) {
	rep->iniq++;
}

QUEUEOFFCONS * AddItemOFFCONS (QUEUEOFFCONS * listpointer, uint offsetL, uint offsetR) {

	if(listpointer!=NULL){
		QUEUEOFFCONS * lp = (QUEUEOFFCONS  *) malloc (sizeof (struct QUEUEOFFCONS));
		finalQUEUEOFFCONS -> link = lp;
		lp -> link = NULL;
		lp -> offsetL = offsetL;
		lp -> offsetR = offsetR;
		finalQUEUEOFFCONS = lp;
		return listpointer;
	}
	else{
		listpointer = (QUEUEOFFCONS  *) malloc (sizeof (struct QUEUEOFFCONS));
		listpointer -> link = NULL;
		listpointer -> offsetL = offsetL;
		listpointer -> offsetR = offsetR;

		finalQUEUEOFFCONS = listpointer;
		return listpointer;
	}
}

QUEUEOFFCONS * RemoveItemOFFCONS (QUEUEOFFCONS * listpointer) {
	QUEUEOFFCONS * tempp;
	tempp = listpointer -> link;
	free (listpointer);
	return tempp;
}


uint exp_pow(uint base, uint pow){
	uint i, result = 1;
	for(i=0;i<pow;i++)
		result*=base;

	return result;
}



MREPCR * compactCreateKTree(uint * xedges, uint *yedges, uint numberOfNodes,ulong numberOfEdges, uint maxl,bool dado){

	MREPCR * rep;
	rep = (MREPCR *) malloc(sizeof(struct matrixRep_cr));
	rep->maxLevel = maxl;
	
	if(dado==true){rep->level=maxl+1;}
	else{rep->level=maxl;}


	rep->numberOfNodes = numberOfNodes;
	rep->numberOfEdges = numberOfEdges;
	rep->div_level_table=0;
	rep->info=0;
	rep->info2[0]=0;
	rep->info2[1]=0;
	rep->element=0;
	rep->basex=0;
	rep->basey=0;

	ulong nedges = numberOfEdges;

	uint sizebt = 0;
	
	uint i,k,j,z,queuecont, conttmp,div_level;

	uint offsetL=0; 
	uint offsetR=0;

	uint postotal=0;
	tedge * tedges =(tedge *) malloc(sizeof(tedge)*nedges);

	
	for(i=0;i<nedges;i++){
		tedges[i].xval=xedges[i];
		tedges[i].yval=yedges[i];
	}

	QUEUEOFFCONS * q = NULL;
	finalQUEUEOFFCONS = q;
	
	
	uint * counterK = (uint *)malloc(sizeof(uint)*K*K);
	uint * boundariesK = (uint *)malloc(sizeof(uint)*(K*K+1));
	uint * pointerK = (uint *)malloc(sizeof(uint)*(K*K));

	uint tempk,tempx,tempy;

	uint * bits;
	int *bits_i;
	BITRSCR *BTL;
	

	uint * bits_BTL=(uint *) malloc(sizeof(uint)*((nedges*maxl*K*K+W-1)/W));
	int * bit_BTLi=(int *)malloc(sizeof(int)*((nedges*maxl*K*K+W-1)/W));
	for(i=0;i<((nedges*maxl*K*K+W-1)/W);i++)
		bits_BTL[i]=0;
		bit_BTLi[i]=0;
	

	bits_i = bit_BTLi;
	bits = bits_BTL;

	q = AddItemOFFCONS(q,0,nedges);
	queuecont = 1;
	for(i=0;i<maxl;i++){
		conttmp = 0;
		div_level = exp_pow(K,maxl-i);		
		for(k=0;k<queuecont;k++){
			sizebt +=K*K;			
			offsetL = q->offsetL;
			offsetR = q->offsetR;
			
			for(j=0;j<K*K;j++){
				counterK[j]=0;
				pointerK[j]=0;
			}
				
			for(j=offsetL;j<offsetR;j++){
				tedges[j].kval= (tedges[j].xval / div_level)*K+tedges[j].yval /div_level;
				tedges[j].xval=tedges[j].xval%div_level;
				tedges[j].yval=tedges[j].yval%div_level;
				 
				counterK[tedges[j].kval]++;
			}
			
			boundariesK[0]=offsetL;			
			for(j=0;j<K*K;j++){
				boundariesK[j+1]=boundariesK[j]+counterK[j];
				pointerK[j]=boundariesK[j];

				if(boundariesK[j+1]!=boundariesK[j]){
					conttmp++;
					q=AddItemOFFCONS(q,boundariesK[j],boundariesK[j+1]);
					bitset(bits,postotal);				
				}
				postotal++;
			}

			for(z=0;z<K*K;z++){
				while(pointerK[z]<boundariesK[z+1]){
					if(tedges[pointerK[z]].kval!=z){
						tempk = tedges[pointerK[z]].kval;
						tempx = tedges[pointerK[z]].xval;
						tempy = tedges[pointerK[z]].yval;

						while(tedges[pointerK[tempk]].kval==tempk){
							pointerK[tempk]++;
						}
							
						tedges[pointerK[z]].kval = tedges[pointerK[tempk]].kval;
						tedges[pointerK[z]].xval = tedges[pointerK[tempk]].xval;
						tedges[pointerK[z]].yval = tedges[pointerK[tempk]].yval;

						tedges[pointerK[tempk]].kval= tempk;
						tedges[pointerK[tempk]].xval= tempx;
						tedges[pointerK[tempk]].yval= tempy;
						pointerK[tempk]++;
					}
					else
						pointerK[z]++;
				}
			}
			
			for(z=offsetL;z<offsetR-1;z++)
				if(tedges[z].kval>tedges[z+1].kval){
					fprintf(stderr,"error aqui: %d\n",z);
					exit(-1);
				}
			q = (QUEUEOFFCONS *)RemoveItemOFFCONS(q);
		}
		queuecont = conttmp;
	}

	rep->bt_len = postotal;

	uint counttotal=0;
	while(q!=NULL){
			
		offsetL = q->offsetL;
		offsetR = q->offsetR;

		for(j=0;j<K*K;j++){
			counterK[j]=0;
		}
		
		div_level = K;	
		for(j=offsetL;j<offsetR;j++){
			tedges[j].xval = tedges[j].xval%K;
			tedges[j].yval = tedges[j].yval%K;
			tedges[j].kval= tedges[j].xval *K+tedges[j].yval;
			counterK[tedges[j].kval]++;
		}	
		
		for(j=0;j<K*K;j++){
			if(counterK[j]>0){
				conttmp++;
				counttotal++;
				bitset(bits_BTL,postotal);
			}
			postotal++;
		
		}
		q = (QUEUEOFFCONS *)RemoveItemOFFCONS(q);
	}
	

	free(counterK);
	free(boundariesK);
	free(pointerK);
	free(tedges);

	BTL = createBITRSCR(bits_BTL, postotal , 1, 20); 

	rep->btl = BTL;
	rep->btl_len = postotal;

	return rep;
}


void saveRepresentation2(MREPCR * rep, char * basename){
	if(rep == NULL){
		printf("Error! La estructura a guardar no existe\n");
		return;
	}

	string filename(basename);
	filename="../Pruebas/Compactados/KT/"+filename+".kt";
	char *dir=const_cast<char*>(filename.c_str());

	FILE * ft = fopen(dir,"w");
	fwrite(&(rep->numberOfNodes),sizeof(uint),1,ft);
	fwrite(&(rep->numberOfEdges),sizeof(ulong),1,ft);
	fwrite(&(rep->maxLevel),sizeof(uint),1,ft);
	fwrite (&(rep->btl_len),sizeof(uint),1,ft);
	fwrite (&(rep->bt_len),sizeof(uint),1,ft);
	fwrite (&(rep->btl->factor),sizeof(uint),1,ft);
	fwrite (&(rep->level),sizeof(uint),1,ft);

	fclose(ft);
	free(dir);
}


MREP * loadRepresentation2(char * basename){

	MREP* rep;

	fstream f;

	string dir(basename);
	dir="../Pruebas/Compactados/DAT/"+dir+".dat";
	
    f.open(dir,ios::in | ios::binary);

	if(f.is_open()){
		
		uint numberOfNodes,maxLevel, btl_len, bt_len, factor, level;
		ulong numberOfEdges;

		f.read(reinterpret_cast<char *>(&numberOfNodes),sizeof(uint));
		f.read(reinterpret_cast<char *>(&numberOfEdges),sizeof(ulong));
		f.read(reinterpret_cast<char *>(&maxLevel),sizeof(uint));
		f.read(reinterpret_cast<char *>(&btl_len),sizeof(uint));
		f.read(reinterpret_cast<char *>(&bt_len),sizeof(uint));
		f.read(reinterpret_cast<char *>(&factor),sizeof(uint));
		f.read(reinterpret_cast<char *>(&level),sizeof(uint));

		rep=loadRepresentation(numberOfNodes,numberOfEdges,maxLevel,btl_len,bt_len,factor,level);
		
		int integers=rep->btl->integers;
		rep->btl->dato=getDatoVector(f,integers);
		uint size=(rep->btl_len/rep->btl->s+1);
		rep->btl->rs=getRsVector(f,size);
	}	
	
	return rep;
}

MREP * loadRepresentation(uint numberOfNodes, ulong numberOfEdges, uint maxLevel, uint  btl_len, uint  bt_len, uint factor , uint level ){
	
	MREP *rep;
	rep=(MREP *)malloc(sizeof(struct matrixRep));
	rep->btl=(BITRS *)malloc(sizeof(struct bitrs));

	rep->numberOfNodes=numberOfNodes;
	rep->numberOfEdges=numberOfEdges;
	rep->maxLevel=maxLevel;
	rep->btl_len=btl_len;
	rep->bt_len=bt_len;
	rep->btl->factor=factor;
	rep->level=level;


	rep->btl->b=32;    
	uint b=rep->btl->b;
	rep->btl->s=b*rep->btl->factor;
    rep->btl->n = rep->btl_len;
	uint n= rep->btl->n;
	rep->btl->integers = n/W;
	
	return rep;
}


void destroyRepresentation(MREPCR * rep){
	destroyBITRS(rep->btl);
	if(rep->info!=NULL)
		free(rep->info);
	if(rep->info2[0]!=NULL)
		free(rep->info2[0]);
	if(rep->info2[1]!=NULL)
		free(rep->info2[1]);
	if(rep->element)
		free(rep->element);
	if(rep->basex)
		free(rep->basex);
	if(rep->basey)
		free(rep->basey);
	if(rep->div_level_table)
		free(rep->div_level_table);
	free(rep);
}

uint * compactAdjacencyList(MREPCR* rep, int x){
	rep->iniq=-1;
	rep->finq=-1;
	uint posInf;
	uint totalAdyNodes =0;
	int i, k, j, queuecont, conttmp,node,div_level,xrelat;
	AddItem2(rep,0,0,x);
	queuecont = 1;
	for(i=0;i<rep->maxLevel;i++){
		conttmp = 0;
		div_level = rep->div_level_table[i];
		for(k=0;k<queuecont;k++){
			for(j=0;j<K;j++){
				xrelat = rep->basey[rep->iniq];
				node = xrelat/div_level*K + j;
				node += rep->element[rep->iniq];
	
				if(isBitSet(rep->btl,node)){
					conttmp++;
					if(i==rep->maxLevel-1)
						AddItem2(rep,rank1(rep->btl,node)*K*K,rep->basex[rep->iniq]+j*K,rep->basey[rep->iniq]);
					else
						AddItem2(rep,rank1(rep->btl,node)*K*K,rep->basex[rep->iniq]+j*div_level,rep->basey[rep->iniq]%div_level);
				}
			}			
			RemoveItem2(rep);
		}
		queuecont = conttmp;
	}
	while(rep->iniq<=rep->finq){
		posInf =  rep->element[rep->iniq];
		for(i=0;i<K;i++){
			if(bitget(rep->btl->data,posInf+(i+(rep->basey[rep->iniq]%K)*K))){
				totalAdyNodes++;
				rep->info[totalAdyNodes]=rep->basex[rep->iniq]+i;
			}
		}
		RemoveItem2(rep);
	}
	rep->info[0]=totalAdyNodes;
	return rep->info;
}



uint * compactInverseList(MREPCR * rep, int y){
	rep->iniq=-1;
	rep->finq=-1;
	uint posInf;

	uint totalAdyNodes =0;
	int i, k, j, queuecont, conttmp,node,div_level,yrelat;

	AddItem2(rep,0,y,0);
	queuecont = 1;
	for(i=0;i<rep->maxLevel;i++){
		conttmp = 0;
		div_level = rep->div_level_table[i];
		for(k=0;k<queuecont;k++){
			for(j=0;j<K;j++){
				yrelat = rep->basex[rep->iniq];
				node = K*j+yrelat/div_level;
				node += rep->element[rep->iniq];
				if(isBitSet(rep->btl,node)){
					conttmp++;
					if(i==rep->maxLevel-1)
						AddItem2(rep,rank1(rep->btl,node)*K*K,rep->basex[rep->iniq],rep->basey[rep->iniq]+j*K);						
					else
						AddItem2(rep,rank1(rep->btl,node)*K*K,rep->basex[rep->iniq]%div_level,rep->basey[rep->iniq]+j*div_level);
				}
			}			
			RemoveItem2(rep);
		}
		queuecont = conttmp;
	}
	while(rep->iniq<=rep->finq){
		posInf = rep->element[rep->iniq];
			for(i=0;i<K;i++){
				if(bitget(rep->btl->data,posInf+(i*K+(rep->basex[rep->iniq]%K)))){
					totalAdyNodes++;
					rep->info[totalAdyNodes]=rep->basey[rep->iniq]+i;
				}
			}
		RemoveItem2(rep);
	}

	rep->info[0]=totalAdyNodes;
	return rep->info;
}


void recursiveAdjacencyList(MREPCR * rep, uint node, uint basex, uint basey, int level){
	uint posInf;
	int i, j,div_level,xrelat,newnode;
	if(level<rep->maxLevel){	
		div_level = exp_pow(K,rep->maxLevel-level);
		for(j=0;j<K;j++){
			xrelat = basey;
			newnode = xrelat/div_level*K + j;
			newnode += node;
			if(isBitSet(rep->btl,newnode)){
				if(level<rep->maxLevel-1)
					recursiveAdjacencyList(rep,rank1(rep->btl,newnode)*K*K,basex+j*div_level,basey%div_level,level+1);
				else{
					posInf = (rank1(rep->btl,newnode))*K*K;
					for(i=0;i<K;i++){
						if(bitget(rep->btl->data,posInf+(i+(basey%K)*K))){
							rep->info[0]++;
							rep->info[rep->info[0]]=basex+i+K*j;
						}
					}
				}		
			}
		}
	}
}

uint * compact2AdjacencyList(MREPCR * rep, int x){
	rep->info[0]=0;
	recursiveAdjacencyList(rep,0,0,x,0);
	uint * retorno = (uint *) malloc(sizeof(uint)*(rep->info[0] + 1));
	uint i;
	for(i=0; i<=rep->info[0]; i++){
		retorno[i] = rep->info[i];
	}
	return retorno;
}

void recursiveRangeQuery(MREP * rep,uint p1, uint p2, uint q1, uint q2, uint dp, uint dq,uint x,int l);

void recursiveRangeQuery(MREP * rep,uint p1, uint p2, uint q1, uint q2, uint dp, uint dq,uint x,int l){
	uint i,j;
	
	int leaf;
	uint y, divlevel, p1new, p2new, q1new, q2new;
	if(l==rep->maxLevel){	
		for(i=p1;i<=p2;i++){
			for(j=q1;j<=q2;j++){
				leaf=x+j;
				if(bitget(rep->btl->dato,leaf)){
					cout<<"mostrando resultados"<<endl;
					//(rep->info2)[0][0]=(rep->info2)[0][0]+1;
					//(rep->info2)[0][(rep->info2)[0][0]]=dp+i;
					//(rep->info2)[1][(rep->info2)[0][0]]=dq+j;
					cout<<dq+j+1<<endl;
					cout<<dp+i+1<<endl;
				}
			}
			leaf+=K;
		}			
	}
		
		
	if(((l==rep->maxLevel-1)&&(bitget(rep->btl->dato,x)))){
		y=rank1_v(rep->btl,x)*K*K;
		
		for(i=p1;i<=p2;i++){
 				for(j=q1;j<=q2;j++){
	 				recursiveRangeQuery(rep,0,0,0,0,dp + i,dq+j,y+K*i+j,l+1);	 		
 				}
 			}
		}
 		
 		if((x==-1)||((l<rep->maxLevel-1)&&(bitget(rep->btl->dato,x)))){
 			y = (rank1_v(rep->btl,x))*K*K;
 			
 			divlevel =	rep->div_level_table[l+1];
 			for(i=p1/divlevel;i<=p2/divlevel;i++){
 				p1new=0;
 				if(i==p1/divlevel)
 					p1new=p1 % divlevel;
 			
 				p2new=divlevel-1;
 				if(i==p2/divlevel)
 					p2new=p2 % divlevel;
 				 				
 				for(j=q1/divlevel;j<=q2/divlevel;j++){
	 				q1new=0;
	 				if(j==q1/divlevel)
	 					q1new=q1 % divlevel;
	 				
	 				q2new=divlevel-1;
	 				if(j==q2/divlevel)
	 					q2new=q2 % divlevel;
	 				recursiveRangeQuery(rep,p1new,p2new,q1new,q2new,dp + divlevel*i,dq+divlevel*j,y+K*i+j,l+1);	 		
 				}
 			}
 		}
 	}
		


uint recursiveCheckLinkQuery(MREPCR * rep, uint p, uint q, uint node, uint level);

uint recursiveCheckLinkQuery(MREPCR * rep, uint p, uint q, uint node, uint level){
	cout<<"estoy en el recursive"<<endl;
	uint posInf;
	int div_level,newnode;
	cout<<"voy a ir al div level"<<endl;
	cout<<rep->div_level_table[level]<<endl;
	div_level = rep->div_level_table[level];
	cout<<"el nivel del div es:"<<div_level<<endl;
	newnode = p/div_level*K + q/div_level;
	newnode += node;
	if(isBitSet(rep->btl,newnode)){
		if(level<rep->maxLevel-1){	

	return recursiveCheckLinkQuery(rep,p%div_level,q%div_level,rank1(rep->btl,newnode)*K*K,level+1);
	}
	else{
			posInf = (rank1(rep->btl,newnode))*K*K;
			if(bitget(rep->btl->data,posInf+(q%K+(p%K)*K))){
				return 1;
			}				
		}
	}
	return 0;
}


uint compact2CheckLinkQuery(MREPCR * rep, uint p, uint q){
	return recursiveCheckLinkQuery(rep,p,q,0,0);
}


uint compact_CheckLinkQuery(MREP * rep, uint p, uint q){
	return recursive_CheckLinkQuery(rep,p,q,0,0);
}


uint recursive_CheckLinkQuery(MREP * rep, uint p, uint q, uint node, uint level){
	uint posInf;
	int div_level,newnode;
	div_level = rep->div_level_table[level];
	newnode = p/div_level*K + q/div_level;
	newnode += node;
	if(isBitSet2(rep->btl,newnode)){
		if(level<rep->maxLevel-1){	

	return recursive_CheckLinkQuery(rep,p%div_level,q%div_level,rank1_v(rep->btl,newnode)*K*K,level+1);
	}
	else{
			posInf = (rank1_v(rep->btl,newnode))*K*K;
			if(bitget(rep->btl->dato,posInf+(q%K+(p%K)*K))){
				return 1;
			}				
		}
	}
	return 0;
}


uint recursiveCheckRangeQuery(MREP * rep,uint p1, uint p2, uint q1, uint q2, uint dp, uint dq,uint x,int l);

uint recursiveCheckRangeQuery(MREP * rep,uint p1, uint p2, uint q1, uint q2, uint dp, uint dq,uint x,int l){
	int i,j,leaf, result;
	uint y, divlevel,divlevel2, p1new, p2new, q1new, q2new;
	
	if(l==rep->maxLevel){	
		for(i=p1;i<=p2;i++){
			for(j=q1;j<=q2;j++){
				leaf=x+j;
				if(bitget(rep->btl->dato,leaf)){
					return 1;
				}
			}
			leaf+=K;
		}
		return 0;
	}
		
	if(((l==rep->maxLevel-1)&&(bitget(rep->btl->dato,x)))){
		divlevel = rep->div_level_table[l+1];
		divlevel2 = rep->div_level_table[l];

		if((p1==0)&&(q1==0)&&(p2==divlevel2-1)&&(q2==divlevel2-1)){
			return 1;
		}
	
		y=(rank1_v(rep->btl,x))*K*K;
		
		for(i=p1;i<=p2;i++){
 			for(j=q1;j<=q2;j++){
				result = recursiveCheckRangeQuery(rep,0,0,0,0,dp + i,dq+j,y+K*i+j,l+1);	 		
 				if(result){
 					return 1;
 				}
 			}
 		}
 		return 0;
	}

	///else{ //internal node
		if((x==-1)||((l<rep->maxLevel-1)&&(bitget(rep->btl->dato,x)))){
			divlevel =	rep->div_level_table[l+1];
			if(l>=0){
				divlevel2 = rep->div_level_table[l];
				if((p1==0)&&(q1==0)&&(p2==divlevel2-1)&&(q2==divlevel2-1)){
					return 1;
				}
			}
 		
 			y = (rank1_v(rep->btl,x))*K*K;

 			for(i=p1/divlevel;i<=p2/divlevel;i++){
 				p1new=0;
 				if(i==p1/divlevel)
 					p1new=p1 % divlevel;

 				p2new=divlevel-1;
 				if(i==p2/divlevel)
 					p2new=p2 % divlevel;

 				 				
 				for(j=q1/divlevel;j<=q2/divlevel;j++){
	 				q1new=0;
	 				if(j==q1/divlevel)
	 					q1new=q1 % divlevel;
	 				
	 				q2new=divlevel-1;
	 				if(j==q2/divlevel)
	 					q2new=q2 % divlevel;

	 				result = recursiveCheckRangeQuery(rep,p1new,p2new,q1new,q2new,dp + divlevel*i,dq+divlevel*j,y+K*i+j,l+1);	 		

 					if(result){
 						return 1;
 					}
	 			}
 			}
 		}
 		return 0;
}

uint compactCheckRangeQuery(MREP * rep, uint p1, uint p2, uint q1, uint q2){
	return recursiveCheckRangeQuery(rep,p1,p2,q1,q2,0,0, -1, -1);	
}

void compactCheckRangeQuery2(MREP * rep, uint p1, uint p2, uint q1, uint q2){
	recursiveRangeQuery(rep,p1,p2,q1,q2,0,0, -1, -1);	
}

uint compactCheckLinkQuery(MREPCR * rep, uint p, uint q){

	uint posInf;
	uint prelat, qrelat;
	uint i, node=0,div_level;

	for(i=0;i<rep->maxLevel;i++){
		

		div_level = rep->div_level_table[i];
		
		prelat = p/div_level;
		qrelat = q/div_level;

		node = node+prelat*K + qrelat;
		if(isBitSet(rep->btl,node)==0){
			return 0;
		}
		node=rank1(rep->btl,node)*K*K;
		p=p%div_level;
		q=q%div_level;
	}
	
		posInf = node;
		if(bitget(rep->btl->data,posInf+(q+(p)*K))){
			return 1;
		}
			
	return 0;	
}

/*
void recursiveDecompression(MREP * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, uint dp, uint dq,uint x,int l);

void recursiveDecompression(MREP * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, uint dp, uint dq,uint x,int l){
	int i,j;
	uint y, divlevel;
	if(l==rep->maxLevel){	
			if(bitget(rep->btl->data,x)){
					tmp_in[*tmp_p]=dp;
					tmp_out[*tmp_p]=dq;
					sizein[dp]++;
					(*tmp_p)++;
				}
	}
		
		
	if(((l==rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
		y=rank(rep->btl,x)*K*K;
		
		for(i=0;i<K;i++){
 				for(j=0;j<K;j++){
	 				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,dp + i,dq+j,y+K*i+j,l+1);	 		
 			}
		}
 			

		
	}
 		if((x==-1)||((l<rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
 			y = (rank(rep->btl,x))*K*K;
 			
 			divlevel =	rep->div_level_table[l+1];
			for(i=0;i<K;i++){
 				for(j=0;j<K;j++){
	 				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,dp + divlevel*i,dq+divlevel*j,y+K*i+j,l+1);	 		
 				}
 			}
 			
 		}
 	}
		
int * compactFullDecompression(MREP * rep){
	ulong p = 0;
	uint * in = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	uint * out = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	ulong * sizein = (ulong *) malloc(sizeof(ulong)*(rep->numberOfNodes+1));
	int i;
	for (i=0;i<rep->numberOfNodes;i++)
		sizein[i]=0;
	recursiveDecompression(rep, in, out, &p, sizein, 0,0,-1,-1);
	
	int *lista = (int *) malloc(sizeof(int)*(rep->numberOfEdges+rep->numberOfNodes));

	ulong suma=0;
	ulong tmp = 0;
	for (i=0;i<rep->numberOfNodes;i++){
		tmp = sizein[i];
		lista[suma]=-(i+1);
		sizein[i]=suma+1;
		suma = suma + tmp+1;
	}

	sizein[rep->numberOfNodes] = suma;
	
	for(i=0;i<rep->numberOfEdges;i++){
		lista[sizein[in[i]]] = out[i]+1;
		sizein[in[i]]++;
	}

	return lista;
	
}

*/

void recursiveDecompression(MREP * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, ulong * pointerL,uint dp, uint dq,uint x,int l);

void recursiveDecompression(MREPCR * rep, uint * tmp_in, uint * tmp_out, ulong * tmp_p, ulong * sizein, ulong * pointerL,uint dp, uint dq,uint x,int l){
	int i,j;
	uint y, divlevel;
	if(l==rep->maxLevel){	
		if(bitget(rep->btl->data,x)){
			tmp_in[*tmp_p]=dp;
			tmp_out[*tmp_p]=dq;
			sizein[dp]++;
			(*tmp_p)++;
		}
	}
		
		
	if(((l==rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
		//y=rank(rep->btl,x)*K*K;
		y = pointerL[l+1];
		pointerL[l+1]+=K*K;
		for(i=0;i<K;i++){
			for(j=0;j<K;j++){
 				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,pointerL,dp + i,dq+j,y+K*i+j,l+1);	 		
 			}
		}
	}

	if((x==-1)||((l<rep->maxLevel-1)&&(bitget(rep->btl->data,x)))){
		y = pointerL[l+1];
		pointerL[l+1]+=K*K;
		
		divlevel =	rep->div_level_table[l+1];
		for(i=0;i<K;i++){
			for(j=0;j<K;j++){
				recursiveDecompression(rep,tmp_in, tmp_out, tmp_p,sizein,pointerL,dp + divlevel*i,dq+divlevel*j,y+K*i+j,l+1);	 		
			}
		}
	
	}
}
		
int * compactFullDecompression(MREPCR * rep){
	ulong p = 0;
	uint * in = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	uint * out = (uint *) malloc(sizeof(uint)*rep->numberOfEdges);
	ulong * sizein = (ulong *) malloc(sizeof(ulong)*(rep->numberOfNodes+1));
	int i;
	for (i=0;i<rep->numberOfNodes;i++)
		sizein[i]=0;

	ulong * pointerL = (ulong *) malloc(sizeof(ulong)*(rep->numberOfNodes+1));
	
	pointerL[0]=0;
	pointerL[1]=K*K;
	for (i=2;i<=rep->maxLevel;i++){
		pointerL[i]=(rank1(rep->btl,pointerL[i-1]-1)+1)*K*K;
	}

	recursiveDecompression(rep, in, out, &p, sizein, pointerL, 0,0,-1,-1);
	
	int *lista = (int *) malloc(sizeof(int)*(rep->numberOfEdges+rep->numberOfNodes));

	ulong suma=0;
	ulong tmp = 0;
	for (i=0;i<rep->numberOfNodes;i++){
		tmp = sizein[i];
		lista[suma]=-(i+1);
		sizein[i]=suma+1;
		suma = suma + tmp+1;
	}

	sizein[rep->numberOfNodes] = suma;
	
	for(i=0;i<rep->numberOfEdges;i++){
		lista[sizein[in[i]]] = out[i]+1;
		sizein[in[i]]++;
	}

	return lista;
}

#endif
