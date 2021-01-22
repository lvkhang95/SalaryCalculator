//============================================================================
// Name        : ReportCounter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <ctime>

#define IN_FILE_NAME "ReportLog.txt"
#define OUT_FILE_NAME "ReportCount.txt"

using namespace std;


template <typename T>
void remove_duplicates(vector<T>& vec)
{
  sort(vec.begin(), vec.end());
  vec.erase(unique(vec.begin(), vec.end()), vec.end());
  vec.shrink_to_fit();
}

void Toggle(bool& state){
	state = (!state);
}

class Employee {
public:
	string name;
	int revenue;
	int cheatedCount;
	Employee(string& empName) {
		this->name = empName;
		this->revenue = 1;
		this->cheatedCount = 0;
	}
	Employee(){
		this->name = "";
		this->revenue = 0;
		this->cheatedCount = 0;
	}
	void setReport(int amount){
		this->revenue += amount;
	}
};

void PushToDatabase(vector<Employee>& vec){
	ofstream outFile;
	auto timeNow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	outFile.open(OUT_FILE_NAME, std::ios_base::app);
	outFile << "---------------------------" << endl <<
			   "From: " << IN_FILE_NAME << endl <<
			   "On: " << ctime(&timeNow) << endl;
	outFile.close();
	for(auto employee : vec){
		outFile.open(OUT_FILE_NAME, std::ios_base::app);//append instead of overwrite
		outFile << employee.name << " " << employee.revenue << endl;
		outFile.close();
	}
}

int main() {
	Employee newEmployee;
	vector<Employee> employeeList{newEmployee};
	ifstream readFile(IN_FILE_NAME);
	string line = "";
	while(getline(readFile, line)){
		line.push_back('@');
		bool isRecording = false;
		vector<string> nameList;
		string name = "";
		for(char toBeCheckedInLine : line){
			if(toBeCheckedInLine == '@'){
				isRecording = true;
				if(name.size() != 0){
					nameList.push_back(name);
					name.clear();
				}
			}
			else if(isRecording && toBeCheckedInLine!=' ')
					name.push_back(toBeCheckedInLine);
		}
		remove_duplicates(nameList);
		for(auto name : nameList){
			int index = 0;
			bool isStored = false;
			for(auto anEmp : employeeList){
				if(anEmp.name == name){
					isStored = true;
					break;
				}
				index++;
			}
			if(isStored){
				employeeList[index].setReport(1);
			}
			else{
				Employee employee{name};
				employeeList.push_back(employee);
			}
		}
	}
	employeeList.erase(employeeList.begin());
	employeeList.shrink_to_fit();
	PushToDatabase(employeeList);
}
