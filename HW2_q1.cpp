//***********************************************************
// Author: D.S. Malik
//
// Program: Postfix Calculator  
// This program evaluates postfix expressions.
//***********************************************************
#include <iostream>  
#include <iomanip>
#include <fstream>
#include "stackType.h"
//typedef stack_mystack stackType;


//commented postfix calculator 
//class
using namespace std; 

//evaluates entire expression
void evaluateExpression(ifstream& inpF, ofstream& outF, 
                        stackType<double>& stack,
                        char& ch, bool& isExpOk);
//evaluate the operator                     
void evaluateOpr(ofstream& out, stackType<double>& stack,
                 char& ch, bool& isExpOk);
//discard expression                 
void discardExp(ifstream& in, ofstream& out, char& ch);
//prints the result
void printResult(ofstream& outF, stackType<double>& stack,
                 bool isExpOk);

int main()
{
    //variable to flag if expression is valid
    bool expressionOk;
    //variable to read in input characters 
    char ch;
    //variable to hold stack
    stackType<double> stack(100);
    //input stream variable
    ifstream infile;
    //output strean variable 
    ofstream outfile;
    
    //open the input file
    infile.open("RpnData.txt");

    //error if there is no input file
    if (!infile)
    {
        cout << "Cannot open the input file. "
             << "Program terminates!" << endl;
        return 1;
    }
    
    //open the file for output
    outfile.open("RpnOutput.txt");
    //format the output to a fixed decimal format 
    //and for 2 decimal places 
    outfile << fixed << showpoint;
    outfile << setprecision(2); 

    //write character to input file 
    infile >> ch;
    //while we haven't reached the end of input file 
    while (infile)
    {
        //initialize stack and output next character
        stack.initializeStack();
        expressionOk = true;
        outfile << ch;

        //evaluate the expression
        evaluateExpression(infile, outfile, stack, ch, 
                           expressionOk);
        //print the result of expression
        printResult(outfile, stack, expressionOk);
        infile >> ch; //begin processing the next expression
    } //end while 

    //close all files at the end of program
    infile.close();
    outfile.close();

    return 0;

} //end main


//function to evaluate expression, will need 2 numbers one the stack
//otherwise expression is not valid 
//if expression is not valid it is discarded
void evaluateExpression(ifstream& inpF, ofstream& outF, stackType<double>& stack,char& ch, bool& isExpOk)
{
    double num;

    //keep going while character is not '='
    //which means expression is at the end
    while (ch != '=')
    {
        //switch depending on the character encountered
        switch (ch)
        {
        //if the character is '#'
        case '#': 
            //read the number
            inpF >> num;
            //output the number
            outF << num << " ";
            //if stack is not yet full
            //push the number on the stack
            if (!stack.isFullStack())
                stack.push(num);
            else
            {
                //if stack is full print error and close 
                cout << "Stack overflow. "
                     << "Program terminates!" << endl;
                exit(0);  //terminate the program
            }
            break;
        //otherwise char is an operator
        //so evaluate the expression
        default: 
            evaluateOpr(outF, stack, ch, isExpOk);
        }//end switch

        if (isExpOk) //if no error
        {
            //get character
            inpF >> ch;
            //output the character
            outF << ch;
            //print space if char is not '#'
            if (ch != '#')
                outF << " ";
        }
        else
            //error was discovered so call 
            //a function to discard this expression
            discardExp(inpF, outF, ch);
    } //end while (!= '=')
} //end evaluateExpression

//this function will evaluate the operation
void evaluateOpr(ofstream& out, stackType<double>& stack,
              char& ch, bool& isExpOk)
{
    double op1, op2;

    //if the stack is empty
    if (stack.isEmptyStack())
    {
        //print out error message
        out << " (Not enough operands)";
        //set expok variable to false
        isExpOk = false;
    }
    else
    {
        //get the top of the stack 
        //for the second operand
        op2 = stack.top();
        //pop the top element from the stack
        stack.pop();

        //if stack is empty 
        if (stack.isEmptyStack())
        {
            //output error message
            out << " (Not enough operands)";
            isExpOk = false;
        }
        else
        {
            //get the top of the stack for the first operand
            op1 = stack.top();
            //pop the top element from the stack 
            stack.pop();

            //check the next operator character 
            switch (ch)
            {
            //if the next operator is '+' addition 
            case '+': 
                //push the result of the addition operation 
                //on top of the stack 
                stack.push(op1 + op2);
                break;
            //if the next operator is '-' subtraction
            case '-': 
                //push the result of the subtraction operation 
                //on top of the stack
                stack.push(op1 - op2);
                break;
            //if the next operator is '*' multiplication
            case '*': 
                //push the result of the multiplication operation 
                //on top of the stack
                stack.push(op1 * op2);
                break;
            //if the next operator is '/' division
            case '/': 
                //if the second operand is not 0
                //push the result of the division operation 
                //on top of the stack
                if (op2 != 0)
                    stack.push(op1 / op2);
                else
                {
                    //if the second operator is 0
                    //display error message
                    out << " (Division by 0)";
                    isExpOk = false;
                }
                break;
            default:  
                //any other operator or character
                //display error message 
                out << " (Illegal operator)";
                isExpOk = false;
            }//end switch
        } //end else
    } //end else
} //end evaluateOpr

//function called after error is found in expression
//each character will be read until '=' char
void discardExp(ifstream& in, ofstream& out, char& ch)
{
    //as long as we are not at the end of expression
    while (ch != '=')
    {
        //read character and output it
        in.get(ch);
        out << ch;
    }
} //end discardExp

//print the result of the operation 
void printResult(ofstream& outF, stackType<double>& stack,
                 bool isExpOk)
{
    double result;

    //print the result if expression is ok,
    // has no errors 
    if (isExpOk) 
    {
        //if the stack is not empty
        if (!stack.isEmptyStack())
        {
            //get the top of the stack
            //into the results variable
            result = stack.top();
            //pop the stack
            stack.pop();
            //if no more elements left on the stack
            if (stack.isEmptyStack())
                //print the result into output file 
                outF << result << endl;
            else
                //otherwise extra elements on the stack
                //output the error message 
                outF << " (Error: Too many operands)" << endl;
        } //end if
        else
            //output error
            outF << " (Error in the expression)" << endl;
    }
    else
        //output error
        outF << " (Error in the expression)" << endl;

    outF << "_________________________________" 
         << endl << endl;
} //end printResult


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

//definition of templates class
//linked stack
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