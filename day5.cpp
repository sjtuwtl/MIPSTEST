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

void RAMready() {
	RAMmap["zero"] = 0;
	RAMmap["at"] = 1;
	RAMmap["v0"] = 2;
	RAMmap["v1"] = 3;
	RAMmap["a0"] = 4;
	RAMmap["a1"] = 5;
	RAMmap["a2"] = 6;
	RAMmap["a3"] = 7;
	RAMmap["t0"] = 8;
	RAMmap["t1"] = 9;
	RAMmap["t2"] = 10;
	RAMmap["t3"] = 11;
	RAMmap["t4"] = 12;
	RAMmap["t5"] = 13;
	RAMmap["t6"] = 14;
	RAMmap["t7"] = 15;
	RAMmap["s0"] = 16;
	RAMmap["s1"] = 17;
	RAMmap["s2"] = 18;
	RAMmap["s3"] = 19;
	RAMmap["s4"] = 20;
	RAMmap["s5"] = 21;
	RAMmap["s6"] = 22;
	RAMmap["s7"] = 23;
	RAMmap["t8"] = 24;
	RAMmap["t9"] = 25;
	RAMmap["k0"] = 26;
	RAMmap["k1"] = 27;
	RAMmap["gp"] = 28;
	RAMmap["sp"] = 29;
	RAMmap["fp"] = 30;
	RAMmap["s8"] = 30;
	RAMmap["ra"] = 31;
}
int init() {
	FILE * fp1 = fopen("1.s", "r");
	
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
void beginset() {
	RAM[29] = 10000000 - 1;
	lo = 0;
	hi = 0;
	for (int i = 0; i < 10000000; i++) dota[i].dtype = -1;
}
int mainprocedure(int l1) {
	FILE * fp2 = fopen("1.in", "r");
	//cout <<str.size()<<endl; 
	while (l1 < str.size()) {
		l1++;
		fout <<l1 <<' '<<str[l1]<<endl;
		for (int j = 0; j <= 33; j++)
			fout <<j <<'\t';
		fout <<endl; 
		for (int j = 0; j <= 31; j++)
			fout <<RAM[j] <<'\t';
		fout <<lo <<'\t' << hi <<'\t';
		fout <<endl<<endl <<endl;
		if (l1 >= str.size()) break;
		if (str[l1].find("li", 0) != string::npos && str[l1][0] == 'l' && str[l1][1] == 'i') {
			int position = str[l1].find("$", 0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s;
			if (t[0] >= '0' && t[0] <= '9') 
				s = atoi(t.c_str());
			else
				s = RAMmap[t];
			position = str[l1].find(",", 0) + 1;
			t = "";
			while (position < str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			//cout <<s <<' '<<atoi(t.c_str()) <<endl;
			RAM[s] = atoi(t.c_str());
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
			
		if (str[l1].find("jalr", 0) != string::npos && str[l1].find("jalr", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ' ' && position <str[l1].size()) 
				{t = t + str[l1][position]; position++;}
			int s;
			if (t[0] >= '0' && t[0] <= '9') 
				s = atoi(t.c_str());
			else
				s = RAMmap[t];
			RAM[31] = l1 + 1;
			l1 = RAM[s] - 1;
			continue;
		}
		
		if (str[l1].find("jal", 0) != string::npos && str[l1].find("jal", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			for (int position = str[l1].find("jal", 0) + 4;position < str[l1].size();position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			RAM[31] = l1 + 1;
			t = t + ':';
		//	cout <<t <<' '<<label[t]<<endl;
			l1 = label[t];
			continue;
		}
		
		if (str[l1].find("jr", 0) != string::npos && str[l1].find("jr", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			//cout <<RAM[31]<<endl; 
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ' ' && position <str[l1].size()) 
				{t = t + str[l1][position]; position++;}
			int s;
			if (t[0] >= '0' && t[0] <= '9') 
				s = atoi(t.c_str());
			else
				s = RAMmap[t];
			l1 = RAM[s] - 1;
			
			continue;
		}
		
		if (str[l1].find("bltz", 0) != string::npos && str[l1].find("bltz", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] < 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("blt", 0) != string::npos && str[l1].find("blt", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] < RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] < s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		
		if (str[l1].find("bgtz", 0) != string::npos && str[l1].find("bgtz", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] > 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("bgt", 0) != string::npos && str[l1].find("bgt", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] > RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] > s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		
		if (str[l1].find("blez", 0) != string::npos && str[l1].find("blez", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] <= 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("ble", 0) != string::npos && str[l1].find("ble", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] <= RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] <= s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		
		if (str[l1].find("bnez", 0) != string::npos && str[l1].find("bnez", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] != 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("bne", 0) != string::npos && str[l1].find("bne", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] != RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] != s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		
		if (str[l1].find("beqz", 0) != string::npos && str[l1].find("beqz", 0) == 0) {
			//cout <<l1 <<' ' <<str[l1] <<' ' << 1<<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] == 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("beq", 0) != string::npos && str[l1].find("beq", 0) == 0) {
			//cout <<l1 <<' ' <<str[l1] <<' ' << 2<<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] == RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] == s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		
		if (str[l1].find("bgez", 0) != string::npos && str[l1].find("bgez", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (RAM[s1] >= 0) {
				t = "";
				for (position = str[l1].find(",", 0) + 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				l1 = label[t];
			}
			continue;
		}
		if (str[l1].find("bge", 0) != string::npos && str[l1].find("bge", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			int position;
			for (position = str[l1].find("$", 0) + 1;str[l1][position] != ',';position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find("$", position) != string::npos) {
				t = "";
				for (position = str[l1].find("$",position) + 1;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] >= RAM[s2]) l1 = label[t];
				
			}
			else {
				t = "";
				for (position += 2;str[l1][position] != ',';position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < str[l1].size() ;position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				t = t + ':';
				if (RAM[s1] >= s2) l1 = label[t];
			}
			//cout <<t <<' '<<label[t]<<endl;
			continue;
		}
		if (str[l1][0] == 'b' || str[l1][0] == 'j') {
			//fout <<l1 <<' '<<str[l1] <<endl;
			string t = "";
			for (int position = 2;position < str[l1].size();position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			t = t + ':';
			//cout <<t <<' '<<label[t]<<endl;
			l1 = label[t];
			continue;
		} 
		
		if (str[l1].find("sb", 0) != string::npos && str[l1].find("sb", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				dota[RAM[s3] + s2].dstring = char(RAM[s1]);
				dota[RAM[s3] + s2].dtype = 4;
				dota[RAM[s3] + s2].size = 1;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				dota[s2].dstring = char(RAM[s1]);
				dota[s2].dtype = 4;
				dota[s2].size = 1;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sh", 0) != string::npos && str[l1].find("sh", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				dota[RAM[s3] + s2].dvalue = RAM[s1];
				dota[RAM[s3] + s2].dtype = 2;
				dota[RAM[s3] + s2].size = 2;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				dota[s2].dvalue = RAM[s1];
				dota[s2].dtype = 2;
				dota[s2].size = 2;
			}
			////fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sw", 0) != string::npos && str[l1].find("sw", 0) == 0) {
			//fout <<l1 <<' '<<str[l1] <<endl;
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				dota[RAM[s3] + s2].dvalue = RAM[s1];
				dota[RAM[s3] + s2].dtype = 1;
				dota[RAM[s3] + s2].size = 4;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				dota[s2].dvalue = RAM[s1];
				dota[s2].dtype = 1;
				dota[s2].size = 4;
			}
			////fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("la", 0) != string::npos && str[l1].find("la", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = RAM[s3] + s2;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				RAM[s1] = s2;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("lb", 0) != string::npos && str[l1].find("lb", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (int)(dota[RAM[s3] + s2].dstring);
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				RAM[s1] = (int)(dota[s2].dstring);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("lh", 0) != string::npos && str[l1].find("lh", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = dota[RAM[s3] + s2].dvalue;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				RAM[s1] = dota[s2].dvalue;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("lw", 0) != string::npos && str[l1].find("lw", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find(",", 0) + 2;
			t = "";
			if (str[l1].find("(", 0) != string::npos) {
				while (str[l1][position] != '(' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2 = atoi(t.c_str());
				t = "";
				for (position = str[l1].find("$", position) + 1; str[l1][position] != ')'; position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = dota[RAM[s3] + s2].dvalue;
			}
			else {
				while (position < str[l1].size())
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				t = t + ':';
				int s2 = labeltodata[t];
				RAM[s1] = dota[s2].dvalue;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("addiu", 0) != string::npos && str[l1].find("addiu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			position += 2;
			t = "";
			while (position < str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			//cout <<s <<' '<<atoi(t.c_str()) <<endl;
			RAM[s1] = (unsigned int)(RAM[s2] + atoi(t.c_str()));
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("addu", 0) != string::npos && str[l1].find("addu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (unsigned int)(RAM[s2] + RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (unsigned int)(RAM[s2] + s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("add", 0) != string::npos && str[l1].find("add", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = RAM[s2] + RAM[s3];
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = RAM[s2] + s3;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("subu", 0) != string::npos && str[l1].find("subu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (unsigned int)(RAM[s2] - RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (unsigned int)(RAM[s2] - s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sub", 0) != string::npos && str[l1].find("sub", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = RAM[s2] - RAM[s3];
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = RAM[s2] - s3;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("divu", 0) != string::npos && str[l1].find("divu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find(",",position + 1) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (str[l1][position] != ',' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					RAM[s1] = (unsigned int)(RAM[s2] / RAM[s3]);
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					RAM[s1] = (unsigned int)(RAM[s2] / s3);
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
			else {
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					lo = (unsigned int)(RAM[s1] / RAM[s3]);
					hi = (unsigned int)(RAM[s1] % RAM[s3]);
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					lo = (unsigned int)(RAM[s1] / s3);
					hi = (unsigned int)(RAM[s1] % s3);
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
		}
		
		if (str[l1].find("div", 0) != string::npos && str[l1].find("div", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find(",",position + 1) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (str[l1][position] != ',' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					RAM[s1] = RAM[s2] / RAM[s3];
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					RAM[s1] = RAM[s2] / s3;
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
			else {
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					lo = RAM[s1] / RAM[s3];
					hi = RAM[s1] % RAM[s3];
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					lo = RAM[s1] / s3;
					hi = RAM[s1] % s3;
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
		}
		
		if (str[l1].find("mulu", 0) != string::npos && str[l1].find("mulu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find(",",position + 1) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (str[l1][position] != ',' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					RAM[s1] = (unsigned int)(RAM[s2] * RAM[s3]);
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					RAM[s1] = (unsigned int)(RAM[s2] * s3);
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
			else {
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					long long ss;
					ss = (unsigned int)(RAM[s1] * RAM[s3]);
					lo = ss & ((1LL << 32) - 1);
					hi = ss >> 32;
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					long long ss;
					ss = (unsigned int)(RAM[s1] * s3);
					lo = ss & ((1LL << 32) - 1);
					hi = ss >> 32;
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
		}
		
		if (str[l1].find("mul", 0) != string::npos && str[l1].find("mul", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (str[l1].find(",",position + 1) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (str[l1][position] != ',' && position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s2;
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					RAM[s1] = RAM[s2] * RAM[s3];
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					RAM[s1] = RAM[s2] * s3;
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
			else {
				if (str[l1].find("$",position) != string::npos) {
					position = str[l1].find("$",position) + 1;
					t = "";
					while (position <str[l1].size()) 
						{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
					int s3;
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					long long ss;
					ss = RAM[s1] * RAM[s3];
					lo = ss & ((1LL << 32) - 1);
					hi = ss >> 32;
				}
				else {
					t = "";
					for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
						if (str[l1][position] != ' ') t = t + str[l1][position];
					int s3 = atoi(t.c_str());
					long long ss;
					ss = RAM[s1] * s3;
					lo = ss & ((1LL << 32) - 1);
					hi = ss >> 32;
				}
				//fout <<l1 <<' '<<str[l1] <<endl;
				continue;
			}
		}
		
		if (str[l1].find("xoru", 0) != string::npos && str[l1].find("xoru", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (unsigned int)(RAM[s2] xor RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (unsigned int)(RAM[s2] xor s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("xor", 0) != string::npos && str[l1].find("xor", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = RAM[s2] xor RAM[s3];
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = RAM[s2] xor s3;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("negu", 0) != string::npos && str[l1].find("negu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			RAM[s1] = -RAM[s2];
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("neg", 0) != string::npos && str[l1].find("neg", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			RAM[s1] = -RAM[s2];
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("remu", 0) != string::npos && str[l1].find("remu", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (unsigned int)(RAM[s2] % RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (unsigned int)(RAM[s2] % s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("rem", 0) != string::npos && str[l1].find("rem", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = RAM[s2] % RAM[s3];
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = RAM[s2] % s3;
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("seq", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] == RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] == s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sge", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] >= RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] >= s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sgt", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] > RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] > s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sle", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] <= RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] <= s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("slt", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] < RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] < s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sne", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			if (str[l1].find("$",position) != string::npos) {
				position = str[l1].find("$",position) + 1;
				t = "";
				while (position <str[l1].size()) 
					{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
				int s3;
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				RAM[s1] = (RAM[s2] != RAM[s3]);
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = (RAM[s2] != s3);
			}
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("mfhi", 0) != string::npos && str[l1].find("mfhi", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			RAM[s1] = hi;
			//cout <<l1 <<' ' <<str[l1] <<' ' << hi <<' ' <<s1<<endl;
			continue;
		}
		
		if (str[l1].find("mflo", 0) != string::npos && str[l1].find("mflo", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			RAM[s1] = lo;
			//cout <<l1 <<' ' <<str[l1] <<' ' << lo <<' ' <<s1<<endl;
			continue;
		}
		
		if (str[l1].find("move", 0) != string::npos && str[l1].find("move", 0) == 0) {
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s1;
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = str[l1].find("$",position) + 1;
			t = "";
			while (str[l1][position] != ',' && position <str[l1].size()) 
				{if (str[l1][position] != ' ') t = t + str[l1][position]; position++;}
			int s2;
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
			RAM[s1] = RAM[s2];
			//fout <<l1 <<' '<<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("syscall", 0) != string::npos) {
			//fout <<RAM[2] <<' ' <<RAM[4]<<endl;
			switch (RAM[2]) {
				case 1:cout <<RAM[4];break;
				case 4:{
					for (int i = RAM[4]; dota[i].dstring != '\0'; i++)
						cout <<dota[i].dstring;
					for (int i = RAM[4]; dota[i].dstring != '\0'; i++)
						fout <<dota[i].dstring;
					//fout <<endl;
					break;
				}
				case 5:fscanf(fp2, "%d" , &RAM[2]);break;
				case 8:{ 
					char c[10000];
					int temp = RAM[5];
					fscanf(fp2, "%s" , &c);
					string t = "";
					for (int j = 0; j < temp - 1; j++ ) t = t + c[j];
					
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
					
					//fout <<t <<endl<<t.size()<<endl;
					int tmp = RAM[4];
					
					for (int j = 0; j < t.size(); ++j) {
						dota[tmp].dtype = 5;
						dota[tmp].dstring = t[j];
						dota[tmp].size = 1;
						tmp++;
					}
					
					dota[tmp].dtype = 7;
					dota[tmp].dstring = '\0';
					dota[tmp].size = 1;
					break;}
				case 9:{ 
					RAM[2] = datanum;
					dota[datanum].dtype = 6;
					dota[datanum].size = RAM[4];
					datanum += RAM[4];
					
					break;} 
				case 10:return 0;break;
				case 17:return RAM[4];break;
			}
			
			/*for (int i = 0; i <= 10000000; i++) {
			if (dota[i].dtype >= 1 && dota[i].dtype <= 3) 
				cout <<i <<' '<<dota[i].dvalue <<' '<<dota[i].size <<endl;
			else if (dota[i].dtype == 4 || dota[i].dtype == 5) 
				cout <<i <<' '<<dota[i].dstring <<' '<<dota[i].size <<endl;
			else if (dota[i].dtype == 6)
				cout <<i <<' '<<dota[i].size <<endl;
			}*/
			continue;
		}
		
	}
	//fclose(fp2);
	return 0;
}
int main() {
	RAMready();
	beginset();
	
	int Begin = init();
	//cout <<Begin<<endl;
	dealdata();
	int t = mainprocedure(Begin);
	/*for (int i = 0; i < 32; i++)
		cout <<i <<' ' <<RAM[i] <<endl;
	cout <<endl;
	for (int i = 0; i <= 10000000; i++) {
		if (dota[i].dtype >= 1 && dota[i].dtype <= 3) 
			cout <<i <<' '<<dota[i].dvalue <<' '<<dota[i].size <<endl;
		else if (dota[i].dtype == 4 || dota[i].dtype == 5) 
			cout <<i <<' '<<dota[i].dstring <<' '<<dota[i].size <<endl;
		else if (dota[i].dtype == 6)
			cout <<i <<' '<<dota[i].size <<endl;
		else if (dota[i].dtype == 7) 
			cout <<i <<"!!!!!!"<<endl;
	}*/
	return t;
}
