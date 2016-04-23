#include <iostream>
#include "libsheet.h"
using namespace std;

void test_colappend(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	vector<int> new_int_col{1, 2};
	vector<double> new_double_col{1., 2.};
	vector<string> new_str_col{"jefiwo", "dsf"};
	sh.col_append(new_int_col, "new");
	sh.col_append(new_double_col, "newd");
	
	sh.print();
	cout << "=================" << endl;
}

void test_get(Sheet sh){
	Sheet element1 = sh.get(1, "col3");
	Sheet element2 = sh.get(0, 0);
	
	vector<int> new_int_col{1, 2};
	Sheet cols1 = sh.get(0, new_int_col);
	
	vector<string> new_str_col{"col1", "col2"};
	Sheet cols2 = sh.get(1, new_str_col);
	
	vector<int> new_rows{0, 1};
	Sheet rows1 = sh.get(new_rows, 1);
	Sheet rows2 = sh.get(new_rows, "col1");
	
	Sheet rows_cols1 = sh.get(new_rows, new_int_col);
	Sheet rows_cols2 = sh.get(new_rows, new_str_col);
	
}

void test_set(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	sh.set(0, 0, 12345);
	sh.set(0, 1, 12345.6789);
	sh.set(0, 2, "We_love_C++");
	sh.set(1, "col1", 1);
	sh.set(1, "col2", 1.0);
	sh.set(1, "col3", "set_a_different_string");
	
	sh.print();
	cout << "=================" << endl;
}

int main(){
	Sheet sh;
	string s = "/Users/mcchu/Documents/16S-C++/sheet/sheet/test.txt";
	load_data(sh, s, true);
	
	// col_append example
	test_colappend(sh);
	
	// get example
	test_get(sh);

	// set example
	test_set(sh);
}
