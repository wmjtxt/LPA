 ///
 /// @file    :lpa.cpp
 /// @author  :wmjtxt(972213032@qq.com)
 /// @date    :2018-11-11 20:08:51(NewYork time)
 /// @quote   :
 ///
 
#include "lpa.h"

using namespace std;


LPA::LPA(string filename){
	g = Graph(filename);
	size = g.nb_nodes;
	commnum = size;
	in.resize(size+1);
	tot.resize(size+1);
	comms.resize(size);
	for(int i = 0; i < size; i++)
		comms[i] = i;
}

void LPA::labeled(){
	lastComms.resize(comms.size());
	for(int i = 0; i < commnum; i++){
		lastComms[i] = comms[i];
	}
	lastLabel.resize(g.label.size());
	for(int i = 0; i < g.label.size(); i++){
		lastLabel[i] = g.label[i];
	}
	lastQ = modularity();
	for(unsigned int i = 0; i < size; i++){
		vector<int> cnt(size,0);
		vector<int> maxv;
		int maxnum = 0, maxcnt = 0;
		for(unsigned int j = 0; j < g.links[i].size(); j++){
			cnt[g.label[g.links[i][j]]]++;
			if(maxnum < cnt[g.label[g.links[i][j]]])
				maxnum = cnt[g.label[g.links[i][j]]];
		}
		//记录最多的标签
		for(unsigned int j = 0; j < cnt.size(); j++){
			if(cnt[j] == maxnum){
				maxv.push_back(j);
				maxcnt++;
			}
		}
		//如果最多标签仅1个，直接赋给当前节点，否则随机选择一个
		if(maxcnt == 1){
			g.label[i] = maxv[0];
		}
		else{
			int randnum = rand()%maxcnt; 
			g.label[i] = maxv[randnum];
		}
	}
	set<int> difflabel;
	for(int i = 0; i < size; i++){
		difflabel.insert(g.label[i]);
	}
	commnum = difflabel.size();
	comms.resize(commnum);
	int cnt = 0;
	for(auto & item : difflabel)
		comms[cnt++] = item;
}

void LPA::display(){
	cout << "***************************************************" << endl;
	cout << "Modularity = " << modularity() << endl;
	cout << "社团个数 : " << commnum << endl;
	cout << "边    数 : " << g.nb_links << endl;
	cout << "节 点 数 : " << g.nb_nodes << endl;
	cout << "\n***************************************************" << endl;
}

void LPA::displayResult(){
	cout << "*********************最终结果***********************" << endl;
	cout << "Modularity = " << lastQ << endl;
	cout << "社团个数 : " << lastComms.size() << endl;
	cout << "边    数 : " << g.nb_links << endl;
	cout << "节 点 数 : " << g.nb_nodes << endl;
	cout << "\n***************************************************" << endl;
}

void LPA::displayLabels(){
	cout << "社团个数 : " << commnum << " = { ";
	for(int i = 0; i < commnum; i++){
		if(i != 0) cout << ",";
		cout << "[" << comms[i] << "]";
	}
	cout << " }\n";
	for(int i = 0; i < size; i++){
		if(i != 0) cout << ",";
		cout << "[" << g.label[i] << "]";
	}
}

double LPA::modularity(){
	double q = 0;
	double m2 = (double)(2*g.nb_links);
	for(int i = 0; i < commnum; i++){
		//in[i]是社团i内部的边数 
		//cout << "in" << endl;
		in[i] = g.nb_selfloops(comms[i]);
		//tot[i]是社团i中所有节点的度值之和
		//cout << "tot" << endl;
		tot[i] = g.weighted_degree(comms[i]);
		if(tot[i] > 0)
			q += (double)in[i]/m2 - ((double)tot[i]/m2)*((double)tot[i]/m2);
	}
	return q;
}
