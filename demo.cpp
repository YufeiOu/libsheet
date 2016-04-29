#include "libsheet.h"
#include <vector>
#include <algorithm>
using namespace std;

int main() {
	Sheet sh;
	string file_path = "student.csv";
	load_data(sh, file_path); // now sh is the sheet we want!
	// easy print! -> sh.print(); 
	// even better! we could print to any file in the disk!
	/*
	sh.sort_by_column("SCORE"); // from F to A
	sh.print();
	sh.sort_by_column("SCORE",true); // from A to F
	sh.print();
	sh.print("test_output.txt");
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
	vector<string> attribute = {"STUDENTID","GRADE","SCORE"};
	((sh.filter(
		sh.dselect("SCORE",[](double score){return score > 80;}) && 
		sh.sselect("GRADE",[](string grade){return grade == "B";}))
	)).get_col(attribute).print();
	*/

	// Now the teacher wants to enhance 5 points to everyone's score:
	/*
	sh.dapply("SCORE",[](double& score){ score=min(100.0,score+5);});
	sh.print();
	*/

	// if user want to pick certain information as there data, we can easily handle this:
	/*
	auto user_data = sh.get_dvec("SCORE");
	sort(user_data.begin(),user_data.end()); // now we are in non-libsheet C++ world!
	for(auto scores:user_data) cout << scores << " ";
	*/

	

}