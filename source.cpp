#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <cstring>
#include <ctime>
#include <vector>
#include<chrono>
#include "bignum.hpp"
using namespace std;

typedef pair<int, int> Edge;

struct Analiz
{
	size_t vertex_count;
	size_t min;
	size_t max;
	vector<double> count_op;
	double avr;
	double sigma;
	double alpha;
	double beta;
};
const size_t min_n = 3;

size_t GetCut(vector<Edge> edges, size_t vertex_count)
{
	auto vertex = new size_t[vertex_count];
	for (size_t i = 0; i < vertex_count; ++i)
		vertex[i] = i;
	
	for (size_t i = 2; i < vertex_count; ++i)
	{
		size_t j = rand() % edges.size();
		auto edge = edges[j];
		while (vertex[edge.first] == vertex[edge.second])
		{
			edges.erase(edges.begin() + j);
			j = rand() % edges.size();
			edge = edges[j];
		}
		size_t min = _Min_value(vertex[edge.first], vertex[edge.second]);
		size_t max = _Max_value(vertex[edge.first], vertex[edge.second]);
		for (size_t k = max; k < vertex_count; ++k)
			if (vertex[k] == max)
				vertex[k] = min;
		edges.erase(edges.begin() + j);
	}

	size_t count = 0;
	for (Edge edge : edges)
		if (vertex[edge.first] != vertex[edge.second])
			++count;
	delete[] vertex;
	return count;
}

size_t Karger(const vector<Edge>& edges, size_t vertex_count)
{
	size_t min_cut = edges.size();
	size_t count = vertex_count * vertex_count * size_t(log(vertex_count));
	for (size_t i = 0; i < count; ++i)
	{
		size_t curCut = GetCut(edges, vertex_count);
		if (curCut < min_cut)
			min_cut = curCut;
	}
	return min_cut;
}

unsigned long long KargerStarter(ifstream& input, size_t vertex_count)
{
	size_t edge_count = 0;
	input >> edge_count;
	vector<Edge> edges(edge_count);
	for (size_t i = 0; i < edge_count; ++i)
		input >> edges[i].first >> edges[i].second;

	auto start = chrono::high_resolution_clock::now();
	Karger(edges, vertex_count);
	auto finish = chrono::high_resolution_clock::now();
	return chrono::duration_cast<chrono::microseconds>(finish - start).count();
}

Analiz KargerAnalizator(size_t vertex_count)
{
	string name = "00.txt";
	name[0] += vertex_count / 10;
	name[1] += vertex_count % 10;
	ifstream input(name);
	size_t count;
	input >> count;
	Analiz res;
	res.vertex_count = vertex_count;
	res.count_op = vector<double>(count);
	for (size_t i = 0; i < count; i++)
		res.count_op[i] = double(KargerStarter(input, vertex_count));
	input.close();

	
	res.min = res.count_op[0];
	res.max = res.count_op[0];
	res.avr = 0;
	for (double val : res.count_op)
	{
		res.min = _Min_value(res.min, size_t(val));
		res.max = _Max_value(res.max, size_t(val));
		res.avr += val;
	}
	res.avr /= res.count_op.size();

	res.sigma = 0;
	for (double val : res.count_op)
		res.sigma += (val - res.avr) * (val - res.avr);

	res.sigma /= (res.count_op.size() - 1);

	//res.alpha = res.avr * (res.avr - res.avr * res.avr - res.sigma) / res.sigma;
	//res.beta = (1 - res.avr) * (res.avr - res.avr * res.avr - res.sigma) / res.sigma;
	
	return res;
}

void DataGenerator_by_vertex(size_t test_count, size_t vertex_count)
{
	string name = "00.txt";
	name[0] += vertex_count / 10;
	name[1] += vertex_count % 10;
	ofstream output(name);

	size_t step = test_count / (vertex_count * vertex_count / 2 - vertex_count + 1);
	test_count = step * (vertex_count * vertex_count / 2 - vertex_count + 1);
	output << test_count << endl;
	for (size_t k = vertex_count - 1; k <= vertex_count * vertex_count / 2; ++k)
	{
		for (size_t i = 0; i < step; ++i)
		{
			
			vector<Edge> edges(k);
			for (size_t vertex_used = 0; vertex_used < vertex_count - 1; ++vertex_used)
			{
				size_t x = rand() % (vertex_used + 1);
				size_t y = vertex_used + 1;
				edges[vertex_used].first = _Min_value(x, y);
				edges[vertex_used].second = _Max_value(x, y);
			}
			for (size_t j = vertex_count - 1; j < k; ++j)
			{
				size_t x = rand() % vertex_count;
				size_t y = rand() % vertex_count;
				edges[j].first = _Min_value(x, y);
				edges[j].second = _Max_value(x, y);
			}
			output << k;
			for (Edge edge : edges)
				output << " " <<edge.first << " "<< edge.second;
			output << endl;
		}
	}

}

void DataGenerator(size_t test_count, size_t vertex__min, size_t vertex_count)
{
	for (size_t i = 0; i < vertex_count; ++i)
		DataGenerator_by_vertex(test_count, vertex__min + i);
}

int main()
{
	srand((unsigned int)time(0));
	//DataGenerator(1000, min_n, 30);
	vector<Analiz> results{ 30 };
	for (size_t i = 0; i < 30; ++i)
	{
		results[i] = KargerAnalizator(i + min_n);
		cout << results[i].avr << " ";
	}
	system("pause");
	return 0;
}