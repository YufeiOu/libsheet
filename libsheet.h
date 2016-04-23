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
	
	template<typename Oper, size_t N, typename T>
	bitset<N> filter(T column_index, Oper p) {
		// Column index can be int or string(for column name)
		// give column number, filter lambda expression, return a bitmap
	}
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

//template <typename T, typename lambda_input>
//void apply(Sheet& sheet, T column_num, function<bool (lambda_input)> func)
