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
//ofstream fout("1.out");

vector<string> str;
map<string, int> label;
map<string, int> labeltodata;
map<string, int> RAMmap;
map<string, int> intmap;
int linenum = 0;
struct datatype{
	int size;
	int dvalue;
	int dtype;
	char dstring;
};
int hazard = 0;
datatype dota[10000000];
long long RAM[32] = {0};
int lock[34] = {0};
long long lo, hi;
int datanum = 0;

int RHazard(int x) {
	if (lock[x] == 1) hazard = 2;
	return 0;
}

int LHazard(int x) {
	int m = lock[x];
	lock[x] = 1;
	return m;
}

int maxx(int x, int y) {
	if (x > y) return x; else return y;
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

class add {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = RAM[s2] + RAM[s3]; else ans = RAM[s2] + s3;
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class addu {
	private:
		int s1,s2,s3,type,ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] + RAM[s3]); else ans = (unsigned int)(RAM[s2] + s3);
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class sub {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = RAM[s2] - RAM[s3]; else ans = RAM[s2] - s3;
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class subu {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] - RAM[s3]); else ans = (unsigned int)(RAM[s2] - s3);
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class mul {
	private:
		int s1, s2, s3, type;
		int ans;
		long long ll, hh;
		int h = 0, h1 = 0, h2 = 0;
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
				h = maxx(h, RHazard(s2));
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					h = maxx(h, RHazard(s3));
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
				h = maxx(h, LHazard(s1));
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
					h = maxx(h, RHazard(s3));h = maxx(h, RHazard(s1));
					h1 = maxx(h1, LHazard(32)); h2 = maxx(h2, LHazard(33));
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
				ll = ss & ((1LL << 32) - 1);
				hh = ss >> 32;
			}
		}
		void Write_Back() {
			if (type == 3) {lock[32] = h1; lock[33] = h2;}
			if (type == 1 || type == 2) {
				RAM[s1] = ans;lock[s1] = h;
			}
			else {lo = ll; hi = hh;}
		}
};

class mulu {
	private:
		int s1, s2, s3, type;
		int ans;
		long long ll, hh;
		int h = 0, h1 = 0, h2 = 0;
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
				h = maxx(h, RHazard(s2));
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					h = maxx(h, RHazard(s3));
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
				h = maxx(h, LHazard(s1));
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
					h = maxx(h, RHazard(s3));h = maxx(h, RHazard(s1));
					h1 = maxx(h1, LHazard(32)); h2 = maxx(h2, LHazard(33));
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
				ll = ss & ((1LL << 32) - 1);
				hh = ss >> 32;
			}
		}
		void Write_Back() {
			if (type == 3) {lock[32] = h1; lock[33] = h2;}
			if (type == 1 || type == 2) {
				RAM[s1] = ans;lock[s1] = h;
			}
			else {lo = ll; hi = hh;}
		}
};

class Div {
	private:
		int s1, s2, s3, type;
		int ans;
		long long ll, hh;
		int h = 0, h1 = 0, h2 = 0;
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
				h = maxx(h, RHazard(s2));
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					h = maxx(h, RHazard(s3));
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
				h = maxx(h, LHazard(s1));
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
					h = maxx(h, RHazard(s3));h = maxx(h, RHazard(s1));
					h1 = maxx(h1, LHazard(32)); h2 = maxx(h2, LHazard(33));
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
				ll = RAM[s1] / RAM[s3];
				hh = RAM[s1] % RAM[s3];
			}
		}
		void Write_Back() {
			if (type == 3) {lock[32] = h1; lock[33] = h2;}
			if (type == 1 || type == 2) {
				RAM[s1] = ans;lock[s1] = h;
			}
			else {lo = ll; hi = hh;}
		}
};

class Divu {
	private:
		int s1, s2, s3, type;
		int ans;
		long long ll, hh;
		int h = 0, h1 = 0, h2 = 0;
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
				h = maxx(h, RHazard(s2));
				if (s.find("$",position) != string::npos) {
					position = s.find("$",position) + 1;
					t = "";
					while (position <s.size()) 
						{if (s[position] != ' ') t = t + s[position]; position++;}
					if (t[0] >= '0' && t[0] <= '9') 
						s3 = atoi(t.c_str());
					else
						s3 = RAMmap[t];
					h = maxx(h, RHazard(s3));
					type = 1; //RAM
				}
				else {
					t = "";
					for (position = s.find(",",position) + 2;position < s.size(); position++)
						if (s[position] != ' ') t = t + s[position];
					s3 = atoi(t.c_str());
					type = 2;
				}
				h = maxx(h, LHazard(s1));
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
					h = maxx(h, RHazard(s3));h = maxx(h, RHazard(s1));
					h1 = maxx(h1, LHazard(32)); h2 = maxx(h2, LHazard(33));
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
				ll = (unsigned int)(RAM[s2] / RAM[s3]);
				hh = (unsigned int)(RAM[s2] % RAM[s3]);
			}
		}
		void Write_Back() {
			if (type == 3) {lock[32] = h1; lock[33] = h2;}
			if (type == 1 || type == 2) {
				RAM[s1] = ans;lock[s1] = h;
			}
			else {lo = ll; hi = hh;}
		}
};

class Xor {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = (RAM[s2] ^ RAM[s3]); else ans = (RAM[s2] ^ s3);
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class Xoru {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] ^ RAM[s3]); else ans = (unsigned int)(RAM[s2] ^ s3);
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class neg {
	private:
		int s1,s2;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			ans = (-1) * RAM[s2];
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class negu {
	private:
		int s1,s2;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			ans = (-1) * RAM[s2];
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class rem {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = RAM[s2] % RAM[s3]; else ans = RAM[s2] % s3;
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class remu {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {
			if (type == 1) ans = (unsigned int)(RAM[s2] % RAM[s3]); else ans = (unsigned int)(RAM[s2] % s3);
		}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class li {
	private:
		int s1, s2;
		int h = 0;
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
			h = maxx(h, LHazard(s1));
		}
		void Execution() {}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = s2;
		}
};

class seq {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] == RAM[s3]); else ans = (RAM[s2] == s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class sge {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] >= RAM[s3]); else ans = (RAM[s2] >= s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class sgt {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] > RAM[s3]); else ans = (RAM[s2] > s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class sle {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] <= RAM[s3]); else ans = (RAM[s2] <= s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class slt {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] < RAM[s3]); else ans = (RAM[s2] < s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class sne {
	private:
		int s1,s2,s3,type;
		int ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			if (s.find("$",position) != string::npos) {
				position = s.find("$",position) + 1;
				t = "";
				while (position <s.size()) 
					{if (s[position] != ' ') t = t + s[position]; position++;}
				if (t[0] >= '0' && t[0] <= '9') 
					s3 = atoi(t.c_str());
				else
					s3 = RAMmap[t];
				h = maxx(h, RHazard(s3));
				type = 1;// RAM
			}
			else {
				t = "";
				for (position = s.find(",",position) + 2;position < s.size(); position++)
					if (s[position] != ' ') t = t + s[position];
				s3 = atoi(t.c_str());
				type = 2;// IM
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = (RAM[s2] != RAM[s3]); else ans = (RAM[s2] != s3);}
		void Write_Back() {
			lock[s1] = h;
			RAM[s1] = ans;
		}
};

class b {
	private:
		string t;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
			if (s.find("$", position) != string::npos) {
				t = "";
				for (position = s.find("$",position) + 1;s[position] != ',';position++)
					if (s[position] != ' ') t = t + s[position];
				if (t[0] >= '0' && t[0] <= '9') 
					s2 = atoi(t.c_str());
				else
					s2 = RAMmap[t];
				h = maxx(h, RHazard(s2));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			t = "";
			int position;
			for (position = s.find("$", 0) + 1;s[position] != ',';position++)
				if (s[position] != ' ') t = t + s[position];
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
			hazard = 1;
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
		int h = 0;
	public:
		void Data_Preparation(string s) {
			hazard = 1;
			int position = s.find("$",0) + 1;
			string t = "";
			while (s[position] != ' ' && position <s.size()) 
				{t = t + s[position]; position++;}
			if (t[0] >= '0' && t[0] <= '9') 
				s1 = atoi(t.c_str());
			else
				s1 = RAMmap[t];
			h = maxx(h, RHazard(s1));
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
			hazard = 1;
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
		int h = 0;
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
				h = maxx(h, RHazard(s3));
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) ans = RAM[s3] + s2; else ans = s2;}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class lb {
	private:
		string t;
		int s1, s2, s3, type, ans, address;
		int h = 0;
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
				h = maxx(h, RHazard(s3));
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
			h = maxx(h, LHazard(s1));			
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {ans = int(dota[address].dstring);}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class lw {
	private:
		string t;
		int s1, s2, s3, type, address, ans;
		int h = 0;
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
				h = maxx(h, RHazard(s3));
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
			h = maxx(h, LHazard(s1));
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {ans = int(dota[address].dvalue);}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class sb {
	private:
		string t;
		int s1, s2, s3, type, address;
		int h = 0;
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
				h = maxx(h, RHazard(s3));
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
			h = maxx(h, RHazard(s1));
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {dota[address].dstring = char(RAM[s1]);}
		void Write_Back() {}
};

class sw {
	private:
		string t;
		int s1, s2, s3, type, address;
		int h = 0;
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
				h = maxx(h, RHazard(s3));
				type = 1;
			}
			else {
				while (position < s.size())
					{if (s[position] != ' ') t = t + s[position]; position++;}
				t = t + ':';
				s2 = labeltodata[t];
				type = 2;
			}
			h = maxx(h, RHazard(s1));
		}
		void Execution() {if (type == 1) address = RAM[s3] + s2; else address = s2;}
		void Memory_Access() {dota[address].dvalue = RAM[s1];}
		void Write_Back() {}
};

class Move {
	private:
		int s1, s2, ans;
		int h = 0;
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
			h = maxx(h, RHazard(s2));
			h = maxx(h, LHazard(s1));
		}
		void Execution() {ans = RAM[s2];}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class mfhi {
	private:
		int s1, ans;
		int h = 0;
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
			h = maxx(h, RHazard(32));
			h = maxx(h, LHazard(s1));
		}
		void Execution() {ans = hi;}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class mflo {
	private:
		int s1, ans;
		int h = 0;
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
			h = maxx(h, RHazard(33));
			h = maxx(h, LHazard(s1));
		}
		void Execution() {ans = lo;}
		void Write_Back() {lock[s1] = h;RAM[s1] = ans;}
};

class syscall {
	private:
		//int v0, a0;
		int h = 0;
	public:
		void Data_Preparation() {
			h = maxx(h, RHazard(2));
			h = maxx(h, RHazard(4));
			if (RAM[2] == 5 || RAM[2] == 9) h = maxx(h, LHazard(2));
		}
		void Execution() {}
		void Memory_Access() {}
		int Write_Back() {
			if (RAM[2] == 5 || RAM[2] == 9) lock[2] = h;
			switch (RAM[2]) {
				case 1:cout <<RAM[4];break;
				case 4:{
					for (int i = RAM[4]; dota[i].dstring != '\0'; i++)
						cout <<dota[i].dstring;
					//for (int i = RAM[4]; dota[i].dstring != '\0'; i++)
						//fout <<dota[i].dstring;
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

int status[5] = {0};
string s[5];
int name[5];
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
	intmap["add"] = 1;intmap["addu"] = 2;intmap["sub"] = 3;intmap["subu"] = 4;intmap["mul"] = 5;
	intmap["mulu"] = 6;intmap["div"] = 7;intmap["divu"] = 8;intmap["xor"] = 9;intmap["xoru"] = 10;
	intmap["neg"] = 11;intmap["negu"] = 12;intmap["rem"] = 13;intmap["remu"] = 14;intmap["li"] = 15;
	intmap["seq"] = 16;intmap["sge"] = 17;intmap["sgt"] = 18;intmap["sle"] = 19;intmap["slt"] = 20;
	intmap["sne"] = 21;intmap["b"] = 22;intmap["beq"] = 23;intmap["bne"] = 24;intmap["bge"] = 25;
	intmap["ble"] = 26;intmap["bgt"] = 27;intmap["blt"] = 28;intmap["beqz"] = 29;intmap["bnez"] = 30;
	intmap["bgez"] = 31;intmap["blez"] = 32;intmap["bgtz"] = 33;intmap["bltz"] = 34;intmap["j"] = 35;
	intmap["jr"] = 36;intmap["jal"] = 37;intmap["la"] = 38;intmap["lb"] = 39;intmap["lw"] = 40;
	intmap["sb"] = 41;intmap["sw"] = 42;intmap["move"] = 43;intmap["mfhi"] = 44;intmap["mflo"] = 45;
	intmap["syscall"] = 46;
}

int init(char * r) {
	FILE * fp1 = fopen(r, "r");
	char c[100];
	linenum = 0;
	while (fscanf(fp1 , "%[^\n]s" , &c) != EOF) {
		linenum++;
		if (linenum == 1) {
			string ss = c;
			while (ss[0] == '\t' || ss[0] == ' ') 
				ss.erase(ss.begin());
			str.push_back(ss);
		}else str.push_back(c);
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
string get_name(string s) {
	string ss ="";
	for (int k = 0;k < s.size() &&  s[k] != ' ';++k) ss += s[k];
	return ss;
}
int main(int agrc, char * agrv[]) {
	//FILE * fp2 = fopen("3.in", "r");
	RAMready();
	beginset();
	int j = init(agrv[1]);
	//fout <<l1 <<endl;
	dealdata();
	int i = 5 ,i0 ,t0,laststone = 0, mark;
	while (true) {
		--i;
		if (i < 0) i += 5;
		if (hazard != 0) {
			bool b = true;
			for (int ii = 0; ii <= 4; ++ii) if (status[ii] == 1) b = false;
			if (b == true) {
				i = 4;
				for (int ii = 0; ii <= 4; ++ii) {
					status[ii] = 0; s[ii] = ""; name[ii] = 0;
				}
				for (int ii = 0; ii <= 33; ++ii) lock[ii] = 0;
				if (hazard == 1) {
					if (laststone == 1) j++;
					laststone = 0;
				}
				else --j;
				while (label[str[j]] != 0)
					++j;
				hazard = 0;
			}
		}else {
			++j;

		}

		for (int k = 4; k >= 0; --k) {
			int t = (k + i) % 5;

			if (k == 0) { 
				if (status[t] != 0 || hazard != 0) continue;
				while (label[str[j]] != 0)
					++j;
				s[t] = str[j];
				name[t] = intmap[get_name(s[t])]; 
				status[t] = 1;
			}
			if (k == 1)	{
				if (status[t] <= 0) continue;
				if (hazard != 0) continue;
				switch(name[t]) {
					case 1: adddata[t].Data_Preparation(s[t]);break;
					case 2: addudata[t].Data_Preparation(s[t]);break;
					case 3: subdata[t].Data_Preparation(s[t]);break;
					case 4: subudata[t].Data_Preparation(s[t]);break;
					case 5: muldata[t].Data_Preparation(s[t]);break;
					case 6: muludata[t].Data_Preparation(s[t]);break;
					case 7: Divdata[t].Data_Preparation(s[t]);break;
					case 8: Divudata[t].Data_Preparation(s[t]);break;
					case 9: Xordata[t].Data_Preparation(s[t]);break;
					case 10: Xorudata[t].Data_Preparation(s[t]);break;
					case 11: negdata[t].Data_Preparation(s[t]);break;
					case 12: negudata[t].Data_Preparation(s[t]);break;
					case 13: remdata[t].Data_Preparation(s[t]);break;
					case 14: remudata[t].Data_Preparation(s[t]);break;
					case 15: lidata[t].Data_Preparation(s[t]);break;
					case 16: seqdata[t].Data_Preparation(s[t]);break;
					case 17: sgedata[t].Data_Preparation(s[t]);break;
					case 18: sgtdata[t].Data_Preparation(s[t]);break;
					case 19: sledata[t].Data_Preparation(s[t]);break;
					case 20: sltdata[t].Data_Preparation(s[t]);break;
					case 21: snedata[t].Data_Preparation(s[t]);break;
					case 22: bdata[t].Data_Preparation(s[t]);break;
					case 23: beqdata[t].Data_Preparation(s[t]);break;
					case 24: bnedata[t].Data_Preparation(s[t]);break;
					case 25: bgedata[t].Data_Preparation(s[t]);break;
					case 26: bledata[t].Data_Preparation(s[t]);break;
					case 27: bgtdata[t].Data_Preparation(s[t]);break;
					case 28: bltdata[t].Data_Preparation(s[t]);break;
					case 29: beqzdata[t].Data_Preparation(s[t]);break;
					case 30: bnezdata[t].Data_Preparation(s[t]);break;
					case 31: bgezdata[t].Data_Preparation(s[t]);break;
					case 32: blezdata[t].Data_Preparation(s[t]);break;
					case 33: bgtzdata[t].Data_Preparation(s[t]);break;
					case 34: bltzdata[t].Data_Preparation(s[t]);break;
					case 35: jdata[t].Data_Preparation(s[t]);break;
					case 36: jrdata[t].Data_Preparation(s[t]);break;
					case 37: jaldata[t].Data_Preparation(s[t], j - 1);break;
					case 38: ladata[t].Data_Preparation(s[t]);break;
					case 39: lbdata[t].Data_Preparation(s[t]);break;
					case 40: lwdata[t].Data_Preparation(s[t]);break;
					case 41: sbdata[t].Data_Preparation(s[t]);break;
					case 42: swdata[t].Data_Preparation(s[t]);break;
					case 43: Movedata[t].Data_Preparation(s[t]);break;
					case 44: mfhidata[t].Data_Preparation(s[t]);break;
					case 45: mflodata[t].Data_Preparation(s[t]);break;
					case 46: syscalldata[t].Data_Preparation();break;
				}
				
				if (hazard == 2) {
					status[t] = -1;
					status[(t + 4) % 5] = -1;
					status[(t + 3) % 5] = -1;
				}
				if (hazard == 1) {
					status[(t + 4) % 5] = -1;
					status[(t + 3) % 5] = -1;
				}
			}
			if (k == 2)	{
				if (status[t] <= 0) continue;
				switch(name[t]) {
					case 1: adddata[t].Execution();break;
					case 2: addudata[t].Execution();break;
					case 3: subdata[t].Execution();break;
					case 4: subudata[t].Execution();break;
					case 5: muldata[t].Execution();break;
					case 6: muludata[t].Execution();break;
					case 7: Divdata[t].Execution();break;
					case 8: Divudata[t].Execution();break;
					case 9: Xordata[t].Execution();break;
					case 10: Xorudata[t].Execution();break;
					case 11: negdata[t].Execution();break;
					case 12: negudata[t].Execution();break;
					case 13: remdata[t].Execution();break;
					case 14: remudata[t].Execution();break;
					case 15: lidata[t].Execution();break;
					case 16: seqdata[t].Execution();break;
					case 17: sgedata[t].Execution();break;
					case 18: sgtdata[t].Execution();break;
					case 19: sledata[t].Execution();break;
					case 20: sltdata[t].Execution();break;
					case 21: snedata[t].Execution();break;
					case 22: bdata[t].Execution();break;
					case 23: beqdata[t].Execution();break;
					case 24: bnedata[t].Execution();break;
					case 25: bgedata[t].Execution();break;
					case 26: bledata[t].Execution();break;
					case 27: bgtdata[t].Execution();break;
					case 28: bltdata[t].Execution();break;
					case 29: beqzdata[t].Execution();break;
					case 30: bnezdata[t].Execution();break;
					case 31: bgezdata[t].Execution();break;
					case 32: blezdata[t].Execution();break;
					case 33: bgtzdata[t].Execution();break;
					case 34: bltzdata[t].Execution();break;
					case 35: jdata[t].Execution();break;
					case 36: jrdata[t].Execution();break;
					case 37: jaldata[t].Execution();break;
					case 38: ladata[t].Execution();break;
					case 39: lbdata[t].Execution();break;
					case 40: lwdata[t].Execution();break;
					case 41: sbdata[t].Execution();break;
					case 42: swdata[t].Execution();break;
					case 43: Movedata[t].Execution();break;
					case 44: mfhidata[t].Execution();break;
					case 45: mflodata[t].Execution();break;
					case 46: syscalldata[t].Execution();break;
				}
			}
			if (k == 3)	{
				if (status[t] <= 0) continue;
				switch(name[t]) {
					case 39: lbdata[t].Memory_Access();break;
					case 40: lwdata[t].Memory_Access();break;
					case 41: sbdata[t].Memory_Access();break;
					case 42: swdata[t].Memory_Access();break;
					case 46: syscalldata[t].Memory_Access();break;
				}
			}
			if (k == 4)	{
				if (status[t] <= 0) continue;
				int tmp = 0;
				switch(name[t]) {
					case 1:{adddata[t].Write_Back(); status[t] = 0;break;}
					case 2:{addudata[t].Write_Back(); status[t] = 0;break;}
					case 3:{subdata[t].Write_Back(); status[t] = 0;break;}
					case 4:{subudata[t].Write_Back(); status[t] = 0;break;}
					case 5:{muldata[t].Write_Back(); status[t] = 0;break;}
					case 6:{muludata[t].Write_Back(); status[t] = 0;break;}
					case 7:{Divdata[t].Write_Back(); status[t] = 0;break;}
					case 8:{Divudata[t].Write_Back(); status[t] = 0;break;}
					case 9:{Xordata[t].Write_Back(); status[t] = 0;break;}
					case 10:{Xorudata[t].Write_Back(); status[t] = 0;break;}
					case 11:{negdata[t].Write_Back(); status[t] = 0;break;}
					case 12:{negudata[t].Write_Back(); status[t] = 0;break;}
					case 13:{remdata[t].Write_Back(); status[t] = 0;break;}
					case 14:{remudata[t].Write_Back(); status[t] = 0;break;}
					case 15:{lidata[t].Write_Back(); status[t] = 0;break;}
					case 16:{seqdata[t].Write_Back(); status[t] = 0;break;}
					case 17:{sgedata[t].Write_Back(); status[t] = 0;break;}
					case 18:{sgtdata[t].Write_Back(); status[t] = 0;break;}
					case 19:{sledata[t].Write_Back(); status[t] = 0;break;}
					case 20:{sltdata[t].Write_Back(); status[t] = 0;break;}
					case 21:{snedata[t].Write_Back(); status[t] = 0;break;}
					case 22:{tmp = bdata[t].Write_Back(); status[t] = 0;break;}
					case 23:{tmp = beqdata[t].Write_Back(); status[t] = 0;break;}
					case 24:{tmp = bnedata[t].Write_Back(); status[t] = 0;break;}
					case 25:{tmp = bgedata[t].Write_Back(); status[t] = 0;break;}
					case 26:{tmp = bledata[t].Write_Back(); status[t] = 0;break;}
					case 27:{tmp = bgtdata[t].Write_Back(); status[t] = 0;break;}
					case 28:{tmp = bltdata[t].Write_Back(); status[t] = 0;break;}
					case 29:{tmp = beqzdata[t].Write_Back(); status[t] = 0;break;}
					case 30:{tmp = bnezdata[t].Write_Back(); status[t] = 0;break;}
					case 31:{tmp = bgezdata[t].Write_Back(); status[t] = 0;break;}
					case 32:{tmp = blezdata[t].Write_Back(); status[t] = 0;break;}
					case 33:{tmp = bgtzdata[t].Write_Back(); status[t] = 0;break;}
					case 34:{tmp = bltzdata[t].Write_Back(); status[t] = 0;break;}
					case 35:{tmp = jdata[t].Write_Back(); status[t] = 0;break;}
					case 36:{tmp = jrdata[t].Write_Back(); status[t] = 0;break;}
					case 37:{tmp = jaldata[t].Write_Back(); status[t] = 0;break;}
					case 38:{ladata[t].Write_Back(); status[t] = 0;break;}
					case 39:{lbdata[t].Write_Back(); status[t] = 0;break;}
					case 40:{lwdata[t].Write_Back(); status[t] = 0;break;}
					case 41:{sbdata[t].Write_Back(); status[t] = 0;break;}
					case 42:{swdata[t].Write_Back(); status[t] = 0;break;}
					case 43:{Movedata[t].Write_Back(); status[t] = 0;break;}
					case 44:{mfhidata[t].Write_Back(); status[t] = 0;break;}
					case 45:{mflodata[t].Write_Back(); status[t] = 0;break;}
					case 46:{mark = syscalldata[t].Write_Back(); status[t] = 0;break;}
				}
				if (tmp != 0) {laststone = 1; j = tmp;}
				if (name[t] == 46 && mark != -1) return mark;
			}
		}
	}
	
}
