#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "group.h"

#define DB_NAME "newdatabase.txt"
/*
#define USERID_LEN 20
#define UNAME_LEN 20
#define ANAME_LEN 40
#define SNAME_LEN 40
#define CODE_LEN  5 */

/*****************************************************************************/

void student_menu(user_t ** head, user_t ** curr_login);
user_t * login(user_t * head);
void user_header(user_t * head, char * userid);
void view_grades(user_t * head, user_t * curr);
void print_users(user_t * head);
void print_subjects(user_t * head, char * userid);
void print_assigns(user_t * head, char * userid, char * code);
void press_enter(void);
void change_pass(user_t ** head, user_t ** curr);
void create_db(user_t ** head);
void teacher_menu(user_t ** head, user_t ** curr_login);
subject_t * select_sub(user_t * head, user_t * curr);
void print_subjects_teacher(user_t * head, char * userid);
void print_students(user_t * head, user_t * curr_login, subject_t * temp);
/*user_t * search_students(user_t ** head, user_t ** curr_login, subject_t * temp);*/
void add_assign(user_t ** head, char * userid, char * code);
void edit_assign(user_t ** head, char * userid, char * code);
void del_assign(user_t ** head, char * userid, char * code);
void manage_assigns(user_t ** head, user_t ** curr_login, subject_t * temp);
void manage_subjects(user_t ** head, user_t ** curr_login);
void add_subject(user_t ** head, char * userid);
void edit_subject(user_t ** head, char * userid);
void del_subject(user_t ** head, char * userid);
void manage_users(user_t ** head, user_t * curr_login);
void add_user(user_t ** head, user_t * curr);
void edit_user(user_t ** head);
void del_user(user_t ** head);
void admin_menu(user_t ** head, user_t ** curr_login);
void print_list(user_t * head);




/*****************************************************************************/

int main(void) {

    user_t * head = NULL;
    
        if(load_db(&head, DB_NAME)==0) {
            create_db(&head);
        }
        
        user_t * curr_login = login(head);
        if(curr_login != NULL) {
            if((curr_login->perm == 1)) {
                student_menu(&head, &curr_login);
            }
            else if((curr_login->perm == 2)) {
                teacher_menu(&head, &curr_login);
            }
            else if((curr_login->perm == 3)) {
                admin_menu(&head, &curr_login);
            }
            else {
                printf("Invalid permission.");
            }
        }
        
        system("clear");
        ufreeall(&head);
        return 0;
}

/*****************************************************************************/

/* returns a pointer to user file on successful login */
user_t * login(user_t * head) {
    char user[21], pass[21];
    user_t * curr = NULL;
    for(;;)
    {
        system("clear");
        printf("\n\n\ttype 'exit' to cancel.\n"
                "\n\tEnter Username: ");
        fgets(user, 21, stdin);
        process_input(user);
        if(strcmp(user, "exit")==0){  
            return 0;
        }
        printf("\n\tEnter Password: ");
        fgets(pass, 20, stdin);
        process_input(pass);
        unsigned long hash = hashpass(user, pass);
        curr = usearch(&head, user);
        if(curr != NULL && curr->hash == hash) {
            return curr;
        }
        else {
            /*printf("hash: %lu", hash);*/
            printf("\n\n\tInvalid Login");
        }
        
    }
    return 0;
}

/*****************************************************************************/

void student_menu(user_t ** head, user_t ** curr_login) {
    int flag = 0;
    char input[10];
    while(!flag) {
        system("clear");
        user_header(*head, (*curr_login)->userid);
        printf("\n\n\t1 View Grades.\n"
                "\t2 Change Password.\n"
                "\t3 Exit Program.\n\n"
                "\t<Enter a value 1-3>\n\t");
        fgets(input, 10, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            system("clear");
            view_grades(*head, *curr_login);
            system("clear");
        }
        else if (strcmp(input, "2")==0) {
            system("clear");
            user_header(*head, (*curr_login)->userid);
            change_pass(head, curr_login);
            system("clear");
        }
        else if (strcmp(input, "3")==0) {
            flag = 1;
        }
        
    }    
}

/*****************************************************************************/

void teacher_menu(user_t ** head, user_t ** curr_login) {
    int flag = 0;
    char input[30];
    subject_t * temp = select_sub(*head, *curr_login);
    if(temp!=NULL) {
        while(!flag) {
            system("clear");
            user_header(*head, (*curr_login)->userid);
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------\n", temp->code, temp->sname);
            printf("\n\t1 Display Roll.\n"
                    "\t2 Search Student.\n"
                    "\t3 Change Password.\n"
                    "\t4 Switch Subject\n"
                    "\t5 Exit Program.\n\n"
                    "\t<Enter a value 1-3>\n"
                    "\t");
            fgets(input, 30, stdin);
            process_input(input);
            if(strcmp(input, "1")==0) {
                system("clear");
                print_students(*head, *curr_login, temp);
                press_enter();
            }
            else if (strcmp(input, "2")==0)
            {
                manage_assigns(head, curr_login, temp);
            }
            else if (strcmp(input, "3")==0)
            {
                change_pass(head, curr_login);
            }
            else if (strcmp(input, "4")==0)
            {
                temp = select_sub(*head, *curr_login);
            }
            else if (strcmp(input, "5")==0)
            {
                flag = 1;
            }
            
        }
    }    
}

/*****************************************************************************/

void admin_menu(user_t ** head, user_t ** curr_login) {
    int flag = 0;
    char input[30];
    
    while(!flag) {
        system("clear");
        user_header(*head, (*curr_login)->userid);
        printf("\n\n\t1 Print database.\n"
                "\t2 Manage accounts.\n"
                "\t3 Manage user data.\n"
                "\t4 Change Password.\n"
                "\t5 Exit Program.\n\n"
                "\t<Enter a value 1-5>\n"
                "\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            /*print_list(*head);*/
            system("clear");
            user_header(*head, (*curr_login)->userid);
            print_list(*head);
            printf("\tPress enter to continue\n\t");
            getchar();
        }
        else if (strcmp(input, "2")==0)
        {
            manage_users(head, *curr_login);
        }
        else if (strcmp(input, "3")==0)
        {
            manage_subjects(head, curr_login);
        }
        else if (strcmp(input, "4")==0)
        {
            change_pass(head, curr_login);
        }
        else if (strcmp(input, "5")==0)
        {
            flag = 1;
        }     
    }    
}

/*****************************************************************************/

/* teacher selects what subject of theirs to manage */
subject_t * select_sub(user_t * head, user_t * curr) {
    int flag=0;

    if(curr->down!=NULL){
        while(!flag){
            system("clear");
            user_header(head, curr->userid);
            print_subjects_teacher(head, curr->userid);
            printf("\n\tEnter subject code to manage or type exit to cancel\n\n\t");
            char code[10];
            fgets(code, 10, stdin);
            process_input(code);
            if(strcmp(code, "exit")==0){
                return 0;
            }
            subject_t * temp = ssearch(&head, curr->userid, code);
            if(temp!=NULL){
                return temp;
            }
            else {
                system("clear");
                user_header(head, curr->userid);
                printf("\n\tSubject doesn't exist or isn't assigned to this account.\n");
                press_enter();
             }
        
        }
    }
    else {
        user_header(head, curr->userid);
        printf("\n\tThis user has no subjects\n");
        press_enter();  
    }   
    return 0;
}

/*****************************************************************************/
/*
user_t * search_students(user_t ** head, user_t ** curr_login, subject_t * temp) {

    char userid[21];
    user_t * curr = NULL;
    system("clear");
    while(curr==NULL) {
        printf("\n\n\tSearch Student\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Student ID: ");
        fgets(userid, 20, stdin);
        process_input(userid);
        if(strcmp(userid, "exit")==0){  
            return 0;
        }
        curr = usearch(head, userid);
        if(curr==NULL || ssearch(head, userid, temp->code) ==NULL) {
            system("clear");
            printf("\n\n\tUser doesn't exist.");
            return 0;
        }
    } 
    return curr;
}
*/
/*****************************************************************************/

void view_grades(user_t * head, user_t * curr) {
    int flag=0;
    if(curr->down!=NULL){
        while(!flag){
            system("clear");
            user_header(head, curr->userid);
            print_subjects(head, curr->userid);
            printf("\tPress enter to return or type subject code for more detail.\n\t");
            char code[10];
            fgets(code, 10, stdin);
            if(strcmp(code, "\n")!=0){
                process_input(code);
                if(ssearch(&head, curr->userid, code)!=NULL) {
                    system("clear");
                    user_header(head, curr->userid);
                    print_assigns(head, curr->userid, code);
                    press_enter();
                }
                else {
                    system("clear");
                    user_header(head, curr->userid);
                    printf("\n\n\tNo subject with code: %s.\n", code);
                    press_enter();
                 }
            }
            else
                flag = 1;
        }
    }
    else {
        user_header(head, curr->userid);
        printf("\n\tThis user has no subjects\n");
        press_enter();
    }
}

/*****************************************************************************/

void change_pass(user_t ** head, user_t ** curr) {
    char pass[21], confirm[21];
    char user[21];
    strcpy(user, (*curr)->userid);
    system("clear");
    user_header(*head, (*curr)->userid);
    if((*curr)->perm == 3) {
        printf("\n\n\ttype 'exit' to cancel.\n"
                "\n\tEnter Username: ");
        fgets(user, 20, stdin);
        process_input(user);
        if(strcmp(user, "exit")==0){  
            return;
        }
        *curr = usearch(head, user);
        if((*curr) ==NULL) {
            printf("\tinvalid user\n");
            return;
        }
    }
    printf("\n\tEnter NEW Password: ");
    fgets(pass, 20, stdin);
    process_input(pass);
    printf("\n\tConfirm NEW password: ");
    fgets(confirm, 20, stdin);
    process_input(confirm);
    if(strcmp(pass, confirm)==0){
    unsigned long hash = hashpass(user, pass);
    (*curr)->hash = hash;
    save_db(*head, DB_NAME);
        printf("\n\tNew password set.\n");
        press_enter();
    }
    else{
        printf("\n\tPasswords do not match\n");
        press_enter();
    }
    
}

/*****************************************************************************/

void create_db(user_t ** head) {
    int flag = 0;
    char input[21], pass[21], userid[21];
    while(!flag) {
        system("clear");
        printf("\n\n\tError: No database found."
               "\n\n\tWould you like to create a new database? y/n\n\n\t");
        fgets(input, 10, stdin);
        process_input(input);
        if(strcmp(input, "y")==0)
        {
            system("clear");
            printf("\n\tCreating Database\n"
                    "\n\ttype 'exit' to cancel.\n"
                    "\n\tEnter Admin Username: ");
            fgets(userid, 20, stdin);
            process_input(userid);
            if(strcmp(userid, "exit")==0){  
                printf("\n\tExiting program\n\n");
                exit(0);
            }
            printf("\n\tEnter Password: ");
            fgets(pass, 20, stdin);
            process_input(pass);
            
            unsigned long hash = hashpass(userid, pass);
            upush(head, userid, "n\\a", hash, 3);
            save_db(*head, DB_NAME);
            flag = 1;
            printf("\n\n\tDatabase Created.\n");
            press_enter();
            
        }
        else if (strcmp(input, "n")==0)
        {
            printf("\n\tExiting program\n\n");
            exit(0);
        }
        else
        {
            system("clear");
            printf("\n\n\tInvalid Input");
        }
    }
}    

/*****************************************************************************/

void press_enter(void) {
    printf("\tPress enter to return\n\t");
    getchar();
}

/*****************************************************************************/

void user_header(user_t * head, char * userid) {
    user_t * curr = usearch(&head, userid);
    printf("\n\n\tUSER ID: %s\n\tLogged in as: %s\n", 
    curr->userid, curr->uname);
}

/*****************************************************************************/

void print_users(user_t * head) {
    user_t * current = head;
    if(head !=NULL) {
        printf("\n\tPrinting currently loaded linked list\n");
        while (current != NULL) {
            printf("\t1 %s '%s' %lu %d\n", current->userid, current->uname, current->hash, current->perm);
            current = current->next;
        }
    }
    else
        printf("No linked list loaded\n");
}

/*****************************************************************************/

void print_students(user_t * head, user_t * curr_login, subject_t * temp) {
    
    user_t * current = head;
    if(head !=NULL) {
        printf("\n\t--------------------------------------------\n"
                "\tClass Roll %s - %s\n"
                "\t--------------------------------------------\n",
                temp->code, temp->sname);
        while (current != NULL) {
            if(ssearch(&head, current->userid, temp->code)!=NULL && current->perm == 1) {
            printf("\tStudent ID: %s Name: '%s'\n", current->userid, current->uname);
            }
            current = current->next;
        }
    }
    else
        printf("Database not loaded.\n");
    printf("\n");
}

/*****************************************************************************/

void print_subjects(user_t * head, char * userid) {
    user_t * top = usearch(&head, userid);
    if(top!=NULL && top->down!=NULL){
        printf(
                "\n\n\tSubject List\n"
                "\t------------\n"
                "\tCode  Subject Name                   Cp AvgMark\n"
                "\t----- ------------------------------ -- -------\n");
        
        subject_t * curr = top->down;
        while(curr!=NULL)
        {
            if(assign_avg(head, userid, curr->code)!=-1) 
            printf("\t%-5s %-30s %d  %.2f%%\n", curr->code, curr->sname, curr->credit, assign_avg(head, userid, curr->code));
            else
            printf("\t%-5s %-30s %d  n/a\n", curr->code, curr->sname, curr->credit);
            curr = curr->down;
        }
        printf("\t----- ------------------------------ -- -----\n"
               "\tWAM: %.2f%% GPA: %.2f\n"
               "\t---------------------\n", WAM(head, userid), GPA(head, userid));
    }
    else
        printf("\n\tThis user has no subjects\n");
}

/*****************************************************************************/

void print_subjects_teacher(user_t * head, char * userid) {
    user_t * top = usearch(&head, userid);
    if(top!=NULL && top->down!=NULL){
        printf("\n\tSubject List\n"
                
                "\tCode  Subject Name\n"
                "\t----- ------------------------------\n");
        subject_t * curr = top->down;
        while(curr!=NULL)
        {
            printf("\t%-5s %-30s\n", curr->code, curr->sname);
            curr = curr->down;
        }
    }
    else
        printf("\n\tThis teacher has no assigned subjects\n");
}

/*****************************************************************************/

void print_assigns(user_t * head, char * userid, char * code) {
    subject_t * top = ssearch(&head, userid, code);
    
    if(top!=NULL && top->next!=NULL) {
        
        printf(
                "\n\n\t%-5s%-30s\n"
                "\t------------------------------\n"
                "\tAssessment Name      Weight Mark  \n"
                "\t-------------------- ------ -----------\n",top->code, top->sname);
        assign_t * curr = top->next;
        while(curr!=NULL)
        {
            printf("\t%-20s %.2f  %.2f/%.2f\n", curr->aname, curr->weight, curr->mark, curr->markmax);
            curr = curr->next;
        }
        printf("\t-------------------- ------ -----------\n"
               "\tWAM: %.2f%%\n"
               "\t-----------\n", assign_avg(head, userid, code));
    }
    else if(top!=NULL)
        printf("\n\n\tThere is currently no assignments\n");
    else
        printf("\n\n\tNo subject with subject code %s\n", code);
}

/*****************************************************************************/

void print_list(user_t * head) {
    user_t * current = head;
    subject_t * temp = NULL;
    assign_t * pmet = NULL;
    if(head !=NULL) {
        printf("\n\tPrinting currently loaded linked list\n");
        while (current != NULL) {
            printf("\t1 '%s' '%s' %lu %d\n", current->userid, current->uname, current->hash, current->perm);
            temp = current->down;
            while(temp!=NULL)
            {
                printf("\t2\t'%s' '%s' %d\n", temp->code, temp->sname, temp->credit);
                pmet = temp->next;
                while(pmet!=NULL)
                {
                    printf("\t3\t\t'%s' %.2f %.2f %.2f\n", pmet->aname, pmet->weight, pmet->mark, pmet->markmax);
                    pmet = pmet->next;
                }
                temp = temp->down;
            }
            current = current->next;
        }
    }
    else
        printf("No linked list loaded\n");
}

/*****************************************************************************/
/* below here needs fixing */
/*****************************************************************************/
/*****************************************************************************/

void manage_assigns(user_t ** head, user_t ** curr_login, subject_t * temp) {
    char input[30];
    char userid[30];
    user_t * curr = NULL;
    system("clear");
    while(curr==NULL) {
        printf("\n\n\tSearch Student\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Student ID: ");
        fgets(userid, 20, stdin);
        process_input(userid);
        if(strcmp(userid, "exit")==0){  
            return;
        }
        curr = usearch(head, userid);
        if(curr==NULL || ssearch(head, userid, temp->code) ==NULL) {
            system("clear");
            printf("\n\n\tUser doesn't exist.");
        }
    } 
    
    int flag=0;
    system("clear");
    while(!flag) {
        printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
        print_assigns(*head, userid, temp->code);
        printf("\n\t1 Add Assign.\n"
                "\t2 Edit Assign.\n"
                "\t3 Delete Assign.\n"
                "\t4 Return.\n"
                "\n\t<Enter a value 1-4>\n\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
            print_assigns(*head, userid, temp->code);
            add_assign(head, userid, temp->code);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "2")==0)
        {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
            print_assigns(*head, userid, temp->code);
            edit_assign(head, userid, temp->code);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "3")==0)
        {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
            print_assigns(*head, userid, temp->code);
            del_assign(head, userid, temp->code);
            save_db(*head, DB_NAME);
            system("clear");
        } 
        else if (strcmp(input, "4")==0)
        {
            flag = 1;
        }         
    
    }
} 

void add_assign(user_t ** head, char * userid, char * code){

        char input[31], aname[21];
        float weight, mark, markmax;
        printf("\n\tAdding Assignment\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter NEW Assignment Name: ");
        fgets(aname, 21, stdin);
        process_input(aname);
        if(strcmp(aname, "exit")==0){
            return;
        }
        assign_t * temp = asearch(head, userid, code, aname);
        if(temp!=NULL) {
            printf("\n\tAssignment already exists, use edit assignment instead.\n");
            press_enter();
            return;
        }
        
        printf("\n\tEnter Weight: ");
        fgets(input, 21, stdin);
        process_input(input);
        if(strcmp(input, "exit")!=0) {
            
            return;
        }
        sscanf(input, "%f", &weight);
        
        printf("\n\tEnter Student Mark: ");
        fgets(input, 21, stdin);
        process_input(input);
        if(strcmp(input, "exit")!=0) {
            
            return;
        }
        sscanf(input, "%f", &mark);
        
        printf("\n\tEnter Assignment Mark total: ");
        fgets(input, 21, stdin);
        process_input(input);
        if(strcmp(input, "exit")!=0) {
            
            sscanf(input, "%f", &markmax);
        }
        sscanf(input, "%f", &markmax);
        
        apush(head, userid, code, aname, weight, mark, markmax);
        
        
} 

void edit_assign(user_t ** head, char * userid, char * code)  {
        char input[31], aname[21];
        float weight, mark, markmax;
        printf("\n\tEditing Assignment\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Assignment Name: ");
        fgets(aname, 21, stdin);
        process_input(aname);
        if(strcmp(aname, "exit")==0){
            return;
        }
        assign_t * temp = asearch(head, userid, code, aname);
        if(temp==NULL) {
            printf("\n\tAssignment doesn't exist, use add assignment instead.\n");
            press_enter();
            return;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter NEW Assignment Name: ");
        fgets(aname, 21, stdin);
        if(strcmp(aname, "\n")!=0) {
            process_input(aname);
            strcpy(temp->aname, aname);
        }
        
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter Weight: ");
        fgets(input, 21, stdin);
        if(strcmp(input, "\n")!=0) {
            process_input(input);
            sscanf(input, "%f", &weight);
            temp->weight = weight;
        }
        
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter Student Mark: ");
        fgets(input, 21, stdin);
        if(strcmp(input, "\n")!=0) {
            process_input(input);
            sscanf(input, "%f", &mark);
            temp->mark = mark;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter Assignment Mark total: ");
        fgets(input, 21, stdin);
        if(strcmp(input, "\n")!=0) {
            process_input(input);
            sscanf(input, "%f", &markmax);
            temp->markmax = markmax;
        }
} 


void del_assign(user_t ** head, char * userid, char * code) {
    
    char aname[21];
    printf("\n\tDeleting Assignment\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Assignment Name: ");
    fgets(aname, 21, stdin);
    process_input(aname);
    if(strcmp(aname, "exit")==0) {
        return;
    }
    else if(asearch(head, userid, code, aname)==NULL)
        printf("\n\tAssignment doesn't exist");
    else {   
    system("clear");
    printf("\n\tAssignment Deleted.");
    adelete(head, userid, code, aname);
    }
}


void manage_subjects(user_t ** head, user_t ** curr_login) {
    char input[30];
    char userid[30];
    user_t * curr = NULL;
    system("clear");
    while(curr==NULL) {
        printf("\n\n\tSearch User\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter User ID: ");
        fgets(userid, 20, stdin);
        process_input(userid);
        if(strcmp(userid, "exit")==0){  
            return;
        }
        curr = usearch(head, userid);
        if(curr==NULL) {
            system("clear");
            printf("\n\n\tUser doesn't exist.");
        }
    } 
     int flag=0;
    system("clear");
    while(!flag) {
        
        printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
        print_subjects(*head, userid);
        printf("\n\t1 Add Subject.\n"
                "\t2 Edit Subject.\n"
                "\t3 Delete Subject.\n"
                "\t4 View Assignments.\n"
                "\t5 Return.\n\n"
                "\t<Enter a value 1-4>\n\t");
        fgets(input, 30, stdin);
        process_input(input);
        
        if(strcmp(input, "1")==0) {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
        print_subjects(*head, userid);
            add_subject(head, userid);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "2")==0)
        {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
        print_subjects(*head, userid);
            edit_subject(head, userid);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "3")==0)
        {
            system("clear");
            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
        print_subjects(*head, userid);
            del_subject(head, userid);
            save_db(*head, DB_NAME);
            system("clear");
        } 
        else if (strcmp(input, "4")==0)
        {
            char code[10];
            int flag2=0;
            while(!flag2){
            printf("\n\tEnter subject code to manage or type exit to cancel\n\n\t");
            
            fgets(code, 10, stdin);
            process_input(code);
            if(strcmp(code, "exit")!=0){
                subject_t * temp = ssearch(head, userid, code);
               
                if(temp==NULL){
                printf("subject doesn't exist or isn't assigned.");
                }
                else{
                    flag2=1;
                    int flag=0;
                    system("clear");
                        while(!flag) {
                            
                            printf("\n\t--------------------------------------------\n"
                    "\tManaging %s - %s\n"
                    "\t--------------------------------------------", curr->userid, curr->uname);
                            print_assigns(*head, userid, code);
                            printf("\n\t1 Add Assign.\n"
                                    "\t2 Edit Assign.\n"
                                    "\t3 Delete Assign.\n"
                                    "\t4 Return.\n"
                                    "\n\t<Enter a value 1-4>\n\t");
                            fgets(input, 30, stdin);
                            process_input(input);
                            if(strcmp(input, "1")==0) {
                                system("clear");
                                printf("\n\t--------------------------------------------\n"
                                        "\tManaging %s - %s\n"
                                        "\t--------------------------------------------", curr->userid, curr->uname);
                                print_assigns(*head, userid, code);
                                add_assign(head, userid, code);
                                save_db(*head, DB_NAME);
                                system("clear");
                            }
                            else if (strcmp(input, "2")==0)
                            {
                                system("clear");
                                printf("\n\t--------------------------------------------\n"
                                        "\tManaging %s - %s\n"
                                        "\t--------------------------------------------", curr->userid, curr->uname);
                                print_assigns(*head, userid, code);
                                edit_assign(head, userid, code);
                                save_db(*head, DB_NAME);
                                system("clear");
                            }
                            else if (strcmp(input, "3")==0)
                            {
                                system("clear");
                                printf("\n\t--------------------------------------------\n"
                                        "\tManaging %s - %s\n"
                                        "\t--------------------------------------------", curr->userid, curr->uname);
                                print_assigns(*head, userid, code);
                                del_assign(head, userid, code);
                                save_db(*head, DB_NAME);
                                system("clear");
                            } 
                            else if (strcmp(input, "4")==0)
                            {
                                flag = 1;
                            }         
                        
                        }
                    }
                    
            }
            else
                flag2 =1;
            }
            
        } 
        else if (strcmp(input, "5")==0)
        {
            flag = 1;
        }             
    
    }
    
}

void add_subject(user_t ** head, char * userid){
        char input[40], sname[20], code[20];
        int credit;
        printf("\n\tAdding Assignment\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Assignment Name: ");
        fgets(code, 6, stdin);
        process_input(code);
        if(strcmp(code, "exit")==0){
            return;
        }
        subject_t * temp = ssearch(head, userid, code);
        if(temp!=NULL) {
            printf("\n\tSubjects exists, use edit subject instead.\n");
            press_enter();
            return;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter NEW Subject Code: ");
        fgets(code, 21, stdin);
        process_input(code);
        if(strcmp(code, "exit")!=0) {
            return;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter NEW Subject Name: ");
        fgets(sname, 21, stdin);
        process_input(sname);
        if(strcmp(sname, "exit")!=0) {
            return;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter Credit Points: ");
        fgets(input, 21, stdin);
        process_input(input);
        if(strcmp(input, "exit")!=0) {
            return;
            
        }
        sscanf(input, "%d", &credit);
        
        spush(head, userid, code, sname, credit);
} 

void edit_subject(user_t ** head, char * userid)  {
        char input[40], sname[20], code[6];
        int credit;
        printf("\n\tEditing Assignment\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Assignment Name: ");
        fgets(code, 6, stdin);
        process_input(code);
        if(strcmp(code, "exit")==0){
            return;
        }
        subject_t * temp = ssearch(head, userid, code);
        if(temp==NULL) {
            printf("\n\tSubjects doesn't exist, use add subject instead.\n");
            press_enter();
            return;
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter NEW Subject Code: ");
        fgets(code, 21, stdin);
        if(strcmp(code, "\n")!=0) {
            process_input(code);
            strcpy(temp->code, code);
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter NEW Subject Name: ");
        fgets(sname, 21, stdin);
        if(strcmp(sname, "\n")!=0) {
            process_input(sname);
            strcpy(temp->sname, sname);
        }
        printf("\n\t(leave blank to leave as is)\n"
                "\tEnter Credit Points: ");
        fgets(input, 21, stdin);
        if(strcmp(input, "\n")!=0) {
            process_input(input);
            sscanf(input, "%d", &credit);
            temp->credit = credit;
        }
        

} 


void del_subject(user_t ** head, char * userid) {
    
    char code[6];
    printf("\n\n\tDeleting Subject\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter Subject Code: ");
    fgets(code, 6, stdin);
    process_input(code);
    if(strcmp(code, "exit")==0) {
        return;
    }
    else if(ssearch(head, userid, code)==NULL)
        printf("\n\tSubject doesn't exist");
    else {   
    system("clear");
    printf("\n\tSubject Deleted.");
    sdelete(head, userid, code);
}
}


void manage_users(user_t ** head, user_t * curr_login) {
    int flag = 0;
    char input[30];

    while(!flag) {
        system("clear");
        user_header(*head, curr_login->userid);
        printf("\n\n\t1 Print Users\n"
                "\t2 Add User\n"
                "\t3 Edit user\n"
                "\t4 Delete user\n"
                "\t5 Return\n"
                "\t<Enter a value 1-5>\n"
                "\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            system("clear");
            user_header(*head, curr_login->userid);
            print_users(*head);
            press_enter();
        }
        else if(strcmp(input, "2")==0) {
            system("clear");
            user_header(*head, curr_login->userid);
            add_user(head, curr_login);
            save_db(*head, DB_NAME);
        }
        else if (strcmp(input, "3")==0)
        {
            system("clear");
            user_header(*head, curr_login->userid);
            edit_user(head);
            save_db(*head, DB_NAME);
        }
        else if (strcmp(input, "4")==0)
        {
            system("clear");
            user_header(*head, curr_login->userid);
            del_user(head);
            save_db(*head, DB_NAME);
        } 
        else if (strcmp(input, "5")==0)
        {
            flag = 1;
        }         
    
    }
}


void add_user(user_t ** head, user_t * curr) {
    
    char userid[21], uname[21], pass[21], temp[21];
    int perm;
    printf("\n\n\tCreating New User\n"
            "\n\ttype 'exit' to cancel\n"
            "\n\tEnter Username: ");
    fgets(userid, 20, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0){  
        return;
    }
    if(usearch(head, userid)!=NULL) {
        printf("\n\tUser already exists, edit user instead.\n"
                "\tPress enter to continue\n\t");
        getchar();
        return;
    }
        
    printf("\n\tEnter Password: ");
    fgets(pass, 20, stdin);
    process_input(pass);
    if(strcmp(pass, "exit")==0){  
        return;
    }
    printf("\n\tEnter Name: ");
    fgets(uname, 20, stdin);
    process_input(uname);
    if(strcmp(uname, "exit")==0){  
        return;
    }
    printf("\n\tEnter Perm (1-3): ");
    fgets(temp, 20, stdin);
    process_input(temp);
    if(strcmp(temp, "exit")==0){  
        return;
    }
    perm = atoi(temp);
    unsigned long hash = hashpass(userid, pass);
    upush(head, userid, uname, hash, perm);
    printf("\n\tUser Added\n"
    "\tPress enter to continue\n\t");
    getchar();
}



void edit_user(user_t ** head) {
    
    char userid[21], uname[21], pass[21], temp[21];
    int perm;
    printf("\n\n\tEdit User\n"
            "\n\ttype 'exit' to cancel\n"
            "\n\tEnter Existing Username: ");
    fgets(userid, 20, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0){  
        return;
    }
    user_t * curr = usearch(head, userid);
    if(curr==NULL) {
        printf("\n\tUser doesn't exist, create user instead.\n");
        press_enter();
        return;
    }
      
    printf("\n\tNOTE: changing userid requires changing password.\n"
    "\t(leave blank to leave as is)\n"
            "\tEnter NEW User ID: ");
    fgets(userid, 21, stdin);
    if(strcmp(userid, "\n")!=0) {
        process_input(userid);
        strcpy(curr->userid, userid);
        printf("\n\tEnter New Password: ");
        fgets(pass, 20, stdin);
        unsigned long hash = hashpass(userid, pass);
        curr->hash = hash;
    }
    else {
        printf("\n\t(leave blank to leave as is)\n"
                "\n\tEnter New Password: ");
        fgets(pass, 20, stdin);
        if(strcmp(pass, "\n")!=0) {
            process_input(pass);
            unsigned long hash = hashpass(curr->userid, pass);
            curr->hash = hash;
        }
        
    }
      
    printf("\n\t(leave blank to leave as is)\n"
           "\n\tEnter New Name: ");
    fgets(uname, 20, stdin);
    if(strcmp(uname, "\n")!=0) {
        process_input(uname);
        strcpy(curr->uname, uname);
    }
    printf("\n\t(leave blank to leave as is)\n"
           "\n\tEnter New Perm (1-3): ");
    fgets(temp, 20, stdin);
    if(strcmp(temp, "\n")!=0) {
        process_input(temp);
        perm = atoi(temp);
        curr->perm = perm;
    }

    printf("\n\tUser Edited\n");
}

void del_user(user_t ** head) {
    
    char userid[21];
    printf("\n\n\tDeleting User Account\n"
                "\n\ttype 'exit' to cancel\n"
                "\n\tEnter User ID: ");
    fgets(userid, 21, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0) {
        return;
    }
    if(usearch(head, userid)==NULL)
        printf("\n\tUser Account doesn't exist");
    else {   
    system("clear");
    printf("\n\tUser Account Deleted.");
    udelete(head, userid);
    }
}

