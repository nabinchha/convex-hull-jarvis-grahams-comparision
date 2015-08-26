// ConvexHull.cpp : Defines the entry point for the console application.
// Author: Nabin Mulepati 2010
// Algorithms
// Susquehanna University
// Convex Hull - Final Project Test Program
///////////////////////////////////////////


#include "stdafx.h"
#include "Point.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <limits>
#include <stack>
#include <shlwapi.h>
#include <fstream>
using namespace std;

int g_TickCount;
#define TIC g_TickCount = GetTickCount();                 
#define TOC g_TickCount = GetTickCount() - g_TickCount + 1;

const double PI = atan(1.0)*4;

//Jarvis march
void CalculateTheta(vector<Point> &points) {
	Point p0 = points[0];
	for (int i = 0; i < points.size(); ++i) {
		double t;
        if(p0.y > points[i].y) {
			t= 2 * PI + atan2((points[i].y-p0.y),(points[i].x-p0.x));
        } else {
			t= atan2((points[i].y-p0.y),(points[i].x-p0.x));
        }
		points[i].theta = t;
	}
}

//Graham scan
Point Top(stack<Point> &s) {
	return s.top();
}

//Graham scan
Point NextToTop(stack<Point> &s) {
	Point z = s.top();
	s.pop();
	Point w = s.top();
	s.push(z);
	return w;
}


//Graham scan
int Partition(vector<Point>& v, int a, int b) {
	int pivot = b;
	int j = a;
	int i = a;
	for (i = a; i<b; ++i) {
		if (v[i].theta < v[pivot].theta) {
			swap(v[i], v[j]);
			j++;
		}
	}
	swap(v[i], v[j]);
	return j;
}

//Graham scan
void QuickSort(vector<Point>& v, int i, int j) {
	if (i>=j) {
		return;
	}
	int k = Partition(v, i, j);
	QuickSort(v, i, k-1);
	QuickSort(v, k+1, j);
}


// Both
void Print(vector<Point> p) {
	cout<<endl<<endl;
	for (int i = p.size()-1; i>=0; i--) {
		cout << p[i].x << "    " << p[i].y <<endl;
	}
}

//Graham scan
void Refine(vector<Point> &p) {
	for (int i = 0; i < p.size()-1; ++i) {
		int j = i;
		double max = 0;
		int maxi;
		int mini;
		while (j<p.size()-1 && p[j].theta == p[j+1].theta) {
			if (p[j].r > p[j+1].r) {
				maxi = j;
				max = p[j].r;
				mini = j+1;
			} else {
				max = p[j+1].r;
				maxi = j+1;
				mini = j;
			}
			p.erase(p.begin()+mini);
			j++;
		}
	}
}

//Graham scan
bool CheckTurn(Point p1, Point p2, Point p3) {
	double t = (p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x);
    if (t <= 0) { //if angle = 0 collinear; if >0 on left; if >0 on right
		return true;
    } else {
		return false;
    }
}

//Graham Scan
void GrahamScan(vector<Point>& points, Point pivot, stack<Point> &s) {
	s.push(pivot);
	s.push(points[0]);
	s.push(points[1]);
	int i = 2;
	double pseudoAngle;
	for (int i = 2; i<points.size(); ++i) {
		while (CheckTurn(NextToTop(s),Top(s),points[i])) {
			s.pop();			
		}
		s.push(points[i]);	
	}
}

//Graham scan
void CalculateDistance(vector<Point> &p, Point pivot) {
	for (int i = 0; i < p.size(); ++i) {
		p[i].r = sqrt((p[i].x-pivot.x)*(p[i].x-pivot.x) + (p[i].y-pivot.y)*(p[i].y-pivot.y));
	}
}

//Both
int FindPivot(const vector<Point> &points) {
	int minY = INT_MAX;
	int minX;
	int index;
	for (int i = 0; i <points.size(); ++i) {
		if (points[i].y < minY) {
			index = i;
			minY = points[i].y;
			minX = points[i].x;
		} else if(points[i].y == minY) {
			if(points[i].x < minX) {
				index = i;
				minX = points[i].x;
			}
		}
	}
	return index;
}

//Jarvis march
int FindNewHullIndex(const vector<Point> &points, Point hull, double theta) {
	double minAngle = INT_MAX;
	int minIndex = 0;
	double maxcs = -INT_MAX;
	for (int i = 0; i < points.size(); ++i) {
		if (points[i] != hull) {
			double dx = points[i].x - hull.x;
			double dy = points[i].y - hull.y;
			double t = atan2(dy, dx);
			double cs = cos(t - theta);
			if(cs > maxcs) {
				maxcs = cs;
				minIndex = i;
			}
		}
	}
	return minIndex;
}

//Jarvis march
vector<Point> JarvisMarch(vector<Point> &points) {
	vector<Point> hull;
	hull.push_back(points[0]);
	int hPointIndex = 0;
	int i = 0;
	double theta = 0;
	while (i < points.size()) {
		int newHullIndex = FindNewHullIndex(points, hull[hPointIndex], theta);
		if (points[newHullIndex].found != true) {
			if (points[newHullIndex] != hull[0]) {
				points[newHullIndex].found = true;
				Point prevHull = hull[hPointIndex];
				Point newHull = points[newHullIndex];
				hull.push_back(newHull);
				hPointIndex++;
				theta = atan2(newHull.y-prevHull.y, newHull.x-prevHull.x);
            } else {
				break;
            }
		}
		i++;
	}
	return hull;
}

int _tmain(int argc, _TCHAR* argv[]) {
	srand(3);
	bool decisions= false;
	do {
		vector<Point> pointsJ;
		vector<Point> pointsG;
		string input;
		cout<<"File(F)/Random numbers(R)/Points on a Circle(C): ";
		cin>>input;
		
		if(input =="f" || input =="F") {
			ifstream in;
			in.open("data.txt");
			if(in.fail()) {
				cerr << "Could not open the input file." << endl;
				return 1;
			}
			while(in.good() && in.eof() != true) {
				double x, y;
				in >> x;
				in >> y;
				pointsG.push_back(Point(x, y));
				pointsJ.push_back(Point(x, y));
			}
		} else if(input == "r" || input == "R") {
			/*----------------Generate Random Points----------------*/
			cout<<"Enter number of random points to generate: ";
			int N;
			cin>>N;												//Number of points to generate
			int a = 150000000;
			int high = 1500000;
			int low = -1500000;
			for(int i = 0; i < N; ++i) {
				int x = rand();
				int y = rand();
				Point p = Point(x, y);
				pointsJ.push_back(p);
				pointsG.push_back(p);
			}
		} else {
			cout<<"Enter step size for angle: ";
			float step; 
			cin >> step;
			for (float i = 0; i <= 2*PI; i = i+step) {
				double x = 1000000000 * cos(i);
				double y = 1000000000 * sin(i);
				Point p = Point(x, y);
				pointsJ.push_back(p);
				pointsG.push_back(p);
			}
		}

		/*--------------------Jarvis March---------------------*/
		TIC;
		int index = FindPivot(pointsJ);
		swap(pointsJ[0], pointsJ[index]);
		vector<Point> convexHull = JarvisMarch(pointsJ);
		TOC;
		cout<<"Convex Hull of Jarvis March:";
		Print(convexHull);
		int jarvisTime = g_TickCount;

		/*--------------------Grahams Scan---------------------*/
		TIC;
		int pindex = FindPivot(pointsG);
		Point pivot = Point(pointsG[pindex].x, pointsG[pindex].y);
		swap(pointsG.front(), pointsG[pindex]);
		CalculateTheta(pointsG);
		pointsG.erase(pointsG.begin());		
		QuickSort(pointsG, 0, pointsG.size()-1); 
		CalculateDistance(pointsG, pivot);
		Refine(pointsG);
		stack<Point> hull;
		GrahamScan(pointsG, pivot, hull);
		TOC;
		int size = hull.size();
		cout<<endl<<"Convex Hull of Graham's Scan:"<<endl;
		while (!hull.empty()) {
			Point p = hull.top();
			cout<<p.x<<"    "<<p.y<<endl;
			hull.pop();
		}
		
		cout <<endl;
		cout <<"Using Jarvis March it took " << jarvisTime << " miliseconds to find "<<convexHull.size()<<" points."<<endl<<endl;
		cout << "Using Graham's Scan it took " << g_TickCount << " miliseconds to find "<< size << " points."<<endl<<endl;

		cout<<endl<<endl<<"Continue?(Y/N) ?";
		string decision; 
		cin >> decision;
        if(decision == "y" || decision == "Y") {
			decisions = true;
        } else {
			decisions = false;
        }
	} while(decisions == true);
	return 0;
}