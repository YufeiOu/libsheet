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
	void row_erase(const vector<int> &rows);
	
	void column_erase(int col);
	void column_erase(const string &col);
	
	void column_erase(const vector<int>& cols);
	void column_erase(const vector<string>& cols);
	
	void row_append(Sheet& new_sheet);
	void row_append(vector<string> &new_row);
	
	void col_append(vector<int>& new_col, const string& col_name);
	void col_append(vector<double>& new_col, const string& col_name);
	void col_append(vector<string>& new_col, const string& col_name);
	
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

void row_shuffle(Sheet& sheet, vector<int>& ref);

void sort(Sheet& sheet, int col, bool descend = true);
void sort(Sheet& sheet, string col, bool descend = true);

//template <typename T, typename lambda_input>
//void apply(Sheet& sheet, T column_num, function<bool (lambda_input)> func)
// Get function
template <typename T>
Sheet Sheet::get(int row, T col) {
	int col_id;
  	// Decide the input argument type
    if (typeid(T) == typeid(string)) {
        auto got = column_map.find(string(col));
        if ( got == column_map.end() )
            throw "No such column";
        else 
            col_id = got->second;
    } else if (typeid(T) == typeid(int)) {
        col_id = int(col);
    } else throw "Illegal argument of col";
      
    string col_name{columns[col_id].column_name};
  	int type_flag{columns[col_id].flag};

  	ColumnHead ch = ColumnHead(col_name, type_flag);
  	
  	// Construct the content in ColumnHead based on the requested element type
  	switch (type_flag) {
		case 0:
			ch.vint.push_back(columns[col_id].vint[row]);
			break;
		case 1:
			ch.vdouble.push_back(columns[col_id].vdouble[row]);
			break;
		case 2:
      ch.vstring.push_back(columns[col_id].vstring[row]);
			break;
		default:
			throw "Unexpected type";
	}
  	
  	Sheet new_sheet;
  	new_sheet.columns.push_back(ch);
  	new_sheet.column_map.insert(pair<string, unsigned int>(col_name, 0));
  	return new_sheet;
}


