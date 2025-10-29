#include "K2tree.hpp"
#include "../estructures/KNN/KNNElementQueue.hpp"
#include <queue>
#include "../Comparators/KNN/CandidatesMaxComparator.hpp"
#include "../Comparators/KNN/KNNElementQueueComparator.hpp"
#include <cstdio>
#include <stdlib.h>
#include <unistd.h>

#ifndef KNN_HPP
#define KNN_HPP




class KNN{
    private:

        int k;
        BITRS *TL;
        int ALTURA;
        int CANTIDADHIJOS;
        K2Tree *K2TREE;
        int cantDistanceCalculation;

        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MINHEAP> pQueue;
        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> candidates;



        Point traductionPointQ(Point q) {  //transformación cordenada cartesiana
            return  Point(q.getY(),abs(q.getX()- K2TREE->getNodes())+1);
        }

        bool isCandidate(uint k, int64_t minD){ 
           
           KNNElementQueue objs=candidates.top();
           int64_t dist=objs.getDistance();
           
           return(candidates.size()<k || minD < dist);
        }

        bool isLeaf(KNNElementQueue tmp){
            return tmp.getLevel() > ALTURA;
        }


        void evaluateCandidates(KNNElementQueue tmp, uint k, Point q){
            

            int accumX=0;
            int accumY=0;
            uint posHijo= tmp.getPos();
            Point S=tmp.getCuadrant().getS();
            Point T=tmp.getCuadrant().getT();
            int secuence= getSecuence(tmp); 
            bool flag=false;
            Rectangle temp;

            if(tmp.getLevel() == K2TREE->getHeight()){
                accumX = S.getX();
                accumY = T.getY();
                flag==true;
            }else{
                accumX = S.getX();
                accumY = T.getY() - secuence;
            }            


            for(int i=1;i<=CANTIDADHIJOS;i++){

                if(accumX >T.getX()){
                    accumY=accumY-secuence-1;
                    accumX=S.getX();
                }

                if(isBitSet2(TL,posHijo)){
                    temp= Rectangle(new Point(accumX,accumY),new Point(accumX+secuence,accumY+secuence));
                    int64_t minD= minDist(q,temp);
                    
                    if(isCandidate(k,minD)){  
                        KNNElementQueue a= KNNElementQueue(rank1_v(TL,posHijo)*K*K,temp,minD,tmp.getLevel()+1);
                        pQueue.push(a);
                    }
                }    
                
                accumX=accumX+secuence+1;
                posHijo++;
            }          
        }


        int getSecuence(KNNElementQueue tmp){
            return (tmp.getCuadrant().getT().getY()- tmp.getCuadrant().getS().getY())/K;
        }


        int64_t minDist( Point p, Rectangle R){
            
            int64_t R1=0;
            int64_t R2=0;
       
            if(R.Contains(p)){
                return 0;
            } else{
                if(p.getX() < R.getS().getX()){
                    R1=R.getS().getX();
                } else{
                    if(p.getX() > R.getT().getX()){
                        R1=R.getT().getX();
                    } else{
                        R1= p.getX();
                    }
                }
            }
            if (p.getY() < R.getS().getY()){
                R2=R.getS().getY();
            } else{
                if( p.getY() > R.getT().getY()){
                    R2=R.getT().getY();
                } else{
                    R2= p.getY();
                }
            }

            cantDistanceCalculation++;
            int64_t value1 = p.getX() - R1;
            int64_t value2 = p.getY() - R2;
            
            return llabs(value1*value1)+ llabs(value2*value2); 
        }


    public:
        

        KNN(K2Tree *k2){
            this->K2TREE=k2;
            this->TL=k2->getBitArrayRS();
            this->k=k2->getK();
            ALTURA=k2->getHeight();
            CANTIDADHIJOS=K*K;
        }

        ~KNN(){};

        void imprimir(){

            cout<<"abriendo knn"<<endl;

            priority_queue<KNNElementQueue,vector<KNNElementQueue>,MINHEAP> resultado=pQueue;
                cout<<"pQueue"<<endl<<endl;

                while(!resultado.empty()){
                    KNNElementQueue a =resultado.top();
                    cout<<a.getDistance()<<" ";
                    resultado.pop();
                }
                cout<<endl;


                priority_queue<KNNElementQueue,vector<KNNElementQueue>,MINHEAP> resultado3=pQueue;
                
                cout<<endl<<"S:"<<endl;

                while(!resultado3.empty()){
                    KNNElementQueue a =resultado3.top();
                     cout <<"X:"<<a.getCuadrant().getS().getX()<< " ";
                     cout <<"Y:"<<a.getCuadrant().getS().getY()<< " "<<endl;
                    resultado3.pop();
                }


                cout<<endl;


                priority_queue<KNNElementQueue,vector<KNNElementQueue>,MINHEAP> resultado2=pQueue;
                
                cout<<"T:"<<endl;

                while(!resultado2.empty()){
                    KNNElementQueue a =resultado2.top();
                     cout <<"X:"<<a.getCuadrant().getT().getX()<< " ";
                     cout <<"Y:"<<a.getCuadrant().getT().getY()<< " "<<endl;
                    resultado2.pop();
                }

                cout<<endl;
                
        	
	}

	priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> transform(){

		priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> queue=candidates;
	 	priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> data;

		while(!queue.empty()){

			KNNElementQueue a=queue.top();
			Point *S= new Point(a.getCuadrant().getS().getY(),K2TREE->getNodes()-a.getCuadrant().getS().getX());
			Point *T= new Point(a.getCuadrant().getT().getY(),K2TREE->getNodes()-a.getCuadrant().getT().getX());
			Rectangle rectangulo(S,T);
			KNNElementQueue b(a.getPos(),rectangulo,a.getDistance(),a.getLevel());
			data.push(b);
		}

	  return data;
	}
	

        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> findNNQ(int k, Point q){

            q=traductionPointQ(q);
            uint dist = -1;

            Rectangle quad= Rectangle( new Point(1,1) , new Point(K2TREE->getNodes(),K2TREE->getNodes()));
            KNNElementQueue e= KNNElementQueue(0,quad,minDist(q,quad),1);

            pQueue.push(e);
            int cantidad_ciclos=0;

            while(!pQueue.empty()){
             
                KNNElementQueue tmp= pQueue.top();
                //imprimir();
		pQueue.pop();
                if(candidates.size()!=0){ //identificar si tiene o no elementos;
                    KNNElementQueue obj= candidates.top();
                    dist=obj.getDistance();
                }
                if((candidates.size()==k)&&(tmp.getDistance() >=dist)){
                    return candidates;
                }
                if(isLeaf(tmp)){
                    if(candidates.size()<k){
                        candidates.push(tmp);
                    } else{
                        KNNElementQueue aux= candidates.top();
                        if(tmp.getDistance() < aux.getDistance()){
                            candidates.pop();
                            candidates.push(tmp);
                        }
                    }
                } else {
                        evaluateCandidates(tmp, k, q);
                }
            }
            
	    
	   //candidates=transform();

            return candidates;  
        }

   
        int getCantDistanceCalculations(){
            return cantDistanceCalculation;
        }

        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MINHEAP> getPqueue(){
            return pQueue;
        }

        priority_queue<KNNElementQueue,vector<KNNElementQueue>,MAXHEAP> getCandidates(){
            return candidates;
        }

};

#endif
