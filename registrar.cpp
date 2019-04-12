#include "registrar.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <cstring>
#include <string>
#include <bits/stdc++.h>

GenQueue <Student> q(10); //instance of queue
DoublyLinked <Student> dll; //linked list for students after being removed from queue, takeTime to know when windows should be idle again
DoublyLinked <Student> dllS; //linked list of students with completed metrics
DoublyLinked <int> idle; //list of idle times for windows
int lines = -1; //size of numArray
int* numArray; //array that stores input from file
Window* windowArray; //array that store windows
Student* studentArray;
int* waitTimeArray;
int* idleTimeArray;
int studentArraySize = 0;
int clocks = 0; // total counter for while loop
int correctIndex = 2; //where to index numArray to get correct number of students
int stops = 0; //kills loop in run

Student::Student()
{

}

Student::Student(int time, int start)
{
  timeNeeded = time;
  startTime = start;
  waitTime = 0;
  isDone = false;
}

Student::~Student()
{

}

Window::Window()
{

}

Window::Window(int id)
{
  unique_id = id;
  withStudent = false;
  idleTime = 0;
}

Window::~Window()
{

}

Simulation::Simulation()
{

}

Simulation::~Simulation()
{
  delete[] ::windowArray;
  delete[] ::numArray;
  delete[] ::studentArray;
  delete[] ::waitTimeArray;
  delete[] ::idleTimeArray;
}

void Simulation::readFile(string location)
{
  string line;
  ifstream locFile;
  locFile.open(location, ios::in);
  if (locFile.is_open())
  {
    while(!locFile.eof())
    {
      getline(locFile, line);
      ::lines++;
    }
    ::numArray = new int[::lines];
  }
  else
  {
    cout << "Not a valid file" << endl;
  }
  locFile.close();
  locFile.open(location, ios::in);
  if (locFile.is_open())
  {
    int i = 0;
    while(!locFile.eof())
    {
      getline(locFile, line);
      stringstream input(line);
      int x = 0;
      input >> x;
      ::numArray[i] = x;
      i++;
    }
  }
  else
  {
    cout << "Not a valid file" << endl;
  }
  locFile.close();
}

void Simulation::outputArray()
{
  for (int i = 0; i < ::lines; i++)
  {
    cout << ::numArray[i] << endl;
  }
}

void Simulation::createWindows()
{
  int numWindows = ::numArray[0];
  ::windowArray = new Window[numWindows];
  for (int i = 0; i < numWindows; i++)
  {
    Window w(i+1);
    ::windowArray[i] = w;
  }
}

void Simulation::createStudents(int idxStud) //parameter is index of array, tells how many student to put in queue
{
  int numStudents = ::numArray[idxStud];
  for (int i = 1; i <= numStudents; i++)
  {
    Student s(::numArray[(idxStud+i)], ::numArray[(idxStud-1)]); //create student with timeNeeded and startTime
    q.insert(s); //inserting students into queue
  }
  ::correctIndex = ::correctIndex + numStudents + 2; //updating index to call create students again
}

void Simulation::times() //check to see if various things work correctly
{
  /*while(q.isEmpty() == false)
  {
    cout << q.remove().startTime << endl;
  }*/
  for (int i = 0; i < ::numArray[0]; i++)
  {
    cout << ::windowArray[i].withStudent << endl;
  }
}

void Simulation::seeQueue()
{
  q.isEmpty();
  while(q.isEmpty() == false)
  {
    q.remove();
  }
}

void Simulation::selectPos()
{
  for (int i = 0; i < ::lines; i++)
  {
    if(i == 0)
    {
      createWindows();
    }
    else if(i == ::correctIndex)
    {
      createStudents(i);
    }
    else
    {
      continue;
    }
  }
}

void Simulation::moveStudent(Student stud) //filters a student into window,
{
  int open = ::numArray[0];
  for (int i = 0; i < ::numArray[0]; i++)
  {
    if(::windowArray[i].withStudent == false)
    {
      stud = q.remove(); //student only removed if there is an open window
      dll.insertFront(stud);
      ::windowArray[i].withStudent = true; //changing boolean withStudent to true
      idle.insertFront(::windowArray[i].idleTime); //adding idle time to linked list
      ::windowArray[i].idleTime = 0; //setting idle time of window back to zero
      break;
    }
    else
    {
      open--;
      ::windowArray[i].idleTime++;
    }
  }
  if(open == 0) //if all windows.withStudent == true, then waitTime++
  {
    stud.waitTime++; //if all windows are full, add to waitTime
  }
  traverseStudent();
  ::clocks++;
  cout << ::clocks << endl;
}

void Simulation::traverseStudent()
{
  if(dll.isEmpty() == false) //trying to access takeTime method with respective students in order to know when Windows are empty
  {
    int size = dll.size;
    //cout << size << endl;
    for (int i = 0; i <= size; i++)
    {
      Student stud = dll.removeFront();
      takeTime(stud, ::windowArray[i]);
      if(stud.isDone = false)
      {
        dll.insertBack(stud);
      }
    }
  }
}

void Simulation::takeTime(Student stud, Window win) //student is at window, exit window when time is up
{
  //cout << "hi" << endl;
  if(::clocks - stud.startTime == stud.timeNeeded)
  {
    stud.isDone = true;
    //times();
  }
  if(stud.isDone == true) //if student is done at window, window is open
  {
    win.withStudent = false;
    //cout << "hi" << endl;
    ::dllS.insertFront(stud); //insert students into linkedlist for complete stats
  }
}

void Simulation::run()
{
  cout << "starting simulation..." << endl;
  while(::stops == 0)
  {
    int count = 0;
    while(q.isEmpty() == false) //iterate through queue to determine which students can move to window
    {
      if(::clocks >= q.front().startTime)
      {
        moveStudent(q.front()); //doesn't remove student from list
      }
      else
      {
        ::clocks++; //clock ticks if moveStudent is not called
        //cout << ::clocks << endl;
      }
    }
    traverseStudent();
    for (int i = 0; i < ::numArray[0]; i++)
    {
      if(windowArray[i].withStudent == true)
      {
        count++;
      }
    }
    if(count == 0) //if queue and windows are empty, simulation is complete
    {
      cout << "simulation is complete" << endl;
      ::stops = 1;
      displayStats();
    }
    ::clocks++;
    //cout << ::clocks << endl;
  }
}

void Simulation::displayStats()
{
  createStudentArray();
  createWaitTimeArray();
  createIdleTimeArray();
  cout << "average student wait time: " << averageWaitTime() << endl;
  cout << "median student wait time: " << medianWaitTime() << endl;
  cout << "longest student wait time: " << longestWaitTime() << endl;
  cout << "number of students waiting over 10 minutes: " << over10() << endl;
  cout << "average window idle time: " << averageIdleTime() << endl;
  cout << "longest window idle time: " << longestIdleTime() << endl;
  cout << "number of windows idle for over 5 minutes: " << over5() << endl;
}

Student* Simulation::createStudentArray() //creating student array from linked list
{
  ::studentArraySize = dllS.size;
  //cout << ::studentArraySize << endl;
  ::studentArray = new Student[::studentArraySize];
  int i = 0;
  while(dllS.isEmpty() == false)
  {
    studentArray[i] = dllS.removeFront();
    i++;
  }
  return ::studentArray;
}

int* Simulation::createWaitTimeArray()
{
  ::waitTimeArray = new int[::studentArraySize];
  for (int i = 0; i < ::studentArraySize; i++)
  {
    ::waitTimeArray[i] = ::studentArray[i].waitTime;
  }
  return ::waitTimeArray;
}

int* Simulation::createIdleTimeArray()
{
  ::idleTimeArray = new int[idle.size];
  int i = 0;
  while(idle.isEmpty() == false)
  {
    ::idleTimeArray[i] = idle.removeFront();
    i++;
  }
}

double Simulation::averageWaitTime()
{
  double sum = 0;
  double count = 0;
  for(int i = 0; i < ::studentArraySize; i++)
  {
    Student stu = studentArray[i];
    double wait = stu.waitTime;
    sum += wait;
    count++;
  }
  double mean = sum/count;
  return mean;
}

double Simulation::medianWaitTime()
{
  int n = ::studentArraySize;
  sort(::waitTimeArray, ::waitTimeArray+n);
  if(::studentArraySize%2 == 0)
  {
    return (double)(::waitTimeArray[(n-1)/2] + ::waitTimeArray[n/2])/2.0;
  }
  else
    return (double)::waitTimeArray[n/2];

}

int Simulation::longestWaitTime()
{
  int max = 0;
  for (int i = 0; i < ::studentArraySize; i++)
  {
    if(i == 0)
    {
      max = ::waitTimeArray[i];
    }
    else
    {
      if(::waitTimeArray[i] > max)
      {
        max = ::waitTimeArray[i];
      }
    }
  }
  return max;
}

int Simulation::over10()
{
  int count = 0;
  for (int i = 0; i < ::studentArraySize; i++)
  {
    if(::waitTimeArray[i] > 10);
    count++;
  }
  return count;
}

double Simulation::averageIdleTime()
{
  double sum = 0;
  double count = 0;
  for(int i = 0; i < idle.size; i++)
  {
    sum += ::idleTimeArray[i];
    count++;
  }
  double mean = sum/count;
  return mean;
}

int Simulation::longestIdleTime()
{
  int max = 0;
  for (int i = 0; i < idle.size; i++)
  {
    if(i == 0)
    {
      max = ::idleTimeArray[i];
    }
    else
    {
      if(::idleTimeArray[i] > max)
      {
        max = ::idleTimeArray[i];
      }
    }
  }
  return max;
}

int Simulation::over5()
{
  int count = 0;
  for (int i = 0; i < idle.size; i++)
  {
    if(::idleTimeArray[i] > 5);
    count++;
  }
  return count;
}
