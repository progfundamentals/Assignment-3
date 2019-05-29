/*preprocessor directives*/

/*for printf function*/
#include <stdio.h> 
/*sizeof, malloc functions*/
#include <stdlib.h> 
/*strlen function*/
#include <string.h>

/*sets max size of source string*/
#define MAX_S 500 

/*creates a function which returns the run length encoded
string from the source string */
char* rle(char* source) 
{ 
    /*no. of times a character occurs in the source string*/
    int rlength; 
    char count[MAX_S]; 
    /*size of source string*/
    int size = strlen(source); 
  

  /*if every character contained in source is different than the rest, 
  the length of the location string will be long, about 2ce the size of 
  the source string*/
    char* floc = (char*)malloc(sizeof(char) * (size * 2 + 1)); 
    /*counting varaibles*/
    int f, p = 0, m; 
  
    /*searches each character of  source string individually*/
    for (f= 0; f <size; f++) { 
  
        /*saves the first instance of a character*/
        floc[p++] = source[f]; 
  
        /*searches through the number of times a character
        occurs in the source string */
        rlength = 1; 
        while (f + 1 < size && source[f] == source[f+ 1]) { 
            rlength++; 
             f++;
        } 
  
        /*saves rlength in the character array*/
        sprintf(count, "%d", rlength); 
  
        /* Copies the count[] to final location */
        for (m = 0; *(count + m); m++, p++) { 
             floc[p] = count[m]; 
        } 
    } 
  
    /*ends the final location string */
    floc[p] = '\0'; 
    return floc; 
} 
  
/*runs the rle function to compress a string with repeating characters*/
int main() 
{ 
    /*enter string here to be compressed, normally with repeating 
    characters*/
    char estring[] = "------"; 
    /*uses the string above and runs the rle function */
    char* comp = rle(estring); 
    /*prints result*/
    printf("%s\n", comp); 
    
} 



