#include "libsheet.h"
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	Sheet sh;
	string file_path = "student.csv";
	load_data(sh, file_path); // now sh is the sheet we want!
	// easy print! -> sh.print(); 
	/*
	sh.sort_by_column("SCORE"); // from F to A
	sh.print();
	sh.sort_by_column("SCORE",true); // from A to F
	sh.print();
	*/

	// what if you want to select students whose SCORE > 80? 
	/*
	Sheet good_student = sh.filter(sh.dselect("SCORE",[](double score){return score > 80;}));
	good_student.print();
	*/

	// and only want to see ID, SCORE, and GRADE?
	/*
	vector<string> attribute = {"STUDENTID","GRADE","SCORE"};
	((sh.filter(sh.dselect("SCORE",[](double score){return score > 80;}))).get_col(attribute)).print();
	*/

	// what if we want SCORE>80 "and" GRADE==B? We have a precise way to express this, like SQL: 
	/*
	((sh.filter(
		sh.dselect("SCORE",[](double score){return score > 80;}) && 
		sh.sselect("GRADE",[](string grade){return grade == "B";}))
	)).get_col(attribute).print();
	*/

	// if user want to pick certain information as there data, we can easily handle this:
	vector<double> user_data ＝ get_dvec("SCORE");
	//sort(user_data);
	double m = user_data[0];
	cout << m << endl;


}