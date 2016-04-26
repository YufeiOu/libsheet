#include <iostream>
#include <functional>
#include "libsheet.h"
using namespace std;

void test_colappend(Sheet &sh){
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

void test_colerase(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	vector<int> int_cols{0, 2};
	vector<string> str_cols{"col1"};
	sh.col_erase(1);
	//sh.col_erase("col3");
	//sh.col_erase(int_cols);
	//sh.col_erase(str_cols);
	
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

void test_getRow(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	vector<int> new_rows{0, 2};
	Sheet rows = sh.get_row(new_rows);
	
	rows.print();
	cout << "=================" << endl;
	
}

void test_getCol(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	vector<int> new_cols{1, 2};
	Sheet cols = sh.get_column(new_cols);
	
	vector<string> new_cols_str{"col1", "col3"};
	Sheet cols2 = sh.get_column(new_cols_str);
	
	cols.print();
	cols2.print();
	cout << "=================" << endl;
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

void test_rowerase(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	vector<int> rows{0, 3};
	sh.row_erase(rows);
	
	sh.print();
	cout << "=================" << endl;
}

void test_reorder(){
	
	vector<int> indices{0, 2, 1, 4, 3};
	vector<int> vec1{1, 2, 3, 4, 5};
	cout << vec1[0] << vec1[1] << vec1[2] << vec1[3] << vec1[4] << endl;
	reorder(indices, vec1);
	cout << vec1[0] << vec1[1] << vec1[2] << vec1[3] << vec1[4] << endl;
}

void test_sort(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	sh.sort_by_column("col1", true);
	
	sh.print();
	cout << "=================" << endl;
}

bool largerThanOne(int n) {
	return (n > 1);
}

bool smallerThanThree(int n) {
	return (n < 3);
}

bool longerThanTwo(string str) {
	return (str.size() > 2);
}

void test_mask(Sheet sh){
	sh.print();
	cout << "-----------------" << endl;
	
	// vector<bool> mask1 = sh.iselect(0, largerThanTwo);
	Sheet f1 = sh.filter(sh.iselect(0, largerThanOne));
	Sheet f2 = sh.filter(sh.dselect(1, largerThanOne));
	Sheet f3 = sh.filter(sh.sselect(2, longerThanTwo));

	
	Sheet f_str1 = sh.filter(sh.iselect("col1", largerThanOne)&&sh.iselect(0, smallerThanThree));
	Sheet f_str2 = sh.filter(sh.dselect("col2", largerThanOne)||sh.dselect(1, [](double d){return (d == 0);}));
	Sheet f_str3 = sh.filter(!sh.sselect("col3", longerThanTwo));

	f_str1.print();
	cout << "-----------------" << endl;
	f_str2.print();
	cout << "-----------------" << endl;
	f_str3.print();
	cout << "=================" << endl;
}

void addOne(int& i){
	i += 1;
}

void test_apply(Sheet  sh){
	sh.print();
	cout << "-----------------" << endl;
	
	// vector<bool> mask1 = sh.iselect(0, largerThanTwo);
	sh.iapply(0, addOne);
	sh.dapply(1, [](double& i){i += 1;});
	sh.sapply(2, [](string& str){str = str.substr(1);});
	
	sh.print();
	cout << "=================" << endl;
}

int main(){
	Sheet sh;
	string s = "test.txt";//"/Users/mcchu/Documents/16S-C++/libsheet/test.txt";
	load_data(sh, s, true);
	
	// col_append example
	//test_colappend(sh);
	
	// col_erase example
	//test_colerase(sh);
	
	// get example
	//test_get(sh);

	// set example
	//test_set(sh);
	
	// reorder
	//test_reorder();
	
	// row_erase
	//test_rowerase(sh);
	
	// sort
	//test_sort(sh);
	
	// get row/ column
	//test_getRow(sh);
	//test_getCol(sh);
	// get_mask
	test_mask(sh);
	
	// apply
	test_apply(sh);
	
}
