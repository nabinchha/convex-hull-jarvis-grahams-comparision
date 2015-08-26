// Author: Nabin Mulepati 2010

#include<iostream>
using namespace std;

typedef double real;

class Point {
public: 
	real x;
	real y;
	bool found;
	double theta;
	double r;
    
	Point() {
		x=0;y=0;theta=NULL;r=NULL;
		found = false;
	}
    
	Point(real a, real b) {
		x = a;
		y = b;
		theta = NULL; 
		r = NULL; 
		found = false;
	}
    
	Point operator=(const Point& e) {
		x = e.x;
		y = e.y;
		theta = e.theta;
		return *this;
	}
    
	bool operator !=(const Point& e)const {
        if (x == e.x && y==e.y) {
			return false;
        } else {
			return true;
        }
	}
    
	bool operator ==(const Point& e)const {
        if (x == e.x && y==e.y) {
			return true;
        } else {
			return false;
        }
	}
    
	void Print() {
		cout<<x << "  " <<y<<endl;
	}
};