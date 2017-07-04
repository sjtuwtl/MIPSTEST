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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = RAM[s2] + RAM[s3]; else ans = RAM[s2] + s3;
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class addu {
	private:
		int s1,s2,s3,type,ans;
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] + RAM[s3]); else ans = (unsigned int)(RAM[s2] + s3);
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class sub {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = RAM[s2] - RAM[s3]; else ans = RAM[s2] - s3;
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class subu {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] - RAM[s3]); else ans = (unsigned int)(RAM[s2] - s3);
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class mul {
	private:
		int s1, s2, s3, type;
		int ans;
		long long l, h;
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
			if (s.find(",",position + 1) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (s[position] != ',' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
			}
			else {
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 3;
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 4;
				}
			}
		}
		void Execution() {
			if (type == 1) ans = RAM[s2] * RAM[s3]; 
			else if (type == 2) ans = RAM[s2] * s3;
			else if (type == 3) {
				long long ss = RAM[s1] * RAM[s3];
				l = ss & ((1LL << 32) - 1);
				h = ss >> 32;
			}
		}
		void Write_Back() {
			if (type == 1 || type == 2) RAM[s1] = ans;
			else {lo = l; hi = h;}
		}
};

class mulu {
	private:
		int s1, s2, s3, type;
		int ans;
		long long l, h;
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
			if (s.find(",",position + 1) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (s[position] != ',' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
			}
			else {
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 3;
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 4;
				}
			}
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] * RAM[s3]); 
			else if (type == 2) ans = (unsigned int)(RAM[s2] * s3);
			else if (type == 3) {
				long long ss = (unsigned int)(RAM[s1] * RAM[s3]);
				l = ss & ((1LL << 32) - 1);
				h = ss >> 32;
			}
		}
		void Write_Back() {
			if (type == 1 || type == 2) RAM[s1] = ans;
			else {lo = l; hi = h;}
		}
};

class Div {
	private:
		int s1, s2, s3, type;
		int ans;
		long long l, h;
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
			if (s.find(",",position + 1) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (s[position] != ',' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
			}
			else {
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 3;
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 4;
				}
			}
		}
		void Execution() {
			if (type == 1) ans = RAM[s2] / RAM[s3]; 
			else if (type == 2) ans = RAM[s2] / s3;
			else if (type == 3) {
				l = RAM[s1] / RAM[s3];
				h = RAM[s1] % RAM[s3];
			}
		}
		void Write_Back() {
			if (type == 1 || type == 2) RAM[s1] = ans;
			else {lo = l; hi = h;}
		}
};

class Divu {
	private:
		int s1, s2, s3, type;
		long long l, h;
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
			if (s.find(",",position + 1) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (s[position] != ',' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
			}
			else {
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					type = 3;
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 4;
				}
			}
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] / RAM[s3]); 
			else if (type == 2) ans = (unsigned int)(RAM[s2] / s3);
			else if (type == 3) {
				l = (unsigned int)(RAM[s2] / RAM[s3]);
				h = (unsigned int)(RAM[s2] % RAM[s3]);
			}
		}
		void Write_Back() {
			if (type == 1 || type == 2) RAM[s1] = ans;
			else {lo = l; hi = h;}
		}
};

class Xor {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = (RAM[s2] ^ RAM[s3]); else ans = (RAM[s2] ^ s3);
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class Xoru {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] ^ RAM[s3]); else ans = (unsigned int)(RAM[s2] ^ s3);
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class neg {
	private:
		int s1,s2;
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
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
		}
		void Execution() {
			ans = (-1) * RAM[s2];
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class negu {
	private:
		int s1,s2;
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
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
		}
		void Execution() {
			ans = (-1) * RAM[s2];
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class rem {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = RAM[s2] % RAM[s3]; else ans = RAM[s2] % s3;
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class remu {
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
				while (position <s.size()) 
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
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] % RAM[s3]); else ans = (unsigned int)(RAM[s2] % s3);
		}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class li {
	private:
		int s1, s2;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$", 0) + 1;
			string t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 1;
			t = "";
			while (position < s.size())
				{if (s[position] != ' ') t = t + s[position]; position++;}
			s2 = atoi(t.c_str());
		}
		void Execution() {}
		void Write_Back() {
			RAM[s1] = s2;
		}
};

class seq {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] == RAM[s3]); else ans = (RAM[s2] == s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class sge {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] >= RAM[s3]); else ans = (RAM[s2] >= s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class sgt {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] > RAM[s3]); else ans = (RAM[s2] > s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class sle {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] <= RAM[s3]); else ans = (RAM[s2] <= s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class slt {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] < RAM[s3]); else ans = (RAM[s2] < s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class sne {
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
				while (position <s.size()) 
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
		void Execution() {if (type == 1) ans = (RAM[s2] != RAM[s3]); else ans = (RAM[s2] != s3);}
		void Write_Back() {
			RAM[s1] = ans;
		}
};

class b {
	private:
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			for (int k = 2; k < s.size(); ++k) if (s[k] != ' ') t = t + s[k];
			t += ':';
		}
		void Execution() {}
		int Write_Back() {
			return label[t];
		}
};

class beq {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] == RAM[s2]); else b = (RAM[s1] == s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bne {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] != RAM[s2]); else b = (RAM[s1] != s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class blt {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] < RAM[s2]); else b = (RAM[s1] < s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bge {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] >= RAM[s2]); else b = (RAM[s1] >= s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class ble {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] <= RAM[s2]); else b = (RAM[s1] <= s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bgt {
	private:
		int s1,s2,type;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 1;
			}
			else {
				t = "";
				for (position += 2;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				s2 = atoi(t.c_str());
				t = "";
				for (position += 2;position < s.size() ;position++)
					if (s[position] != ' ') t = t + s[position];
				t = t + ':';
				type = 2;
			}
		}
		void Execution() {if (type == 1) b = (RAM[s1] > RAM[s2]); else b = (RAM[s1] > s2);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class beqz {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] == 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bnez {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] != 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class blez {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] <= 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bgez {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] >= 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bgtz {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] > 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class bltz {
	private:
		int s1;
		bool b;
		string t;
	public:
		void Data_Preparation(string s) {
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			t = "";
			for (position = s.find(",", 0) + 2;position < s.size() ;position++)
				if (s[position] != ' ') t = t + s[position];
			t = t + ':';
		}
		void Execution() {b = (RAM[s1] < 0);}
		int Write_Back() {if (b) return label[t]; else return 0;}
};

class j {
	private:
		string t;
		int ans;
	public:
		void Data_Preparation(string s) {
			t = "";
			for (int k = 2; k < s.size(); ++k) if (s[k] != ' ') t = t + s[k];
			t += ':';
		}
		void Execution() {ans = label[t];}
		int Write_Back() {return ans;}
};

class jr {
	private:
		int s1, ans;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			string t = "";
			while (s[position] != ' ' && position <s.size()) 
				{t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
		}
		void Execution() {ans = RAM[s1];}
		int Write_Back() {return ans - 1;}
};

class jal {
	private:
		int l, ans;
		string t;
	public:
		void Data_Preparation(string s, int m) {
			t = "";
			for (int position = s.find("jal", 0) + 4;position < s.size();position++)
				if (s[position] != ' ') t = t + s[position];
			l = m + 1;
			t = t + ':';
		}
		void Execution() {ans = label[t];}
		int Write_Back() {RAM[31] = l;return ans;}
};

class la {
	private:
		string t;
		int s1, s2, s3, type, ans;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 2;
			t = "";
			if (s.find("(", 0) != string::npos) {
				while (s[position] != '(' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				s2 = atoi(t.c_str());
				t = "";
				for (position = s.find("$", position) + 1; s[position] != ')'; position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
		}
		void Execution() {if (type == 1) ans = RAM[s3] + s2; else ans = s2;}
		void Write_Back() {RAM[s1] = ans;}
};

class lb {
	private:
		string t;
		int s1, s2, s3, type, ans, address;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 2;
			t = "";
			if (s.find("(", 0) != string::npos) {
				while (s[position] != '(' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				s2 = atoi(t.c_str());
				t = "";
				for (position = s.find("$", position) + 1; s[position] != ')'; position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {ans = int(dota[address].dstring);}
		void Write_Back() {RAM[s1] = ans;}
};

class lw {
	private:
		string t;
		int s1, s2, s3, type, address, ans;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 2;
			t = "";
			if (s.find("(", 0) != string::npos) {
				while (s[position] != '(' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				s2 = atoi(t.c_str());
				t = "";
				for (position = s.find("$", position) + 1; s[position] != ')'; position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {ans = int(dota[address].dvalue);}
		void Write_Back() {RAM[s1] = ans;}
};

class sb {
	private:
		string t;
		int s1, s2, s3, type, address;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 2;
			t = "";
			if (s.find("(", 0) != string::npos) {
				while (s[position] != '(' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				s2 = atoi(t.c_str());
				t = "";
				for (position = s.find("$", position) + 1; s[position] != ')'; position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {dota[address].dstring = char(RAM[s1]);}
		void Write_Back() {}
};

class sw {
	private:
		string t;
		int s1, s2, s3, type, ans, address;
	public:
		void Data_Preparation(string s) {
			int position = s.find("$",0) + 1;
			t = "";
			while (s[position] != ',' && position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			position = s.find(",", 0) + 2;
			t = "";
			if (s.find("(", 0) != string::npos) {
				while (s[position] != '(' && position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				s2 = atoi(t.c_str());
				t = "";
				for (position = s.find("$", position) + 1; s[position] != ')'; position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {dota[address].dvalue = RAM[s1];}
		void Write_Back() {}
};

class Move {
	private:
		int s1, s2, ans;
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
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s2 = atoi(t.c_str());
			else
				s2 = RAMmap[t];
		}
		void Execution() {ans = RAM[s2];}
		void Write_Back() {RAM[s1] = ans;}
};

class mfhi {
	private:
		int s1, ans;
	public:
		void Data_Preparation(string s)	{
			int position = s.find("$",0) + 1;
			string t = "";
			while (position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
		}
		void Execution() {ans = hi;}
		void Write_Back() {RAM[s1] = ans;}
};

class mflo {
	private:
		int s1, ans;
	public:
		void Data_Preparation(string s)	{
			int position = s.find("$",0) + 1;
			string t = "";
			while (position <s.size()) 
				{if (s[position] != ' ') t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
		}
		void Execution() {ans = lo;}
		void Write_Back() {RAM[s1] = ans;}
};

class syscall {
	private:
		//int v0, a0;
	public:
		void Data_Preparation() {}
		void Execution() {}
		void Memory_Access() {}
		int Write_Back() {
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
				case 5:cin>>RAM[2];break;
				case 8:{ 
					char c[10000];
					int temp = RAM[5];
					cin>>c;
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
			return -1;
		}
};

int status[5] = { 0 };
string s[5];
string name[5];
add adddata[5];
addu addudata[5];
sub subdata[5];
subu subudata[5];
mul muldata[5];
mulu muludata[5];
Div Divdata[5];
Divu Divudata[5];
Xor Xordata[5];
Xoru Xorudata[5];
neg negdata[5];
negu negudata[5];
rem remdata[5];
remu remudata[5];
li lidata[5];
seq seqdata[5];
sge sgedata[5];
sgt sgtdata[5];
sle sledata[5];
slt sltdata[5];
sne snedata[5];
b bdata[5];
beq beqdata[5];
bne bnedata[5];
bge bgedata[5];
blt bltdata[5];
ble bledata[5];
bgt bgtdata[5];
beqz beqzdata[5];
bnez bnezdata[5];
blez blezdata[5];
bgez bgezdata[5];
bgtz bgtzdata[5];
bltz bltzdata[5];
j jdata[5];
jr jrdata[5];
jal jaldata[5];
la ladata[5];
lb lbdata[5];
lw lwdata[5];
sb sbdata[5];
sw swdata[5];
Move Movedata[5];
mfhi mfhidata[5];
mflo mflodata[5];
syscall syscalldata[5];

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

void beginset() {
	RAM[29] = 10000000 - 1;
	lo = 0;
	hi = 0;
	for (int i = 0; i < 10000000; i++) dota[i].dtype = -1;
}
string get_name(string s)
{
	string ss ="";
	for (int k = 0;k < s.size() &&  s[k] != ' ';++k) ss += s[k];
	return ss;
}
int main() {
	//FILE * fp2 = fopen("3.in", "r");
	RAMready();
	beginset();
	int l1 = init();
	//fout <<l1 <<endl;
	dealdata();
	while (l1 < str.size()) {
		l1++;
		fout <<l1 <<' '<<str[l1] <<endl;
		/*for (int j = 0; j <= 33; j++)
			fout <<j <<'\t';
		fout <<endl;
		for (int j = 0; j <= 31; j++)
			fout <<RAM[j] <<'\t';
		fout <<lo <<'\t' << hi <<'\t';
		fout <<endl<<endl <<endl;*/
		string g = get_name(str[l1]);
		if (g == "add") {adddata[1].Data_Preparation(str[l1]);adddata[1].Execution();adddata[1].Write_Back();continue;}
		if (g == "addu") {addudata[1].Data_Preparation(str[l1]);addudata[1].Execution();addudata[1].Write_Back();continue;}
		if (g == "sub") {subdata[1].Data_Preparation(str[l1]);subdata[1].Execution();subdata[1].Write_Back();continue;}
		if (g == "subu") {subudata[1].Data_Preparation(str[l1]);subudata[1].Execution();subudata[1].Write_Back();continue;}
		if (g == "mul") {muldata[1].Data_Preparation(str[l1]);muldata[1].Execution();muldata[1].Write_Back();continue;}
		if (g == "mulu") {muludata[1].Data_Preparation(str[l1]);muludata[1].Execution();muludata[1].Write_Back();continue;}
		if (g == "div") {Divdata[1].Data_Preparation(str[l1]);Divdata[1].Execution();Divdata[1].Write_Back();continue;}
		if (g == "divu") {Divudata[1].Data_Preparation(str[l1]);Divudata[1].Execution();Divudata[1].Write_Back();continue;}
		if (g == "xor") {Xordata[1].Data_Preparation(str[l1]);Xordata[1].Execution();Xordata[1].Write_Back();continue;}
		if (g == "xoru") {Xorudata[1].Data_Preparation(str[l1]);Xorudata[1].Execution();Xorudata[1].Write_Back();continue;}
		if (g == "neg") {negdata[1].Data_Preparation(str[l1]);negdata[1].Execution();negdata[1].Write_Back();continue;}
		if (g == "negu") {negudata[1].Data_Preparation(str[l1]);negudata[1].Execution();negudata[1].Write_Back();continue;}
		if (g == "rem") {remdata[1].Data_Preparation(str[l1]);remdata[1].Execution();remdata[1].Write_Back();continue;}
		if (g == "seq") {seqdata[1].Data_Preparation(str[l1]);seqdata[1].Execution();seqdata[1].Write_Back();continue;}
		if (g == "sge") {sgedata[1].Data_Preparation(str[l1]);sgedata[1].Execution();sgedata[1].Write_Back();continue;}
		if (g == "sgt") {sgtdata[1].Data_Preparation(str[l1]);sgtdata[1].Execution();sgtdata[1].Write_Back();continue;}
		if (g == "sle") {sledata[1].Data_Preparation(str[l1]);sledata[1].Execution();sledata[1].Write_Back();continue;}
		if (g == "slt") {sltdata[1].Data_Preparation(str[l1]);sltdata[1].Execution();sltdata[1].Write_Back();continue;}
		if (g == "sne") {snedata[1].Data_Preparation(str[l1]);snedata[1].Execution();snedata[1].Write_Back();continue;}
		if (g == "li") {lidata[1].Data_Preparation(str[l1]);lidata[1].Write_Back();continue;}
		if (g == "b") {bdata[1].Data_Preparation(str[l1]);bdata[1].Execution();l1 = bdata[1].Write_Back();continue;}
		if (g == "j") {jdata[1].Data_Preparation(str[l1]);jdata[1].Execution();l1 = jdata[1].Write_Back();continue;}
		if (g == "beq") {beqdata[1].Data_Preparation(str[l1]);beqdata[1].Execution();if (beqdata[1].Write_Back() != 0) l1 = beqdata[1].Write_Back();continue;}
		if (g == "bge") {bgedata[1].Data_Preparation(str[l1]);bgedata[1].Execution();if (bgedata[1].Write_Back() != 0) l1 = bgedata[1].Write_Back();continue;}
		if (g == "bgt") {bgtdata[1].Data_Preparation(str[l1]);bgtdata[1].Execution();if (bgtdata[1].Write_Back() != 0) l1 = bgtdata[1].Write_Back();continue;}
		if (g == "ble") {bledata[1].Data_Preparation(str[l1]);bledata[1].Execution();if (bledata[1].Write_Back() != 0) l1 = bledata[1].Write_Back();continue;}
		if (g == "blt") {bltdata[1].Data_Preparation(str[l1]);bltdata[1].Execution();if (bltdata[1].Write_Back() != 0) l1 = bltdata[1].Write_Back();continue;}
		if (g == "bne") {bnedata[1].Data_Preparation(str[l1]);bnedata[1].Execution();if (bnedata[1].Write_Back() != 0) l1 = bnedata[1].Write_Back();continue;}
		if (g == "beqz") {beqzdata[1].Data_Preparation(str[l1]);beqzdata[1].Execution();if (beqzdata[1].Write_Back() != 0) l1 = beqzdata[1].Write_Back();continue;}
		if (g == "bgez") {bgezdata[1].Data_Preparation(str[l1]);bgezdata[1].Execution();if (bgezdata[1].Write_Back() != 0) l1 = bgezdata[1].Write_Back();continue;}
		if (g == "bgtz") {bgtzdata[1].Data_Preparation(str[l1]);bgtzdata[1].Execution();if (bgtzdata[1].Write_Back() != 0) l1 = bgtzdata[1].Write_Back();continue;}
		if (g == "blez") {blezdata[1].Data_Preparation(str[l1]);blezdata[1].Execution();if (blezdata[1].Write_Back() != 0) l1 = blezdata[1].Write_Back();continue;}
		if (g == "bltz") {bltzdata[1].Data_Preparation(str[l1]);bltzdata[1].Execution();if (bltzdata[1].Write_Back() != 0) l1 = bltzdata[1].Write_Back();continue;}
		if (g == "bnez") {bnezdata[1].Data_Preparation(str[l1]);bnezdata[1].Execution();if (bnezdata[1].Write_Back() != 0) l1 = bnezdata[1].Write_Back();continue;}
		if (g == "jr") {jrdata[1].Data_Preparation(str[l1]);jrdata[1].Execution();l1 = jrdata[1].Write_Back();continue;}
		if (g == "jal") {jaldata[1].Data_Preparation(str[l1], l1);jaldata[1].Execution();l1 = jaldata[1].Write_Back();continue;}
		
		if (g == "la") {ladata[1].Data_Preparation(str[l1]);ladata[1].Execution();ladata[1].Write_Back();continue;}
		if (g == "lb") {lbdata[1].Data_Preparation(str[l1]);lbdata[1].Execution();lbdata[1].Memory_Access();lbdata[1].Write_Back();continue;}
		if (g == "lw") {lwdata[1].Data_Preparation(str[l1]);lwdata[1].Execution();lwdata[1].Memory_Access();lwdata[1].Write_Back();continue;}
		if (g == "sb") {sbdata[1].Data_Preparation(str[l1]);sbdata[1].Execution();sbdata[1].Memory_Access();continue;}
		if (g == "sw") {swdata[1].Data_Preparation(str[l1]);swdata[1].Execution();swdata[1].Memory_Access();continue;}
		if (g == "move") {Movedata[1].Data_Preparation(str[l1]);Movedata[1].Execution();Movedata[1].Write_Back();continue;}
		if (g == "mflo") {mflodata[1].Data_Preparation(str[l1]);mflodata[1].Execution();mflodata[1].Write_Back();continue;}
		if (g == "mfhi") {mfhidata[1].Data_Preparation(str[l1]);mfhidata[1].Execution();mfhidata[1].Write_Back();continue;}
		if (g == "syscall") {int mark = syscalldata[1].Write_Back();if (mark != -1) return mark;continue;}
	
		
	}
	
}
