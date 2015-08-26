// Jarvis March.cpp : Defines the entry point for the console application.
// Author: Nabin Mulepati 2010

#include "stdafx.h"
#include "Point.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
#include <shlwapi.h>
using namespace std;

int g_TickCount;
#define TIC g_TickCount = GetTickCount();                 
#define TOC g_TickCount = GetTickCount() - g_TickCount + 1;

int Partition(vector<Point>& v, int a, int b, string choice) {
	int pivot = b;
	int j = a;
	int i = a;
    
	if (choice == "sort y") {
		for (i = a; i < b; ++i) {
            if (v[i].y < v[pivot].y) {//ascending
				swap(v[i], v[j]);
				j++;
			}
		}
	} else if(choice == "sort theta") {
		for (i = a; i < b; ++i) {
            if (v[i].theta < v[pivot].theta) {//ascending
				swap(v[i], v[j]);
				j++;
			}
		}
	}
	swap(v[i], v[j]);
	return j;
}

void QuickSort(vector<Point>& v, int i, int j, string choice) {
	if (i >= j) {
		return;
	}
	int k = Partition(v, i, j, choice);
	QuickSort(v, i, k-1, choice);
	QuickSort(v, k+1, j, choice);
}


int FindP0(vector<Point> v) {
	int temp = v[0].y;
	int min = INT_MAX;
	int i = 0;
	int index;
	while (temp == v[i].y) {
		if (v[i].x < min) {
			min = v[i].x;
			index = i;
		}
		i++;
	}
	return index;
}


void Print(vector<Point> p) {
	cout<<endl<<endl;
	for (int i = 0; i<p.size(); i++) {
		cout << p[i].x << "          " << p[i].y   << "          " <<endl;
	}
}

void CalculateTheta(vector<Point> &points) {
	Point p0 = points[0];
	for (int i = 0; i < points.size(); ++i) {
		double t;
        if (p0.y > points[i].y) {
			t= 2* 3.14159265 + atan2((points[i].y-p0.y),(points[i].x-p0.x));
        } else {
			t= atan2((points[i].y-p0.y),(points[i].x-p0.x));
        }
		points[i].theta = t;
	}
}


int FindNewHullIndex(vector<Point> points, Point hull, int index) {
	Point p0 = hull;
	double minAngle = INT_MAX;
	int minIndex;
	for (int i = index; i < points.size(); ++i) {
		if (points[i] != p0) {
			double t;
            if (p0.y > points[i].y) {
				t= 2* 3.14159265 + atan2((points[i].y-p0.y),(points[i].x-p0.x));
            } else {
				t= atan2((points[i].y-p0.y),(points[i].x-p0.x));
            }
            
			if (t <= minAngle) {
				minAngle = t;
				minIndex = i;
			}
		}
	}
	return minIndex;
}

vector<Point> JarvisMarch(vector<Point> points) {
	vector<Point> hull;
	hull.push_back(points[0]);
	int hPointIndex = 0;
	int i = 0;
	while (i < points.size()) {
		int newHullIndex = FindNewHullIndex(points, hull[hPointIndex], i);
		Point newHull = points[newHullIndex];
		hull.push_back(newHull);
		hPointIndex++;
		i = newHullIndex+1;
	}
	return hull;
}

int _tmain(int argc, _TCHAR* argv[]) {
	
	bool decisions= false;
	do {
		vector<Point> points;
		string input;
		cout<<"Use File?(f/F) ";
		cin>>input;
		if (input == "f" || input == "F") {
			ifstream in;
			in.open("data1.txt");
			if (in.fail()) {
				cerr << "Could not open the input file." << endl;
				return 1;
			}
			while (in.good() && in.eof() != true) {
				double x, y;
				in >> x;
				in >> y;
				points.push_back(Point(x, y));
			}
		} else {
			/*----------------Generate Random Points----------------*/
			cout<<"Enter number of random points to generate: ";
			int N;
			cin>>N;												//Number of points to generate
			int a = 150000000;
			int high = 1500000;
			int low = -1500000;
			for (int i = 0; i < N; ++i) {
				int x = a*rand()/RAND_MAX-9872;
				int y = a*rand()/RAND_MAX-7861;
				Point p = Point(x, y);
				points.push_back(p);
			}
		}

        //find the index of the lowest pivot
        QuickSort(points, 0, points.size()-1, "sort y");
        int index = FindP0(points);
        //bring pivot to the front
        swap(points[0], points[index]);
        //calculate theta & sort according to theta
        CalculateTheta(points);
        QuickSort(points, 1, points.size()-1, "sort theta");

        vector<Point> convexHull = JarvisMarch(points);

        cout<<endl<<"Convex Hull:"<<endl;
        Print(convexHull);

		cout <<"Using Jarvis March it took " << g_TickCount << " miliseconds to find "<<convexHull.size()<<" points."<<endl<<endl;
		

		cout<<endl<<endl<<"Continue?(Y/N) ?";
		string decision; 
		cin >> decision;
        if (decision == "y" || decision == "Y") {
			decisions = true;
        } else {
			decisions = false;
        }
	} while(decisions == true);
	return 0;
}