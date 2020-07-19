/************************************************* 
Copyright:
Author: Yu Liu, Xiaodong He, Zhewei Wei
Date: 2017-12-25 
Libraries Required: 
	SFMT-1.4.1: A random number generator, which can be downloaded here:
	www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/SFMT
**************************************************/

#include "SimStruct.h"
#include "util.h"
#include "json.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

using json = nlohmann::json;

void usage() {
	cerr << "./ssrank [data_dir] [query_file] [-o <output dir, default: 'data_dir_name_output'>] [ -e<eps, default: 0.005>] [ -k<topk, default: 200>]" << endl;
	cerr << "data_dir format :\ndata_dir/\n\tgraph.edgelist\n\tid2node.json\n\tnode2id.json\n";
	exit(EXIT_SUCCESS);
}


int main(int argc, char** argv){
	string file = "";
	string qf = "";
	vector<string> queryNode;
	double eps = 0.005;
	int topk = -1;

	if (argc < 3) 
		usage();

	string data_dir(argv[1]), queryfile(argv[2]);
	if (data_dir.back() == '/')
		data_dir.pop_back();
	string output_dir(data_dir + "_output");

	for (int i = 3; i < argc; i++) {
		if (!strncmp(argv[i], "-e", 2))
			eps = strtod(argv[++i], NULL);
		else if (!strncmp(argv[i], "-o", 2))
			output_dir = string(argv[++i]);
		else if (!strncmp(argv[i], "-k", 2))
			topk = strtol(argv[++i], NULL, 10);
		else
			usage();
	}

	queryNode = readfile(queryfile);
	std::ifstream node2idStream;
	std::ifstream id2nodeStream;
	struct stat st = {0}; 
	errno = 0;
	node2idStream.open(data_dir + "/node2id.json", ios_base::in);
	id2nodeStream.open(data_dir + "/id2node.json", ios_base::in);
	if (errno != 0) {
		cerr << "Failed to open mapping files. " + string(strerror(errno)) << endl;
		return 0;
	}
	if (stat(output_dir.c_str(), &st) == -1)
    	mkdir(output_dir.c_str(), 0700);

	json node2id; 
	json id2node; 
	if (!((node2idStream >> node2id) && (id2nodeStream >> id2node))) {
		std::cout << "json read error!\n";
		return 0;
	}
	node2idStream.close();
	id2nodeStream.close();
	
	queryNode.pop_back();

	if (queryNode.empty()) {
		std::cout << "Query file is empty.\n";
		return 0;
	}
	SimStruct sim = SimStruct(data_dir + "/graph.edgelist", eps); 

	for(int i = 0; i < queryNode.size(); i++){
		if (queryNode[i] == "\n") break;	
		try {
			id2node[queryNode[i]];
		} catch (...) {
			// std::cout << e.what() << std::endl;
			cout << "Mapping error, node " << queryNode[i] << endl;
			cout << "Skipping this node..\n";
			continue;
		}
		cout << "node: " << queryNode[i] << " " << id2node[queryNode[i]] << "\t";

		auto sims = sim.batch(id2node[queryNode[i]]);

		stringstream ss_out;
		ss_out << output_dir << "/" << "result" << i;

		ofstream of_res(ss_out.str());
		std::cout << ss_out.str() << " " << sims.size() << std::endl;

		if (topk <= 0)
			topk = min(size_t(200), sims.size());
		for(int j = 0; j < topk; j++) {
			try {
				node2id[to_string(sims[j].first)].get<std::string>();
			} catch (exception& e) {
				// cerr << "node2id failed! Strange error.\n";
				cerr << e.what() << endl;
				continue;
			}
			of_res << queryNode[i] << "," << node2id[to_string(sims[j].first)].get<std::string>() << "," << sims[j].second << '\n';
		}
		of_res.flush(); 
		of_res.close();
	}

	return 0;
}
