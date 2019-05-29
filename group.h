#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*****************************************************************************/
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

/*****************************************************************************/
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


/*****************************************************************************/



/* produces a hash of input string */
unsigned long hash(unsigned char * str)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; 
    return hash;
}

/* pads user input and then hashes */
unsigned long hashpass(char * user, char * pass)
{
    char string[42];
    strcpy(string, pass);
    strcat(string, user);
    unsigned char padding[100] = "12345678901234567890123456789012345678901234567890123456789012345678901234567890";
    memcpy(padding, string, strlen(string));
    unsigned long digest;
    digest = hash(padding);
    return digest;
}

/* creates new usernode. returns 0 if failed.*/
int upush(user_t ** head, char * userid, char * uname, unsigned long hash, int perm)
{
    user_t * new_node = (user_t *)malloc(sizeof(user_t));
    if(new_node != NULL) {
        strcpy(new_node->userid, userid);
        strcpy(new_node->uname, uname);
        new_node->hash = hash;
        new_node->perm = perm;
        new_node->next = *head;
        new_node->down = NULL;
     
        *head = new_node;
        return 1;
    }
    return 0;
}

/* creates subject node at specified user node. returns 0 if failed. */
int spush(user_t ** head, char * userid, char * code, char * sname, int credit) {
   user_t * current = usearch(head, userid);
   if(current!=NULL)
   {
        subject_t * new_node = (subject_t *)malloc(sizeof(subject_t));
        if(new_node != NULL) {
            strcpy(new_node->code, code);
            strcpy(new_node->sname, sname);
            new_node->credit = credit;
            new_node->next=NULL;
            new_node->down = current->down;

            current->down = new_node;
            return 1;
        }
    }
    return 0;
}   

/* creates assignment node at specified user->subject node. returns 0 if failed. */
int apush(user_t ** head, char * userid, char * code, char * aname, float weight, float mark, float markmax) {
    subject_t * curr = ssearch(head, userid, code);
    if(curr!=NULL)
    {
        assign_t * new_node = (assign_t *)malloc(sizeof(assign_t));
        if(new_node != NULL) {
            strcpy(new_node->aname, aname);
            new_node->weight = weight;
            new_node->mark = mark;
            new_node->markmax = markmax;
            new_node->next = curr->next;

            curr->next = new_node;
            return 1;
        }
    }
    return 0;
} 


/* deletes searched user node. */
void udelete(user_t ** head, char * userid) {   
    sfreeall(head, userid);
    user_t *prev = NULL, *curr = *head;
    /* check head node */ 
    if(*head !=NULL && strcmp(userid, (*head)->userid)==0) {
        prev = *head;
        if((*head)->next != NULL)
            *head = (*head)->next;
        else
            *head = NULL;
        free(prev);
        return; 
    }
    /* check following nodes */
    while(curr !=NULL) {
        if(strcmp(userid, curr->userid)==0) {
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

/* deletes searched subject node */
void sdelete(user_t ** head, char * userid, char * code) {
    afreeall(head, userid, code);
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

/* deletes searched assignment node */
void adelete(user_t ** head, char * userid, char * code, char * aname) {
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

/* frees all user nodes */
void ufreeall(user_t ** head) {
    user_t *next_node;
    while(*head != NULL) {
        next_node = (*head)->next;
        sfreeall(head, (*head)->userid);
        free(*head);
        *head = next_node;
    }
}  

/* frees all subjects nodes of a user node */
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
        }
    }
} 

/* free all assignment nodes of a subject node */
void afreeall(user_t ** head, char * userid, char * code) {
    subject_t * top = ssearch(head, userid, code);
    if(top !=NULL) {
        assign_t *next_node, *curr = top->next;
        top->next = NULL;
        while(curr != NULL) {
            next_node = curr->next;
            free(curr);
            curr = next_node;
        }
    }
}  

/* returns a pointer to searched usernode. NULL if none found */
user_t * usearch(user_t ** head, char * userid) {
    user_t * current = *head;
    while(current !=NULL) {
        if(strcmp(userid, current->userid)==0)
            return current;
        current = current->next;
    }
    return 0;
}


/* returns a pointer to searched subject node. NULL if none found */
subject_t * ssearch(user_t ** head, char * userid, char * code) {
    user_t *top = usearch(head, userid);
    if(top != NULL) {
        subject_t * curr = top->down;
        while(curr !=NULL) {
            if(strcmp(code, curr->code)==0)
                return curr;
            curr = curr->down;
        }
    }
    return 0;
}

/* returns a pointer to searched assignment node. NULL if none found */
assign_t * asearch(user_t ** head, char * userid, char * code, char * aname) {
    subject_t *top = ssearch(head, userid, code);
    if(top != NULL) {
        assign_t * curr = top->next;
        while(curr !=NULL) {
            if(strcmp(aname, curr->aname)==0)
                return curr;
            curr = curr->next;
        }
    }
    return 0;
}

/* saves currently loaded linked list to text file. return 0 if failed*/
int save_db(user_t * head, char * str) {
    FILE* fp = fopen(str, "w");
    if (fp!=NULL) {
        user_t * top = head;
        subject_t * middle = NULL;
        assign_t * bottom = NULL;
        if(head !=NULL) {
            while (top != NULL) {
                fprintf(fp, "1 '%s' '%s' %lu %d\n", top->userid, top->uname, top->hash, top->perm);
                middle = top->down;
                while(middle!=NULL)
                {
                    fprintf(fp, "2 \t'%s' '%s' %d\n", middle->code, middle->sname, middle->credit);
                    bottom = middle->next;
                    while(bottom!=NULL)
                    {
                        fprintf(fp, "3 \t\t'%s' %.2f %.2f %.2f\n", bottom->aname, bottom->weight, bottom->mark, bottom->markmax);
                        bottom = bottom->next;
                    }
                    middle = middle->down;
                }
                top = top->next;
            }
        }
        fclose(fp);
        return 1;
    }
    return 0;
}

/* loads formatted text file into linked list. return 0 if failed. */
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
                sscanf(input, "%*d '%[^']' '%[^']' %lu %d\n", userid, uname, &hash, &perm);
                upush(head, userid, uname, hash, perm);
            }
            else if (input[0] == '2') {
                sscanf(input, "%*d '%[^']' '%[^']' %d\n", code, sname, &credit);
                spush(head, userid, code, sname, credit);
            }
            else if (input[0] == '3') {
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

/* returns the current average mark in a particular subject. -1 if there are no assignments */
float assign_avg(user_t * head, char * userid, char * code) {
    subject_t * top = ssearch(&head, userid, code);
    if(top!=NULL && top->next!=NULL) {
        assign_t * curr = top->next;
        float totalweight = 0;
        float adjustedmark = 0;
        while(curr!=NULL)
        {
            adjustedmark = adjustedmark + (curr->weight * curr->mark / curr->markmax);
            totalweight = totalweight + curr->weight;
            curr = curr->next;
        }
        float average = adjustedmark / totalweight * 100;
        return average;
    }
    return -1;
}

/* returns the overall average mark for all subjects in database. -1 if there are no subjects */
float WAM(user_t * head, char * userid) {
    user_t * top = usearch(&head, userid);
    if(top!=NULL) {
        subject_t * curr = top->down;
        float num=0, den=0;
        while(curr!=NULL)
        {
            if(assign_avg(head, userid, curr->code)!=-1) {
            num = num + (curr->credit * assign_avg(head, userid, curr->code));
            den = den + curr->credit;
            }
            curr = curr->down;
        }
        float WAM = num / den;
        return WAM;
    }
    return -1;
}


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

/* returns GPA of all assessable subjects. -1 if no subjects */
float GPA(user_t * head, char * userid) {
    user_t * top = usearch(&head, userid);
    if(top!=NULL && top->down != NULL) {
        subject_t * curr = top->down;
        float num=0, den=0;
        while(curr!=NULL)
        {
            if(creditpoint(head, userid, curr->code)!=-1) { 
            num = num + (curr->credit * creditpoint(head, userid, curr->code));
            den = den + curr->credit;
            }
            curr = curr->down;
        }
        float GPA = num / den;
        return GPA;
    }
    return 0;
}
/* checks for buffer overflow and removes \n from fgets input. returns 1 if buffer overflow occurs */
int process_input(char *str)
{
    char *p;
    if((p=strchr(str, '\n'))==NULL)     /* returns '\n' location if present */
    {
        while (getchar() != '\n');      /* clears buffer until '\n' */
        return(1);
    } 
    *p = '\0';                          /* replaces '\n' with '\0' */
    return(0);
}


