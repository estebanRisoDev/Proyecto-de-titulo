#ifndef KNNELEMENTQUEUE_HPP
#define KNNELEMENTQUEUE_HPP
#include "../../BitArray/defines.hpp"
#include "../../estructures/Rectangle.hpp"





class KNNElementQueue{

    private:
        uint pos;
        Rectangle cuadrant;
        int64_t distance;
        int level;
    
    public:

        KNNElementQueue(uint pos, Rectangle cuadrant, int64_t distance, int level){
            this->pos=pos;
            this->cuadrant=cuadrant;
            this->distance=distance;
            this->level=level;
        }

        ~KNNElementQueue(){}
       
        int getLevel(){
            return level;
        }
        

        uint getPos(){
            return pos;
        }

        int64_t getDistance(){
            return distance;
        }

        Rectangle getCuadrant(){
            return cuadrant;
        }

        void toString(){
            return;
        }

};


#endif