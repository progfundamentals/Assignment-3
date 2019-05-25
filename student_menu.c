/* Wassim Kheireddine - 12934081*/

#include <stdio.h>
#define SUB_NUM 5
typedef struct 
{

	
}student_assessment;

typedef struct 
{


} student_t;

void print_student_menu(void);
void student_menu(int counter,student_t student);
void search_subcode(int counter, student_t student);
void display_gpa(int counter,student_t student);

int main()
{

}
void print_student_menu(void)
{
	printf("\n"
    "1. Search Subject Code\n"
    "2. Display Current GPA\n"
    "3. Exit the program\n"
    "Enter choice (number between 1-3)>\n");
}

int student_menu()
{	
	student_t student;
	print_student_menu();
	scanf("%d",&num_choice);
	int num_choice;
	int counter;
	counter=0;
	while(num_choice!=3)
	{		 
		if(num_choice!=1 && num_choice!=2 &&
		   num_choice!=3)
    	{
    	    printf("Invalid choice\n");
    	} 
    	else if(num_choice==1)
    	{	
    		search_subcode();
    		counter++;
    	}
    	else if(num_choice==2)
    	{
    		display_gpa();
    	}
    	else if(num_choice==3)
    	{

    	}
    	return 0;

	}
}

void search_subcode(int counter, student_t student)
{

	char subject_code[SUB_NUM];
	int valid =0;
	int i;
	printf("Enter Subject Code\n");
	scanf("%s",subject_code);
	for(i=0;i<counter;i++)
	{
		if(strcmp(student[i].sub_code,subject_code)==valid)
		{
			printf(" Assessment Name Total Mark \n");
			printf(" --------------- ---------- \n");
			if(valid==0 && counter>0)
			{
				printf("%s\n",student[i].sub_code);
				printf("%s",student[i].subject.Assessment.Ass_Name);
				printf("%d/%d",student[i].subject.Assessment.mark,
						student[i].subject.Assessment.Grade);
				printf("Total Subject Mark is : %d/100\n",
					   subject.student.total_result);
			}
			else()
			{
				printf("Invalid Subject Code\n");
			}

	} 


void display_gpa(int counter, student_t student)
{
	int i;
	printf("Completed Subjects\n");
	if(counter>0)
	{
		printf("Code  Subject Name         Result            Mark \n");
		printf("----- -------------------- ----------------  ---\n");
		for (i = 0; i < counter ; i++)
		{
			printf("%-5s,-%20s,%-16s,%-3s ",
		   			student[i].student.subject_code, 
		   			student[i].subject.Assessment.Ass_Name,
		   			/*HD, D,etc.*/, 
		   			student[i].subject.student.total_result)
		}
	}
}

