#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

enum month {january, february, march, april, may, june, july, august, september, 
october, november, decemer};

const char *month_names[] = {
    "January", "February", "March", "April", "May", "June", 
    "July", "August", "September", "October", "November", "December"
};//more efficient as compiler only needs to allocate 12 ponter to memeory, takes up less memory

#define MAX_TASKS 10
#define MAX_NAME_LEN 40


typedef struct {// basic struct for each task
    char name[MAX_NAME_LEN];
    int start_month;
    int end_month;
    int num_dependencies;
    int dependencies[MAX_TASKS];
} task_t;

void sampleGannt(task_t tasks[], int num_tasks) {//prints out sample diagram if user selcts so
    num_tasks = 10;

    strcpy(tasks[0].name, "Find_Bank");
    tasks[0].start_month = 1;
    tasks[0].end_month = 2;
    tasks[0].num_dependencies = 0;

    strcpy(tasks[1].name, "Get_mortgage_approval");
    tasks[1].start_month = 2;
    tasks[1].end_month = 2;
    tasks[1].num_dependencies = 1;
    tasks[1].dependencies[0] = 3;

    strcpy(tasks[2].name, "Draw_Down");
    tasks[2].start_month = 4;
    tasks[2].end_month = 7;
    tasks[2].num_dependencies = 2;
    tasks[2].dependencies[0] = 0;
    tasks[2].dependencies[1] = 1;

    strcpy(tasks[3].name, "Build_Foundation");
    tasks[3].start_month = 3;
    tasks[3].end_month = 4;
    tasks[3].num_dependencies = 1;
    tasks[3].dependencies[0] = 2;

    // strcpy(tasks[4].name, "Build_Walls");
    // tasks[4].start_month = 4;
    // tasks[4].end_month = 5;
    // tasks[4].num_dependencies = 1;
    // tasks[4].dependencies[0] = 7;

    // strcpy(tasks[5].name, "Roof_and_Ceiling");
    // tasks[5].start_month = 5;
    // tasks[5].end_month = 6;
    // tasks[5].num_dependencies = 2;
    // tasks[5].dependencies[0] = 3;
    // tasks[5].dependencies[1] = 4;

    // strcpy(tasks[6].name, "Plumbing");
    // tasks[6].start_month = 7;
    // tasks[6].end_month = 7;
    // tasks[6].num_dependencies = 1;
    // tasks[6].dependencies[0] = 5;

    
    // strcpy(tasks[7].name, "Electrics");
    // tasks[7].start_month = 8;
    // tasks[7].end_month = 9;
    // tasks[7].num_dependencies = 1;
    // tasks[7].dependencies[0] = 6;

    // strcpy(tasks[8].name, "Inspect_Building");
    // tasks[8].start_month = 9;
    // tasks[8].end_month = 10;
    // tasks[8].num_dependencies = 1;
    // tasks[8].dependencies[0] = 4;
   
    // strcpy(tasks[9].name, "Snagging");
    // tasks[9].start_month = 9;
    // tasks[9].end_month = 10;
    // tasks[9].num_dependencies = 2;
    // tasks[9].dependencies[0] = 7;
    // tasks[9].dependencies[1] = 8;
    // //tasks[9].dependencies[2] = 6;
}


int get_int_input(char *prompt, int min, int max) {
    int input;
    do {
        printf("%s (%d-%d): ", prompt, min, max);
        scanf("%d", &input);
    } while (input < min || input > max);
    return input;
}

void add_task(task_t *tasks, int index, int num_tasks) {// change this to basic printf and scanf statements
    char name[MAX_NAME_LEN];
    int start_month, end_month, num_dependencies, dep_task;

    printf("Please enter the task name: ");
    scanf("%s", name);
    tasks[index].num_dependencies = 0;
    strcpy(tasks[index].name, name);
    start_month = get_int_input("Start month", 1, 12);//make sure only number between 1 and 12
    end_month = get_int_input("End month", 1, 12);
    tasks[index].start_month = start_month;
    tasks[index].end_month = end_month;
    printf("Enter how many dependencies this task has");
    scanf("%d", &num_dependencies);
    tasks[index].num_dependencies = num_dependencies;
    
    for (int i = 0; i < num_dependencies; i++) {
        printf("Enter dependent task");
        scanf("%d", &dep_task);
        tasks[index].dependencies[i] = dep_task;
    }
}

void generate_gantt(task_t *tasks, int num_tasks) {
    int i, j, k;
    int timeline[12] = {0};
    
    printf("\n");
    printf("Gantt chart:\n");
    printf("\n");
    printf("                         ");
    for (i = 0; i < 12; i++) {
        printf("| %9s ", month_names[i]);
    }
    printf("| dependencies |\n");
    
    for (i = 0; i < 31; i++) {
        printf("------");
    }
    printf("\n");
    for (i = 0; i < num_tasks; i++) {
        printf("%-25s", tasks[i].name);
        for (j = 0; j < 12; j++) {
            if (j >= tasks[i].start_month-1 && j < tasks[i].end_month) {
                printf("|    XXX    ");
            } else {
                printf("|           ");
            }
        }
        
        if (tasks[i].num_dependencies > 0) {
            printf("|");
            for (k = 0; k < tasks[i].num_dependencies; k++) {
                printf(" %d ", tasks[i].dependencies[k]+1);
            }
            printf("%*s  |", 12 - tasks[i].num_dependencies * 3, "");//prints a blank string that length depends on the number of dependencies
        } else {
            printf("|              |");
        }
        printf("\n");
    }

    for (i = 0; i < 31; i++) {
        printf("------");
    }
    printf("\n");
}

void editGannt(task_t *tasks, int num_tasks) {
    char taskToEdit[MAX_NAME_LEN];

    printf("Please enter task you would like to edit");
    scanf("%s", taskToEdit);

    for(int i=0; i<num_tasks; i++) {
        if(strcmp(tasks[i].name, taskToEdit)==0) {
            printf("task found\n");
            add_task(tasks, i, num_tasks);
        }
    }

}

int checkIfCircular(task_t *tasks, int dependentTaskIndex, int resetVisitedTasks[], int taskToBeChecked) {
    for (int i = 0; i < tasks[dependentTaskIndex].num_dependencies; i++) {
        int childTaskIndex = tasks[dependentTaskIndex].dependencies[i];
        if (resetVisitedTasks[taskToBeChecked] == 1) {
            //printf("circular dependency found\n");
            return 1;
        } 
        else if (checkIfCircular(tasks, childTaskIndex, resetVisitedTasks, taskToBeChecked) == 1) {
                return 1;
            }
        
    }
    resetVisitedTasks[dependentTaskIndex] = 0; // reset visited flag when all children of a node have been checked
    return 0;
}








void printDependentTasks(task_t *tasks, int taskIndex, int visitedTasks[]) {// there is no gaurd statement
    int taskToBeChecked;
    
    printf("%d -> ", taskIndex+1);
    visitedTasks[taskIndex] = 1;//the starting task is given a one 
    int resultFromCircular;
    for(int i=0; i < tasks[taskIndex].num_dependencies; i++) { //for loop until reaches number of dependencies
        int dependentTaskIndex = tasks[taskIndex].dependencies[i];//goes through dependent tasks and quit
        
        if(visitedTasks[dependentTaskIndex] == 0) {
            printDependentTasks(tasks, dependentTaskIndex, visitedTasks);
            
        }

        else 
        {
            int resetVisitedTasks[MAX_TASKS]={0};
            taskToBeChecked=dependentTaskIndex;
            printf("( !!!! warning potential circular dependency !!!!!) \n");
            resultFromCircular=checkIfCircular(tasks, dependentTaskIndex, resetVisitedTasks, taskToBeChecked);
           printf("circular dependency result is %d", resultFromCircular);
            if(resultFromCircular==1) {
                printf("( !!!! circular dependency found !!!!!) \n");
            }
            else if(resultFromCircular==0) {
                printf("( !!!!  circular dependency not found !!!!!) \n");
            }
        }

    }
}


int taskIndex(task_t *tasks, int num_tasks) {

    char testTask[MAX_NAME_LEN];

    printf("Please enter the task name to test");                           //no neared 
    scanf("%39s", testTask);

    for(int i=0; i<num_tasks; i++) {
        if(strcmp(tasks[i].name, testTask)==0) {
            printf("task found\n");
            return i;
        }
    }
    
    printf("Task not found please try again");
    return -1;
}
int main() {
    task_t tasks[MAX_TASKS];// array of structs
    int num_tasks;
    char choice[3];
    char userChoice[7];
    int i, j, index;

    
    printf("Welcome to the Gantt Generator\n");
    printf("Would you like to use the test example or create your own Gantt from scratch? (yes or no)\n");
    scanf("%s", choice);
    if (strcmp(choice, "no") == 0) {//creates sample Gannt diagram
        num_tasks = 4;

        sampleGannt(tasks, num_tasks);

        generate_gantt(tasks, num_tasks);
    } 
    
    else {
        num_tasks = get_int_input("How many tasks would you like to add?", 1, MAX_TASKS);
        for (i = 0; i < num_tasks; i++) {
            add_task(tasks, i, num_tasks);
            
        }
        generate_gantt(tasks, num_tasks);
    }

   

    do{
        
        printf("If you wish to edit the Gantt please type 'edit' / If you wish to run a test, type 'test' or to exit type 'quit' and press enter to exucute\n");
        scanf("%6s", userChoice);//safer
        if(strcmp("edit", userChoice)==0) {
            editGannt(tasks, num_tasks);
            generate_gantt(tasks, num_tasks);//something wrong with dependencies. Including inputs and printing. Index has me confused is find_ban 1 or 0 ect
            //error if you enter a number larger than it dependent
        }
        else if (strcmp("test", userChoice)==0) {
            int visitedTasks[MAX_TASKS]={0};
            index=taskIndex(tasks, num_tasks);
            printDependentTasks(tasks, index, visitedTasks) ;//starts at 0
           
        }
    }while(strcmp("quit", userChoice)!=0);

    return 0;
}

