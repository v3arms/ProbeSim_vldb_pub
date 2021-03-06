#ifndef _UTIL_H_
#define _UTIL_H_

#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <unordered_map>
#include "Graph.h"

vector<string> readfile(string file)
{
	vector<string> vec;
	errno = 0;
	ifstream infile(file);
	if (errno != 0) {
		cerr << "Cannot open query file.\n";
		exit(EXIT_FAILURE);
	}
	while(infile.good())
	{
		std::string id;
		std::getline(infile, id);
		if (id == "\n") break;
		vec.push_back(id);
		std::cout << id << std::endl;
	}
	infile.close();
	return vec;
}

int readfile(string file, vector<int> &vec, int k)
{
	errno = 0;
	ifstream infile(file);
	if (errno != 0) {
		cerr << strerror(errno);
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	while(infile.good() && cnt < k)
	{
		int id;
		double score;
		infile >> id >> score;
		if(find(vec.begin(), vec.end(), id) == vec.end())
		{
			vec.push_back(id);
		}
		cnt ++;
	}
	infile.close();
	return cnt;
}

int readfile2(string file, vector<int> &vec, int k)
{
	errno = 0;
	ifstream infile(file);
	if (errno != 0) {
		cerr << strerror(errno);
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	double thres = 0;
	while(infile.good())
	{
		int id;
		double score;
		infile >> id >> score;

		if(cnt >= k && thres > score)
			break;

		if(find(vec.begin(), vec.end(), id) == vec.end())
			vec.push_back(id);
		cnt ++;
		if(cnt == k)
			thres = score;
	}
	infile.close();
	return cnt;
}

int readfile(string file, vector<pair<int, double> > &vec)
{
	errno = 0;
	ifstream infile(file);
	if (errno != 0) {
		cerr << strerror(errno);
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	while(infile.good())
	{
		int id;
		double score;
		infile >> id >> score;

		if(score <= 0)
			break;

		if(find(vec.begin(), vec.end(), pair<int,double>(id, score)) == vec.end())
		{
			vec.push_back(pair<int,double>(id, score));
		}
		cnt ++;
	}
	infile.close();
	return cnt;
}

int readfile(string file, unordered_map<int, double> &map)
{
	errno = 0;
	ifstream infile(file);
	if (errno != 0) {
		cerr << strerror(errno);
		exit(EXIT_FAILURE);
	}
	int cnt = 0;
	while(infile.good())
	{
		int id;
		double score;
		infile >> id >> score;

		if(map.find(id) == map.end())
		{
			map[id] = score;
		}
		cnt ++;
	}
	infile.close();
	return cnt;
}

#endif