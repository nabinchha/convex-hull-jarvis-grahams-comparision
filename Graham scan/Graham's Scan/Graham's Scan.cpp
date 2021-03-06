// Graham's Scan.cpp : Defines the entry point for the console application.
// Author: Nabin Mulepati 2010

#include "stdafx.h"
#include "Point.h"
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <limits.h>
#include <stack>
#include <shlwapi.h>

using namespace std;

int g_TickCount;
#define TIC g_TickCount = GetTickCount();                 
#define TOC g_TickCount = GetTickCount() - g_TickCount + 1;

vector<Point> Refine(vector<Point> p) {
	vector<Point> refined(p.size()-1);
	for (int i = 0; i < p.size() - 1; ++i) {
		int j = i;
		double max = 0;
		int maxi;
		if (p[j].theta == p[j+1].theta) {
			while(j < p.size()-1 && p[j].theta == p[j+1].theta) {
				if(p[j].r > p[j+1].r) {
					maxi = j;
					max = p[j].r;
				} else {
					max = p[j+1].r;
					maxi = j+1;
				}
				j++;
			}
			refined[i] = p[maxi];
		} else {
			refined[i] = p[i];
		}
	}
	return refined;
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

int Partition(vector<Point>& v, int a, int b, int t) {
	if(t == 0) {
		int pivot = b;
		int j = a;
		int i = a;
		for(i = a; i<b; ++i) {
			if (v[i].y < v[pivot].y) { //ascending
				swap(v[i], v[j]);
				j++;
			} else if(v[i].y == v[pivot].y && v[i].x < v[pivot].x) {
				swap(v[i], v[j]);
				j++;
			}

		}
		swap(v[i], v[j]);
		return j;
	} else {
		int pivot = b;
		int j = a;
		int i = a;
		for(i = a; i < b; ++i) {
			if (v[i].theta < v[pivot].theta) {//ascending
				swap(v[i], v[j]);
				j++;
			}
		}
		swap(v[i], v[j]);
		return j;
	}
}

void QuickSort(vector<Point>& v, int i, int j, int t) {
	if (i >= j) {
		return;
	}
	int k = Partition(v, i, j, t);
	QuickSort(v, i, k-1, t);
	QuickSort(v, k+1, j, t);
}

void Print(vector<Point> p) {
	cout<<endl<<endl;
	for (int i = 0; i < p.size(); i++) {
		cout << p[i].x << "          " << p[i].y   << "          " <<endl;
	}
}

Point Top(stack<Point> s) {
	return s.top();
}

Point NextToTop(stack<Point> s) {
	Point z = s.top();
	s.pop();
	Point w = s.top();
	s.push(z);
	return w;
}

void CalculateTheta(vector<Point> &points, Point p0) {
	for (int i = 0; i < points.size(); ++i) {
		if (points[i] != p0) {
			double t;
			double slope = ((points[i].y-p0.y)/(points[i].x-p0.x));
			if (slope > 0) {
				if (points[i].y > p0.y) {
					t = atan(slope);
				} else {
					t = abs(atan(slope)) + 3.1415;
				}
			} else if(slope < 0) {
				t = 3.1415 - abs(atan(slope));
			} else {
				t = 0;
			}
			double radius = sqrt( (points[i].x - p0.x)*(points[i].x - p0.x) + (points[i].y - p0.y)*(points[i].y - p0.y) );
			points[i].r = radius;
			points[i].theta = t;
		} else {
			points.erase(points.begin()+i);
			i--;
		}
	}
}

void GrahamScan(Point pivot, vector<Point> points, stack<Point> &s) {
	s.push(pivot);
	s.push(points[0]);
	int i = 1;
	double pseudoAngle;

	while ( i < points.size()) {
		Point pI = points[i];
		Point pT = Top(s);
		Point pNT = NextToTop(s);
		pseudoAngle = (pT.x-pNT.x)*(pI.y-pNT.y) - (pT.y-pNT.y)*(pI.x-pNT.x); //if angle = 0 collinear; if >0 on; if >0 on right 
		if (pseudoAngle > 0) {
			s.push(points[i]);
			i++;
		} else {
			s.pop();
		}
	}
}

int _tmain(int argc, _TCHAR* argv[]) {
	bool decisions= false;
	do {
		vector<Point> points;
		string input;
		cout<<"Use File?(f/F) ";
		cin>>input;
		if (input =="f" || input =="F") {
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
				//float x = (rand() / (static_cast<double>(RAND_MAX) + 1.0))* (high - low) + low;
				//float y = (rand() / (static_cast<double>(RAND_MAX) + 1.0))* (high - low) + low;
				int x = a*rand()/RAND_MAX - 500;
				int y = a*rand()/RAND_MAX - 500;
				Point p = Point(x, y);
				points.push_back(p);
			}
		}
	
		TIC;

		//find p0
		QuickSort(points, 0, points.size() - 1, 0);
		int index = FindP0(points);
		Point p0 = Point(points[index].x, points[index].y);
		
		CalculateTheta(points, p0);				    //calculate theta for each point with respect to p0
		QuickSort(points, 0, points.size() - 1, 1); //sort the rest according to the polar angle with respect to p0
		
		stack<Point> hull;
		GrahamScan(p0, points, hull);
		TOC;
		cout<<endl<<"Convex Hull:"<<endl;
		while (!hull.empty()) {
			Point p = hull.top();
			cout<<p.x<<"     "<<p.y<<endl;
			hull.pop();
		}
		cout << "Using Graham's Scan Took " << g_TickCount << " miliseconds"<<endl;

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

