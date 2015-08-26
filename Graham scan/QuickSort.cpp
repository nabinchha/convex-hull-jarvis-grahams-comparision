// QuickSort.cpp : Defines the entry point for the console application.
// Author: Nabin Mulepati 2010

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int partition(vector<int>& v, int a, int b) {
	int pivot = b;
	int j = a;
	int i = a;
	for(i = a; i<b; ++i) {
		//if(v[i] < v[pivot])//ascending
		if(v[i] > v[pivot]) {//descending
			swap(v[i], v[j]);
			j++;
		}
	}
	swap(v[i], v[j]);
	return j;
}

void quickSort(vector<int>& v, int i, int j) {
	if(i >= j) {
		return;
	}
	int k = partition(v, i, j);
	quickSort(v, i, k-1);
	quickSort(v, k+1, j);
}

int _tmain(int argc, _TCHAR* argv[]) {
	ifstream in;
	string filename;
	cout << "Input the path to an input file: ";
	cin >> filename;
	in.open(filename.c_str());

	if(in.fail()) {
		cerr << "Failed to open " << filename << endl;
		return 1;
	}

	vector<int> v;
	int p;
	in >> p;
	v.push_back(p);
	in >> p;
	v.push_back(p);

	while(1) {
		int a;
		in >> a;
		if(in.good() && !in.eof()) {
			v.push_back(a);
		} else {
			break;
		}
	}

	int i;
	cout << "Pre-sort:" << endl;
	for(i=0; i<v.size(); ++i) {
		cout << v[i] << " ";
	}

	cout << endl;

	quickSort(v, 0, v.size()-1);

	cout << "Post-sort:" << endl;
	for(i=0; i < v.size(); ++i) {
		cout << v[i] << " ";
	}
	cout << endl;

	return 0;
}