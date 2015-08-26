// Author: Nabin Mulepati 2010

#include<iostream>
using namespace std;

class Point {
public: 
	double x;
	double y;
	double theta;
    
	Point() {
		x=0;
        y=0;
        theta=NULL;
	}
    
	Point(double a, double b) {
		x = a;
		y = b;
	}
    
	Point operator=(const Point& e) {
		x = e.x;
		y = e.y;
		theta = e.theta;
		return *this;
	}
    
	bool operator !=(const Point& e) {
        if(x == e.x && y == e.y) {
			return false;
        } else {
			return true;
        }
	}
    
	bool operator ==(const Point& e) {
        if(x == e.x && y == e.y) {
			return true;
        } else {
			return false;
        }
	}
    
	void Print() {
		cout<<x << "  " <<y<<endl;
	}
};