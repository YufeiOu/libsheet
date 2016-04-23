#include <iostream>
#include "libsheet.h"
using namespace std;

void test1(Sheet sh){
	vector<int> new_int_col{1, 2};
	vector<double> new_double_col{1., 2.};
	vector<string> new_str_col{"jefiwo", "dsf"};
	sh.col_append(new_int_col, "new");
	sh.col_append(new_double_col, "newd");
}

void test2(Sheet sh){
	Sheet element1 = sh.get(0, 0);
	Sheet element2 = sh.get(1, "col3");
	
}

int main (){

	
	
	Sheet sh;
	string s = "/Users/mcchu/Documents/16S-C++/sheet/sheet/test.txt";
	load_data(sh, s, true);
	
	// col_append example
	test1(sh);
	
	// get example
	test2(sh);
	
}