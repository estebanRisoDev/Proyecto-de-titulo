#include "defines.hpp"
#include "basic.hpp"
#include "vector"
#include <time.h>
#include <chrono>

#ifndef BITARRAYRS_HPP
#define BITARRAYRS_HPP


typedef struct bitrs{
    vector<uint> dato;
    vector<uint> rs;
    char owner; //x definir 1
    int integers; // 4
    uint factor,b,s; // 4+4+4
    uint n;     //4     5+24=29
}BITRS;


typedef struct bitrs_cr{
    uint *data;  // 4
    char owner; //x definir 1
    uint integers; // 4
    uint factor,b,s; // 4+4+4
    uint *Rs;  			//4		//superblock array
    uint n;     //4     5+24=29
}BITRSCR;




void buildRank(BITRSCR * br);
uint buildRankSub(BITRSCR * br, uint ini,uint bloques);
uint bselect(BITRSCR * br,uint x);



BITRSCR * createBITRSCR( uint *bitarray, uint _n, char owner, uint _factor) {
  BITRSCR * br =(BITRSCR *) malloc(sizeof(struct bitrs_cr));
  br->data=bitarray;
  br->owner = owner;
  br->n=_n;
  uint lgn=bits(br->n-1);
  br->factor=_factor;
  if (_factor==0) br->factor=lgn;
  else br->factor=_factor;
  br->b=32;
  br->s=br->b*br->factor;
  br->integers = br->n/W;
  buildRank(br);
  return br;
}





void destroyBITRS(BITRSCR *br) {
  free(br->Rs);
  if (br->owner) free(br->data);
  free(br);
}


//Metodo que realiza la busqueda d
void buildRank(BITRSCR * br) {
	uint i;
  uint num_sblock = br->n/br->s;
  br->Rs = (uint *) malloc(sizeof(uint)*(num_sblock+1));   // +1 pues sumo la pos cero
  for(i=0;i<num_sblock+1;i++)
    br->Rs[i]=0;
  uint j;
  br->Rs[0]=0;
  for (j=1;j<=num_sblock;j++) {
    br->Rs[j]=br->Rs[j-1];
    br->Rs[j]+=buildRankSub(br, (uint)(j-1)*(br->factor),br->factor);
  }
}


uint buildRankSub(BITRSCR * br, uint ini,uint bloques) {
  uint i;
  uint rank=0,aux;
  for(i=ini;i<ini+bloques;i++) {
    if (i <= br->integers) {
      aux=br->data[i];
      rank+=popcount(aux);
    }
  }
  return rank;                   //retorna el numero de 1's del intervalo

}


uint rank1(BITRSCR * br, uint i) {
  uint a;
  if(i+1==0) return 0;
  ++i; 
  uint resp=br->Rs[i/br->s];
  uint aux=(i/br->s)*(br->factor);
  for (a=aux;a<i/W;a++)
    resp+=popcount(br->data[a]);
  resp+=popcount(br->data[i/W]  & ((1<<(i & mask31))-1));
  

  return resp;
}


uint rank1_v(BITRS * br, uint i) {

  uint a;
  if(i+1==0) return 0;
  ++i; 
  uint resp=br->rs[i/br->s];
  uint aux=(i/br->s)*(br->factor);

  for (a=aux;a<i/W;a++)
      resp+=popcount(br->dato[a]);
  

  resp+=popcount(br->dato[i/W]  & ((1<<(i & mask31))-1));

  return resp;
}


uint isBitSet(BITRSCR * br, uint i) {
  if(i<0){
    return 0u;
  }
  uint resultado;

  resultado = (1u << (i % W)) & br->data[i/W];

  return resultado;
}

int isBitSetInt(vector<int> data, uint i) {
  if(i<0){
    return 0u;
  }
  uint resultado;

  resultado = (1u << (i % W)) & data[i/W];

  return resultado;
}




bool isBitSet2(BITRS * br, uint i) {

  if(i<0){
    return 0u;
  }

  uint resultado= (1u << (i % W)) & br->dato[i/W];
  
  if(resultado==0)return false;
  else return true;
}




/*
int save(BITRS * br, FILE *f) {
	uint s,n;
	s=br->s;
	n=br->n;
  if (f == NULL) return 20;
  if (fwrite (&(n),sizeof(uint),1,f) != 1) return 21;
  if (fwrite (&(br->factor),sizeof(uint),1,f) != 1) return 21;
  if (fwrite (br->data,sizeof(uint),n/W+1,f) != n/W+1) return 21;
  if (fwrite (br->Rs,sizeof(uint),n/s+1,f) != n/s+1) return 21;
  return 0;
}


int load(BITRS * br, FILE *f) {
  if (f == NULL) return 23;
  if (fread (&(br->n),sizeof(uint),1,f) != 1) return 25;
  br->b=32;    
  uint b=br->b;                      // b is a word
  if (fread (&(br->factor),sizeof(uint),1,f) != 1) return 25;
  br->s=b*br->factor;
  uint s=br->s;
  uint n= br->n;
  //uint aux=(n+1)%W;
  //if (aux != 0)
  //  integers = (n+1)/W+1;
  //else
  //  integers = (n+1)/W;
  br->integers = n/W;
  br->data= (uint *) malloc(sizeof( uint) *(n/W+1));
  if (!br->data) return 1;
  if (fread (br->data,sizeof(uint),br->n/W+1,f) != n/W+1) return 25;
  br->owner = 1;
  br->Rs=(uint*)malloc(sizeof(uint)*(n/s+1));
  if (!br->Rs) return 1;
  if (fread (br->Rs,sizeof(uint),n/s+1,f) != n/s+1) return 25;
  return 0;
}



BITRS * createBITRSFile(FILE *f, int *error) {
	 BITRS * br = (BITRS *) malloc(sizeof(struct sBITRS));
  *error = load(br,f);
  return br;
}
*/

uint spaceRequirementInBits(BITRS * br) {
  return (br->owner?br->n:0)+(br->n/br->s)*sizeof(uint)*8 +sizeof(struct bitrs)*8;
}

uint lenght_in_bits(BITRSCR * br) { return br->n; };

uint prev(BITRSCR * br,uint start) {
  // returns the position of the previous 1 bit before and including start.
  // tuned to 32 bit machine

  uint i = start >> 5;
  int offset = (start % W);
  uint answer = start;
  uint val = br->data[i] << (Wminusone-offset);

  if (!val) { val = br->data[--i]; answer -= 1+offset; }

  while (!val) { val = br->data[--i]; answer -= W; }
                //
  if (!(val & 0xFFFF0000)) { val <<= 16; answer -= 16; }
  if (!(val & 0xFF000000)) { val <<= 8; answer -= 8; }

  while (!(val & 0x80000000)) { val <<= 1; answer--; }
  return answer;
}


uint select1(BITRSCR * br,uint x) {
  return bselect(br,x);
}


uint bselect(BITRSCR * br,uint x) {
  if(x==0) return 0;
  // returns i such that x=rank(i) && rank(i-1)<x or n if that i not exist
  // first binary search over first level rank structure
  // then sequential search using popcount over a int
  // then sequential search using popcount over a char
  // then sequential search bit a bit

  //binary search over first level rank structure
  uint n= br->n;
  uint s= br->s;
  uint b=br->b;
  uint l=0, r=n/s;
  uint integers = br->integers;
  uint factor = br->factor;
  uint mid=(l+r)/2;
  uint rankmid = br->Rs[mid];
  while (l<=r) {
    if (rankmid<x)
      l = mid+1;
    else
      r = mid-1;
    mid = (l+r)/2;
    rankmid = br->Rs[mid];
  }
  //sequential search using popcount over a int
  uint left;
  left=mid*factor;
  x-=rankmid;
  uint j=br->data[left];
  uint ones = popcount(j);
  while (ones < x) {
    x-=ones;left++;
    if (left > integers) return n;
    j = br->data[left];
    ones = popcount(j);
  }
  //sequential search using popcount over a char
  left=left*b;
  rankmid = popcount8(j);
  if (rankmid < x) {
    j=j>>8;
    x-=rankmid;
    left+=8;
    rankmid = popcount8(j);
    if (rankmid < x) {
      j=j>>8;
      x-=rankmid;
      left+=8;
      rankmid = popcount8(j);
      if (rankmid < x) {
        j=j>>8;
        x-=rankmid;
        left+=8;
      }
    }
  }

  // then sequential search bit a bit
  while (x>0) {
    if  (j&1) x--;
    j=j>>1;
    left++;
  }
  return left-1;
}


uint select0(BITRSCR * br,uint x) {
  // returns i such that x=rank_0(i) && rank_0(i-1)<x or n if that i not exist
  // first binary search over first level rank structure
  // then sequential search using popcount over a int
  // then sequential search using popcount over a char
  // then sequential search bit a bit

  //binary search over first level rank structure
    uint n= br->n;
  uint s= br->s;
  uint factor = br->factor;
  uint integers = br->integers;
  uint b= br->b;
  uint l=0, r=n/s;
  uint mid=(l+r)/2;
  uint rankmid = mid*factor*W-(br->Rs)[mid];
  while (l<=r) {
    if (rankmid<x)
      l = mid+1;
    else
      r = mid-1;
    mid = (l+r)/2;
    rankmid = mid*factor*W-(br->Rs)[mid];
  }
  //sequential search using popcount over a int
  uint left;
  left=mid*factor;
  x-=rankmid;
  uint j=br->data[left];
  uint zeros = W-popcount(j);
  while (zeros < x) {
    x-=zeros;left++;
    if (left > integers) return n;
    j = br->data[left];
    zeros = W-popcount(j);
  }
  //sequential search using popcount over a char
  left=left*b;
  rankmid = 8-popcount8(j);
  if (rankmid < x) {
    j=j>>8;
    x-=rankmid;
    left+=8;
    rankmid = 8-popcount8(j);
    if (rankmid < x) {
      j=j>>8;
      x-=rankmid;
      left+=8;
      rankmid = 8-popcount8(j);
      if (rankmid < x) {
        j=j>>8;
        x-=rankmid;
        left+=8;
      }
    }
  }

  // then sequential search bit a bit
  while (x>0) {
    if  (j%2 == 0 ) x--;
    j=j>>1;
    left++;
  }
  left--;
  if (left > n)  return n;
  else return left;
}

#endif
