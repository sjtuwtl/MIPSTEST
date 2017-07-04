#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <map>
#include <cmath>

using namespace std;

vector<string> str;
map<string, int> label;
map<string, int> labeltodata;
map<string, int> RAMmap;
int linenum;
struct datatype{
	int size;
	int dvalue;
	int dtype;
	string dstring;
};
datatype data[10000000];
int RAM[32] = {0};
int lo, hi;
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
		if (c[strlen(c) - 1] == ':') label[c] = linenum;
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
						cout <<1 <<endl;
						int s =  atoi(t.c_str());
						position++;
	
						data[datanum].dtype = 1;
						data[datanum].dvalue = s;
						data[datanum].size = 4;
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
						cout <<2 <<endl;
						int s = atoi(t.c_str());
						position++;
	
						data[datanum].dtype = 1;
						data[datanum].dvalue = s;
						data[datanum].size = 2;
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
						cout <<3 <<endl;
						int s =  atoi(t.c_str());
						position++;
	
						data[datanum].dtype = 1;
						data[datanum].dvalue = s;
						data[datanum].size = 1;
						datanum += 1;
					}
				}
				
				if (str[i].find(".asciiz",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t;
					while (position < str[i].size() - 1) {
						t = t + str[i][position];
						position++;
					}
					
					cout <<5 <<endl;

					data[datanum].dtype = 5;
					data[datanum].dstring = t;
					data[datanum].size = str[i].size() - 1 - str[i].find("\"",0);
					datanum += data[datanum].size;
				}
				else if (str[i].find(".ascii",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t;
					while (position < str[i].size() - 1) {
						t = t + str[i][position];
						position++;
					}
					cout <<4 <<endl;

					data[datanum].dtype = 4;
					data[datanum].dstring = t;
					data[datanum].size = str[i].size() - 2 - str[i].find("\"",0);
					datanum += data[datanum].size;
				}
				
				
				
				if (str[i].find(".align",0) != string::npos) {
					int position = str[i].find(".align",0) + 7;
					string t = "";
					while (position < str[i].size()) {
						t = t + str[i][position];
						position++;
					}
					cout <<6 <<endl;
					int s = atoi(t.c_str());
					s = pow(2, s);
					int tmp = s;
					while (tmp < datanum - 1) tmp += s;
					datanum = tmp + 1;
				}
				
				if (str[i].find(".space",0) != string::npos) {
					int position = str[i].find(".space",0) + 7;
					string t;
					while (position < str[i].size()) {
						t = t + str[i][position];
						position++;
					}
					cout <<7 <<endl;
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
	RAM[29] = 1;
	lo = 0;
	hi = 0;
	for (int i = 0; i < 10000000; i++) data[i].dtype = -1;
}
int mainprocedure(int l1) {
	FILE * fp2 = fopen("1.in", "r");
	cout <<str.size()<<endl; 
	while (l1 < str.size()) {
		l1++;
		if (l1 >= str.size()) break;
		if (str[l1].find("li", 0) != string::npos) {
			int position = str[l1].find("$", 0);
			position++;
			string t = "";
			while (str[l1][position] != ' ' && position <str[l1].size()) 
				{t = t + str[l1][position]; position++;}
			int s;
			if (t[0] >= '0' && t[0] <= '9') 
				s = atoi(t.c_str());
			else
				s = RAMmap[t];
			position = str[l1].find(",", 0);
			position++;
			t = "";
			while (position < str[l1].size()) {t = t + str[l1][position]; position++;}
			//cout <<s <<' '<<atoi(t.c_str()) <<endl;
			RAM[s] = atoi(t.c_str());
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
			
		if (str[l1].find("jal", 0) != string::npos) {
			cout <<l1 <<' ' <<str[l1] <<endl;
			string t = "";
			for (int position = str[l1].find("jal", 0) + 4;position <= str[l1].size();position++)
				if (str[l1][position] != ' ') t = t + str[l1][position];
			RAM[31] = l1 + 1;
			l1 = label[t] + 1;
			
			continue;
		}
		
		if (str[l1].find("jr", 0) != string::npos) {
			cout <<l1 <<' ' <<str[l1] <<endl;
			int position = str[l1].find("$",0) + 1;
			string t = "";
			while (str[l1][position] != ' ' && position <str[l1].size()) 
				{t = t + str[l1][position]; position++;}
			int s;
			if (t[0] >= '0' && t[0] <= '9') 
				s = atoi(t.c_str());
			else
				s = RAMmap[t];
			l1 = RAM[s];
			
			continue;
		}
		
		if (str[l1].find("add", 0) != string::npos) {
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
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("sub", 0) != string::npos) {
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
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("mul", 0) != string::npos) {
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
				RAM[s1] = RAM[s2] * RAM[s3];
			}
			else {
				t = "";
				for (position = str[l1].find(",",position) + 2;position < str[l1].size(); position++)
					if (str[l1][position] != ' ') t = t + str[l1][position];
				int s3 = atoi(t.c_str());
				RAM[s1] = RAM[s2] * s3;
			}
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("move", 0) != string::npos) {
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
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
		
		if (str[l1].find("syscall", 0) != string::npos) {
			switch (RAM[2]) {
				case 1:cout <<RAM[4];break;
				case 4:cout <<data[RAM[4]].dstring;break;
				case 5:fscanf(fp2 , "%d" , &RAM[2]);break;
				case 8:{ 
					char c[10000];
					fscanf(fp2 , "%[^\n]s" , &c);
					string t = c;
					data[datanum].dtype = 5;
					data[datanum].dstring = t;
					data[datanum].size = strlen(c) + 1;
					datanum += data[datanum].size;
					RAM[4] = datanum;
					RAM[5] = strlen(c) + 1;
					break;} 
				case 9:{ 
					data[datanum].dtype = 6;
					data[datanum].size = RAM[4];
					datanum += RAM[4];
					RAM[2] = datanum;
					break;} 
				case 10:return 0;break;
				case 17:return RAM[4];break;
			}
			cout <<l1 <<' ' <<str[l1] <<endl;
			continue;
		}
		
	}
	fclose(fp2);
	return 0;
}
int main() {
	RAMready();
	beginset();
	int Begin = init();
	//cout <<Begin<<endl;
	dealdata();
	//int t = mainprocedure(Begin);
	/*for (int i = 0; i < 32; i++)
		cout <<i <<' ' <<RAM[i] <<endl;*/
	for (int i = 0; i <= 10000000; i++) {
		if (data[i].dtype >= 1 && data[i].dtype <= 3) 
			cout <<i <<' '<<data[i].dvalue <<' '<<data[i].size <<endl;
		else if (data[i].dtype == 4 || data[i].dtype == 5) 
			cout <<i <<' '<<data[i].dstring <<' '<<data[i].size <<endl;
		else if (data[i].dtype == 6)
			cout <<i <<' '<<data[i].size <<endl;
	}
	return 0;
} 
