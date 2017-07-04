#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <cmath>
#include <fstream>

using namespace std;
ofstream fout("1.out");

vector<string> str;
map<string, int> label;
map<string, int> labeltodata;
map<string, int> RAMmap;
int linenum = 0;
struct datatype{
	int size;
	int dvalue;
	int dtype;
	char dstring;
};

datatype dota[10000000];
long long RAM[32] = {0};
long long lo, hi;
int datanum = 0;

class add {
	private:
		int s1,s2,s3,type;
		int ans;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			string t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find("$",position) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t +s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
		}
}

class addu {
	private:
		int s1,s2,s3,type,ans;
	public:
		void Data_Preparation(string s) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1; //RAM 
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				type = 2; //Im
			}
		}
};


void RAMready() {
	RAMmap["zero"] = 0;	RAMmap["at"] = 1;RAMmap["v0"] = 2;RAMmap["v1"] = 3;RAMmap["a0"] = 4;
	RAMmap["a1"] = 5;RAMmap["a2"] = 6;RAMmap["a3"] = 7;RAMmap["t0"] = 8;RAMmap["t1"] = 9;
	RAMmap["t2"] = 10;RAMmap["t3"] = 11;RAMmap["t4"] = 12;RAMmap["t5"] = 13;RAMmap["t6"] = 14;
	RAMmap["t7"] = 15;RAMmap["s0"] = 16;RAMmap["s1"] = 17;RAMmap["s2"] = 18;RAMmap["s3"] = 19;
	RAMmap["s4"] = 20;RAMmap["s5"] = 21;RAMmap["s6"] = 22;RAMmap["s7"] = 23;RAMmap["t8"] = 24;
	RAMmap["t9"] = 25;RAMmap["k0"] = 26;RAMmap["k1"] = 27;RAMmap["gp"] = 28;RAMmap["sp"] = 29;
	RAMmap["fp"] = 30;RAMmap["s8"] = 30;RAMmap["ra"] = 31;
}

int init() {
	FILE * fp1 = fopen("3.s", "r");
	char c[100];
	linenum = 0;
	while (fscanf(fp1 , "%[^\n]s" , &c) != EOF) {
		linenum++;
		if (linenum == 1) {
			string ss = c;
			while (ss[0] == '\t' || ss[0] == ' ') 
				ss.erase(ss.begin());
			str.push_back(ss);
		}
		else str.push_back(c);
		//cout <<c <<"	";
		if (c[strlen(c) - 1] == ':') label[c] = linenum - 1;
		fscanf(fp1 ,"\n");
	}
	fclose(fp1);
	return label["main:"];
}
void dealdata() {
	for (int i = 0 ; i < linenum; i++)
		if (str[i] == ".data") {
			while (str[i] != ".text") {
				if (str[i].find(".word",0) != string::npos) {
					int position = str[i].find(".word",0) + 6;
					while (position < str[i].size()) {
						string t = "";
						while (str[i][position] != ',' && position < str[i].size()) {
							t = t + str[i][position];
							position++;
						}
						//cout <<1 <<endl;
						int s =  atoi(t.c_str());
						position++;
	
						dota[datanum].dtype = 1;
						dota[datanum].dvalue = s;
						dota[datanum].size = 4;
						datanum += 4;
					}
				}
				
				if (str[i].find(".half",0) != string::npos) {
					int position = str[i].find(".half",0) + 6;
					while (position < str[i].size()) {
						string t = "";
						while (str[i][position] != ',') {
							t = t + str[i][position];
							position++;
						}
						//cout <<2 <<endl;
						int s = atoi(t.c_str());
						position++;
	
						dota[datanum].dtype = 2;
						dota[datanum].dvalue = s;
						dota[datanum].size = 2;
						datanum += 2;
					}
				}
				
				if (str[i].find(".byte",0) != string::npos) {
					int position = str[i].find(".byte",0) + 6;
					while (position < str[i].size()) {
						string t = "";
						while (str[i][position] != ',') {
							t = t + str[i][position];
							position++;
						}
						//cout <<3 <<endl;
						int s =  atoi(t.c_str());
						position++;
	
						dota[datanum].dtype = 3;
						dota[datanum].dvalue = s;
						dota[datanum].size = 1;
						datanum += 1;
					}
				}
				
				if (str[i].find(".asciiz",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t = "";
					while (position < str[i].size() - 1) {
						t = t + str[i][position];
						position++;
					}
					
					//cout <<5 <<endl;
					string ss = "";
					for (int j = 0;j < t.size();++j)
					{
						if (t[j] == '\\') 
						{
							if (t[j+1] == '\\') ss = ss + '\\';
							if (t[j+1] == '\'') ss = ss + '\'';
							if (t[j+1] == '\"') ss = ss + '\"';
							if (t[j+1] == 'n') ss = ss + '\n';
							if (t[j+1] == 't') ss = ss + '\t';
							if (t[j+1] == '0') ss = ss + '\0';
							if (t[j+1] == 'r') ss = ss + '\r';
							++j;
						} else ss = ss + t[j];
					}
					t = ss;
					
					for (int j = 0; j < t.size(); ++j) {
						dota[datanum].dtype = 5;
						dota[datanum].dstring = t[j];
						dota[datanum].size = 1;
						datanum++;
					}
					
					dota[datanum].dtype = 7;
					dota[datanum].dstring = '\0';
					dota[datanum].size = 1;
					datanum++;
					
				}
				else if (str[i].find(".ascii",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t = "";
					while (position < str[i].size() - 1) {
						t = t + str[i][position];
						position++;
					}
					//cout <<4 <<endl;
					string ss;
					for (int j = 0;j < t.size();++j)
					{
						if (t[j] == '\\') 
						{
							if (t[j+1] == '\\') ss = ss + '\\';
							if (t[j+1] == '\'') ss = ss + '\'';
							if (t[j+1] == '\"') ss = ss + '\"';
							if (t[j+1] == 'n') ss = ss + '\n';
							if (t[j+1] == 't') ss = ss + '\t';
							if (t[j+1] == '0') ss = ss + '\0';
							if (t[j+1] == 'r') ss = ss + '\r';
							++j;
						} else ss = ss + t[j];
					}
					t = ss;
					
					for (int j = 0; j < t.size(); ++j) {
						dota[datanum].dtype = 4;
						dota[datanum].dstring = t[j];
						dota[datanum].size = 1;
						datanum++;
					}
					
				}
				
				if (str[i].find(".align",0) != string::npos) {
					int position = str[i].find(".align",0) + 7;
					string t = "";
					while (position < str[i].size()) {
						t = t + str[i][position];
						position++;
					}
					//cout <<6 <<endl;
					int s = atoi(t.c_str());
					s = pow(2, s);
					int tmp = s;
					while (tmp < datanum) tmp += s;
					datanum = tmp;
				}
				
				if (str[i].find(".space",0) != string::npos) {
					int position = str[i].find(".space",0) + 7;
					string t;
					while (position < str[i].size()) {
						t = t + str[i][position];
						position++;
					}
					//cout <<7 <<endl;
					int s = atoi(t.c_str());

					datanum += s;
				}
				
				if (str[i][str[i].size() - 1] == ':') {
					labeltodata[str[i]] = datanum;
				}
				
				i++;
			}
		} 
}
