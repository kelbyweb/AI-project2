//
//  Project 2: Towers of Hanoi
//  towersOfHanoi.cpp Heuristic Functions Driver
//  Created by Kelby Webster (klw231)
//  CS 4346 Intro to AI TXST Fall 2019
//  Instructed by Dr. Moonis Ali
//

#include <iostream>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <math.h>
#include <stack>
#include <limits.h>
#include <fstream>
#include <unistd.h>

using namespace std;

int total = 0; // global variable

// a class to represent the recursive stack
class Stack_recur
{
    public:
        int name;
        int data[10];
        int top;
    Stack_recur() // constructor
    {
        top = -1;
    }
    void push(int n)
    {
        top++;
        data[top] = n;
    }
    int pop()
    {
        return(data[top--]);
    }
};

// A structure to represent the iterative stack
struct Stack_iter
    {
    public:
        int name;
        int data[10];
        unsigned capacity;
        int top;
        int *array;
        void push(struct Stack_iter *stack, int item);
        void pushDisplay(int n);
    };

// function to create a stack of given capacity
struct Stack_iter* createStack(unsigned capacity)
{
    struct Stack_iter* stack = (struct Stack_iter*) malloc(sizeof(struct Stack_iter));
    stack -> capacity = capacity;
    stack -> top = -1;
    stack -> array = (int*) malloc(stack -> capacity * sizeof(int));
    return stack;
}
  
// Stack is full when top is equal to the last index
int isFull(struct Stack_iter* stack)
    {
        return (stack->top == stack->capacity - 1);
    }
  
// Stack is empty when top is equal to -1
int isEmpty(struct Stack_iter* stack)
    {
        return (stack->top == -1);
    }
  
// Function to add an item to stack. It increases
// top by 1
void push(struct Stack_iter *stack, int item)
{
    if (isFull(stack))
        return;
    stack -> array[++stack -> top] = item;
}
  
// Function to remove an item from stack. It
// decreases top by 1
int pop(struct Stack_iter* stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack -> array[stack -> top--];
}

void show(struct Stack_iter &A_iter, struct Stack_iter &B_iter, int disk)
{
    cout << "Move disk " << disk << " from rod " << A_iter.name << " to rod " << B_iter.name << endl;
}

// get memory usage for each algorithm
void process_mem_usage(double& vm_usage, double& resident_set)
{
    vm_usage = 0.0;
    resident_set = 0.0;

    // the two fields we want
    unsigned long vsize;
    long rss;
    {
        std::string ignore;
        std::ifstream ifs("/proc/self/stat", std::ios_base::in);
        ifs >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore >> ignore
                >> ignore >> ignore >> vsize >> rss;
    }

    long page_size_kb = sysconf(_SC_PAGE_SIZE) / 1024; // in case x86-64 is configured to use 2MB pages
    vm_usage = vsize / 1024.0;
    resident_set = rss * page_size_kb;
}

void moveDiskIter(struct Stack_iter &A_iter, struct Stack_iter &B_iter)
{
    int rod1TopDisk = pop(&A_iter);
    int rod2TopDisk = pop(&B_iter);
  
    // When rod 1 is empty
    if (rod1TopDisk == INT_MIN)
    {
        push(&A_iter, rod2TopDisk);
        show(B_iter, A_iter, rod2TopDisk);
    }
  
    // When rod2 rod is empty
    else if (rod2TopDisk == INT_MIN)
    {
        push(&B_iter, rod1TopDisk);
        show(A_iter, B_iter, rod1TopDisk);
    }
  
    // When top disk of rod1 > top disk of rod2
    else if (rod1TopDisk > rod2TopDisk)
    {
        push(&A_iter, rod1TopDisk);
        push(&A_iter, rod2TopDisk);
        show(B_iter, A_iter, rod2TopDisk);
    }
  
    // When top disk of rod1 < top disk of rod2
    else
    {
        push(&B_iter, rod2TopDisk);
        push(&B_iter, rod1TopDisk);
        show(A_iter, B_iter, rod1TopDisk);
    }
}

// A* iterative approach function
int aStarSearch(int disks, struct Stack_iter *sourceRod, struct Stack_iter *auxiliaryRod, struct Stack_iter *destinationRod)
    {
     int i, total_num_of_moves;
 
    // If number of disks is even, then interchange dest rod and aux rod
      if (disks % 2 == 0)
        {
            struct Stack_iter temp = *destinationRod;
            destinationRod = auxiliaryRod;
            auxiliaryRod = &temp;
        }
       total_num_of_moves = pow(2, disks) - 1;
        
        //Larger disks will be pushed first
        for (i = disks; i >= 1; i--)
            push(sourceRod, i);
      
        for (i = 1; i <= total_num_of_moves; i++)
        {
            if (i % 3 == 1)
            {
                moveDiskIter(*sourceRod, *destinationRod);
            }
      
            else if (i % 3 == 2)
            {
                moveDiskIter(*sourceRod, *auxiliaryRod);
            }
      
            else if (i % 3 == 0)
            {
                moveDiskIter(*auxiliaryRod, *destinationRod);
            }
        }
        return total_num_of_moves;
}


void moveDiskRecur(Stack_recur &A_recur, Stack_recur &B_recur)
{
    int disk = A_recur.pop();
    cout << "Move disk " << disk << " from rod " << A_recur.name << " to rod "
    << B_recur.name << endl;
    B_recur.push(disk);
    total += 1;
}

// RBFS recursion approach function
void recursiveBestFirstSearch(int disks, Stack_recur &A, Stack_recur &B, Stack_recur &C)
{
          if(disks >= 1)
          {
              recursiveBestFirstSearch((disks - 1), A, C, B);
              moveDiskRecur(A,B);
              recursiveBestFirstSearch((disks - 1), C, B, A);
          }
}
  

// function to display results
void displayResults(int disks, int moves)
{
    int base = 2;
    int exponent = disks;
    int optimal = pow(base, exponent) - 1;
    
    cout << "Number of nodes generated = " << endl;
    cout << "Number of nodes expanded = " << endl;
    cout << "Total number of disks = " << disks << endl;
    cout << "Optimal number of moves = " << optimal << endl;
    cout << "Total/Actual number of moves = " << moves << endl;
    cout << "**********************************" << endl;
}


// main function
int main()
{
    int disks, userInput;
    double vm, rss;
    bool continueProgram = true;
    
    clock_t start_recursive, start_iterative;
    clock_t end_recursive, end_iterative;
    
    
   while(continueProgram == true)
     {
    // user inputs disk number
    cout << "\nPlease enter the number of disks (must be at least 3) :" << endl;
    cin >> disks;
    
    // for invalid disk number input
    while(disks < 3)
    {
        cout << "Try again: the number of disks must be at least 3." << endl;
        cout << "Please enter the number of disks : " << endl;
        cin >> disks;
    }

         // initializing stack objects for recursive function
         Stack_recur A_recur, B_recur, C_recur;
         A_recur.name = 1;
         B_recur.name = 2;
         C_recur.name = 3;
         
         // initializing struct stack objects for iterative function
         struct Stack_iter *A_iter, *B_iter, *C_iter;
         A_iter = createStack(disks);
         B_iter = createStack(disks);
         C_iter = createStack(disks);
         A_iter->name = 1;
         B_iter->name = 2;
         C_iter->name = 3;
         
    // user picks algorithm or exits
        cout << endl << "Which algorithm would you like to run? " << endl;
        cout << "Please enter one of the following options:" << endl <<
        "\t 1: A*" << endl <<
        "\t 2: Recursive Best-First Search" << endl <<
        "\t 3: Exit program" << endl;
        cin >> userInput;
        cout << endl;
        
        switch(userInput)
        {
            case 1:
            {
                for(int i = disks; i >= 1; i--)
                {
                    push(A_iter, i);
                }
                start_iterative = clock();
                cout << "Initial state of the 3 rods : " << endl;
                cout << "Rod 1 has " << disks << " disks : ";
                for(int i = 0; i <= A_iter->top; i++)
                    {
                        cout << A_iter->array[i];
                        cout << " ";
                    }
                cout<<"\nRod 2 has " << B_iter->top + 1 << " disks : ";
                for(int i = 0; i <= B_iter->top; i++)
                    {
                        cout << B_iter->array[i];
                        cout << " ";
                    }
                cout << "\nRod 3 has " << C_iter->top + 1 << " disks : ";
                for(int i = 0; i <= C_iter->top; i++)
                    {
                        cout << C_iter->array[i];
                        cout << " ";
                    }
                cout << "\n**********************************" << endl;
                // call function to get number of moves
                int aStarMoves = aStarSearch(disks, A_iter, B_iter, C_iter);
                process_mem_usage(vm, rss);
                end_iterative = clock();
                cout << "**********************************" << endl;
                cout << "Final state of the 3 rods : " << endl;
                cout << "Rod 1 has " << A_iter->top + 1 << " disks : ";
                for(int i = 0; i <= A_iter->top; i++)
                    {
                        cout << A_iter->array[i];
                        cout << " ";
                    }
                cout<<"\nRod 2 has " << B_iter->top + 1 << " disks : ";
                for(int i = 0; i <= B_iter->top; i++)
                    {
                        cout << B_iter->array[i];
                        cout << " ";
                    }
                cout << "\nRod 3 has " << C_iter->top + 1 << " disks : ";
                for(int i = 0; i <= C_iter->top; i++)
                    {
                        cout << C_iter->array[i];
                        cout << " ";
                    }
                cout << "\n**********************************" << endl;
                cout << "Execution Time = " << static_cast < double >(end_iterative - start_iterative) / CLOCKS_PER_SEC << " seconds" << endl;
                cout << "Memory Used : " << endl;
                cout << "Virtual Memory = " << vm << " KB ; Resident Set Size : " << rss << " KB " <<  endl;
                displayResults(disks, aStarMoves);
            }
                break;
            
            case 2:
            {
                start_recursive = clock();
                for(int i = disks; i >= 1; i--)
                {
                    A_recur.push(i);
                }
                cout << "Initial state of the 3 rods : " << endl;
                cout << "Rod 1 has " << disks << " disks : ";
                for(int i = 0; i <= A_recur.top; i++)
                    {
                        cout << A_recur.data[i];
                        cout << " ";
                    }
                cout<<"\nRod 2 has " << B_recur.top + 1 << " disks : ";
                for(int i = 0; i <= B_recur.top; i++)
                    {
                        cout << B_recur.data[i];
                        cout << " ";
                    }
                cout << "\nRod 3 has " << C_recur.top + 1 << " disks : ";
                for(int i = 0; i <= C_recur.top; i++)
                    {
                        cout << C_recur.data[i];
                        cout << " ";
                    }
                cout << "\n**********************************" << endl;
                // call function to get number of moves
                recursiveBestFirstSearch(disks, A_recur, C_recur, B_recur);
                process_mem_usage(vm, rss);
                end_recursive = clock();
                cout << "**********************************" << endl;
                cout << "Final state of the 3 rods : " << endl;
                cout << "Rod 1 has " << A_recur.top + 1 << " disks : ";
                for(int i = 0; i <= A_recur.top; i++)
                    {
                        cout << A_recur.data[i];
                        cout << " ";
                    }
                cout<<"\nRod 2 has " << B_recur.top + 1 << " disks : ";
                for(int i = 0; i <= B_recur.top; i++)
                    {
                        cout << B_recur.data[i];
                        cout << " ";
                    }
                cout << "\nRod 3 has " << C_recur.top + 1 << " disks : ";
                for(int i = 0; i <= C_recur.top; i++)
                    {
                        cout << C_recur.data[i];
                        cout << " ";
                    }
                cout << "\n**********************************" << endl;
                cout << "Execution Time = " << static_cast < double >(end_recursive - start_recursive) / CLOCKS_PER_SEC << " seconds" << endl;
                cout << "Memory Used : " << endl;
                cout << "Virtual Memory = " << vm << " KB ; Resident Set Size : " << rss << " KB " <<  endl;
                displayResults(disks, total);
            }
                break;
            case 3:
                cout << "Closing program... " << endl;
                return 0;

            default:
                cout << "Invalid input - Try again." << endl;
        }
        
        char continueInput;
        cout << endl << "Would you like to run the program again? " << endl <<
        "Enter Y for yes or N for no : " << endl;
        cin >> continueInput;
            if(continueInput == 'Y' || continueInput == 'y')
            {
                continueProgram = true;
            }
        else if(continueInput == 'N' || continueInput == 'n')
            {
                cout << endl << "Closing program..." << endl;
                continueProgram = false;
                return 0;
            }
        else if (continueInput != 'Y' || continueInput != 'y' || continueInput != 'N' || continueInput != 'n' )
            {
                cout << "Invalid input - Try again." << endl;
            }
    }
    return 0;
}
