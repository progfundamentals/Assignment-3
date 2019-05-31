/*******************************************************************************
 * A merged library including 3 layered linked list related functions,
 * grade calculations and mathetmatics. RLE and huffman. 
 * 
 * 
*******************************************************************************/
 
 /*******************************************************************************
 * Header files 
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
 /*******************************************************************************
 * Preprocessor directives
*******************************************************************************/
#define MAX_S 500 


/*******************************************************************************
 * List Structs
*******************************************************************************/
/* Assignment node (bottom level) */
struct assignnode {
    char aname[31];
    float weight, mark, markmax;
    struct assignnode * next;
};
typedef struct assignnode assign_t;

/* Subject node (mid level) */
struct subjectnode {
    char code[6], sname[31];
    int credit;
    struct subjectnode * down;
    assign_t * next;
};
typedef struct subjectnode subject_t;

/* User node (top level) */
struct usernode {
    char userid[21], uname[21];  
    unsigned long hash;
    int perm;
    struct usernode * next;
    subject_t * down;
};
typedef struct usernode user_t;

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/

unsigned long hash(unsigned char * str);
unsigned long hashpass(char * user, char * pass);
int upush(user_t ** head, char * userid, char * uname, unsigned long hash, int perm);
int spush(user_t ** head, char * userid, char * code, char * sname, int credit);
int apush(user_t ** head, char * userid, char * code, char * aname, float weight, float mark, float markmax);
void udelete(user_t ** head, char * userid);
void sdelete(user_t ** head, char * userid, char * code);
void adelete(user_t ** head, char * userid, char * code, char * aname);
void ufreeall(user_t ** head);
void sfreeall(user_t ** head, char * userid);
void afreeall(user_t ** head, char * userid, char * code);
user_t * usearch(user_t ** head, char * userid);
subject_t * ssearch(user_t ** head, char * userid, char * code);
assign_t * asearch(user_t ** head, char * userid, char * code, char * aname);
int save_db(user_t * head, char * str);
int load_db(user_t ** head, char * str);
float assign_avg(user_t * head, char * userid, char * code);
float WAM(user_t * head, char * userid);
float creditpoint(user_t * head, char * userid, char * code);
float GPA(user_t * head, char * userid);
int process_input(char *str);
void compress_db(char *str);
void uncompress_db(char *str);
int check_db(char *str);
char* rle(char* source);

/*******************************************************************************
 * This function pushes data to a newly allocated user_t node.
 * INPUTS: user_t ** head, char * userid, char * uname, unsigned long hash, 
 * int perm    OUTPUTS: Returns int 1 on success, 0 otherwise.
 * Author: Max McKay
*******************************************************************************/

int upush(user_t ** head, char * userid, char * uname, unsigned long hash, 
        int perm)
{
    /* allocating memory */
    user_t * new_node = (user_t *)malloc(sizeof(user_t));
    if(new_node != NULL) {
        /* copying data to node */
        strcpy(new_node->userid, userid);
        strcpy(new_node->uname, uname);
        new_node->hash = hash;
        new_node->perm = perm;
        /* arranging pointers */
        new_node->next = *head;
        new_node->down = NULL;
        
        *head = new_node;
        /* new node is now head of list*/
        return 1;
    }
    return 0;
}

/*******************************************************************************
 * This function pushes data to a newly allocated subject_t node.
 * INPUTS: user_t ** head, char * userid, char * code, char * sname, int credit
 * OUTPUTS: Returns int 1 on success, 0 otherwise.
 * Author: Max McKay
*******************************************************************************/
/* creates subject node at specified user node. returns 0 if failed. */
int spush(user_t ** head, char * userid, char * code, char * sname, int credit){
   /* pointer to appropriate user_t node */
   user_t * current = usearch(head, userid);
   if(current!=NULL)
   {
        /* allocate memory for subject_t node */
        subject_t * new_node = (subject_t *)malloc(sizeof(subject_t));
        if(new_node != NULL) {
            /* copy data into node */
            strcpy(new_node->code, code);
            strcpy(new_node->sname, sname);
            new_node->credit = credit;
            
            /* arranging pointers */
            new_node->next=NULL;
            new_node->down = current->down;
            
            current->down = new_node;
            /* new node is now between existing subject node and usernode.*/
            return 1;
        }
    }
    return 0;
}   

/*******************************************************************************
 * This function pushes data to a newly allocated assign_t node.
 * INPUTS: user_t ** head, char * userid, char * code, char * aname, 
 * float weight, float mark, float markmax
 * OUTPUTS: Returns int 1 on success, 0 otherwise.
 * Author: Max McKay
*******************************************************************************/
int apush(user_t ** head, char * userid, char * code, char * aname, 
float weight, float mark, float markmax) {
    /* pointer to appropriate subject_t node */
    subject_t * curr = ssearch(head, userid, code);
    if(curr!=NULL)
    {
        /* allocating memory for assign_t node */
        assign_t * new_node = (assign_t *)malloc(sizeof(assign_t));
        if(new_node != NULL) {
            /* copy data to new node */
            strcpy(new_node->aname, aname);
            new_node->weight = weight;
            new_node->mark = mark;
            new_node->markmax = markmax;
            /* setting up pointers */
            new_node->next = curr->next;

            curr->next = new_node;
            /* new node appended infront of existing assign nodes if any. */
            return 1;
        }
    }
    return 0;
} 


/*******************************************************************************
 * This function deletes selected usernode. Repairs pointers and frees mallocs.
 * INPUTS: user_t ** head, char * userid
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void udelete(user_t ** head, char * userid) {   
    /* free sub nodes of node about to delete */
    sfreeall(head, userid);
    user_t *prev = NULL, *curr = *head;
    /* check head node for desired userid */ 
    if(*head !=NULL && strcmp(userid, (*head)->userid)==0) {
        /* set next node as new head and free prev node*/
        prev = *head;
        if((*head)->next != NULL)
            *head = (*head)->next;
        else
            *head = NULL;
        free(prev);
        return; 
    }
    /* check following nodes for userid */
    while(curr !=NULL) {
        if(strcmp(userid, curr->userid)==0) {
            /* make previous node point to next node and free middle */
            if(curr->next != NULL)
                prev->next = curr->next;
            else
                prev->next = NULL;
            free(curr);
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

/*******************************************************************************
 * This function deletes selected subjectnode.
 * Repairs pointers and frees mallocs.
 * INPUTS: user_t ** head, char * userid, char * code
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void sdelete(user_t ** head, char * userid, char * code) {
    /* free sub nodes of node about to delete */
    afreeall(head, userid, code);
    /* pointer to appropriate user_t node */
    user_t *top = usearch(head, userid);
    if(top != NULL) {
        subject_t *prev = NULL, *curr = top->down;
        /* check head node */
        if(curr != NULL && strcmp(code, curr->code)==0) {
            if(curr->down != NULL)
                top->down = curr->down;
            else
                top->down = NULL;
            free(curr);
            return; 
        }
        /* check following nodes */
        while(curr !=NULL) {
            if(strcmp(code, curr->code)==0) {
                if(curr->down != NULL)
                    prev->down = curr->down;
                else
                    prev->down = NULL;
                free(curr);
            }
            prev = curr;
            curr = curr->down;
        }
    }
}

/*******************************************************************************
 * This function deletes selected assignnode. Repairs pointers and frees mallocs
 * INPUTS: user_t ** head, char * userid, char * code, char * aname
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void adelete(user_t ** head, char * userid, char * code, char * aname) {
    /* pointer to appropriate subject_t node */
    subject_t * top = ssearch(head, userid, code);
    if(top != NULL) {
        assign_t *prev = NULL, *curr = top->next;
        /* check head node */
        if(curr != NULL && strcmp(aname, curr->aname)==0) {
            if(curr->next != NULL)
                top->next = curr->next;
            else
                top->next = NULL;
            free(curr);
            return; 
        }
        /* check following nodes */
        while(curr !=NULL) {
            if(strcmp(aname, curr->aname)==0) {
                if(curr->next != NULL)
                    prev->next = curr->next;
                else
                    prev->next = NULL;
                free(curr);
            }
            prev = curr;
            curr = curr->next;
        }
    }
}

/*******************************************************************************
 * This function frees all nodes.
 * INPUTS: user_t ** head
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void ufreeall(user_t ** head) {
    user_t *next_node;
    while(*head != NULL) {
        next_node = (*head)->next;
        sfreeall(head, (*head)->userid);
        free(*head);
        *head = next_node;
        /*starting at header, increment head forward and free prev*/
    }
}  

/*******************************************************************************
 * This function frees all subject nodes under a particular user node.
 * INPUTS: user_t ** head, char * userid
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void sfreeall(user_t ** head, char * userid) {
    user_t * top = usearch(head, userid);
    if(top !=NULL) {
        subject_t *next_node, *curr = top->down;
        top->down = NULL;
        while(curr != NULL) {
            next_node = curr->down;
            afreeall(head, userid, curr->code);
            free(curr);
            curr = next_node;
            /*starting at user->down, increment curr downward and free prev*/
        }
    }
} 

/*******************************************************************************
 * This function frees all assign nodes under a particular subject node.
 * INPUTS: user_t ** head, char * userid
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void afreeall(user_t ** head, char * userid, char * code) {
    subject_t * top = ssearch(head, userid, code);
    if(top !=NULL) {
        assign_t *next_node, *curr = top->next;
        top->next = NULL;
        while(curr != NULL) {
            next_node = curr->next;
            free(curr);
            curr = next_node;
            /* starting at subject->next, increment forward and free prev */
        }
    }
}  


/*******************************************************************************
 * This function searches linked list for a userid match.
 * INPUTS: user_t ** head, char * userid
 * OUTPUTS: returns pointer to usernode on match found, null otherwise.
 * Author: Max McKay
*******************************************************************************/
user_t * usearch(user_t ** head, char * userid) {
    user_t * current = *head;
    while(current !=NULL) {
        /* check for match */
        if(strcmp(userid, current->userid)==0)
            return current;
        /* increment pointer forward */
        current = current->next;
    }
    return 0;
}


/*******************************************************************************
 * This function searches linked list for a userid & subject match.
 * INPUTS: user_t ** head, char * userid
 * OUTPUTS: returns pointer to subject node on match found, null otherwise.
 * Author: Max McKay
*******************************************************************************/
subject_t * ssearch(user_t ** head, char * userid, char * code) {
    /* start at appropriate usernode */
    user_t *top = usearch(head, userid);
    if(top != NULL) {
        subject_t * curr = top->down;
        while(curr !=NULL) {
            /* check for match */
            if(strcmp(code, curr->code)==0)
                return curr;
            /* increment pointer downward */
            curr = curr->down;
        }
    }
    return 0;
}

/*******************************************************************************
 * This function searches linked list for a userid & subject & assign match.
 * INPUTS: user_t ** head, char * userid, char * code, char * aname
 * OUTPUTS: returns pointer to assign node on match found, null otherwise.
 * Author: Max McKay
*******************************************************************************/
assign_t * asearch(user_t ** head, char * userid, char * code, char * aname) {
    /* start at appropriate subjectnode*/
    subject_t *top = ssearch(head, userid, code);
    if(top != NULL) {
        assign_t * curr = top->next;
        while(curr !=NULL) {
            /* check for match */
            if(strcmp(aname, curr->aname)==0)
                return curr;
            /*increment forward*/
            curr = curr->next;
        }
    }
    return 0;
}

/*******************************************************************************
 * This function saves all of the linked list to a text file. Nodes type is
 * retained through numbers at the begging of each nested printf.
 * INPUTS: user_t * head, char * str
 * OUTPUTS: returns 1 on success, 0 otherwise.
 * Author: Max McKay
*******************************************************************************/
int save_db(user_t * head, char * str) {
    FILE* fp = fopen(str, "w");
    if (fp!=NULL) {
        user_t * top = head;
        subject_t * middle = NULL;
        assign_t * bottom = NULL;
        if(head !=NULL) {
            /* user node loop start */
            while (top != NULL) {
                fprintf(fp, "1 '%s' '%s' %lu %d\n", top->userid, top->uname, top->hash, top->perm);
                middle = top->down;
                /* subject node loop start */
                while(middle!=NULL)
                {
                    fprintf(fp, "2 \t'%s' '%s' %d\n", middle->code, middle->sname, middle->credit);
                    bottom = middle->next;
                    /* assign node loop start */
                    while(bottom!=NULL)
                    {
                        fprintf(fp, "3 \t\t'%s' %.2f %.2f %.2f\n", bottom->aname, bottom->weight, bottom->mark, bottom->markmax);
                        /* increment through assign nodes */
                        bottom = bottom->next;
                    }
                    /* increment through subject nodes */
                    middle = middle->down;
                }
                /* increment through user nodes */
                top = top->next;
            }
        }
        fclose(fp);
        return 1;
    }
    return 0;
}

/*******************************************************************************
 * This function loads textfile into linked list. The first character of each
 * line is read to determine what type of sscanf and push to use to create list.
 * INPUTS: user_t * head, char * str
 * OUTPUTS: returns 1 on success, 0 otherwise.
 * Author: Max McKay
*******************************************************************************/
int load_db(user_t ** head, char * str) {
    ufreeall(head);
    FILE* fp = fopen(str, "r");  
    if (fp!=NULL)
    {
        char input[120], userid[21], uname[21], code[6], sname[31], aname[31];
        int perm, credit;
        unsigned long hash;
        float weight, mark, markmax;
        while(fgets(input, 120, fp)){  
            if(input[0] == '1') {     
                /* line is usernode */
                sscanf(input, "%*d '%[^']' '%[^']' %lu %d\n", userid, uname, &hash, &perm);
                upush(head, userid, uname, hash, perm);
            }
            else if (input[0] == '2') {
                /* line is subject node*/
                sscanf(input, "%*d '%[^']' '%[^']' %d\n", code, sname, &credit);
                spush(head, userid, code, sname, credit);
            }
            else if (input[0] == '3') {
                /* line is assign node*/
                sscanf(input, "%*d '%[^']' %f %f %f\n", aname, &weight, &mark, &markmax);
                apush(head, userid, code, aname, weight, mark, markmax);
            }
            else {
                return 0;
            }
        }
        fclose(fp);
        return 1;
    }
    return 0;
}
/*******************************************************************************
 * This function runs a system command to compress the database file.
 * INPUTS: char *str
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void compress_db(char *str) {
    char command[100];
    /* appending commands */
    sprintf(command, "gzip %s -q", str);
    system(command);
}
/*******************************************************************************
 * This function runs a system command to uncompress the database file.
 * INPUTS: char *str
 * OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void uncompress_db(char *str) {
    char command[100];
    /* appending commands */
    sprintf(command, "gunzip %s.gz -q", str);
    system(command);
}
/*******************************************************************************
 * This function hashes the input using a commonly used hashing algorithm.
 * INPUTS: unsigned char * str
 * OUTPUTS: unsigned long hash
 * Author: djb2 original function.
*******************************************************************************/
unsigned long hash(unsigned char * str)
{
    unsigned long hash = 5381;
    int c;
    /* increments through chars of string */
    while ((c = *str++))
        /* hash = hash*33 + c is repeated until string is depleted.*/
        hash = ((hash << 5) + hash) + c; 
        /* (hash<<5) is bitwise version of *33 */
    return hash;
}
/*******************************************************************************
 * This function appends the user and pass together and pads the input with a
 * long string before feeding into hash function.
 * INPUTS: char * user, char * pass
 * OUTPUTS: unsigned long hash
 * Author: Max McKay
*******************************************************************************/
unsigned long hashpass(char * user, char * pass)
{
    char string[42];
    strcpy(string, pass);
    /* appending user to password */
    strcat(string, user);
    unsigned char padding[100] = "1234567890123456789012345678901234\
5678901234567890123456789012345678901234567890";
    /* copy string to padding */
    memcpy(padding, string, strlen(string));
    unsigned long digest;
    /* produce hash of padded string */
    digest = hash(padding);
    return digest;
}

/*******************************************************************************
 * This function produces a weighted subject average of completed assigns.
 * INPUTS: user_t * head, char * userid, char * code
 * OUTPUTS: float average. -1 if there are no assignments.
 * Author: Max McKay
*******************************************************************************/

float assign_avg(user_t * head, char * userid, char * code) {
    /* appropriate subject_t node pointer for userid and code */
    subject_t * top = ssearch(&head, userid, code);
    if(top!=NULL && top->next!=NULL) {
        assign_t * curr = top->next;
        float num = 0;
        float den = 0;
        while(curr!=NULL)
        {
            num = num + (curr->weight * curr->mark / curr->markmax);
            den = den + curr->weight;
            curr = curr->next;
            /* ongoing calculation of denominator and numerator of average */
        }
        float average = num / den * 100;
        return average;
    }
    return -1;
}

/*******************************************************************************
 * This function produces a weighted average mark of all subjects together.
 * INPUTS: user_t * head, char * userid
 * OUTPUTS: float average. -1 if there are no subjects.
 * Author: Max McKay
*******************************************************************************/
float WAM(user_t * head, char * userid) {
    /* pointer to appropriate userid */
    user_t * top = usearch(&head, userid);
    if(top!=NULL) {
        subject_t * curr = top->down;
        float num=0, den=0;
        while(curr!=NULL)
        {
            /* skips subject if no assignments */
            if(assign_avg(head, userid, curr->code)!=-1) {
            num = num + (curr->credit * assign_avg(head, userid, curr->code));
            den = den + curr->credit;
            }
            curr = curr->down;
            /* ongoing calculation of denominator and numerator of average */
        }
        float WAM = num / den;
        return WAM;
    }
    return -1;
}

/*******************************************************************************
 * This function produces a credit point equivalent of a subjects average mark.
 * INPUTS: user_t * head, char * userid, char * code
 * OUTPUTS: float credit. -1 if assign_avg fails.
 * Author: Max McKay
*******************************************************************************/
/* returns a creditpoint equivalent of subject mark. -1 assign_avg fails */
float creditpoint(user_t * head, char * userid, char * code) {
    float credit;
    if(assign_avg(head, userid, code)==-1)
        credit = -1;
    else if(assign_avg(head, userid, code)>=0 &&assign_avg(head, userid, code)<50) 
        credit = 0.5;
    else if (assign_avg(head, userid, code)>=50 && assign_avg(head, userid, code)<65) 
        credit = 1.5;
    else if (assign_avg(head, userid, code)>=65 && assign_avg(head, userid, code)<75) 
        credit = 2.5;
    else if (assign_avg(head, userid, code)>=75 && assign_avg(head, userid, code)<85) 
        credit = 3.5;
     else if (assign_avg(head, userid, code)>=85)
         credit = 4;
     return credit;
}

/*******************************************************************************
 * This function produces an overall GPA of all subject creditpoint values.
 * INPUTS: user_t * head, char * userid
 * OUTPUTS: float GPA. -1 if no subjects.
 * Author: Max McKay
*******************************************************************************/

float GPA(user_t * head, char * userid) {
    /* pointer to appropriate userid */
    user_t * top = usearch(&head, userid);
    if(top!=NULL && top->down != NULL) {
        subject_t * curr = top->down;
        float num=0, den=0;
        while(curr!=NULL)
        {
            if(creditpoint(head, userid, curr->code)!=-1) { 
            num = num + (curr->credit * creditpoint(head, userid, curr->code));
            den = den + curr->credit;
            /*GPA formula*/
            }
            curr = curr->down;
        }
        float GPA = num / den;
        return GPA;
    }
    return 0;
}
/*******************************************************************************
 * Checks for the presence of the null terminator in a string to determine if
 * fgets overflow occurs. If it has occured, clear buff.
 * INPUTS: char *str
 * OUTPUTS: 1 if no overflow. 0 if overflow occured.
 * Author: Max McKay
*******************************************************************************/

int process_input(char *str)
{
    char *p;
    if((p=strchr(str, '\n'))==NULL) /* returns '\n' location if present */
    {
        while (getchar() != '\n');  /* clears buffer until '\n' */
        return(0);
    } 
    *p = '\0';                      /* replaces '\n' with '\0' */
    return(1);
}



/*******************************************************************************
 * Run length encoding compresses inputed string. 
 * INPUTS: char *str
 * OUTPUTS: char *str
 * Author: Felicity Rodricks
*******************************************************************************/
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

/*******************************************************************************
 * Xor encrypt string.  
 * INPUTS: char* input,char* key,unsigned long inLen,unsigned long keyLen
 * OUTPUTS: char *str
 * Author: Felicity Rodricks
*******************************************************************************/
char* crypt(char* input,char* key,unsigned long inLen,unsigned long keyLen) 
{
    /*allocates memory to the encrypted string*/
    char* output = (char*)malloc(sizeof(char)*inLen+1);
    /*sets the input string's length to 0*/
    output[inLen] = 0;
    /*decrypts the input string one character at a time*/
    int i;
    for(i = 0; i < inLen; ++i) {
        
        output[i] = input[i] ^ key[i % keyLen];
    }
    return output;
}
