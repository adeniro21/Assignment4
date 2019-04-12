#include <iostream>

using namespace std;

template <class Q>
class GenQueue
{
  public:
    GenQueue(); //default constructor
    GenQueue(int maxSize); //overloaded constructor
    GenQueue(const GenQueue& queue); //copy constructor
    ~GenQueue();

    void insert(Q d); //enqueue
    Q remove(); //dequeue
    Q front(); //peek

    bool isFull();
    bool isEmpty();
    int getSize();


    int nSize;
    int head; //front of queue
    int tail; //rear of the queue
    int numElements;

    Q *myQueue; //data stucture
};

template <class T>
class ListNode
{
  public:
    ListNode();
    ListNode(T d);
    ~ListNode();

    T data;
    ListNode<T> *next;
    ListNode<T> *prev;
};

template <class T>
class DoublyLinked
{
  public:
    DoublyLinked();
    ~DoublyLinked();
    void insertFront(T d);
    T removeFront();
    T deletePos(int pos);
    void printList();
    int find(T val); //search for value, return if it exsit
    bool isEmpty();
    unsigned int getSize();
    void insertBack(T d);
    T removeBack();
    ListNode<T>* remove(int key);

    ListNode<T> *front;
    ListNode<T> *back;
    unsigned int size; //no negative numbers
};

class Window
{
  public:
    Window();
    Window(int id);
    ~Window();

    int unique_id;
    int idleTime;
    bool withStudent; //whether window is open or not
};

class Student
{
  public:
    Student();
    Student(int time, int start);
    ~Student();

    int timeNeeded; //time need by student at window
    int startTime; // time they arrived at
    int waitTime; //time the waited to be served
    bool isDone; // is student finshed at window
};

class Simulation
{
  public:
    Simulation();
    ~Simulation();
    void readFile(string location);
    void createStudents(int number);
    void createWindows();
    void outputArray();
    void createQueue();
    void seeQueue();
    void selectPos();
    int arriveTime(int i);
    void moveStudent(Student stud);
    void run();
    void displayStats();
    Student* createStudentArray();
    double averageWaitTime();
    double medianWaitTime();
    int longestWaitTime();
    int over10();
    double averageIdleTime();
    int longestIdleTime();
    int over5();
    void times();
    int* createWaitTimeArray();
    void takeTime(Student stud, Window win);
    int* createIdleTimeArray();
    void traverseStudent();
};

//// implementation for queue //////////

template <class Q>
GenQueue<Q>::GenQueue()
{
  myQueue = new Q[5];
  nSize = 5;
  numElements = 0;
  head = 0;
  tail = -1;
}

template <class Q>
GenQueue<Q>::GenQueue(int maxSize)
{
  myQueue = new Q[maxSize];
  nSize = maxSize;
  numElements = 0;
  head = 0;
  tail = -1;
}

template <class Q>
GenQueue<Q>::GenQueue(const GenQueue& queue)
{
  myQueue = queue.myQueue;
  nSize = queue.nSize;
  numElements = queue.numElements;
  head = queue.head;
  tail = queue.tail;
}

template <class Q>
GenQueue<Q>::~GenQueue()
{
  delete[] myQueue;
}

template <class Q>
void GenQueue<Q>::insert(Q d)
{
  //error checking bounds is full
  if(isFull() == true)
  {
    cout << "queue is full" << endl;
  }
  else
  {
    //circular queue code
    if(tail == nSize-1)
    {
      tail = -1;
    }

    myQueue[++tail] = d;
    //cout << "element inserted" << endl;
    ++numElements;
 }
}

template <class Q>
Q GenQueue<Q>::remove()
{
  Q c;
  Q *p = &c;
  p = NULL;
  //error checking bounds is empty
  if(isEmpty() == true)
  {
    return c;
  }
  else
  {
    c = myQueue[head];
    ++head;

    //circular queue
    if(head == nSize)
    {
      head = 0;
    }
    numElements--;

    return c;
  }
}

template <class Q>
Q GenQueue<Q>::front()
{
  return myQueue[head];
}

template <class Q>
bool GenQueue<Q>::isFull()
{
  return (numElements == nSize);
}

template <class Q>
bool GenQueue<Q>::isEmpty()
{
    return (numElements == 0);
}

template <class Q>
int GenQueue<Q>::getSize()
{
  return numElements;
}

///////// implementation for ListNode//////

template <class T>
ListNode<T>::ListNode()
{
  //default constructor
}

template <class T>
ListNode<T>::ListNode(T d)
{
  data = d;
  next = NULL; //nullptr
  prev = NULL;
}

template <class T>
ListNode<T>::~ListNode()
{
  //research this on your own
  //set next and previous pointers to null
  next = NULL;
  prev = NULL;
}

/////implementation for DoublyLinked list///////

template <class T>
DoublyLinked<T>::DoublyLinked()
{
  size =0;
  front = NULL;
  back =  NULL;
}

template <class T>
DoublyLinked<T>::~DoublyLinked()
{
  ListNode<T>* current = front;
  while(current != 0 )
  {
    ListNode<T>* next = current->next;
    delete current;
    current = next;
  }
  front = 0;
}

template <class T>
void DoublyLinked<T>::insertFront(T data)
{
  ListNode<T> *node = new ListNode<T>(data);

  //doubly linked if else
  if(size == 0)
  {
    back = node;
  }
  else //not empty
  {
    node->next = front;
    front->prev = node;
  }
  front = node;
  size++;
}

template <class T>
T DoublyLinked<T>::removeFront()
{
  //check if empty
  ListNode<T> *node = front;
  //check if only one element exists
  if(front->next == NULL) //only one element in the list
  {
    back = NULL;
  }
  else //more than one element
  {
    front->next->prev = NULL;
  }
  front = front->next;
  T temp = node->data;
  node->next = NULL;

  delete node;
  size--;

  return temp;
}

template <class T>
T DoublyLinked<T>::deletePos(int pos)
{
  int idx = 0;
  //add check to make sure position is valid
  ListNode<T> *curr = front;
  ListNode<T> *prev = front;

  while(idx != pos)
  {
    prev = curr; //stay back
    curr = curr->next;
    ++idx;
  }
  //when we find the correct position, update pointers
  prev->next = curr->next; //bypass the index you are going to delete
  curr->next = NULL;
  T temp = curr->data;
  delete curr;
  size--;

  return temp;
}

template <class T>
void DoublyLinked<T>::printList()
{
  ListNode<T> *curr = front;
  while(curr)
  {
    cout << curr->data << endl;
    curr = curr->next;
  }
}

template <class T>
int DoublyLinked<T>::find(T val)
{
  int idx = -1;
  ListNode<T> *curr = front;

  while(curr != NULL)
  {
    ++idx;
    if(curr->data == val)
    {
      break;
    }
    else
    {
      curr = curr->next;
    }
  }

  if(curr == NULL)
  {
    idx = -1;
  }

  return idx;
}

template <class T>
void DoublyLinked<T>::insertBack(T data)
{
  ++size;
  ListNode<T>* node = new ListNode<T>(data);
  if(size == 0)
  {
    front = node;
  }
  else
  {
    back->next = node;
    node->prev = back;
  }
  back = node;
}

template <class T>
T DoublyLinked<T>::removeBack()
{
  //very similar to remove front, on your own
  //check if empty
  ListNode<T>* node = back;
  if(back == NULL)
  {
    return;
  }

  //check if only one element exists
  if(front->next == NULL) //only one element in the list
  {
    back = NULL;
  }
  else //more than one element
  {
    back->prev->next = NULL;
  }
  back = front->prev;
  T temp = node->data;
  node->prev = NULL;

  delete node;
  size--;

  return temp;
}

template <class T>
unsigned int DoublyLinked<T>::getSize()
{
  return size;
}

template <class T>
bool DoublyLinked<T>::isEmpty()
{
  return size == 0;
}

template <class T>
ListNode<T>* DoublyLinked<T>::remove(int key)
{
  //could call find() here

  //is going to search manually
  ListNode<T> *curr = front;
  //let's look for the value
  while(curr->data != key)
  {
    curr = curr->next;
    if(curr == NULL)
    {
      return NULL; //break, do not want to continue
    }
  }

  if(curr == front) //front node
  {
    front = curr->next;
  }
  else //in the middle
  {
    curr->prev->next = curr->next; //jumping over current
  }
  if(curr == back)
  {
    back = curr->prev;
  }
  else
  {
    curr->next->prev = curr->prev;
  }
  curr->next = NULL;
  curr->prev = NULL;

  size--;

  return curr;
}
