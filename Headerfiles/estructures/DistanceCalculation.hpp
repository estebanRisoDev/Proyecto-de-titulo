#include "Rectangle.hpp"
#ifndef DISTANCECALCULATION_HPP
#define DISTANCECALCULATION_HPP


class DistanceCalculation{
    private: 
    
    int cantDistanceCalculation;
    
    public:
    
    int getCantDistanceCalculation(){
        return cantDistanceCalculation;
    }
    
    void setCantDistanceCalculation(int cantDistanceCalculation){
        this->cantDistanceCalculation = cantDistanceCalculation;
    }


};

#endif