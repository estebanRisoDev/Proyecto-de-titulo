#ifndef KNNELEMENTQUEUECOMPARATOR_HPP
#define KNNELEMENTQUEUECOMPARATOR_HPP
#include "../../estructures/KNN/KNNElementQueue.hpp"



struct MINHEAP{

    bool operator()(KNNElementQueue const& o1 , KNNElementQueue const& o2){

        KNNElementQueue a=o1;
        KNNElementQueue b=o2;

        return a.getDistance() > b.getDistance();
    }
};

#endif