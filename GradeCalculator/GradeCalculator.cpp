#include "json.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

// for convenience
using json = nlohmann::json;

const std::string directory = "file.json";

struct Grade
{
	float percent = 100;
	int credits = 1;
};

// write JSON to file
void WriteToJson(std::vector<Grade>& grades, std::string fileName = "file.json")
{
	std::ofstream o(fileName);

	for (auto& grade : grades)
	{
		json j;

		j["credits"] = grade.credits;
		j["percent"] = grade.percent;

		std::cout << j << std::endl;
		o << j << std::endl;
	}

	o.close();
}

// read a JSON file
void LoadJsonData(std::vector<Grade>& grades, std::string fileName = "file.json")
{
	std::ifstream fs;
	fs.open(fileName);

	if (fs.is_open() == false) return;

	while (fs.eof() == false)
	{
		std::string content;
		Grade item;

		std::getline(fs, content);

		if (content.empty() == false)
		{
			auto j = json::parse(content);

			item.percent = j.at("percent");
			item.credits = j.at("credits");

			grades.push_back(item);
		}

		content.clear();
	}

	fs.close();

	std::cout << "Found grades file, and grades have been loaded." << std::endl;
}

// REDUNDANT, but cool
Grade ReadGrade(json j)
{
	return {
		j["grades"].get<float>(),
		j["credits"].get<int>()
	};
}

void ClearExistingGrades(std::string fileName = "file.json")
{
	remove(fileName.c_str());
}

void DisplayExistingGrades(const std::vector<Grade>& grades)
{
	if (grades.size() > 0)
	{
		std::cout << "Grades:" << std::endl;
		std::cout << "\t[GRADE] : [CREDITS]" << std::endl;
		for (const auto& g : grades)
		{
			std::string space = g.percent < 100 ? "\t   " : "\t  ";
			std::cout << std::setprecision(2) << std::fixed << space << g.percent << " : " << g.credits << "" << std::endl;
		}

		std::cout << std::endl << std::endl;
	}
}

void CalculateGrade(const std::vector<Grade>& grades)
{
	if (grades.size() == 0)
	{
		std::cout << "No grades exist. Please input your grades." << std::endl;
		return;
	}

	// Clear screen
	std::system("cls");

	int count = grades.size();
	float cgpa = 0;
	float weightedGpa = 0;
	int totalCredits = 0;

	DisplayExistingGrades(grades);

	for (const auto& g : grades)
	{
		cgpa += g.percent;

		totalCredits += g.credits;
		float fourPoint = (g.percent / 100) * g.credits;

		weightedGpa += fourPoint;
	}

	// Formula for % cgpa = grades/total grade count

	// Simple 4 gpa = A=4, B=3, C=2, D=1, F=0

	// To convert to 4.0 do (grades * credits)/credits
	// ex. (90/100)*4 = 3.6.

	// For weighted grades
	// Foreach grade multiply by the credits for that grade, then add that to the total.
	// Divide that total by the total credits, this is your weighted 4.0 gpa.

	// Calculate all values
	cgpa /= count;
	weightedGpa /= totalCredits;
	weightedGpa *= 4;

	int simpleGpa = 0;

	if (cgpa >= 60.0f)
	{
		if (cgpa >= 70.0f)
		{
			if (cgpa >= 80.0f)
			{
				if (cgpa >= 90.0f)
				{
					simpleGpa = 4;
				}
				else simpleGpa = 3;
			}
			else simpleGpa = 2;
		}
		else simpleGpa = 1;
	}

	// Display all grades to the user in a nice format
	std::cout << "Your CGPA = " << cgpa << std::endl;
	std::cout << "Your 4.0 GPA = " << weightedGpa << std::endl;
	std::cout << "Your Simple GPA = " << simpleGpa << std::flush;
}

void InputGrades(std::vector<Grade>& grades)
{
	// Clear screen
	std::system("cls");

	int globalCredits = 0;
	std::string in;

	DisplayExistingGrades(grades);

	if (globalCredits == 0)
	{
		std::cout << "Are credits universal? Type number of credits or 'n' for no." << std::endl;
		std::cin >> in;

		if (in[0] != 'n' && in[0] != 'N')
		{
			globalCredits = std::stoi(in);
		}
		else
		{
			globalCredits = -1;
		}
	}

	while (true)
	{
		//std::system("cls");

		Grade g;
		in = "";

		std::cout << "Please input grade, or type 'done' to finish." << std::endl;
		std::cin >> in;

		if (in == "done" || in == "DONE") {
			break;
		}

		g.percent = std::stof(in);

		in = "";

		if (globalCredits <= 0)
		{
			std::cout << "Please input credits" << std::endl;
			std::cin >> in;

			g.credits = std::stoi(in);
		}
		else
		{
			g.credits = globalCredits;
		}

		grades.push_back(g);
		in.clear();
	}

	// Save the grades
	WriteToJson(grades);
}

int main()
{
	// Load users grades, and add them to a list
	// If grades exist, then
	// validate that these grades are current, then
	// ask for users grades, and credits for each grade (default 1)

	std::vector<Grade> grades;
	LoadJsonData(grades);

	bool applicationRunning = true;

	while (applicationRunning)
	{
		std::cout << "1. Input grades" << std::endl;
		std::cout << "2. Display grades" << std::endl;
		std::cout << "3. Clear existing grades" << std::endl;
		std::cout << "4. Exit application" << std::endl;
		int input = 0;
		std::cin >> input;

		switch (input)
		{
		case 1:
			InputGrades(grades);
			break;
		case 2:
			CalculateGrade(grades);
			break;
		case 3:
			ClearExistingGrades();
			grades.clear();

			std::cout << "Cleared." << std::endl;
			break;
		case 4:
			applicationRunning = false;
			break;
		default:
			std::cout << "Unable to parse input! Please make sure your value is between 1-4 exclusively!" << std::endl;
			break;
		}

		std::cout << std::endl;
		system("pause");
		system("cls");
	}

	return 0;
}