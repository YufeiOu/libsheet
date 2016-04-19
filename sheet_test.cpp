#include "libsheet.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

int main() {
	vector<string> s_v{"shit", "1", "0.1"};
	vector<string> cols{"c1", "c2", "c3"};
	Sheet s(s_v, cols);
	
	cout << s.columns[0].vstring[0] << s.columns[0].column_name << endl;
	cout << s.columns[1].vint[0] << s.columns[1].column_name << endl;
	cout << s.columns[2].vdouble[0] << s.columns[2].column_name << endl;
	
	return 0;
}