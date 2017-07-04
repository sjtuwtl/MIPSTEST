#include <iostream>
#include <cstdio>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <cmath>

using namespace std;

vector<string> str;
map<string, int> label;
int linenum;
struct datatype{
	int begin;
	int end;
	int dvalue;
	int dtype;
	string dstring;
};
datatype data[10000000];
int datanum = 0;
int timeclock = 1;

int init() {
	char c[100];
	FILE * fp1 = fopen("1.s", "r");
	FILE * fp2 = fopen("1.in", "r");
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
	fclose(fp2);
	//for (int k = 0; k < i; k++)
	//	cout <<str[k]<<"		";
	return label["main:"];
}

void dealdata() {
	for (int i = 0 ; i < linenum; i++)
		if (str[i] == ".data") {
			while (str[i] != ".text") {
				if (str[i].find(".word",0) != string::npos) {
					int position = str[i].find(".word",0) + 6;
					int t = 0;
					while (position < str[i].size()) {
						t = t * 10 + str[i][position] - 48;
						position++;
					}
					cout <<1 <<endl;

					datanum++;
					data[datanum].dtype = 1;
					data[datanum].dvalue = t;
					data[datanum].begin = timeclock;
					timeclock += 4;	
					data[datanum].end = timeclock - 1;
				}
				
				if (str[i].find(".half",0) != string::npos) {
					int position = str[i].find(".half",0) + 6;
					int t = 0;
					while (position < str[i].size()) {
						t = t * 10 + str[i][position] - 48;
						position++;
					}
					cout <<2 <<endl;

					datanum++;
					data[datanum].dtype = 2;
					data[datanum].dvalue = t;
					data[datanum].begin = timeclock;
					timeclock += 2;	
					data[datanum].end = timeclock - 1;
				}
				
				if (str[i].find(".byte",0) != string::npos) {
					int position = str[i].find(".byte",0) + 6;
					int t = 0;
					while (position < str[i].size()) {
						t = t * 10 + str[i][position] - 48;
						position++;
					}
					cout <<3 <<endl;

					datanum++;
					data[datanum].dtype = 3;
					data[datanum].dvalue = t;
					data[datanum].begin = timeclock;
					timeclock += 1;	
					data[datanum].end = timeclock - 1;
				}
				
				if (str[i].find(".asciiz",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t;
					while (str[i][position] != '\"') {
						t = t + str[i][position];
						position++;
					}
					cout <<5 <<endl;

					datanum++;
					data[datanum].dtype = 5;
					data[datanum].dstring = t;
					data[datanum].begin = timeclock;
					timeclock += position - str[i].find("\"",0);
					data[datanum].end = timeclock - 1;
				}
				else if (str[i].find(".ascii",0) != string::npos) {
					int position = str[i].find("\"",0) + 1;
					string t;
					while (str[i][position] != '\"') {
						t = t + str[i][position];
						position++;
					}
					cout <<4 <<endl;

					datanum++;
					data[datanum].dtype = 4;
					data[datanum].dstring = t;
					data[datanum].begin = timeclock;
					timeclock += position - str[i].find("\"",0) - 1;
					data[datanum].end = timeclock - 1;
				}
				
				if (str[i].find(".align",0) != string::npos) {
					int position = str[i].find(".align",0) + 7;
					int t = 0;
					while (position < str[i].size()) {
						t = t * 10 + str[i][position] - '0';
						position++;
					}
					cout <<6 <<endl;
					t = pow(2, t);
					int tmp = t;
					while (tmp < timeclock - 1) tmp += t;
					timeclock = tmp + 1;
				}
				
				i++;
			}
		} 
}
int main() {
	int Begin = init();
	//cout <<Begin<<endl;
	dealdata();
	for (int i = 1; i <= datanum; i++) {
		if (data[i].dtype >= 1 && data[i].dtype <= 3) {
			cout <<data[i].dvalue <<' '<<data[i].begin <<' ' <<data[i].end <<endl;
		}
		else if (data[i].dtype == 4 || data[i].dtype == 5) {
			cout <<data[i].dstring <<' '<<data[i].begin <<' ' <<data[i].end <<endl;
		}
	}
	return 0;
} 
