#include "json.hpp"

#include <iostream>
#include <fstream>
#include <iomanip>

// for convenience
using json = nlohmann::json;

const std::string directory = "file.json";

struct Class
{
	std::string name = "";
	float percent = 100;
	int credits = 1;
	double cost = 0.0;
};

#define GRADE_NAME "name"
#define GRADE_COST "cost"
#define GRADE_PERCENT "percent"
#define GRADE_CREDITS "credits"

// write JSON to file
void WriteToJson(std::vector<Class>& grades, std::string fileName = "file.json")
{
	std::ofstream o(fileName);

	for (auto& grade : grades)
	{
		json j;

		j[GRADE_NAME] = grade.name;
		j[GRADE_COST] = grade.cost;
		j[GRADE_PERCENT] = grade.percent;
		j[GRADE_CREDITS] = grade.credits;

		//std::cout << j << std::endl;
		o << j << std::endl;
	}

	o.close();
}

// read a JSON file
void LoadJsonData(std::vector<Class>& grades, std::string fileName = "file.json")
{
	std::ifstream fs;
	fs.open(fileName);

	if (fs.is_open() == false)
	{
		std::cout << "No grades file found." << std::endl;
		return;
	}

	std::cout << "Found grades file. ";

	while (fs.eof() == false)
	{
		std::string content;
		Class item;

		std::getline(fs, content);

		if (content.empty() == false)
		{
			auto j = json::parse(content);

			item.percent = j.at(GRADE_PERCENT);
			item.credits = j.at(GRADE_CREDITS);

			/*
			* Use a try loop to access these since they are new additions
			* Maintaining a try loop ensures that we can still support the old format
			* of just grade percent and credits.
			*/
			try
			{
				item.name = j.at(GRADE_NAME);
				item.cost = j.at(GRADE_COST);
			}
			catch (const std::exception& e)
			{
				// Do nothing if exception is thrown
			}

			grades.push_back(item);
		}

		content.clear();
	}

	fs.close();

	std::cout << "Grades loaded." << std::endl;
}

void ClearExistingGrades(std::vector<Class>& grades, std::string fileName = "file.json")
{
	while (true)
	{
		std::string in;

		system("cls");
		std::cout << "Are you sure you want to clear all grades? This cannot be undone." <<
			"\n(y/n)" << std::endl;

		std::cin >> in; // Recieve input

		if (in[0] == 'y')
		{
			// Delete the file itself
			remove(fileName.c_str());

			// Clear out the existing items
			grades.clear();

			std::cout << "Cleared." << std::endl;
			break;
		}
		else if (in[0] == 'n')
		{
			std::cout << "Canceled." << std::endl;
			break;
		}
	}
}

void DisplayExistingGrades(const std::vector<Class>& grades)
{
	if (grades.size() > 0)
	{
		std::cout << "Grades:" << std::endl;
		std::cout << "\t [GRADE] : [CREDITS]\t[NAME]\t\t[COST]" << std::endl;
		for (const auto& g : grades)
		{
			std::string space = g.percent < 100 ? "\t   " : "\t  ";
			std::cout << std::setprecision(2) << std::fixed << space
				<< g.percent << " : " << g.credits;

			if (g.name != "")
			{
				std::string tabs = "\t\t";
				if (g.name.length() > 7)
				{
					tabs = "\t";
				}
				std::cout << "\t\t" << g.name << tabs << "(" << g.cost << ") ";
			}

			std::cout << std::endl;
		}

		std::cout << std::endl << std::endl;
	}
}

void CalculateGrade(const std::vector<Class>& grades)
{
	if (grades.size() == 0)
	{
		std::cout << "No grades exist. Please input your grades." << std::endl;
		return;
	}

	// Clear screen
	std::system("cls");

	int count = static_cast<int>(grades.size());
	float cgpa = 0;
	float weightedGpa = 0;
	int totalCredits = 0;
	double totalCost = 0.0;

	DisplayExistingGrades(grades);

	for (const auto& g : grades)
	{
		cgpa += g.percent;

		totalCredits += g.credits;
		float fourPoint = (g.percent / 100) * g.credits;

		totalCost += g.cost;

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
	std::cout << "Simple 4 GPA = " << simpleGpa << std::endl;

	std::cout << "\nTotal credits: " << totalCredits << std::endl;
	std::cout << "Total cost: (" << totalCost << ")" << std::endl;
}

void InputGrades(std::vector<Class>& grades)
{
	// Clear screen
	std::system("cls");

	int globalCredits = 0;
	double globalCosts = 0.0;
	std::string in;

	DisplayExistingGrades(grades);

	std::cout << "Are credits universal? Input number of credits or 'n' for no." << std::endl;
	std::cin >> in;

	if (in[0] != 'n' && in[0] != 'N')
	{
		// FIXME: Add validation before recieving credits

		globalCredits = std::stoi(in);
	}
	else
	{
		globalCredits = -1;
	}

	in.clear();
	std::cout << "Are costs universal? Input costs or 'n' for no." << std::endl;
	std::cin >> in;

	if (in[0] != 'n' && in[0] != 'N')
	{
		// FIXME: Add validation before recieving credits

		globalCosts = std::stod(in);
	}
	else
	{
		globalCosts = -1;
	}

	while (true)
	{
		std::system("cls");

		DisplayExistingGrades(grades);

		Class g;
		in = "";

		std::cout << "Please input grade, or type 'done' to finish." << std::endl;
		std::cin >> in;

		if (in == "done" || in == "DONE")
		{
			break;
		}

		// Recieve grade

		// FIXME: Add validation before recieving grade percent

		g.percent = std::stof(in);

		// Recieve class name
		in.clear();
		std::cout << "\nPlease input class name: ";
		std::cin >> in;

		g.name = in;

		// Recieve costs paid
		if (globalCosts < 0)
		{
			in.clear();
			std::cout << "\nPlease input class costs: ";
			std::cin >> in;

			// FIXME: Add validation before recieving costs

			g.cost = std::stod(in);
		}

		// Recieve credits
		if (globalCredits <= 0)
		{
			in.clear();
			std::cout << "\nPlease input credits: ";
			std::cin >> in;

			// FIXME: Add validation before recieving credits

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

	system("Title Grade Calculator v2 - Forrest H. Lowe 2021");

	std::vector<Class> grades;
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

			CalculateGrade(grades);
			break;
		case 2:
			CalculateGrade(grades);
			break;
		case 3:
			ClearExistingGrades(grades);
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