#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#define MAX_NUM_SUB 10
#define MAX_stu_num_len 9

typedef int bool;
#define true 1
#define false 0

typedef struct Student
{
    char student_ID;
    char student_first_name;
    char student_last_name;
    int total_result;
} student_t;
typedef struct Assessment
{
    char ass_name;
    int mark;
    int grades;
}assessment_t;
typedef struct subject
{
    char subject_ID;
    char subject_name;
    char teacher_ID;
    char t_first_name;
    char t_last_name;
    struct Student student_t;
    struct Assessment assessment_t;
} subject_t;

subject_t subjects[MAX_NUM_SUB];
int NUM_SUB = 0;


void subjct_menu(void);
void student_search(void);
void assessment_manage(void);
void ignore_chars(void);
void View_grade(void);
void Edit_grade(void);
int Check_input(void);
void stumenu(void);

int main(void)
{
    subjct_menu();
    return 0;
}

void subjct_menu(void)
{
    int sub_menu_choice = 0;
    while(sub_menu_choice != 3 )
    {
        printf("\n"
               "1. Search students \n"
               "2. Manage assessment \n"
               "3. Exit \n"
               "please enter your choice (number 1-3) > \n");
        scanf("%d", &sub_menu_choice);
        ignore_chars();
        switch(sub_menu_choice)
        {
            case 1:
            {
                student_search();
                break;
            }
            case 2:
            {
                assessment_manage();
                break;
            }
            case 3:break;
            default:
            {
                printf("\n"
                       "Invalid input\n");
                break;
            }

        }
    }
}

void ignore_chars(void)
{
    char ch = getchar();
    while (ch != EOF)
    {
        if (ch == '\n' || ch == ' ' )
        {
            break;
        }
        ch = getchar();
    }
    return;
}

int Check_input(void)
{
    char num = getchar();
    int Num_input = 0;
    while (num != '\n')
    {
        num = getchar();
        Num_input++;
    }
    return Num_input;
}

bool check_STUID(char STUID[], int len)
{
    if (len != 9  ||
        STUID[0] != 'Z')
    {
        return false;
    }
    int i = 1;
    while(i < len)
    {
        if ('0' > STUID[i] || '9' < STUID[i])
        {
            return false;
        }
        i++;
    }
    return true;
}

void student_search(void)
{
    bool find = false;
    char stuID_input[MAX_stu_num_len + 1] = {0};
    printf("Please enter student ID > \n");
    scanf("%s", stuID_input);
    ignore_chars();

    int i;
    while(i < NUM_SUB)
    {
        if (strncmp(stuID_input, subjects[i].student_t.student_ID,
                    MAX_stu_num_len + 1) == 0)
        {
            find = true;
            break;
        }
        i++;
    }
    if (!find)
    {
        printf("No student found \n");
    }
    else
    {
        printf("%-10s %-10s %-10s\n", "Student ID", "First Name", "Last Name");
        printf("%-10s %-10s %-10s\n", "Student ID", "First Name", "Last Name");
        int i = 0;
        while(i < NUM_SUB)
        {
            if(strncmp(stuID_input, subjects[i].student_t.student_ID,
                       MAX_stu_num_len + 1) == 0);
            {
                printf("%-10s %-10s %-10s\n",
                subjects->student_t.student_ID,
                subjects->student_t.student_first_name,
                subjects->student_t.student_last_name);
            }
        }
        int stumenu_choice = 0;
        while(stumenu_choice != 2 )
        {
            printf("Do you want edit grade for that student? \n"
                   "1.Yes \n"
                   "2.No and Exit\n");
            scanf("%d", &stumenu_choice);
            ignore_chars();
            if (stumenu_choice < 1 || stumenu_choice > 2)
            {
                printf("\n"
                       "Invalid input \n");
                continue;
            }
            if (2 == stumenu_choice)
            {
                break;
            }
            if (1 == stumenu_choice)
            {
                int M_ASS1 = 0;
                while(1)
                {
                    printf("Please enter the grade for assessment1: \n");
                    scanf("%d", &M_ASS1);
                    if(M_ASS1 > subjects[i].assessment_t.mark)
                    {
                        printf("please enter a reasonable grade for assessment1 \n");
                        continue;
                    }
                    else
                    {
                        strcpy(subjects[i].assessment_t.grades, M_ASS1);
                        break;
                    }
                }
            }
        }
    }

}

void assessment_manage(void)
{
    printf("not done\n");
}


/*
void save_data(void)
{
    FILE *file = fopen(DB_NAME, "wb");

    fwrite(&subjects, sizeof(subject_t), MAX_NUM_SUB, file);
    fwrite(&NUM_SUB, sizeof(int), 1, file);
    fclose(file);
}

void load_data(void)
{
    FILE *file = fopen(DB_NAME, "rb");

    if (file == NULL)
    {
        printf("Read error\n");
        return;
    }

    fread(&subjects, sizeof(subject_t), MAX_NUM_SUB, file);
    fread(&NUM_SUB, sizeof(int), 1, file);
    fclose(file);
}
*/
