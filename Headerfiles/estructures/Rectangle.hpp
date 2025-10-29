#include "../k2tree/Point.hpp"
#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP

#include <algorithm>


class Rectangle{
    private:
        Point *S;
        Point *T;

        Point min(Point p1, Point p2){
            return Point(std::min(p1.getX(),p2.getX()), std::min(p1.getY(),p2.getY()) );
        }

        Point max(Point p1, Point p2){
            return Point(std::max(p1.getX(),p2.getX()), std::max(p1.getY(),p2.getY()) );
        }
    
    public:



        Rectangle(){};//constructor default, no se va usar, pero es para solucionar errores relacionados

        ~Rectangle(){};
            
        Rectangle(Point *S, Point *T){
            this->S=S;
            this->T=T;
        }

        Point getS(){
            return *S;
        }

        Point getT(){
            return *T;
        }

        bool Contains(Point point){
            return point.equalsxy(max(*S, min(*T, point)));;
        }
        
};

#endif