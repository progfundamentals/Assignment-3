/*******************************************************************************
 * This program is the main process for my grades 2.0 
 * 
 * 
*******************************************************************************/
 
 /*******************************************************************************
 * Header files 
*******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*custom header file*/
#include "group.h"
#include "huff.h"

/*******************************************************************************
 * Preprocessing Directives
*******************************************************************************/
#define DB_NAME "gradesdatabase.txt"

/*******************************************************************************
 * List Structs
*******************************************************************************/

/*******************************************************************************
 * Function Prototypes
*******************************************************************************/
user_t * login(user_t * head);
void print_logo(void);
void print_box(char * str);
void press_enter(char * str);
void create_db(user_t ** head);
void user_header(char * userid, char * uname);
void manage_header(char * str1, char * str2, char * str3);
void student_menu(user_t ** head, user_t ** curr);
void view_grades(user_t * head, user_t * curr);
void print_subjects(user_t * head, char * userid);
void print_assigns(user_t * head, char * userid, char * code);
void change_pass(user_t ** head, user_t ** curr);
subject_t * select_sub(user_t * head, user_t * curr);
void teacher_menu(user_t ** head, user_t ** curr_login);
void print_students(user_t * head, user_t * curr_login, char * code);

void add_assign(user_t ** head, char * userid, char * code);
void edit_assign(user_t ** head, char * userid, char * code);
void del_assign(user_t ** head, char * userid, char * code);
void admin_menu(user_t ** head, user_t ** curr);
void print_list(user_t * head);
void print_users(user_t * head);
void manage_users(user_t ** head, user_t * curr);
void add_user(user_t ** head);
void edit_user(user_t ** head);
void del_user(user_t ** head);
void manage_subjects(user_t ** head, user_t ** curr_login);
void add_subject(user_t ** head, char * userid);
void edit_subject(user_t ** head, char * userid);
void del_subject(user_t ** head, char * userid);
void manage_assigns(user_t ** head, user_t ** curr_login, char * code);
void extras(void);

/*******************************************************************************
 * Main
*******************************************************************************/

int main(void) {
    /*pointer to header node*/
        user_t * head = NULL;
      
        
        /*load database*/
        if(load_db(&head, DB_NAME)==0) {
            /*creates database*/
            create_db(&head);
        }
        /*pointer to current login*/
        user_t * curr_login = login(head);
        if(curr_login != NULL) {
            /*check permission level*/
            if((curr_login->perm == 1)) {
                /*runs student menu*/
                student_menu(&head, &curr_login);
            }
            else if((curr_login->perm == 2)) {
                /*runs teacher menu*/
                teacher_menu(&head, &curr_login);
            }
            else if((curr_login->perm == 3)) {
                /*runs admin menu*/
                admin_menu(&head, &curr_login); 
            }
            else {
                printf("Invalid permission.");
            }
        }
        system("clear");
        /*free entire linked list*/
        ufreeall(&head);
        

        return 0;
}


/*******************************************************************************
 * This function creates a fresh database and gets user input for admin account.
 * INPUTS: user_t ** head    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void create_db(user_t ** head) {
    int flag = 0;
    char input[21], pass[21], userid[21];
    /* infinite loop */
    for(;;){
        system("clear");
        print_logo();
        printf("\n\n\tError: No database found."
               "\n\n\tWould you like to create a new database? (y/n)\n\n\t");
        fgets(input, 10, stdin);
        process_input(input);
        if(strcmp(input, "y")==0)
        {
            /* repeats loop until userid is less than 20 chars */
            while(!flag){
                system("clear");
                print_box("Creating Database. Type exit to cancel");
                printf("\n\t Enter Admin Username: ");
                /* gets first 21 chars of input */
                fgets(userid, 21, stdin);
                /* returns 1 if string contains \n char */
                flag = process_input(userid);
                if(!flag){
                    printf("\n\tInput must not exceed 20 characters\n");
                    press_enter("try again");
                }
            }
            /* if user inputed 'exit' */
            if(strcmp(userid, "exit")==0){  
                printf("\n\tExiting program\n\n");
                exit(0);
            }
            flag = 0;
            /* repeats loop until password is less than 20 chars */
            while(!flag){
                system("clear");
                print_box("Creating Database. Type exit to cancel");
                printf("\n\t Enter Password: ");
                /* gets first 21 chars of input */
                fgets(pass, 21, stdin);
                /* returns 1 if string contains \n char */
                flag = process_input(pass);
                if(!flag){
                    printf("\n\tInput must not exceed 20 characters\n");
                    press_enter("try again");
                }
            }
            if(strcmp(pass, "exit")==0){
                printf("\n\tExiting program\n\n");
                exit(0);
            }
            /* hashing user input */
            unsigned long hash = hashpass(userid, pass);
            /* pushes new usernode to front of list */
            /* name is n/a and perm level hardcoded as 3 */
            upush(head, userid, "n\\a", hash, 3);
            /* saves change to database */
            save_db(*head, DB_NAME);
            system("clear");
            print_box("Database Created");
            press_enter("return");
            return;
            
        }
        else if (strcmp(input, "n")==0)
        {
            system("clear");
            printf("\n\tExiting program\n\n");
            exit(0);
        }
        else
        {
            printf("\n\tInvalid Input\n");
            press_enter("try again");
        }
    }
}   

/*******************************************************************************
 * This function compares entered username and password hash with those in the
 * database. Returns a pointer to usernode on successful login. Null otherwise
 * INPUTS: user_t * head    OUTPUTS: user_t * curr
 * Author: Max McKay
*******************************************************************************/

user_t * login(user_t * head) {
    char userid[21], pass[21];
    user_t * curr = NULL;
    /* infinite loop */
    for(;;)
    {
        system("clear");
        print_logo();
        print_box("User Login. Type exit to cancel.");
        printf("\n\t Enter Username: ");
        fgets(userid, 21, stdin);
        process_input(userid);
        if(strcmp(userid, "exit")==0){  
            return 0;
        }
        printf("\n\t Enter Password: ");
        fgets(pass, 20, stdin);
        process_input(pass);
        /*hashes inputed password*/
        unsigned long hash = hashpass(userid, pass);
        /*pointer to user node*/
        curr = usearch(&head, userid);
        if(curr != NULL &&curr->hash == hash){
            /*if hash matches user node*/
            return curr;
        }
        else {
            /*printf("hash: %lu", hash);*/
            printf("\n\tInvalid Login\n");
            press_enter("try again");
        }
    }
    return 0;
}

/*******************************************************************************
 * Students option menu. Runs various student functions based on input.
 * INPUTS: user_t * head    OUTPUTS: user_t * curr
 * Authors: Max McKay, Tatiana Khoury, Wassim Kheireddine
*******************************************************************************/

void student_menu(user_t ** head, user_t ** curr) {
    char input[10];
    for(;;) {
        /* print current login details */
        user_header((*curr)->userid, (*curr)->uname);
        printf("\n\t1 View Grades.\n"
                "\t2 Change Password.\n"
                "\t3 Exit Program.\n\n"
                "\t<Enter a value 1-3>\n\t");
        fgets(input, 10, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            /* view subject and assign grades */
            view_grades(*head, *curr);
            system("clear");
        }
        else if (strcmp(input, "2")==0) {
            /* change password */
            change_pass(head, curr);
            system("clear");
        }
        else if (strcmp(input, "3")==0) {
            /* quit menu function */
            return;
        }
        
    }    
}

/*******************************************************************************
 * Teachers option menu. Runs various teacher functions based on input.
 * INPUTS: user_t ** head, user_t ** curr_login    OUTPUTS: n/a
 * Authors: Max McKay, Nolan Zhang
*******************************************************************************/

void teacher_menu(user_t ** head, user_t ** curr_login) {
    char input[30];
    /* pointer to subject teacher wishes to manage */
    subject_t * temp = select_sub(*head, *curr_login);
    while(temp!=NULL) {
        /* print current login details */
        user_header((*curr_login)->userid, (*curr_login)->uname);
        /* print current subject managing */
        manage_header("Managing",temp->code, temp->sname);
        printf("\n\t1 Display Roll.\n"
                "\t2 Manage Student Grades.\n"
                "\t3 Change Password.\n"
                "\t4 Switch Subject\n"
                "\t5 Exit Program.\n\n"
                "\t<Enter a value 1-5>\n\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            system("clear");
            /* print a list of all students in subject*/
            print_students(*head, *curr_login, temp->code);
        }
        else if (strcmp(input, "2")==0)
        {
            /* manage student grades for subject */
            manage_assigns(head, curr_login, temp->code);
        }
        else if (strcmp(input, "3")==0)
        {
            /* change password*/
            change_pass(head, curr_login);
        }
        else if (strcmp(input, "4")==0)
        {
            /* return new pointer to subject */
            temp = select_sub(*head, *curr_login);
        }
        else if (strcmp(input, "5")==0)
        {
            /* quit menu function */
            return;
        }
        
    }
   
}
/*******************************************************************************
 * Admins option menu. Runs various other functions based on input.
 * INPUTS: user_t ** head, user_t ** curr   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void admin_menu(user_t ** head, user_t ** curr) {
    char input[30];
    
    for(;;){
        user_header((*curr)->userid, (*curr)->uname);
        printf("\n\t1 Print Entire Database.\n"
                "\t2 Manage accounts.\n"
                "\t3 Manage user data.\n"
                "\t4 Change Password.\n"
                "\t5 Extras (not fully implemented)\n"
                "\t6 Exit Program.\n\n"
                "\t<Enter a value 1-6>\n"
                "\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            user_header((*curr)->userid, (*curr)->uname);
            /* print entire linked list */
            print_list(*head);
            printf("\tPress enter to continue\n\t");
            getchar();
        }
        else if (strcmp(input, "2")==0)
        {
            /*manage user accounts*/
            manage_users(head, *curr);
        }
        else if (strcmp(input, "3")==0)
        {
            /* manage subject assignments */
            manage_subjects(head, curr);
        }
        else if (strcmp(input, "4")==0)
        {
            /* change password */
            change_pass(head, curr);
        }
        else if (strcmp(input, "5")==0)
        {
            extras();
        }
        else if (strcmp(input, "6")==0)
        {
            /* quit menu function */
            return;
        }          
    }    
}

/*******************************************************************************
 * This function implements some features that we didn't implement in time.
 * INPUTS: n/a    OUTPUTS: n/a
 * Author: Felicity Rodricks, Max McKay
*******************************************************************************/

void extras(void) {
    char str[200];
    char key[200];
    print_box("Huffman");
    printf("\n\tEnter any string: ");
    fgets(str, 200, stdin);
    process_input(str);
    char buf[1024];
	init(str);
	/*launches compression*/
	compress(str, buf);
	printf("\n\tCompressed string: %s\n", buf);
    /*prints the decompressed string*/
	printf("\n\tDecompressed string: ");
	/*launches decompression*/
	decompress(buf, q[1]);
    
    
    print_box("Run length encoding");
    printf("\n\tEnter any string: ");
    fgets(str, 200, stdin);
    process_input(str);
    char * comp = rle(str);
    printf("\n\tCompressed string: %s\n", comp);
    
    print_box("XOR encryption");
    printf("\n\tEnter any string: ");
    fgets(str, 200, stdin);
    process_input(str);
    unsigned long inLen = strlen(str);
    printf("\n\tEnter key: ");
    fgets(key, 200, stdin);
    process_input(key);
    unsigned long keyLen = strlen(key);
    
    char* xorout = crypt(str, key, inLen, keyLen);
    printf("\n\tEncrypted string: %s\n", xorout);
    xorout = crypt(xorout, key, inLen, keyLen);
    printf("\n\tUnencrypted string: %s\n", xorout);
    press_enter("Continue");
    print_box("GZIP is the currently implemented compression");
    print_box("Password hashing is implemented to safely store passwords");
    press_enter("return");

}

/*******************************************************************************
 * This function lets the admin to search a user and edit their subjects.
 * INPUTS: user_t ** head, user_t * curr_login    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void manage_subjects(user_t ** head, user_t ** curr_login) {
    char input[30];
    char userid[30];
    user_t * curr = NULL;
    while(curr==NULL) {
        system("clear");
        print_box("Search User. Type exit to cancel.");
        printf("\n\t Enter User ID: ");
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
  
    while(!flag) {
        system("clear");
        manage_header("Managing", curr->userid, curr->uname);
        print_subjects(*head, curr->userid);
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
            manage_header("Managing", curr->userid, curr->uname);
            print_subjects(*head, curr->userid);
            add_subject(head, curr->userid);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "2")==0)
        {
            system("clear");
            manage_header("Managing", curr->userid, curr->uname);
            print_subjects(*head, curr->userid);
            edit_subject(head, curr->userid);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "3")==0)
        {
            system("clear");
            manage_header("Managing", curr->userid, curr->uname);
            print_subjects(*head, curr->userid);
            del_subject(head, curr->userid);
            save_db(*head, DB_NAME);
            system("clear");
        } 
        else if (strcmp(input, "4")==0)
        {
            system("clear");
            manage_header("Managing", curr->userid, curr->uname);
            print_subjects(*head, curr->userid);
            char code[6];
            printf("\n\tEnter subject code to manage or type exit to cancel\n\n\t");
            fgets(code, 6, stdin);
            process_input(code);
            manage_assigns(head, &curr, code);
        } 
        else if (strcmp(input, "5")==0)
        {
            flag = 1;
        }             
    
    }
    
}

/*******************************************************************************
 * This function is allows an admin to manage user accounts in the database
 * INPUTS: user_t ** head, user_t * curr    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void manage_users(user_t ** head, user_t * curr) {
    int flag = 0;
    char input[30];

    while(!flag) {
        user_header(curr->userid, curr->uname);
        printf("\n\t1 Print Users\n"
                "\t2 Add User\n"
                "\t3 Edit user\n"
                "\t4 Delete user\n"
                "\t5 Return\n"
                "\n\t<Enter a value 1-5>\n\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            user_header(curr->userid, curr->uname);
            print_users(*head);
            press_enter("return");
        }
        else if(strcmp(input, "2")==0) {
            add_user(head);
            save_db(*head, DB_NAME);
        }
        else if (strcmp(input, "3")==0)
        {
            user_header(curr->userid, curr->uname);
            edit_user(head);
            save_db(*head, DB_NAME);
        }
        else if (strcmp(input, "4")==0)
        {
            user_header(curr->userid, curr->uname);
            del_user(head);
            save_db(*head, DB_NAME);
        } 
        else if (strcmp(input, "5")==0)
        {
            flag = 1;
        }         
    
    }
}

/*******************************************************************************
 * This function is allows a teacher or admin to manage assignments of a subject
 * INPUTS: user_t ** head, user_t ** curr    OUTPUTS: subject_t * curr
 * Author: Max McKay
*******************************************************************************/

void manage_assigns(user_t ** head, user_t ** curr_login, char * code) {
    char input[30];
    char userid[30];
    user_t * curr = NULL;
    if((*curr_login)->perm == 2){
        while(curr==NULL) {
            system("clear");
            print_box("Search Student. Type exit to cancel.");
            printf("\n\t Enter Student ID: ");
            fgets(userid, 20, stdin);
            process_input(userid);
            if(strcmp(userid, "exit")==0){  
                return;
            }
            curr = usearch(head, userid);
            if(curr==NULL || ssearch(head, userid, code) ==NULL || strcmp((*curr_login)->userid, userid)==0) {
                
                printf("\n\tUser doesn't exist.\n");
                press_enter("try again");
                curr=NULL;
            }
        }
    }
    else{
        curr = *curr_login;
    }
    
    int flag=0;
    while(!flag) {
        system("clear");
        manage_header("Managing", curr->userid, curr->uname);
        
        print_assigns(*head, curr->userid, code);
        printf("\n\t1 Add Assignment.\n"
                "\t2 Edit Assignment.\n"
                "\t3 Delete Assignment.\n"
                "\t4 Return.\n"
                "\n\t<Enter a value 1-4>\n\t");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0) {
            manage_header("Managing", curr->userid, curr->uname);
            print_assigns(*head, curr->userid, code);
            add_assign(head, curr->userid, code);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "2")==0)
        {
            manage_header("Managing", curr->userid, curr->uname);
            print_assigns(*head, curr->userid, code);
            edit_assign(head, curr->userid, code);
            save_db(*head, DB_NAME);
            system("clear");
        }
        else if (strcmp(input, "3")==0)
        {
            manage_header("Managing", curr->userid, curr->uname);
            print_assigns(*head, curr->userid, code);
            del_assign(head, curr->userid, code);
            save_db(*head, DB_NAME);
            system("clear");
        } 
        else if (strcmp(input, "4")==0)
        {
            flag = 1;
        }         
    
    }
} 

/*******************************************************************************
 * This function is allows a teacher to select what subject they wish to manage. 
 * Returns pointer to selected subject.
 * INPUTS: user_t * head, user_t * curr    OUTPUTS: subject_t * curr
 * Author: Max McKay
*******************************************************************************/

subject_t * select_sub(user_t * head, user_t * curr) {
    int flag;
    char code[6];
    /* teacher has assigned subjects */
    if(curr->down!=NULL){
        /* infinite loop */
        for(;;){
            flag = 0;
            /* repeat until */
            while(!flag){
                user_header(curr->userid, curr->uname);
                /* prints assigned subjects*/
                print_subjects(head, curr->userid);
                printf("\n\tEnter subject code to manage or "
                        "type exit to quit.\n\t");
                /* gets first 6 chars */
                fgets(code, 6, stdin);
                /* returns 1 if string contains \n char */
                flag = process_input(code);
                if(!flag){
                    printf("\n\tInput must not exceed 5 characters\n");
                    press_enter("try again");
                }
            }
            if(strcmp(code, "exit")==0){
                return 0;
            }
            /* pointer to subject location */
            subject_t * temp = ssearch(&head, curr->userid, code);
            if(temp!=NULL){
                /* if match */
                return temp;
            }
            else {
                /* pointer returned null */
                user_header(curr->userid, curr->uname);
                printf("\n\tSubject code doesn't exist or "
                        "isn't assigned to this account.\n");
                press_enter("try again");
             }
        }
    }
    else {
        /* no associated subjects */
        user_header(curr->userid, curr->uname);
        printf("\n\tThis user has no subjects\n");
        press_enter("exit");  
    }   
    return 0;
}

/*******************************************************************************
 * This function prints out a students grades for viewing. Top level displays
 * subjects in a list with overall grades. Subjects codes can be entered to
 * view assignments of that subject node.
 * INPUTS: user_t * head, user_t * curr  OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void view_grades(user_t * head, user_t * curr) {
    int flag=0;
    /* check to see if subjects exist */
    if(curr->down!=NULL){
        while(!flag){
            /* prints a box containing user details */
            user_header(curr->userid, curr->uname);
            /* prints subjects associated with user */
            print_subjects(head, curr->userid);
            printf("\tPress enter to return or "
            "type subject code for more detail.\n\t");
            char code[10];
            fgets(code, 10, stdin);
            if(strcmp(code, "\n")!=0){
                process_input(code);
                /* check to see if assigns exist */
                if(ssearch(&head, curr->userid, code)!=NULL) {
                    user_header(curr->userid, curr->uname);
                    /* prints assigns associated with user subject */
                    print_assigns(head, curr->userid, code);
                    press_enter("return");
                }
                else {
                    /* if no assigns */
                    user_header(curr->userid, curr->uname);
                    printf("\n\tNo subject with code: %s.\n", code);
                    press_enter("try again");
                 }
            }
            
            else
                flag = 1;
        }       /* if enter is pressed exit loop */
    }
    else {
        /* if no subjects */
        user_header(curr->userid, curr->uname);
        printf("\n\tThis user has no subjects\n");
        press_enter("return");
    }
}

/*******************************************************************************
 * This function prints changes the password of the currently logged in user.
 * INPUTS: user_t ** head, user_t ** curr  OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void change_pass(user_t ** head, user_t ** curr) {
    char pass[21], confirm[21];
    int flag;
    for(;;){
        flag = 0;
        while(!flag){
            system("clear");
            print_box("Overwriting Password. Type exit to cancel");
            printf("\n\t Enter NEW Password: ");
            /* gets first 21 chars of input */
            fgets(pass, 21, stdin);
            /* returns 1 if string contains \n char */
            flag = process_input(pass);
            if(!flag){
                printf("\n\tInput must not exceed 20 characters\n");
                press_enter("try again");
            }
        }
        /* if user inputed 'exit' */
        if(strcmp(pass, "exit")==0){  
            return;
        }
        system("clear");
        print_box("Overwriting Password. Type exit to cancel");
        printf("\n\t Confirm NEW Password: ");
        /* gets first 21 chars of input */
        fgets(confirm, 21, stdin);
        /* returns 1 if string contains \n char */
        process_input(confirm);
        if(strcmp(confirm, "exit")==0){  
            return;
        }
        if(strcmp(pass, confirm)==0){
            /*hashes inputed details */
            unsigned long hash = hashpass((*curr)->userid, pass);
            /* overwrites existing hash */
            (*curr)->hash = hash;
            /* saves changes to database */
            save_db(*head, DB_NAME);
            system("clear");
            print_box("New password set.");
            press_enter("return");
            return;
        }
        else{
            printf("\n\tPasswords do not match\n");
            press_enter("try again");
        }
    }
}



/*******************************************************************************
 * This function pushes a new subject node to appropriate usernode.
 * INPUTS: user_t ** head, char * userid, char * code    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void add_subject(user_t ** head, char * userid){

    char input[10], sname[20], code[20];
    int credit;
    system("clear");
    print_box("Adding New Subject. Type exit to cancel");
    printf("\n\t Enter Subject Code: ");
    fgets(code, 6, stdin);
        process_input(code);
        if(strcmp(code, "exit")==0){
            return;
        }
    /* check if the subject already exists. */
    if(ssearch(head, userid, code)!=NULL) {
        printf("\n\tSubjects exists, use edit subject instead.\n");
        press_enter("return");
        /* return if subject does exist. */
        return;
    }
    printf("\n\t Enter Subject Name: ");
    fgets(sname, 31, stdin);
    process_input(sname);
    if(strcmp(sname, "exit")==0) {
        return;
    }
    printf("\n\t Enter Credit Points: ");
    fgets(input, 10, stdin);
    process_input(input);
    if(strcmp(input, "exit")==0) {
        return;  
    }
    sscanf(input, "%d", &credit);
        
    spush(head, userid, code, sname, credit);
    system("clear");
    print_box("Subject Added Successfully.");
    press_enter("return");        
} 
/*******************************************************************************
 * This function edits an existing subject node.
 * INPUTS: user_t ** head, char * userid, char * code   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void edit_subject(user_t ** head, char * userid)  {
    char input[10], sname[20], code[6];
    int credit;
    system("clear");
    print_box("Editing Subject. Type exit to cancel");
    printf("\n\t Enter Subject Code: ");
    fgets(code, 6, stdin);
    process_input(code);
    if(strcmp(code, "exit")==0){
        return;
    }
    /* check if the subject already exists. */
    subject_t * temp = ssearch(head, userid, code);
    if(temp==NULL) {
        printf("\n\tSubjects doesn't exist, use add subject instead.\n");
        press_enter("return");
        /* return if subject doesn't exist*/
        return;
    }
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter NEW Subject Code: ");
    fgets(code, 21, stdin);
    if(strcmp(code, "\n")!=0) {
        process_input(code);
        strcpy(temp->code, code);
    }
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter NEW Subject Name: ");
    fgets(sname, 21, stdin);
    if(strcmp(sname, "\n")!=0) {
        process_input(sname);
        strcpy(temp->sname, sname);
    }
    printf("\n\t(leave blank to leave as is)\n"
            "\tEnter Credit Points: ");
    fgets(input, 10, stdin);
    if(strcmp(input, "\n")!=0) {
        process_input(input);
        sscanf(input, "%d", &credit);
        temp->credit = credit;
    }
    system("clear");
    print_box("Subject Edited Successfully.");
    press_enter("return"); 
} 
/*******************************************************************************
 * This function deletes the selected user node.
 * INPUTS: user_t ** head, char * userid, char * code OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void del_subject(user_t ** head, char * userid) {
    
    char code[6];
    system("clear");
    print_box("Deleting Subject. Type exit to cancel");
    printf("\n\t Enter User ID: ");
    fgets(code, 6, stdin);
    process_input(code);
    if(strcmp(code, "exit")==0) {
        return;
    }
    /* searches for  subject associated with userid*/
    else if(ssearch(head, userid, code)==NULL){
        printf("\n\tSubject doesn't exist.\n");
        press_enter("return");
    }
    else {   
    system("clear");
    /* frees and deletes user node. */
    sdelete(head, userid, code);
    print_box("Subject Deleted Successfully.");
    press_enter("return"); 
    }
} 


/*******************************************************************************
 * This function pushes a new user node to the head of the linked list
 * INPUTS: user_t ** head   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void add_user(user_t ** head){

    char userid[21], uname[21], pass[21], temp[21];
    int perm;
    system("clear");
    print_box("Creating New User. Type exit to cancel");
    printf("\n\t Enter NEW Username: ");
    fgets(userid, 21, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0){
        return;
    }
    /* check if the user already exists. */
    if(usearch(head, userid)!=NULL) {
        printf("\n\tUser already exists, edit user instead.\n");
        press_enter("return");
        /* return if user does exist. */
        return;
    }
    
    printf("\n\t Enter Password: ");
    fgets(pass, 21, stdin);
    process_input(pass);
    if(strcmp(pass, "exit")==0){  
        return;
    }
    printf("\n\t Enter Name: ");
    fgets(uname, 21, stdin);
    process_input(uname);
    if(strcmp(uname, "exit")==0){  
        return;
    }
    printf("\n\t Enter Perm (1-3): ");
    fgets(temp, 21, stdin);
    process_input(temp);
    if(strcmp(temp, "exit")==0){  
        return;
    }
    perm = atoi(temp);
    unsigned long hash = hashpass(userid, pass);
    upush(head, userid, uname, hash, perm);
    system("clear");
    print_box("User Added Successfully.");
    press_enter("return");        
} 
/*******************************************************************************
 * This function edits an existing assign node for users.
 * INPUTS: user_t ** head   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void edit_user(user_t ** head)  {
    char userid[21], uname[21], pass[21], temp[21];
    int perm;
    system("clear");
    print_box("Editing User. Type exit to cancel");
    printf("\n\t Enter Username: ");
    fgets(userid, 21, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0){
        return;
    }
    /* check if the user already exists. */
    user_t * curr = usearch(head, userid);
    if(curr==NULL) {
        printf("\n\tUser doesn't exist, create user instead.\n");
        press_enter("return");
        /* return if user doesn't exist*/
        return;
    }
    printf("\n\tNOTE: changing userid requires changing password.\n"
    "\t(leave blank to leave as is)\n"
        "\t Enter NEW User ID: ");
    fgets(userid, 21, stdin);
    if(strcmp(userid, "\n")!=0) {
        process_input(userid);
        strcpy(curr->userid, userid);
        printf("\n\tEnter New Password: ");
        fgets(pass, 21, stdin);
        unsigned long hash = hashpass(userid, pass);
        curr->hash = hash;
    }
    else {
        printf("\n\t(leave blank to leave as is)\n"
                "\n\t Enter New Password: ");
        fgets(pass, 21, stdin);
        if(strcmp(pass, "\n")!=0) {
            process_input(pass);
            unsigned long hash = hashpass(curr->userid, pass);
            curr->hash = hash;
        }
    }
    printf("\n\t(leave blank to leave as is)\n"
           "\n\t Enter New Name: ");
    fgets(uname, 21, stdin);
    if(strcmp(uname, "\n")!=0) {
        process_input(uname);
        strcpy(curr->uname, uname);
    }
    printf("\n\t(leave blank to leave as is)\n"
           "\n\t Enter New Perm (1-3): ");
    fgets(temp, 21, stdin);
    if(strcmp(temp, "\n")!=0) {
        process_input(temp);
        perm = atoi(temp);
        curr->perm = perm;
    }
        system("clear");
        print_box("User Edited Successfully.");
        press_enter("return"); 
} 
/*******************************************************************************
 * This function deletes the selected user node.
 * INPUTS: user_t ** head OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void del_user(user_t ** head) {
    
    char userid[21];
    system("clear");
    print_box("Deleting User Account. Type exit to cancel");
    printf("\n\t Enter User ID: ");
    fgets(userid, 21, stdin);
    process_input(userid);
    if(strcmp(userid, "exit")==0) {
        return;
    }
    /* searches for user*/
    else if(usearch(head, userid)==NULL){
        printf("\n\tAccount doesn't exist.\n");
        press_enter("return");
    }
    else {   
    system("clear");
    /* frees and deletes user node. */
    udelete(head, userid);
    print_box("Account Deleted Successfully.");
    press_enter("return"); 
    }
} 


/*******************************************************************************
 * This function pushes a new assign node to selected subject for users.
 * INPUTS: user_t ** head, char * userid, char * code    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void add_assign(user_t ** head, char * userid, char * code){

    char input[31], aname[21];
    float weight, mark, markmax;
    system("clear");
    print_box("Adding New Assignment. Type exit to cancel");
    printf("\n\t Enter NEW Assignment Name: ");
    fgets(aname, 21, stdin);
    process_input(aname);
    if(strcmp(aname, "exit")==0){
        return;
    }
    /* check if the assignment already exists. */
    assign_t * temp = asearch(head, userid, code, aname);
    if(temp!=NULL) {
        printf("\n\tAssignment already exists, use edit assignment "
        "instead.\n");
        press_enter("return");
        /* return if assign does exist. */
        return;
    }
    
    printf("\n\t Enter Weight: ");
    fgets(input, 21, stdin);
    process_input(input);
    if(strcmp(input, "exit")==0) {
        return;
    }
    /* scan float from string */
    sscanf(input, "%f", &weight);
    
    printf("\n\t Enter Student Mark: ");
    fgets(input, 21, stdin);
    process_input(input);
    if(strcmp(input, "exit")==0) {
        return;
    }
    /* scan float from string */
    sscanf(input, "%f", &mark);
    printf("\n\t Enter Assignment Mark total: ");
    fgets(input, 21, stdin);
    process_input(input);
    if(strcmp(input, "exit")==0) {
        return;
    }
    /* scan float from string */
    sscanf(input, "%f", &markmax);
    /* pushes new assign node to start of linked list */
    apush(head, userid, code, aname, weight, mark, markmax);
    system("clear");
    print_box("Assignment Added Successfully.");
    press_enter("return");        
} 
/*******************************************************************************
 * This function edits an existing assign node for users.
 * INPUTS: user_t ** head, char * userid, char * code   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void edit_assign(user_t ** head, char * userid, char * code)  {
    char input[31], aname[21];
    float weight, mark, markmax;
    system("clear");
    print_box("Editing Assignment. Type exit to cancel");
    printf("\n\t Enter Assignment Name: ");
    fgets(aname, 21, stdin);
    process_input(aname);
    if(strcmp(aname, "exit")==0){
        return;
    }
    /* check if the assignment already exists. */
    assign_t * temp = asearch(head, userid, code, aname);
    if(temp==NULL) {
        printf("\n\tAssignment doesn't exist, use add assignment "
        "instead.\n");
        press_enter("return");
        /* return if assign doesn't exist*/
        return;
    }
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter NEW Assignment Name: ");
    fgets(aname, 21, stdin);
    if(strcmp(aname, "\n")!=0) {
        process_input(aname);
        /* overwrite existing data with input */
        strcpy(temp->aname, aname);
    }
    
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter Weight: ");
    fgets(input, 21, stdin);
    if(strcmp(input, "\n")!=0) {
        process_input(input);
        sscanf(input, "%f", &weight);
        /* overwrite existing data with input */
        temp->weight = weight;
    }
    
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter Student Mark: ");
    fgets(input, 21, stdin);
    if(strcmp(input, "\n")!=0) {
        process_input(input);
        sscanf(input, "%f", &mark);
        /* overwrite existing data with input */
        temp->mark = mark;
    }
    printf("\n\t(leave blank to leave as is)\n"
            "\t Enter Assignment Mark total: ");
    fgets(input, 21, stdin);
    if(strcmp(input, "\n")!=0) {
        process_input(input);
        sscanf(input, "%f", &markmax);
        /* overwrite existing data with input */
        temp->markmax = markmax;
    }
    system("clear");
    print_box("Assignment Edited Successfully.");
    press_enter("return"); 
} 
/*******************************************************************************
 * This function deletes the selected assign node for users.
 * INPUTS: user_t ** head, char * userid, char * code OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void del_assign(user_t ** head, char * userid, char * code) {
    
    char aname[21];
    system("clear");
    print_box("Deleting Assignment. Type exit to cancel");
    printf("\n\t Enter Assignment Name: ");
    fgets(aname, 21, stdin);
    process_input(aname);
    if(strcmp(aname, "exit")==0) {
        return;
    }
    /* searches for assignment associated with userid and subject code*/
    else if(asearch(head, userid, code, aname)==NULL){
        printf("\n\tAssignment doesn't exist.\n");
        press_enter("return");
    }
    else {   
    system("clear");
    /* frees and deletes assign node. */
    adelete(head, userid, code, aname);
    print_box("Assignment Deleted Successfully.");
    press_enter("return"); 
    }
}


/*******************************************************************************
 * This function prints a users assigned subjects in 2 different formats
 * depending on the perm level of the associated user.
 * INPUTS: user_t * head, char * userid   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void print_subjects(user_t * head, char * userid) {
    user_t * top = usearch(&head, userid);
    if(top!=NULL && top->down!=NULL){
        /* for student account */
        if(top->perm == 1){    
            printf(
                    "\n\tSubject List\n"
                    "\t────────────\n"
                    "\tCode  Subject Name                   Cp AvgMark\n"
                    "\t───── ────────────────────────────── ── ───────\n");
            
            subject_t * curr = top->down;
            /* loop while subjects exist */
            while(curr!=NULL)
            {
                if(assign_avg(head, userid, curr->code)!=-1){ 
                printf("\t%-5s %-30s  %d %.2f%%\n", curr->code, curr->sname, 
                curr->credit, assign_avg(head, userid, curr->code));
                }
                else{
                printf("\t%-5s %-30s  %d n/a\n", curr->code, curr->sname, 
                curr->credit);
                }
                /* increment depth */
                curr = curr->down;
            }
            printf("\t───── ────────────────────────────── ── ───────\n"
                   "\tWAM: %.2f%% GPA: %.2f\n"
                   "\t─────────────────────\n", WAM(head, userid), 
                   GPA(head, userid));
        }
        /* for teacher account */
        else if(top->perm == 2) {
            printf("\n\tSubject List\n"
                    "\t────────────\n"
                    "\tCode  Subject Name\n"
                    "\t───── ──────────────────────────────\n");
            subject_t * curr = top->down;
            /* loop while subjects exist */
            while(curr!=NULL)
            {
                printf("\t%-5s %-30s\n", curr->code, curr->sname);
                /* increment depth */
                curr = curr->down;
            }
        }
    }
    else
        printf("\n\tThis user has no subjects.\n");
        /* if no associated subjects exist */
}

/*******************************************************************************
 * This function prints a subjects assigned assignments 
 * INPUTS: user_t * head, char * userid, char * code  OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void print_assigns(user_t * head, char * userid, char * code) {
    subject_t * top = ssearch(&head, userid, code);
    
    if(top!=NULL && top->next!=NULL) {
        
        printf("\n\t%s - %s\n"
            "\t─────────────────────────────────────\n"
            "\tAssignment Name                Weight Mark  \n"
            "\t────────────────────────────── ────── ───────────\n",top->code, 
                top->sname);
        assign_t * curr = top->next;
        /* loop while assigns exist */
        while(curr!=NULL)
        {
            printf("\t%-30s  %.2f %.2f/%.2f\n", curr->aname, curr->weight, 
            curr->mark, curr->markmax);
            /* increment depth */
            curr = curr->next;
        }
        printf("\t────────────────────────────── ────── ───────────\n"
               "\tWAM: %.2f%%\n"
               "\t───────────\n", assign_avg(head, userid, code));
    }
    else if(top!=NULL)
        printf("\n\n\tThere is currently no assignments.\n");
    else
        printf("\n\n\tNo subject with subject code %s.\n", code);
}

/*******************************************************************************
 * This function prints the logo.
 * INPUTS: n/a   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void print_logo(void) {
    printf(
 "\n  ┌─────────────────────────────────────────────────────────────────┐\n"
 "  │  __  __          _____               _             ___    ___   │\n"  
 "  │ |  \\/  |        / ____|             | |           |__ \\  / _ \\  │\n" 
 "  │ | \\  / |_   _  | |  __ _ __ __ _  __| | ___  ___     ) || | | | │\n"
 "  │ | |\\/| | | | | | | |_ | '__/ _` |/ _` |/ _ \\/ __|   / / | | | | │\n"
 "  │ | |  | | |_| | | |__| | | | (_| | (_| |  __/\\__ \\  / /_ | |_| | │\n"
 "  │ |_|  |_|\\__, |  \\_____|_|  \\__,_|\\__,_|\\___||___/ |____(_)___/  │\n"
 "  └──────── __/ |───────────────────────────────────────────────────┘\n"                                                
 "           |___/ ");
}

/*******************************************************************************
 * This function prints all the students that have a subject node the same as
 * the one currently being managed.
 * INPUTS: user_t * head, user_t * curr_login, char * code   OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void print_students(user_t * head, user_t * curr_login, char * code) {
    /* start at header */
    int flag = 0;
    user_t * current = head;
    subject_t * temp = ssearch(&head, curr_login->userid, code);
    if(head !=NULL && temp!=NULL) {
        
        manage_header("Class Roll", temp->code, temp->sname);
        /* loop while users exist */
        while (current != NULL) {
            /* check for subject node and that user account is a student*/
            if(ssearch(&head, current->userid, temp->code)!=NULL && 
            current->perm == 1) {
                if(!flag)
                    printf("\tUser ID              Name                 Avg"
                    "\n\t──────────────────── ──────────────────── ─────");
                printf("\n\t%-20s %-20s %0.2f", current->userid, 
                current->uname, assign_avg(head, current->userid, 
                temp->code));
                flag = 1;
            }
            /* increment depth*/
            current = current->next;
        }
        if(flag)
            printf("\n\t──────────────────── ──────────────────── ─────");
        else
            printf("\tThere are no students in this subject.");
    }
    else
        printf("ERROR: Database not loaded.\n");
    printf("\n");
    press_enter("return");
    
}

/*******************************************************************************
 * This function prints all users in the database.
 * INPUTS: user_t * head  OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/
void print_users(user_t * head) {
    user_t * current = head;
    if(head !=NULL) {
        printf("\n\tPrinting All Users Accounts\n\n");
        while (current != NULL) {
            printf("\t│USER: ID:%-20s UNAME:%-20s HASH:%-22lu PERM:%d\n",
            current->userid, current->uname, current->hash, current->perm);
            printf("\t└───────────────────────────────────────────────────"
            "────────────────────────────────────────\n");
            current = current->next;
        }
    }
    else
        printf("No linked list loaded\n");
}


/*******************************************************************************
 * This function prints the entire database currently loaded in linked lists. 
 * prints contents of all low level nodes before moving onto next top node.
 * INPUTS: user_t * head, user_t * curr_login, subject_t * temp    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void print_list(user_t * head) {
    user_t * current = head;
    subject_t * temp = NULL;
    assign_t * pmet = NULL;
    if(head !=NULL) {
        printf("\n\tPrinting Entire Database\n\n");
        /* user node loop start */
        while (current != NULL) {
            printf("\t┌───────────────────────────────────────────────────────"
            "────────────────────────────────────\n");
            printf("\t│USER: ID:%-20s UNAME:%-20s HASH:%-22lu PERM:%d\n", 
            current->userid, current->uname, current->hash, current->perm);
            /* subject node loop start */
            temp = current->down;
            while(temp!=NULL)
            {
                printf("\t│ └──►SUBJECT: CODE:%-5s SNAME:%-30s CREDIT:%d\n", 
                temp->code, temp->sname, temp->credit);
                /* assign node loop start */
                pmet = temp->next;
                while(pmet!=NULL)
                {
                    printf("\t│      └──►ASSIGN: ANAME:%-30s WEIGHT:%.2f "
                    "MARK:%.2f/%.2f\n", pmet->aname, pmet->weight, pmet->mark,
                    pmet->markmax);
                    /* increment through assign nodes */
                    pmet = pmet->next;
                }
                /* increment through subject nodes */
                temp = temp->down;
            }
            printf("\t└────────────────────────────────────────────────────"
            "───────────────────────────────────────\n");
            /* increment through user nodes */
            current = current->next;
        }
    }
    else
        printf("No linked list loaded\n");
}

/*******************************************************************************
 * This function prints a string surrounded by an ansi box, indented by \t.
 * INPUTS: char * str    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void print_box(char * str) {
    int x;
    for(x=0; x<=2*(strlen(str)+3); x++) {
        if(x==0)
            printf("\n\t┌"); /*top left corner*/
        else if (x>0 && x<(strlen(str)+3))
            printf("─"); /*top horizontal*/
        else if (x==(strlen(str)+3))
            printf("┐\n\t│ %s │\n\t└", str); /*t right, string, sides, b left*/
        else if (x>(strlen(str)+3) && x<2*(strlen(str)+3))
            printf("─"); /*bottom horizontal*/
        else if (x==2*(strlen(str)+3))
            printf("┘\n"); /*bottom right*/  
    }
}

/*******************************************************************************
 * This function pauses the program, waiting for user input.
 * INPUTS: char * str    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void press_enter(char * str) {
    printf("\tPress enter to %s\n\t", str);
    getchar();
}

/*******************************************************************************
 * This function prints the details of a user in a box.
 * INPUTS: char * userid, char * uname    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void user_header(char * userid, char * uname) {
    system("clear");
    char str[100];
    sprintf(str, "USER ID: %s Logged in as: %s", userid, uname);
    print_box(str);
}

/*******************************************************************************
 * This function prints the detail of a subject in a box.
 * INPUTS: char * str1, char * str2, char * str3    OUTPUTS: n/a
 * Author: Max McKay
*******************************************************************************/

void manage_header(char * str1, char * str2, char * str3) {
    char str4[100];
    sprintf(str4, "%s %s - %s", str1, str2, str3);
    print_box(str4);  
}


