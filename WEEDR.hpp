#pragma once
#include <iostream>
#include <vector>
using namespace std;


//HELPER FUNCTIONS
bool inString(string, char); //Application::setInfo
int digitsIn(string word); //Application::setInfo
bool has(string word, char c); //Application::setResume, Hash::get
vector<string> splitString(const string& str, char c); //Application::setResume

struct LL{
  string word;
  LL* next;
};

class Hash{
  LL** table;
  int size;
  int getHash(string); //hash function
  void add(string); //add string to table
public:
  Hash(); //ignore object
  Hash(string, Hash); //keywords object
  ~Hash();
  LL* search(string);
  bool exists(string);
  void get(); //for ignore object
  void get(string, Hash); //for keywords object
};

struct Application{
  string first, last, email;
  char phone [10];
  int position; //when they applied
  vector<string> keywords;
  Application(string, int);
  void setInfo(string, Hash);
  void setKeywords(string);
};

class PriorityQueue{
  Application* queue;
  int currentSize, maxSize;
  void repairUp(int);
  void repairDown(int);
  bool priority(Application, Application);
  void swap(Application*, Application*);
public:
  PriorityQueue(int);
  ~PriorityQueue();
  void enqueue(Application);
  void dequeue();
  void peek();
};
