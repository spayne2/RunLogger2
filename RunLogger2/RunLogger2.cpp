#include <iostream>
#include <string>
#include <fstream>
using namespace std;
//2nd version of Runlogger using binary file storage, with fixed size data structures
//run class to represent a run object.
const int TERRAIN_LENGTH = 30;
const int AMOUNT_OF_RUNS = 5;
const int NUMBER_OF_PERSONS = 5;
int number_of_people_stored = 0;
const int NAME_LENGTH = 40;
class Run
{
private:
	double distanceInKm; //overall distnace of the run
	int timeMins; // time in minutes 
	int timeSeconds; //time in seconds
	char terrain[TERRAIN_LENGTH]; //the terrain type of the run e.g. road, track, trail //v2 changed from string to c-string

public:
	Run()
	{
		distanceInKm = 0.0;
		timeMins = 0;
		timeSeconds = 0;
		strcpy_s(terrain, "");
	}
	//function to ask user to add data to run, called from wherever a new run object is created
	void addData()
	{ //get data from user input
		cout << "Type in distance" << endl;
		cin >> distanceInKm;
		cout << "Type in minutes" << endl;
		cin >> timeMins;
		cout << "Type in seconds" << endl;
		cin >> timeSeconds;
		cout << "Type in terrain type" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		cin.getline(terrain, TERRAIN_LENGTH);
	}
	//function to caclculate the average pace of the run, to be called when the runs are displayed
	string calculateAvgPace()
	{
		int fullSeconds = (timeMins * 60) + timeSeconds;

		int secondsPerKm = fullSeconds / distanceInKm;

		double minutesPerKm = secondsPerKm / 60;
		int leftOverSeconds = secondsPerKm % 60;

		return to_string(minutesPerKm) + "'" + to_string(leftOverSeconds) + "\"";
	}
	//function to display the run information
	void display()
	{
		if (distanceInKm <= 0)
		{
			cout << "No Data " << endl;
		}
		else
		{
			cout << "Disance: " << distanceInKm << "Kms, Time " << timeMins << "mm:" << timeSeconds << "ss, Pace: " << calculateAvgPace() << " Terain: " << terrain << endl;
		}		
	}
};

//class to represent a person object
class Person
{
private:

	char name[NAME_LENGTH]; //store the name
	int age; //store the age
	Run runs[AMOUNT_OF_RUNS]; //store a list of runs; //composition
public:
	Person()
	{
		strcpy_s(name, "");
		age = 0;
		Run run;
		runs[0] = run;
		runs[1] = run;
		runs[2] = run;
		runs[3] = run;
		runs[4] = run;
	}
	void addRun() //function to add run for a user
	{
		cout << "Add info of most recent run" << endl;
		Run run; //initiliase
		run.addData(); //add data
		//loop backwards through the array of previous weights shifting by one drop the oldest weight
		for (int i = AMOUNT_OF_RUNS - 1; i >= 0; i--)
		{
			if(i!=0)
			{
				runs[i] = runs[i - 1]; //"Knocks back" each of the previous weights
			}
		}
		runs[0] = run; // sets the new current weight
	}

	void addData() //function to add data from the user to the person object
	{
		cout << "Type in persons name" << endl;
		cin.ignore(1000, '\n');
		cin.clear();
		cin.getline(name, NAME_LENGTH);
		cout << "Type in persons age" << endl;
		cin >> age;
		addRun();
	}
	//function to return the name of the person
	string getName()
	{
		return name;
	}
	//function to return the age of the person
	int getAge()
	{
		return age;
	}
	//function to return the runs that the person has stored
	Run* getRuns()
	{
		return runs;
	}
	//function to print the runs to the console;
	void printRuns()
	{
		for (int i = 0; i < AMOUNT_OF_RUNS; i++)
		{
			cout << "Run " << (i + 1) << endl;
			runs[i].display();
		}
	}
	//display function to show all user infor
	void display()
	{
		cout << "Name: " << name << " Age: " << age << endl;
		cout << "Runs: " << endl;
		printRuns();
	}
	//edit function to edit the users info
	void edit()
	{
		cout << "Type in new name" << endl;
		cin.ignore(1000, '\n'); //clears the input buffer before using getline
		cin.clear();
		cin.getline(name, NAME_LENGTH);

		cout << "Type in new age" << endl;
		cin >> age;
	}
		
};

//Menu class, controls the program running, holds a vector of persons
class Menu
{
private:
	Person persons[NUMBER_OF_PERSONS]; //vector to hold all the persons
	bool keepMenuGoing = true;
	//function to print person record
	void printPersonVector()
	{
		for (int i = 0; i < NUMBER_OF_PERSONS; i++)
		{
			if (persons[i].getAge()>=0)
			{
				cout << (i + 1) << " " << persons[i].getName() << endl;
			}
			
		}
	}
	//function to add a new person
	void addNewPerson()
	{
		if (number_of_people_stored >= 5)
		{
			cout << "Application is full " << endl;
		}
		else
		{
			Person person; //initialize a person object
			person.addData(); //call the add data function;
			persons[number_of_people_stored] = person; // add to the array stored in the menu object
			number_of_people_stored++;
		}		
	}

	//function to add a new run for a user, called from the main menu
	void addNewRun()
	{
		cout << "Add run for which person?" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].addRun();
	}

	//function to show person, called from the main menu
	void showPerson() {
		cout << "Display which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].display();
	}
	//function to edit person, called from the main menu
	void editPerson() {
		cout << "Edit which person" << endl;

		printPersonVector();

		int input;
		cin >> input;

		persons[input - 1].edit();
	}
	//function for writing all the info to a file
	void writeToFile()
	{
		ofstream outPutFile; //create output file objec
		outPutFile.open("run_logger.dat", ios::binary); //open the file wanted
		for (int i = 0; i < NUMBER_OF_PERSONS; i++) //loop over all the person records
		{
			Person person = persons[i];
			outPutFile.write(reinterpret_cast<char*>(&person), sizeof(person));
		}
		outPutFile.close();
	}

	void loadFromFile()
	{
		fstream inputFile("run_logger.dat",ios::in || ios::binary); //open  file needed
		if (inputFile)
		{
			long i = 0;
			while (!inputFile.eof())
			{
				Person person;
				inputFile.seekg(i * sizeof(person), ios::beg);				
				inputFile.read(reinterpret_cast<char*>(&person), sizeof(person));
				if (i < NUMBER_OF_PERSONS)
				{
					persons[i] = person;
					i++;
					if (person.getAge() >= 0)
					{
						number_of_people_stored++;
					}					
				}				
			}
			inputFile.close(); //good practice to close files.
		}

	}

	void saveAndQuit()
	{
		writeToFile();
		keepMenuGoing = false;
		cout << "Goodbye" << endl;
	}

public:
	//function to display the start menu
	void start()
	{
		loadFromFile();

		while (keepMenuGoing)
		{
			cout << "Welcome to the run logger, please choose an option" << endl;
			cout << "1. Add a new person" << endl;
			cout << "2. Add a new run" << endl;
			cout << "3. Show Person" << endl;
			cout << "4. Edit Person" << endl;
			cout << "5. Write to File" << endl;
			cout << "6. Load from file" << endl;
			cout << "7. Save a Quit" << endl;

			int input;
			cin >> input;

			switch (input)
			{
			case 1:
				addNewPerson();
				break;
			case 2:
				addNewRun();
				break;
			case 3:
				showPerson();
				break;
			case 4:
				editPerson();
				break;
			case 5:
				writeToFile();
				break;
			case 6:
				loadFromFile();
				break;
			case 7:
				saveAndQuit();
				break;
			}
		}
	}
};

int main()
{
	//initilize menu object
	Menu m;
	m.start(); //start the menu;
}


