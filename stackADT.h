#ifndef H_StackADT
#define H_StackADT

template <class Type>
class stackADT
{
public:
    //Method to initialize the stack to an empty state.
    virtual void initializeStack() = 0;
    
    //Function to determine whether the stack is empty.
    virtual bool isEmptyStack() const = 0;

    //Function to determine whether the stack is full.
    virtual bool isFullStack() const = 0;
    
    //Function to add newItem to the stack.
    virtual void push(const Type& newItem) = 0;
    
    //Function to return the top element of the stack.
    virtual Type top() const = 0;

    //Function to remove the top element of the stack.
    virtual void pop() = 0;

};

#endif