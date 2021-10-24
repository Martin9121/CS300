//============================================================================
// Name        : Project2HashTable.cpp
// Author      : Martin Reyes
// Version     : 1.0
// Description : This program will create a menu of 4 options for the user to
// choose from. If option 1 is chosen, a file with courses will be opened and
// read. the courses will then be added to a hash table. If the file is not
// found then the user will be notified. Option 2 and 3 will not work if 
// option one has not been selected. Option 2 will sort the list od courses in 
// alphanumeric order and print them. Oiption 3 will search for a specific 
// course and print it along with prerequisites. Option 4 will ebd the program.
//============================================================================


#include <fstream>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <sstream>

using namespace std;
unsigned int DEFAULT_SIZE = 10;


// creates the course structure
struct Course {
    string courseNum;
    string courseName;
    string prerequisites;

};

//converts the course num to an int to be hashed
int convertToASCII(string courseNum){

    int numbers = 0;
    for (int i = 0; i < courseNum.length(); i++){
        char x = courseNum.at(i);

        numbers = numbers + x;
    }
 
    return numbers;
}


class HashTable {

private:
    // Define structures to hold courses
    struct Node {
        Course course;
        unsigned int key;
        Node* next;

        // default constructor
        Node() {
            key = UINT_MAX;
            next = nullptr;
        }

        // initialize with a course
        Node(Course aCourse) : Node() {
            course = aCourse;
        }

        // initialize with a course and a key
        Node(Course aCourse, unsigned int aKey) : Node(aCourse) {
            key = aKey;
        }

        int a = 0;
        int b = 0;
        int c = 0;
        int d = 0;
        //This allows me to use the '<' operator to compare variables declared 
    //with the Course structure with variables of other structures.
        bool operator <(const Node& rhs) const

        {
            int a = 0;
            int b = 0;
            int c = 0;
            int d = 0;
            if (a < rhs.a) { return true; }
            else if (rhs.a < a) { return false; }

            // if neither of the above were true then 
            // we are consdidered equal using strict weak ordering
            // so we move on to compare the next item in the struct

            if (b < rhs.b) { return true; }
            if (rhs.b < b) { return false; }

            if (c < rhs.c) { return true; }
            if (rhs.c < c) { return false; }

            if (d < rhs.d) { return true; }
            if (rhs.d < d) { return false; }

            // if both are completely equal (based on strict weak ordering)
            // then just return false since equality doesn't yield less than
            return false;
        }
    };


    unsigned int tableSize = DEFAULT_SIZE;

    unsigned int hash(int key);

public:
    vector<Node> nodes;
    HashTable();
    HashTable(unsigned int size);
    virtual ~HashTable();
    void Insert(Course course);
    Course Search(string coursenum);
    void selectionSort(vector<Node>& nodes);
    void readFile(HashTable* hashTable);

};

// This function will first add all the courses to a 
// seperate vector then sort and print the courses in 
// the vector
void HashTable::selectionSort(vector<Node>& vecToSort) {
    cout << "Here is a sample schedule: " << endl << endl;
    
    vector<Node> sortedVec;
    unsigned int i;
    unsigned int j;
    unsigned int lowestIndex = 0;
    Node temp;
    vector <Node> vecs;
    
    for (unsigned int p = 0; p < nodes.size(); ++p) {
        unsigned key = hash(convertToASCII(nodes.at(p).course.courseNum));  

        Node* node = &(nodes.at(key));

        if (nodes.at(key).key != UINT_MAX) { 

            sortedVec.push_back(nodes.at(p));

            // while node not equal to nullptr
            // this while looop also takes the 
            //chained course and adds it to 
            //the vector to seperate from the
            // same bucket
            while (node->next != nullptr) {

                // node is equal to next node
                node = node->next;
                sortedVec.push_back(*node);
            }

        }

    }
    // this is the selection sort portion of the fucntion 
    for (i = 0; i < sortedVec.size() - 1; ++i) {
        lowestIndex = i;
        for (j = i + 1; j < sortedVec.size(); ++j) {

            if (sortedVec[j].course.courseNum < sortedVec[lowestIndex].course.courseNum) {
                lowestIndex = j;
            }
        }
        // adds elemt to sorted section
        temp = sortedVec[i];
        sortedVec[i] = sortedVec[lowestIndex];
        sortedVec[lowestIndex] = temp;

    }
    // print sorted vector
    for (i = 0; i < sortedVec.size(); i++){

        cout << sortedVec.at(i).course.courseNum << ", " << sortedVec.at(i).course.courseName  << endl;
    }
}

/**
 * Default constructor
 */
HashTable::HashTable() {

    // Initalizes the node structure by resizing tableSize
    nodes.resize(tableSize);
}

/**
 * Constructor for specifying size of the table
 * Use to improve efficiency of hashing algorithm
 * by reducing collisions without wasting memory.
 */
HashTable::HashTable(unsigned int size) {
    // invoke local tableSize to size with this->
    this->tableSize = size;
    // resize nodes size
    nodes.resize(tableSize);

}

/**
 * Destructor
 */
HashTable::~HashTable() {

    // erases nodes to free up storage
    nodes.erase(nodes.begin());
}


/**
 * Calculate the hash value of a given key.
 * Note that key is specifically defined as
 * unsigned int to prevent undefined results
 * of a negative list index.
 *
 * @param key The key to hash
 * @return The calculated hash
 */
unsigned int HashTable::hash(int key) {

    //calculates and returns hash value
    key = key % tableSize;
    return key;
}

/**
 * Insert a course
 *
 * @param The course to insert
 */
 // will insert loaded courses into the hash table
void HashTable::Insert(Course course) {

    // create the key for the given course
    unsigned key = hash(convertToASCII(course.courseNum));//.c_str()

    // retrieve node using key
    Node* oldNode = &(nodes.at(key));

    // if no entry found for the key
    if (oldNode == nullptr) {

        // assign this node to the key position
        Node* newNode = new Node(course, key);
        nodes.insert(nodes.begin() + key, (*newNode));
       
        
    }
    // else if node is not used
    else {
        // if old node key == UNIT_MAX, set key, set old node to course and old node next to null pointer
        if (oldNode->key == UINT_MAX) {
            oldNode->key = key;
            oldNode->course = course;
            oldNode->next = nullptr;
            
        }
        // else find the next open node
        else {
         
            while (oldNode->next != nullptr) {
                oldNode = oldNode->next;
           }
            // add new newNode to end
            oldNode-> next = new Node(course, key);
           
        }
    }
}

Course HashTable::Search(string coursenum) {
    Course course;

    // create the key for the given course
    unsigned key = hash(convertToASCII(coursenum));

    //retrieve node using the key
    Node* node = &(nodes.at(key));

    // if entry found for the key
    if (node != nullptr && node->key != UINT_MAX
        && node->course.courseNum.compare(coursenum) == 0) {

        //return node course
        return node->course;
    }

    // if no entry found for the key
    if (node == nullptr || node->key == UINT_MAX) {
        // return course which is empty
        return course;
    }

    // while node not equal to nullptr
    while (node != nullptr) {
        // if the current node matches, return it
        if (node->key != UINT_MAX && node->course.courseNum.compare(coursenum) == 0) {
            return node->course;
        }
        //node is equal to next node
        node = node->next;
    }

    return course;
}

// this function will open the file and load the data 
// into a hashtable 
void HashTable::readFile(HashTable* hashTable) {

    ifstream infile;
    string fileName = "Project2Input.txt";
    infile.open(fileName);

    // if file not open, return
    if (!infile.is_open()) {
        cout << endl << "Sorry, the file was not found" << endl;
        return;
    }
    cout << endl << "File has opened" << endl;
    string line;
    char seperator = ',';

    // if file is open
    if (infile.is_open()) {
        while (!infile.eof()) {

            // gets each line in the file
            getline(infile, line);
            stringstream s_stream(line); //create string stream from the string
            vector<string> courseInfo;
            while (s_stream.good()) {

                string substr;
                getline(s_stream, substr, ','); //get strings delimited by comma
                courseInfo.push_back(substr); //and add strings to vector
            }

            Course mycourse;
            // assigns course num and name for every course
            mycourse.courseNum = courseInfo.at(0);
            mycourse.courseName = courseInfo.at(1);

            if (courseInfo.size() > 2) {
                for (int i = 2; i < courseInfo.size() - 1; ++i) {
                    // if course has prerequisites they are assigned
                    mycourse.prerequisites = courseInfo.at(i) + ", " + courseInfo.at(i + 1);
                }
            }
            hashTable->Insert(mycourse);
        }
        cout << "Data structure is loaded" << endl << endl;
    } 
}


int main() {
    Course course;
    int choice = 0;
    
    HashTable* courseTable = nullptr;
    while (choice != 4) {
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;
        choice = choice;
 
            while (choice > 4 || choice < 1) {
               
                cout << "Invalid choice try again" << endl;
                break;
            }

        switch (choice) {
        case 1:
            // crete new hashtable
            courseTable = new HashTable();
            //call readfile function
            courseTable->readFile(courseTable);

            break;
        case 2:
            //data structure must be loaded first
            if (courseTable == nullptr ) {
                cout << "Must load data structure first"<< endl;
                break;
            }
            //call selection sort function
            courseTable->selectionSort(courseTable->nodes);
            break;
        case 3:
            // data structure must be loaded first
            if (courseTable == nullptr) {
                cout << "Must load data structure first" << endl;
                break;
            }

            string coursenum;
            // prompt user for courseNum
            cout << endl << "What course do you want to know about?" << endl;
            cin >> coursenum;

            // search for coruse with given course number
            course = courseTable->Search(coursenum);

            // if found
            if (!course.courseNum.empty()) {
                cout << course.courseNum << ", " << course.courseName << endl <<
                    " Prerequisites: " << course.prerequisites << endl; 
            }
            // if not found
            else {
                cout << "Course Number: " << coursenum << " not found." << endl;
            } 
            break;
        }  
    }
    cout << "Thank you for using the course planner!" << endl;
    return 0;
}