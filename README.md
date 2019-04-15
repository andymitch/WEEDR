### Final Project
Andrew Mitchell
CSCI 2270-302

# WEEDR![alt text](https://github.com/andymitch/WEEDR/blob/master/leaf-logo.png "WEEDR logo")
### Resume Parser

## Overview

WEEDR is a simple resume parser designed to match the most qualified candidates for a specific job opportunity based on matching keywords. WEEDR is written in C++ and is command line dependent. The program is able to take one job listing in .txt format and compare it to any number of resumes that are also in .txt format. It will the set up a priority queue of the applicants based on number of keywords matched from the job listing and position in run arguments.
  
## Data Structures

  * Priority Queue/Max Heap - ranks potential candidates by keywords and time of submission.
  * Hash Table - allows for a quick reference to keywords as well as words to ignore.
  * Class - retains each candidate’s identifying and contact information as well as their matched keywords.
  * STD::Vector - dynamically stores a candidate’s matched keywords.

## How to Use

1. Be sure that you have at least C++11 installed.
  
2. Download the source files from Github.
  
   * From terminal: `$ git clone https://github.com/andymitch/WEEDR.git`
  
   * From browser: [WEEDR](https://github.com/andymitch/WEEDR).
  
3. Insert any job listings into the positions folder.
  
4. Insert any resumes into the applicants folder.
  
5. Compile: `g++ -std=c++11 WEEDR.cpp WEEDR_Driver.cpp -o weedr`.
  
6. Run: `./weedr [job listing] [resume] [resume…]`.

## Future Development

  * Develop a UI (user interface) by either utilizing a GUI library (i.e. Qt, wxWidgets, etc.) or by rewriting program in the Javascript language to utilize HTML/CSS capabilities.
  * Allow for other file formats to be read than just .txt files.
  * Continue to improving information parcing accuracy.

## Open Issues/Bugs

