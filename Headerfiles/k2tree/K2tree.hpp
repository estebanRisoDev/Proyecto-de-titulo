#include <iostream>
#include <vector>
#include "../BitArray/MatrixRep.hpp"

#ifndef K2TREE_HPP
#define K2TREE_HPP

using namespace std;

class K2Tree{

    private:

    MREP* tl;
    string bitarray;
  

    public:
    K2Tree();
    ~K2Tree(){};

    K2Tree(MREP *a, string bitarray){
      tl=a;
      this->bitarray=bitarray;
    }

     K2Tree(MREP *a){
      tl=a;
    }


    string get_StringBitarray(){
        return bitarray;     
    }

    BITRS* getBitArrayRS(){
        return tl->btl;
    }

    uint btlLenght(){
        return tl->btl_len;
    }

    int getNodes(){
      return tl->numberOfNodes;
    }

    int getK(){
      return 2;
    }

    int getHeight(){
      return tl->level;
    }

    bool checklink(uint i, uint j){
      if(compactCheckLinkQuery(tl,i,j)==0){
        return false;
      }
      return true;
    }



};

#endif



