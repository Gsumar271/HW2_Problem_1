//Header File: linkedStack.h 
#ifndef H_StackLinkedList
#define H_StackLinkedList
 
#include <iostream>
#include <cassert> 
 
#include "stackADT.h"
using namespace std;

//define the node class for
//linked list structure 
template <class Type>
struct nodeType
{
    Type info;
    nodeType<Type> *link;
};

//the class inherits from the stack
//abstract data type class
template <class Type>
class linkedStackType: public stackADT<Type>
{
public:
    //overload assignment operator 
    const linkedStackType<Type>& operator=
                              (const linkedStackType<Type>&);

    //function returns true if the stack is empty
    bool isEmptyStack() const;

    //determines if the stack is full
    //returns true if it is 
    bool isFullStack() const;

    //initializes stack by making it empty
    void initializeStack();

    //pushes the element on top of the stack
    //inserts or adds new element
    void push(const Type& newItem);

    //returns the top of the stack 
    Type top() const;

    //pops the element from the top of the stack
    //removing that element
    void pop();

    //constructor for this class
    linkedStackType(); 

    //copy constructor for this stack 
    linkedStackType(const linkedStackType<Type>& otherStack); 
  
    //destructor for this class
    ~linkedStackType();

private:
    nodeType<Type> *stackTop; //pointer to the stack
    //makes copy of the other stack 
    void copyStack(const linkedStackType<Type>& otherStack); 

};


//constructor intitializes the stack 
//by making stackTop variable point to NULL
//thus stack is empty in the beginning    
template <class Type> 
linkedStackType<Type>::linkedStackType()
{
    stackTop = nullptr;
}

//this function returns null if stackTop points to null pointer
//which means stack is empty 
template <class Type>
bool linkedStackType<Type>::isEmptyStack() const
{
    return(stackTop == nullptr);
} 

//this stack is full only when program runs out of memory 
//simce it is allocated dynamically, so it always returns false
template <class Type>
bool linkedStackType<Type>:: isFullStack() const
{
    return false;
} 

//this function initializes the stack back to an original
//empty state, all the elements in previous stack must be deleted
template <class Type>
void linkedStackType<Type>:: initializeStack()
{
    //use temp node variable
    nodeType<Type> *temp; 
    //while end of list is not reached
    while (stackTop != nullptr) 
    {   
        //make temp equal to current pointer
        temp = stackTop;    
        //advance the pointer to the next node
        stackTop = stackTop->link; 
        //delete temp node                           
        delete temp;   
    }
} 

//function to insert element on the stack 
template <class Type>
void linkedStackType<Type>::push(const Type& newElement)
{
    //new node variable
    nodeType<Type> *newNode;  
    //create new node
    newNode = new nodeType<Type>; 
    //set the info of new node to new element
    newNode->info = newElement; 
    //set the link of that node equal to previous top
    //inserting it on top 
    newNode->link = stackTop; 
    //set the new top equal to new node 
    stackTop = newNode;        
                             
} //end push

//to return the top element
template <class Type>
Type linkedStackType<Type>::top() const
{
    //make sure the stack is not empty 
    assert(stackTop != nullptr);             
    //return the top element info 
    return stackTop->info; 
}//end top

//function to remove the top element 
template <class Type>
void linkedStackType<Type>::pop()
{
    //make a temp node varable 
    nodeType<Type> *temp;   
    //if stack is not empty 
    if (stackTop != nullptr)
    {
        //set the temp node equal to the top
        //node element 
        temp = stackTop;  
        //advance the top element to the next node in 
        //the list 
        stackTop = stackTop->link; 
        //delete the node pointed to by temp
        //removing it from memory                             
        delete temp;    
    }
    else
        cout << "Cannot remove from an empty stack." << endl;
}//end pop

//function to copy the stack 
template <class Type> 
void linkedStackType<Type>::copyStack
                     (const linkedStackType<Type>& otherStack)
{
    //used 3 new node variable for this operation 
    nodeType<Type> *newNode, *current, *last;
    //make the stack empty if it is not empty 
    if (stackTop != nullptr) 
        initializeStack();
    //if stack to copy is empty
    //make this stack empty 
    if (otherStack.stackTop == nullptr)
        stackTop = nullptr;
    else
    {
        //set current node to point 
        //to the top of other stack
        current = otherStack.stackTop;  

        //allocate new node stackTop
        stackTop = new nodeType<Type>;  

        //copy the info from other stack node
        //to this stack 
        stackTop->info = current->info; 
        //set the nodes link to null
        stackTop->link = nullptr;  

        //set the last node to point to this node                       
        last = stackTop; 
        //set the current node to the next node 
        //in the list        
        current = current->link;    

        //while current node is not empty
        //we are adding nodes in reverse order 
        //so that stack is copied correctly
        //current points to the other stack
        //while newNode is being inserted into new stack
        while (current != nullptr)
        {
            //allocate new node
            newNode = new nodeType<Type>;
            //copy info from other stack into new node
            newNode->info = current->info;
            //set newnode's link to null
            newNode->link = nullptr;
            //make pointer last point to new node
            //thus building stack backwards
            last->link = newNode;
            //set last as newnode
            last = newNode;
            //advance current in the other stack
            current = current->link;
        }
    }
} 

 //copy constructor function
template <class Type>   
linkedStackType<Type>::linkedStackType(
                      const linkedStackType<Type>& otherStack)
{
    stackTop = nullptr;
    //call the copystack method
    copyStack(otherStack);
}

//destructor method
template <class Type> 
linkedStackType<Type>::~linkedStackType()
{
    initializeStack();
}

//overload the assignment operator by calling copyStack function
template <class Type>   
const linkedStackType<Type>& linkedStackType<Type>::operator=
    			  (const linkedStackType<Type>& otherStack)
{ 
    //make sure the stack is not being
    //reassigned to itself
    if (this != &otherStack) 
        copyStack(otherStack);

    return *this; 
}

#endif