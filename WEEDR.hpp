//WEEDR Resume Parser
/*
Resume, job listing, common professional words - hash table
Applicants - priority queue heap
Keywords - vector
Parse thru text to populate hash tables
*/

#pragma once
#include <iostream>
using namespace std;


struct LL{
  string word;
  LL* next;
};

class Hash{
protected:
  LL** table;
  int size;
  int getHash(string); //hash function
  void add(string); //add string to table
public:
  Hash(string);
  ~Hash();
  void load(string); //load full resume to Hash table
  bool exists(string); //if string is in table
};

struct Application{
  string first, last, email;
  char phone [10];
  int position; //when they applied
  vector<string> keywords;
  void setInfo(string);
  void loadResume(string);
};

/* priority is based on keywords.size() and position */
class PriorityQueue{
  Application* queue;
  int currentSize, maxSize;
  void repairUpward(int);
  void repairDownward(int);
  bool priority(Application, Application);
  void swap(Application*, Application*);
public:
  PriorityQueue(int);
  ~PriorityQueue();
  void enqueue();
  void dequeue();
  Applicant peek();
  bool isFull();
  bool isEmpty();
};

bool inString(string, char);
bool isPhone(string);
