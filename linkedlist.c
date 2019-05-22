#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define DB_NAME "newdatabase7.txt"

struct info { 
    char username[21], firstname[21], lastname[21]; 
    int perm;
};
typedef struct info info_t;

struct node {
    char username[21], firstname[21], lastname[21];  
    long hash;
    int perm;
    struct node * next;
};
typedef struct node node_t;

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

long hash(unsigned char *str)
{
    long hash = 5381;
    int c;

    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    return hash;
}
void print_list(node_t * head) {
    node_t * current = head;
    printf("\nprinting user details\n");
    while (current != NULL) {
        printf("%s %s %s %li %d\n", current->username, current->firstname, current->lastname, current->hash, current->perm);
        current = current->next;
    }
}
void freelist(node_t ** head)
{
   node_t * next_node;

   while (*head != NULL)
    {   
       next_node = (*head)->next;
       free(*head);
       *head = next_node;
    }

}

void login(node_t * head, info_t *temp)
{
    char user[21], pass[21];
    int flag = 0;
    system("clear");
    while(!flag)
    {
        printf("    type 'exit' to cancel\n\n    Enter Username: ");
        fgets(user, 20, stdin);
        process_input(user);
        
        if(strcmp(user, "exit")==0){  
        exit(0);
        }
        
        printf("\n    Enter Password: ");
        fgets(pass, 20, stdin);
        process_input(pass);
        char string[50];
        node_t * current = head;
        strcpy(string, pass);
        strcat(string, user);
        unsigned char input2[200] = "1234567890123456789012345678901234567890";
        memcpy(input2, string, strlen(string));
        long digest;
        digest = hash(input2);
        
        
    while (current != NULL && !flag){  
        if(strcmp(user, current->username)==0 && (digest == current->hash))
        {
            system("clear");
            printf("\nSuccessful Login\n");
            
            strcpy(temp->username, current->username);
            strcpy(temp->firstname, current->firstname);
            strcpy(temp->lastname, current->lastname);
            temp->perm = current->perm;
            flag = 1;
        }
        current = current->next;
    }
        if(!flag)
        {
            system("clear");
            printf("hash: %li", digest);

            printf("\n\n    Invalid Login\n");
            
       
           
        }
        
    }
    
    
}


void push(node_t ** head, char* username, char* firstname, char* lastname, long hash, int perm) {
    node_t * current = *head;
    if(current == NULL){
    current = malloc(sizeof(node_t));
    strcpy(current->username, username);
    strcpy(current->firstname, firstname);
    strcpy(current->lastname, lastname);
    current->hash = hash;
    current->perm = perm;
    current->next = NULL;
    *head = current;
    }
    else{
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = malloc(sizeof(node_t));
    strcpy(current->next->username, username);
    strcpy(current->next->firstname, firstname);
    strcpy(current->next->lastname, lastname);
    current->next->hash = hash;
    current->next->perm = perm;
    current->next->next = NULL;
    }
}

void delete(node_t ** head, char* user){
    
    node_t * current = *head;
    node_t * temp_node = NULL;
    int flag=0;
    while (current->next != NULL &&!flag){  
        if(strcmp(user, "admin")==0)
        {
            printf("cannot delete original account");
        }
        else if(strcmp(user, current->next->username)==0)
        {
            
            printf("Match found");
            
            temp_node = current->next;
            if(current->next->next !=NULL){
                current->next = temp_node->next;
                free(temp_node);
            temp_node = NULL;
            }
            else
            {
            free(current->next);
            current->next = NULL;               
            }
            flag = 1;
        }
        else{
        current = current->next;
        }
    }
    if(!flag)
        {
            printf("no match");
           
        }
     
}

/*
void push(node_t ** head, char* username, long hash, int perm) {
    node_t * new_node;
    new_node = malloc(sizeof(node_t));

    strcpy(new_node->username, username);
    new_node->hash = hash;
    new_node->perm = perm;
    new_node->next = *head;
    *head = new_node;
}

*/
int load_db(node_t ** head) {
    freelist(head);
    FILE* fp = fopen(DB_NAME, "r");  
    if (fp==NULL)
    {
        printf("Read error\n");
        return 0;
    }
    else {
    char u[21], fn[21], ln[21];
    long h;
    int p;
    while(!feof(fp)){
        fscanf(fp,"%s %s %s %li %d\n", u, fn, ln, &h, &p);
        push(head, u, fn, ln, h, p);
    }
    fclose(fp);
    }
    return 1;
}

void save_db(node_t * head) {
    FILE* fp = fopen(DB_NAME, "w");                             /* open file */
    if (fp==NULL)
    {
        printf("Write error\n");
    }
    else{
    node_t * current = head;
    printf("\nprinting user details\n");
    while (current != NULL) {
        fprintf(fp,"%s %s %s %li %d\n", current->username, current->firstname, current->lastname, current->hash, current->perm);
        current = current->next;
    }
    fclose(fp); 
    }
}

void create_db(node_t ** head){
    
    system("clear");
    
    node_t * current = *head;
    int flag= 0;
    char input[51];
    
    
    while(!flag){
        
        printf("\n\n    Error: No database found."
               "\n\n    Would you like to create a new database? y/n\n\n    ");
        fgets(input, 10, stdin);
        process_input(input);
        if(strcmp(input, "y")==0)
        {
            printf("\n    Creating database\n\n");
            
            char user[21], pass[21], fn[21], ln[21], string[50];
            long h;
            int p;
            printf("username, firstname, lastname, pass, perm>\n");
            fgets(input, 50, stdin);
            process_input(input);
            sscanf(input, "%s %s %s %s %d", user, fn, ln, pass, &p);
            
            strcpy(string, pass);
            strcat(string, user);
            unsigned char input2[200] = "1234567890123456789012345678901234567890";
            memcpy(input2, string, strlen(string));
            h = hash(input2);
            flag=1;
            push(&current, user, fn, ln, h, p);
            save_db(current);
            
        }
        else if (strcmp(input, "n")==0)
        {
            printf("\n    Exiting program\n\n");
            exit(0);
        }
        else
        {
            system("clear");
            printf("\n\n    Invalid Input");
        }
        }
    }




void student_menu(info_t *temp, node_t ** head){
    printf("\n    |STUDENT MENU|\n    USER ID: %s\n", temp->username);
    printf("    Logged in as: %s %s\n    Perm: %d\n", temp->firstname, temp->lastname, temp->perm);
    int flag=0;
    node_t * current = *head;
    char input[51];
    while(!flag){
    printf("\n1 change password\n2 exit program\n");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0)
        {
            printf("enter new password>\n");
            
            fgets(input, 30, stdin);
            process_input(input);
            
            char string[50];
            long h;
            
                  strcpy(string, input);
            strcat(string, temp->username);
        unsigned char input2[200] = "1234567890123456789012345678901234567890";
        memcpy(input2, string, strlen(string));
        h = hash(input2);
            delete(&current, temp->username);
            push(&current, temp->username, temp->firstname, temp->lastname, h, temp->perm);
            save_db(current);
        }
        else if (strcmp(input, "2")==0)
        {
            flag = 1;
        }
        }
}        


void teacher_menu(info_t *temp){
    printf("\n    |TEACHER MENU|\n    USER ID: %s\n", temp->username);
    printf("    Logged in as: %s %s\n    Perm: %d\n", temp->firstname, temp->lastname, temp->perm);
}

void admin_menu(info_t *temp, node_t ** head){
    printf("\n    |ADMIN MENU|\n    USER ID: %s\n", temp->username);
    printf("    Perm: %d\n", temp->perm);
    node_t * current = *head;
    int flag= 0;
    char input[51];
    while(!flag)
    {
        printf("\n1 add entry\n2 print entries \n3 save linked list\n4 load db\n5 delete entry\n6 exit program\n");
        fgets(input, 30, stdin);
        process_input(input);
        if(strcmp(input, "1")==0)
        {
            char user[21], pass[21], fn[21], ln[21], string[50];
            long h;
            int p;
            printf("username, firstname, lastname, pass, perm>\n");
            fgets(input, 50, stdin);
            process_input(input);
            sscanf(input, "%s %s %s %s %d", user, fn, ln, pass, &p);
            
            strcpy(string, pass);
            strcat(string, user);
        unsigned char input2[200] = "1234567890123456789012345678901234567890";
        memcpy(input2, string, strlen(string));
        h = hash(input2);
            
            
            push(&current, user, fn, ln, h, p);
        }
        else if (strcmp(input, "2")==0)
        {
               print_list(current);
        }
        else if (strcmp(input, "3")==0)
        {
            save_db(current);
        }
        else if (strcmp(input, "4")==0)
        {
            load_db(&current);
        }
        else if (strcmp(input, "5")==0)
        {
            
            printf("enter username to delete>\n");
            fgets(input, 30, stdin);
            process_input(input);
            delete(&current, input);
        }
        else if (strcmp(input, "6")==0)
        {
            flag = 1;
        }
 
    }
}





int main(void){
    
    node_t * head = NULL;
    /*
    head = (node_t *) malloc(sizeof(node_t));
    if (head == NULL) {
    printf("failed to allocate memory");
    }
    */
    if(load_db(&head)==0){
        create_db(&head);
        freelist(&head);
        load_db(&head);
    }
    
    info_t current_login;
    login(head, &current_login);
    
    

    if((current_login.perm == 1))
    {
        
        student_menu(&current_login, &head);
    }
    else if((current_login.perm == 2))
    {
        freelist(&head);
        teacher_menu(&current_login);
    }
    else if((current_login.perm == 3))
    {
        admin_menu(&current_login, &head);
    }
    else
    {
        printf("invalid permission. terminating program");
        exit(0);
    }
    
    return 0;
}
