//============================================================================
// Name        : ReportCounter.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

#define IN_FILE_NAME "ReportLog.txt"
#define OUT_FILE_NAME "ReportCount.txt"

using namespace std;

class Employee {
public:
	string m_name;
	int m_hours;
	int m_minutes;
	Employee(string empName) {
		m_name = empName;
		m_hours = 0;
		m_minutes = 0;
	}
};

void PushToDatabase(vector<Employee>& vec){
	ofstream outFile;
	auto timeNow = chrono::system_clock::to_time_t(chrono::system_clock::now());
	outFile.open(OUT_FILE_NAME);
	outFile << "---------------------------" << endl <<
			   "From: " << IN_FILE_NAME << endl <<
			   "On: " << ctime(&timeNow) << endl;
	for(auto employee : vec){
		if (employee.m_minutes < 60)
			outFile << employee.m_name << " " << employee.m_hours << ":" << employee.m_minutes << endl;
		else {
			employee.m_hours += (int)(employee.m_minutes / 60);
			employee.m_minutes %= 60;
			outFile << employee.m_name << " " << employee.m_hours << ":" << employee.m_minutes << endl;
		}
	}
	outFile.close();
}

int main() {
	vector<Employee> employeeList;
	ifstream readFile(IN_FILE_NAME);
	string line = "";
	while(getline(readFile, line)){
		if (line.find('@') != string::npos){
			string storedName = line.substr(line.find('@') + 1);
			while (storedName.back() == ' '){
				storedName.pop_back();
				storedName.shrink_to_fit();
			}
			string valueStr = line.substr(0, line.find('@'));
			while (valueStr.find(' ') != string::npos){
				valueStr.erase(valueStr.find(' '));
				valueStr.shrink_to_fit();
			}
			Employee swat(storedName);
			swat.m_hours = stoi(valueStr.substr(0, valueStr.find('h')));
			swat.m_minutes = stoi(valueStr.substr(valueStr.find('h') + 1));
			cout << swat.m_name << " " << swat.m_hours << " " << swat.m_minutes << endl;
			if (employeeList.size()) {
				bool found = false;
				for (auto swatIt = employeeList.begin(); swatIt < employeeList.end(); swatIt++){
					if (storedName == swatIt->m_name){
						swatIt->m_hours += swat.m_hours;
						swatIt->m_minutes += swat.m_minutes;
						found = true;
						break;
					}
				}
				if (!found){
					employeeList.push_back(swat);
				}
			}
			else{
				employeeList.push_back(swat);
			}
		}
	}
	PushToDatabase(employeeList);
}
