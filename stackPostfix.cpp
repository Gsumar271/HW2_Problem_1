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
