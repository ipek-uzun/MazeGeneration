//
//  Stack.hpp
//
//
//
//

#ifndef Stack_h
#define Stack_h
#include <iostream>
#include <fstream>
#include <string>

// Template class representing a stack data structure
template <class Object>
class Stack
{
public:
    //Constructor
    Stack() {
        topOfStack = NULL;
    }
 
//Destructor
    ~Stack() {
        while (!isEmpty())
            pop();
    }
    
    //Member function to check if the stack is empty
    bool isEmpty() const {
        return (topOfStack == nullptr);
    }
    
    
    
    // Member function to pop up elements from the stack
    void pop()
    {
        if (!isEmpty())
        {
            ListNode * newTop = topOfStack->next;
            delete topOfStack;
            topOfStack = newTop;
            
        }
    }
    // Insert x into stack
    void push(const Object &x) {
        topOfStack = new ListNode(x, topOfStack);
    }
    
    // Return the top element of the stack
    const Object &top() const
    {
        if (!isEmpty())
        {
            return topOfStack->element;
        }
        return topOfStack->element;
    }
  
    // Member function to set the attributes of the top element based on the given direction
    void setTop(int direction)
    {
        if (!isEmpty())
        {
            if(direction == 1){topOfStack->element.left = false;}
            else if(direction == 2) {topOfStack->element.right = false;}
            else if(direction == 3) {topOfStack->element.up =false;}
            else if(direction == 4) {topOfStack->element.down = false;}
        }
    }

private:
    // Struct representing a node in the stack
    struct ListNode
    {
        Object element;
        ListNode *next;
        // Constructor for a ListNode with given element and next node
        ListNode(const Object &theElement, ListNode *n = NULL)
            : element(theElement), next(n) {}
    };
    // Pointer to the top of the stack
    ListNode *topOfStack;
};



#endif /* Stack_hpp */
