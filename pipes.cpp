#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <stdlib.h>

using namespace std;

int processCounter = 0;
    
int prim(int nr)
{
    for(int i = 2; i <= nr / 2; i++)
    {
        if(nr % i == 0)
        {
            return 0;
        }
    }
    return nr;
}

void childProc(int readPipe, int writePipe) {
    int leftInterval;
    int rightInterval;
    int nr;
    int primeNumbers[200];
    int position = 0;
    int size = 0;
    
    read(readPipe, &leftInterval, sizeof(leftInterval));
    read(readPipe, &rightInterval, sizeof(rightInterval));
    
    for(int i = leftInterval; i < rightInterval; i++)
    {
        nr = prim(i);
        if(nr != 0)
        {
            primeNumbers[position++] = nr;
            size++;
        }
    }
    
    write(writePipe, &size, sizeof(int));
    write(writePipe, primeNumbers, sizeof(primeNumbers));
    
    cout << "Process "<< processCounter <<" has ended."<<endl;
    
    exit(1);
}

void parentProc(int readPipe, int writePipe) {
    int leftInterval;
    int rightInterval;
    int primeNumbers[200];
    int size;
    
    leftInterval = 1000 * (processCounter - 1);
    rightInterval = 1000 * processCounter;
    
    write(writePipe, &leftInterval, sizeof(int));
    write(writePipe, &rightInterval, sizeof(int));
    
    read(readPipe, &size, sizeof(int));
    read(readPipe, primeNumbers, sizeof(primeNumbers));
    
    for(int i = 0; i < size; i++)
    {
        cout << primeNumbers[i] << " ";   
    }
    cout << endl;
}

int main() {

        int firstPipe[2];
        int secondPipe[2];
        int pid;
       
        if (pipe(firstPipe)) {
                cout << "Error creating pipe!\n";
                exit(-1);
        }
	
        if (pipe(secondPipe)) {
                cout << "Error creating pipe!\n";
                exit(-1);
        }
    
	for(int i = 0; i < 10; i++)
        {
                pid = fork();
                processCounter++;
                
                if (pid)
		        {
                        if (pid == -1) 
			            {
                           cout << "Error creating child process!\n";
                                exit(-1);
                        }
                        parentProc(secondPipe[0],firstPipe[1]);
                }
                else
                {
                        childProc(firstPipe[0], secondPipe[1]);
		        }
                              
        }
}

