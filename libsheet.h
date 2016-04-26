// libsheet.h
#include <iostream>
#include <functional>
#include <unordered_map>
#include <string>
#include <vector>
#include <typeinfo>
#include <bitset>
using namespace std;

class Sheet {
public:
	Sheet() {};
	Sheet(vector<string>& entry, vector<string>& col_names);
	// Sheet(Sheet const &);
	
	// get mask, used for filter on many coditions
	template <typename Function>
	vector<bool> iselect(int col, Function fn);
	template <typename Function>
	vector<bool> iselect(string col, Function fn);
	
	template <typename Function>
	vector<bool> dselect(int col, Function fn);
	template <typename Function>
	vector<bool> dselect(string col, Function fn);
	
	template <typename Function>
	vector<bool> sselect(int col, Function fn);
	template <typename Function>
	vector<bool> sselect(string col, Function fn);

	template <typename Function>
	void iapply(int col, Function fn);
  
        template <typename Function>
	void dapply(int col, Function fn);
  
        template <typename Function>
	void sapply(int col, Function fn);
	
        template <typename Function>
	void iapply(string col, Function fn);
  
        template <typename Function>
	void dapply(string col, Function fn);
  
        template <typename Function>
	void sapply(string col, Function fn);
	
	// apply functions
	template <typename Function>
	void iapply(int col, Function fn);
	template <typename Function>
	void dapply(int col, Function fn);
	template <typename Function>
	void sapply(int col, Function fn);
	template <typename Function>
	void iapply(string col, Function fn);
	template <typename Function>
	void dapply(string col, Function fn);
	template <typename Function>
	void sapply(string col, Function fn);
	
	// get sheet by mask
	Sheet filter(const vector<bool>& vb);
	
  void print(bool header = true);
	
	void set(const int &y, const int& x, const int &value);
  void set(const int &y, const int& x, const double &value);
  void set(const int &y, const int& x, const string &value);
  void set(const int &y, const string& x, const int &value);
  void set(const int &y, const string& x, const double &value);
  void set(const int &y, const string& x, const string &value);
	
	Sheet get(const int& row, const vector<string>& cols);
	Sheet get(const int& row, const vector<int>& cols);
	Sheet get(const int& row, const string& col);
	Sheet get(const int& row, const int& col);
	Sheet get(const vector<int>& rows, const string& cols);
	Sheet get(const vector<int>& rows, const int& cols);
	Sheet get(const vector<int>& rows, const vector<string>& cols);
	Sheet get(const vector<int>& rows, const vector<int>& cols);
	Sheet get_row(const int& row);
	Sheet get_column(const int& col);
	Sheet get_row(const vector<int>& rows);
	Sheet get_column(const vector<int>& cols);
    Sheet get_column(const vector<string>& cols);
	
	void row_erase(int row);
	void row_erase(vector<int> &rows);
  
	void col_erase(int col);
    void col_erase(const string &col);

	void col_erase(const vector<int>& cols);
    void col_erase(const vector<string>& cols);
	
	void row_append(Sheet& new_sheet);
	void row_append(vector<string> &new_row);
	
	void col_append(vector<int>& new_col, const string& col_name);
	void col_append(vector<double>& new_col, const string& col_name);
	void col_append(vector<string>& new_col, const string& col_name);
	
	void sort_by_column(int col, bool descend = false);
	void sort_by_column(const string& col, bool descend = false);
	
private:
	unordered_map<string, unsigned int>  column_map;  // column name to index
	
	class ColumnHead {
		friend class Sheet;
	public:
		ColumnHead(string _column_name, int _flag) : column_name(_column_name), flag(_flag) {}
		// 		template<typename T>
		// 		void push_back(T input);
		
	private:
		std::string column_name;
		int flag; // int:0 double:1 string:2
		
		vector<int> vint;
		vector<double> vdouble;
		vector<std::string> vstring;
		/* other possible field */
		
	};
	vector<ColumnHead> columns;
	
};

//vector<int> bit2int(bitset& filter_result);

void load_data(Sheet& sheet, const string& path, bool header = true);
void dump_data(Sheet& sheet, const string& file_path, bool header = true);

template <typename T>
void reorder(const vector<int>& indices, vector<T>& vec) {
	vector<T> new_vec;
	for (int i = 0; i < indices.size(); ++i) {
		new_vec.push_back(vec.at(indices.at(i)));
	}
	vec = new_vec;
};

// get mask, used for filter on many coditions
// Get mask function
template <typename Function>
vector<bool> Sheet::iselect(int col, Function fn){
	vector<bool> result;
	ColumnHead& ch = columns.at(col);
	for (auto& r : ch.vint) result.push_back(fn(r));
	return result;
}

// Get mask function
template <typename Function>
vector<bool> Sheet::dselect(int col, Function fn){
	vector<bool> result;
	ColumnHead& ch = columns.at(col);
	for (auto& r : ch.vdouble) result.push_back(fn(r));
	return result;
}

// Get mask function
template <typename Function>
vector<bool> Sheet::sselect(int col, Function fn){
	vector<bool> result;
	ColumnHead& ch = columns.at(col);
	for (auto& r : ch.vstring) result.push_back(fn(r));
	return result;
}

template <typename Function>
vector<bool> Sheet::iselect(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	vector<bool> result;
	ColumnHead& ch = columns.at(col_id);
	for (auto& r : ch.vint) result.push_back(fn(r));
	return result;
}

// Get mask function
template <typename Function>
vector<bool> Sheet::dselect(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	vector<bool> result;
	ColumnHead& ch = columns.at(col_id);
	for (auto& r : ch.vdouble) result.push_back(fn(r));
	return result;
}

// Get mask function
template <typename Function>
vector<bool> Sheet::sselect(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
	
	vector<bool> result;
	ColumnHead& ch = columns.at(col_id);
	for (auto& r : ch.vstring) result.push_back(fn(r));
	return result;
}

template <typename Function>
void Sheet::iapply(int col, Function fn){
	for (auto& c : columns.at(col).vint) fn(c);
}

template <typename Function>
void Sheet::dapply(int col, Function fn){
	for (auto& c : columns.at(col).vdouble) fn(c);
}

template <typename Function>
void Sheet::sapply(int col, Function fn){
	for (auto& c : columns.at(col).vstring) fn(c);
}

template <typename Function>
void Sheet::iapply(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
  
	
	for (auto& c : columns.at(col_id).vint) fn(c);
}

template <typename Function>
void Sheet::dapply(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
  
	
	for (auto& c : columns.at(col_id).vdouble) fn(c);
}

template <typename Function>
void Sheet::sapply(string col, Function fn){
	int col_id;
	auto got = column_map.find(col);
	if (got == column_map.end())
		throw "No such column";
	col_id = got->second;
  
	
	for (auto& c : columns.at(col_id).vstring) fn(c);
}

vector<bool> operator&&(const vector<bool>& mask1, const vector<bool>& mask2);
vector<bool> operator||(const vector<bool>& mask1, const vector<bool>& mask2);
vector<bool> operator!(const vector<bool>& mask1);

//template <typename T, typename lambda_input>
//void apply(Sheet& sheet, T column_num, function<bool (lambda_input)> func)


