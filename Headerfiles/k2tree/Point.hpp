#ifndef POINT_HPP
#define POINT_HPP

#include <string>
#include <math.h>

using namespace std;

class Point{
  private:
    int x;
    int y;
    int id;
    int correlative;
    double distance;

  public:
    Point( int x, int y ){
        this->x = x;
        this->y = y;
    }


    Point();

    ~Point(){}
    
    int getX(){
        return x;
    }

    void setX( int x){
        this->x=x;
    }

    int getY(){
        return y;
    }

    void setY(int y){
        this->y = y;
    }

    bool equalsxy(Point o){
      if(this->x!=o.x){
          return false;
      }
      return this->y == o.y;
        
    }

    double getDistance(){
        return distance;
    }

    void setDistance(double distance){
        this->distance = distance;
    }

};


#endif
