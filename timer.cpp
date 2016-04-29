#include "libsheet.h"
#include <chrono>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[])
{
	Sheet sh;
	auto t0 = std::chrono::high_resolution_clock::now();
	load_data(sh,"hotel_recommendation.csv");
	auto t1 = std::chrono::high_resolution_clock::now();
	cout << "Data loading time: " << (t1-t0).count()/1000000 << " ms" << endl; 
	//vector<int> v{0};
	//(sh.get_row(v)).print();
	auto t6 = std::chrono::high_resolution_clock::now();
	sh.print("result.txt");
	auto t7 = std::chrono::high_resolution_clock::now();
	cout << "Data dump time: " << (t7-t6).count()/1000000 << " ms" << endl;

	vector<int> column{3,4,5,6,7};
	vector<int> row;
	for(int i=0;i<100000;i++) row.push_back(i);
	auto t8 = std::chrono::high_resolution_clock::now();
	sh.get(row,column);
	auto t9 = std::chrono::high_resolution_clock::now();
	cout << "Data get time: " << (t9-t8).count()/1000000 << " ms" << endl;

	auto t2 = std::chrono::high_resolution_clock::now();
	sh.sort_by_column("orig_destination_distance");
	auto t3 = std::chrono::high_resolution_clock::now();
	cout << "Data sorting time: " << (t3-t2).count()/1000000 << " ms" << endl; 

	auto t4 = std::chrono::high_resolution_clock::now();
	sh.filter(sh.iselect("site_name",[](int site_name){return site_name < 10;}));
	auto t5 = std::chrono::high_resolution_clock::now();
	cout << "Data filtering time: " << (t5-t4).count()/1000000 << " ms" << endl;

	Sheet sh1;
	load_data(sh1,"hotel_recommendation.csv");
	auto t10 = std::chrono::high_resolution_clock::now();
	sh1.sapply(1,[](string& s){reverse(s.begin(),s.end());});
	auto t11 = std::chrono::high_resolution_clock::now();
	cout << "Data apply time: " << (t5-t4).count()/1000000 << " ms" << endl;

	
	
	
	return 0;
}