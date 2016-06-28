
/*

-This program takes two files for input. 1) StudentData.tsv contains student grade data where each student ID contains
information for one class that student took.  This file is not sorted. 2) HunterCourses.tsv contains data about individual courses.
For both files each column is tab separated contains specific info about that student's course (SD file), or about that course(HC file).

The program then outputs two files. 1) StudentDataPlus.tsv contains all of the thins StudentData contains, but with the hours and
designation code added to the end of the file, each separated by a tab as per the format. This is done by matching a course from the
StudentData file by its subject code and catalog number and finding a match in the HunterCourses file, and then outputing the information
to the StudentDataPlus File. Some courses will not exist in HunterCourses if they are retired or renumbered, so in that case we add
3.0 and RLA as the hours and designation code. This file is then sorted by Student ID.
2) the second output file StudentSummary.tsv contains one student ID per line, along with the calculated overall Gpa, Gpa in csci courses,
and a percent of hours spent taking non liberal arts courses. Codes RNL, MNL, and GNL are considered non liberal arts.

-The program uses structs to represent each student and each course. It uses linked lists to represent all of the classes
a student took. ex: each Student struct contains the student ID and a linked list of classes associated with that student ID (a list
of classes that student took). The linked list is implemented in a class.
The program first loads the data from the StudentData file into a vector of Student objects. It then uses insertion sort to sort the
vector of student objects by student ID in ascending order. The program then loads the Hunter Courses file into a vector of Course objects.
Then the program generates a file called StudentDataPlus.tsv by going through the data loaded in both of the previous vectors. It then
generates another file called StudentSummary.tsv, which summarizes the information that was loaded into StudentDataPlus.tsv for each studentID

*/



#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;

// data structs
struct Class
{
	int term;
	string subject;
	string catalog;
	string letGrade;
	double numGrade;
	Class* next;
};

// linked list of Classes (for each individual student)
class ClassList
{
public:
	// set function inline
	// set what the head of the linked list points to
	void setFront(Class* &newNode)
	{
		front = newNode;
	}
	// get function inline
	// get the head of the list
	Class* getFront()
	{
		return front;
	}
	// constructor
	ClassList()
	{	}
	// destructor
	~ClassList()
	{	}
private:
	Class* front; // points to the head of the Classlist
};


struct Student
{
	string ID;
	ClassList ClassesTaken;  // a linked list of Class objects
};

struct Course
{
	string subject;
	string catalog;
	double hours;
	string DR;  // designation requirement
};

// Function Definitions
void insertionSort(vector<Student> &students);
int CheckIdinVector(string emplID, vector<Student> &students); // look in student vector for student ID
void addFirstIdandClass(vector<Student> &students, Student &tempEachStudent, string emplID, string subjectCode, string catalogCode, string letterGrade, int semesterCode, double numericGrade);
void addClassToList(vector<Student> &students, int checkID, string emplID, string subjectCode, string catalogCode, string letterGrade, int semesterCode, double numericGrade);
void loadStudentData(ifstream &studentDataFile, vector<Student> &students);
void loadCourseData(ifstream &coursesDataFile, vector<Course> &courses);
void loadSdPlusFile(ofstream &studentDataPlus, vector<Student> &students, vector<Course> &courses);
void loadSdSummaryFile(ofstream &studentSummary, ifstream &inputStudentDataPlus);


int main()
{
	// =================================================
	// load the Student data file into a vector of Student objects
	// =================================================

	ifstream studentDataFile;
	studentDataFile.open("StudentData.tsv");
	if (studentDataFile.fail())
	{
		cout << "Error opening student data file\n";
	}
	vector<Student> students; // holds info for each student ID
							  // load student data into a vector of student objects
	loadStudentData(studentDataFile, students);

	// sort ID's in the vector in increasing order
	insertionSort(students);

	// =================================================
	// load the HunterCourses file into a vector of Course objects.
	// =================================================

	ifstream coursesDataFile;
	coursesDataFile.open("HunterCourses.tsv");
	if (coursesDataFile.fail())
	{
		cout << "Error opening Hunter courses data file\n";
	}
	vector<Course> courses;  // holds info for each course
							 // load course data into a vector of course object
	loadCourseData(coursesDataFile, courses);


	// =================================================
	// create the StudentDataPlus.tsv file
	// =================================================

	ofstream studentDataPlus;
	studentDataPlus.open("StudentDataPlus.tsv");
	if (studentDataPlus.fail())
	{
		cout << "Error opening studentDataPlus\n";
	}
	// load our SDPlus file by searching through hc and sd vectors
	loadSdPlusFile(studentDataPlus, students, courses);

	// =================================================
	// create the StudentSummary.tsv file
	// =================================================

	ofstream studentSummary; // new file sd summary to output data into
	studentSummary.open("StudentSummary.tsv");
	if (studentSummary.fail())
	{
		cout << "Error opening studentSummary\n";
	}

	// open SdPlus file for input
	ifstream inputStudentDataPlus;
	inputStudentDataPlus.open("StudentDataPlus.tsv");
	if (inputStudentDataPlus.fail())
	{
		cout << "Error opening input studentDataPlus\n";
	}

	loadSdSummaryFile(studentSummary, inputStudentDataPlus);

	// close all files just in case
	studentDataFile.close();
	coursesDataFile.close();
	studentDataPlus.close();
	studentSummary.close();
	inputStudentDataPlus.close();



	return 0;
}

// =================================================
// =================================================
// Function definitions
// =================================================
// =================================================

// (look in student vector for studentID). check if ID is in vector, if it is return the index, if not return -1
// Precondition: Cannot have two or more students with the same ID's
// Postcondition: return the index if ID found in vector or -1 if ID not found in vector
int CheckIdinVector(string emplID, vector<Student> &students)
{
	int result = -1; // will store index if id is in vector
	for (int i = 0; i < students.size(); i++)
	{
		// if in vector return the index
		if (emplID == students[i].ID)
		{
			result = i;
			break;
		}
	}
	return result;
}

// if a student ID does not exist in this passed vector, this func pushes a Student struct with the student ID
// along with the first class that is associated with the ID to the vector
// Precondition: the passed emplID does not exist in the vector of student objects
// Postcondition: a student struct with a student ID and with a list containing the first class that is
// associated with the ID is pushed to the vector
void addFirstIdandClass(vector<Student> &students, Student &tempEachStudent, string emplID, string subjectCode, string catalogCode, string letterGrade, int semesterCode, double numericGrade)
{
	Class* newNode = NULL;  // for dynamically allocating new nodes (pointer to a class object)
	// add the student ID to the temp struct
	tempEachStudent.ID = emplID;
	// === add the first class node for that ID
	newNode = new Class;  // create new node
	// store info in the new node
	newNode->term = semesterCode;
	newNode->subject = subjectCode;
	newNode->catalog = catalogCode;
	newNode->letGrade = letterGrade;
	newNode->numGrade = numericGrade;
	newNode->next = NULL; // this first new node is the last node as well
	tempEachStudent.ClassesTaken.setFront(newNode);  // head points to our created first node
	// === push it into the vector
	students.push_back(tempEachStudent);
	return;
}

// (add a class to a class list) Adds a class to a class list for a student ID found in the checkID'th index of students vector (students[checkID])
// Precondition: The passed emplID already exists in the students vector and is unique. The vector with that student ID already has at
// least one class in its linked list of classes
// Postcondition: a class is added to a class list for a student ID found in the checkID'th index of the vector of student objects
void addClassToList(vector<Student> &students, int checkID, string emplID, string subjectCode, string catalogCode, string letterGrade, int semesterCode, double numericGrade)
{
	Class* newNode = NULL;  // for dynamically allocating new nodes (pointer to a class object)
	// === add another Class node
	newNode = new Class;  // create new node
	 // store course info in the new node
	newNode->term = semesterCode;
	newNode->subject = subjectCode;
	newNode->catalog = catalogCode;
	newNode->letGrade = letterGrade;
	newNode->numGrade = numericGrade;
	// push new node to front of list
	newNode->next = students[checkID].ClassesTaken.getFront(); // newNode->next points to head(head will be 2nd node), since newNode will be first node
	students[checkID].ClassesTaken.setFront(newNode);  // head now points to first node (newNode)
	return;
}

// load student data into a vector of Student objects
// Precondition: student data file is opened and is in the proper format. students vector should be empty
// Postcondition: the vector of student objects contains all the students found in the Student data file.
// each student object has an emplid ID and a linked list of all classes associated with that student's emplID in the file
void loadStudentData(ifstream &studentDataFile, vector<Student> &students)
{
	// local variables to read from SD file
	string emplID, subjectCode, catalogCode, letterGrade;
	int semesterCode;
	double numericGrade; // num grades can be floating point
	Student tempEachStudent; // store the necessary data into Student struct from file, then push it back to the vector
	int checkID; // stores -1 if id not in vector, or index if id in vector

	while (studentDataFile >> emplID >> semesterCode >> subjectCode >> catalogCode >> letterGrade >> numericGrade)
	{
		// check if ID is in vector
		checkID = CheckIdinVector(emplID, students);
		// if not in vector, add it along with the first class that comes with the ID
		if (checkID == -1)
		{
			addFirstIdandClass(students, tempEachStudent, emplID, subjectCode, catalogCode, letterGrade, semesterCode, numericGrade);
		}
		// if it is in vector with index i, add a new class to that index
		else
		{
			addClassToList(students, checkID, emplID, subjectCode, catalogCode, letterGrade, semesterCode, numericGrade);
		}
	}
	studentDataFile.close();
	studentDataFile.clear();
	return;
}

// load course data into a vector of course objects
// Precondition: courses data file is opened and is in the proper format. courses vector should be empty
// Postcondition: the vector of course objects contains all the courses found in the CoursesDataFile.
void loadCourseData(ifstream &coursesDataFile, vector<Course> &courses)
{
	// local variables to read from HC file
	string subjectName, catalogCodeCourseFile, desigCode;
	double contactHours;
	Course localEachCourse;  // store necessary variables into Course struct from file, then push it back to vector

	while (coursesDataFile >> subjectName >> catalogCodeCourseFile >> contactHours >> desigCode)
	{
		// store data for the course
		localEachCourse.subject = subjectName;
		localEachCourse.catalog = catalogCodeCourseFile;
		localEachCourse.hours = contactHours;
		localEachCourse.DR = desigCode;
		// push course to vector
		courses.push_back(localEachCourse);
	}
	coursesDataFile.close();
	coursesDataFile.clear();
	return;
}


// go through all classes of sd vector
// search for matching subject and catalog number of the class in the hc vector.
// output the required data into SD Plus file
// Precondition: the students vector is loaded with the data from the StudentDataFile, and the courses vector is loaded with
// the data from the HunterCoursesFile. StudentDataPLus file is opended
// Postcondition: StudentDataPlus file contains the same data that was in the StudentData file but with added contact hours and designation
// code as the last two columnd.
void loadSdPlusFile(ofstream &studentDataPlus, vector<Student> &students, vector<Course> &courses)
{
	Class* current; // walks through list
	bool match = false, storedData = false; // flag if match was found, and flag if we stored the data
	for (int i = 0; i < students.size(); i++)
	{
		// walk through the i'th student's classes
		current = students[i].ClassesTaken.getFront(); // current starts off by pointing to head of list
		while (current != NULL)
		{
			// Look for match in HC
			// stop searching if we stored the data (there was a match) or end of file(no match)
			for (int j = 0; j < courses.size() && (!storedData); j++)
			{
				// if match found
				if ((current->subject == courses[j].subject) && (current->catalog == courses[j].catalog))
				{
					// input required data into SDPlus and continue to next search(add hours and desig code to last 2 lines)
					studentDataPlus << students[i].ID << "\t" << current->term << "\t"
						<< current->subject << "\t" << current->catalog << "\t" << current->letGrade << "\t"
						<< current->numGrade << "\t" << courses[j].hours << "\t" << courses[j].DR << "\n";
					match = true;
					storedData = true;
				}
			}
			// if no match (searched through entire file), then course is retired/renumbered
			// add 3.0 and RNL, to end of line of SD Plus
			if (!(match))
			{
				// input required data into SDPlus
				studentDataPlus << students[i].ID << "\t" << current->term << "\t"
					<< current->subject << "\t" << current->catalog << "\t" << current->letGrade << "\t"
					<< current->numGrade << "\t" << "3.0" << "\t" << "RNL" << "\n";
				storedData = true; // not needed since outside loop
			}
			// reset flags
			match = false;
			storedData = false;
			current = current->next; // go to next class
		}
	}
	studentDataPlus.close();
	studentDataPlus.clear();
}

// Sort a student vector by the ID member variable of the student objects in the vector.
// we are sorting a vector of struct objects by ID name so we need to associate the rest of the struct
// members when reassigning accordingly(e.g the linked list)
// Precondition: The students vector is loaded with the data from the StudentDataFile.
// Postcondition: The student objects in the students vector are sorted by their ID.
void insertionSort(vector<Student> &students)
{
	int j;
	string val;
	Class* valList;
	for (int i = 1; i < students.size(); i++)
	{
		val = students[i].ID;
		valList = students[i].ClassesTaken.getFront();
		j = i - 1;

		while (j >= 0 && students[j].ID > val)
		{
			students[j + 1].ID = students[j].ID;
			students[j + 1].ClassesTaken = students[j].ClassesTaken;
			j--;
		}
		students[j + 1].ID = val;
		students[j + 1].ClassesTaken.setFront(valList);

	}
	return;
}

// walk through the studentDataPlus file, calculate for each studentID the: overall Gpa, Gpa in csci
// courses, and percent of hours spent taking non liberal arts courses. Codes RNL, MNL, and GNL are non liber arts
// Output Emplid along with the calculated data(gpa, csciGpa, percent) for that emplID into the studentSummary file
// Precondition: studentSummary is opened and empty, and inputStudentDataPlus is opened and contains the properly formatted data
// Postcondition: The studentSummary file contains a line for each Emplid along with the calculated overall Gpa, Gpa in csci
// courses, and percent of hours spent taking non liberal arts courses in each column separated by a tab.
void loadSdSummaryFile(ofstream &studentSummary, ifstream &inputStudentDataPlus)
{
	// local variables to read from SDPlus file
	string emplID, subjectCode, catalogCode, letterGrade, desigCode;
	int semesterCode;
	double numericGrade, contactHours; // num grades can be floating point

	string prevID = ""; // track if same or next student
	bool oldStudent = true;
	bool calculatedAll = false; // if true allows us to know we switched to a new student, and now we can compute the info for the old(prev) student
	bool onlyOneClass = true; // keep track if student had only one class

	// Step 1: calculate overall GPA: gradepoints/creditHours
	double creditHours = 0; // accumulate credits for all classes per student
	double gradePoints = 0; // accumulate grade points for all classes per student (Grade points =  for each class: HOURS * NUMGRADE)
	double GPA = 0; // gpa per student

	// Step 2: calculate CSCI GPA: only CSCI courses, gradepoints/creditHours
	double csCreditHours = 0; // accumulate credits for all csci classes per student
	double csGradePoints = 0; // accumulate grade points for all csci classes per student (Grade points =  for each class: HOURS * NUMGRADE)
	double csGPA = 0; // csci gpa per student
	string csCourseName = "CSCI";
	bool csCourse = false; // check if csci course

	// Step 3: calculate percent of hours spent taking non liberal arts courses:
	string nonLib1 = "RNL", nonLib2 = "MNL", nonLib3 = "GNL"; // non liberal arts have these desig code
	bool nonLibRead = false;
	double nonLibCourses = 0, totalCourses = 0;
	double percentNonLib;

	while (inputStudentDataPlus >> emplID >> semesterCode >> subjectCode >> catalogCode >> letterGrade >> numericGrade >> contactHours >> desigCode)
	{
		// = check if a csci course is being read
		if (subjectCode == csCourseName)
		{
			csCourse = true;
		}
		else
		{
			csCourse = false;
		}
		// = check if nonLib course is being read, ignoring those with -1 numgrades
		if (desigCode == nonLib1 || desigCode == nonLib2 || desigCode == nonLib3)
		{
			if (numericGrade >= 0)
			{
				nonLibRead = true;
			}
		}
		else
		{
			nonLibRead = false;
		}

		//  = first iteration (first student ID in file)
		if (prevID == "")
		{
			// == for first student ID start calculations
			// = gpa, total courses, total nonlib csci courses calc
			if (numericGrade >= 0) // ignore -1 grades
			{
				creditHours = contactHours;
				gradePoints = (numericGrade * contactHours);
				totalCourses++;

				if (nonLibRead)
				{
					nonLibCourses = contactHours;
				}
			}
			// = csci gpa get data
			if (csCourse && numericGrade >= 0) // ignore -1 grades
			{
				csCreditHours = contactHours;
				csGradePoints = (numericGrade * contactHours);
			}

			studentSummary << emplID << "\t";
			prevID = emplID;
			onlyOneClass = true; // only got the students first class with this block
		}

		// = On studentID SWITCH (when new student ID is beaing read)
		else if (emplID != prevID)
		{
			// == we switched to a new studentID, so print for old(prev) student
			if (calculatedAll)
			{
				if (creditHours > 0) // if no classes cant divide by 0
				{
					GPA = gradePoints / creditHours;
				}
				if (csCreditHours > 0) // if no csci classes cant divide by 0
				{
					csGPA = csGradePoints / csCreditHours;
				}
				if (nonLibCourses > 0)
				{
					percentNonLib = (creditHours / nonLibCourses);
				}
				studentSummary << GPA << "\t" << csGPA << "\t" << percentNonLib << "\n";
				calculatedAll = false;
			}
			//==  if old (prev) student had only one class print his gpa
			else if (onlyOneClass)
			{
				if (creditHours > 0) // if no classes cant divide by 0
				{
					GPA = gradePoints / creditHours;
				}
				if (csCreditHours > 0) // if no csci classes cant divide by 0
				{
					csGPA = csGradePoints / csCreditHours;
				}
				if (nonLibCourses > 0)
				{
					percentNonLib = (creditHours / nonLibCourses);
				}
				studentSummary << GPA << "\t" << csGPA << "\t" << percentNonLib << "\n";
			}
			// == reset gpa, csci gpa, courses variables for new student
			creditHours = 0;
			gradePoints = 0;
			csCreditHours = 0;
			csGradePoints = 0;
			GPA, csGPA, percentNonLib = 0;
			nonLibCourses = 0;
			// == for the new switched to student ID
			prevID = emplID;
			studentSummary << emplID << "\t";
			onlyOneClass = true; // only got the students first class with this block

								 // == for new student ID start calculations
								 // = gpa calc
			if (numericGrade >= 0) // ignore -1 grades
			{
				creditHours = contactHours;
				gradePoints = (numericGrade * contactHours);
			}
			// = csci gpa calc
			if (csCourse && numericGrade >= 0) // ignore -1 grades
			{
				csCreditHours = contactHours;
				csGradePoints = (numericGrade * contactHours);
			}

			totalCourses = 0; // reset since new student ID
			nonLibCourses = 0;
			if (numericGrade >= 0) // if numgrade is not -1 add this courses total courses and total nonlib if noonlib
			{
				totalCourses++;
				if (nonLibRead)
				{
					nonLibCourses = contactHours;
				}
			}
		}
		// = reading same student ID
		else if (emplID == prevID)
		{
			onlyOneClass = false;
			// == for same student ID continue calculations
			// = gpa, total courses, total nonlib courses calc
			if (numericGrade >= 0) // ignore -1 grades
			{
				creditHours += contactHours;
				gradePoints += (numericGrade * contactHours);
				if (nonLibRead)
				{
					nonLibCourses += contactHours;
				}
			}
			// = csci gpa calc
			if (csCourse && numericGrade >= 0) // ignore -1 grades
			{
				csCreditHours += contactHours;
				csGradePoints += (numericGrade * contactHours);
			}

			// == if this happens to be last time we are on same student, we will calculate the students info once
			// we recognize we are reading a new student ID.
			calculatedAll = true;
		}

	} //<-- end while

	  // for last line when studentID does not switch we do not print, so we print the info now
	if (inputStudentDataPlus.eof())
	{
		if (creditHours > 0) // if no classes cant divide by 0
		{
			GPA = gradePoints / creditHours;
		}
		if (csCreditHours > 0) // if no csci classes cant divide by 0
		{
			csGPA = csGradePoints / csCreditHours;
		}
		if (nonLibCourses > 0)
		{
			percentNonLib = (creditHours / nonLibCourses);
		}
		studentSummary << GPA << "\t" << csGPA << "\t" << percentNonLib << "\n";
		nonLibCourses = 0;
		calculatedAll = false;
	}

	// close all files
	studentSummary.close();
	studentSummary.clear();
	inputStudentDataPlus.close();
	inputStudentDataPlus.clear();

	return;
}
