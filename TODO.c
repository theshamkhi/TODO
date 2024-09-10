#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 100

typedef struct {
    char fullName[MAX];
    char email[MAX];
} User;


typedef struct {
    int ID;
    char title[MAX];
    char description[MAX];
    char status[10];
    char deadline[11];
    char creationDate[MAX];
    User *collaborators;
    int numCollaborators;
} Task;

int i, choice;
int numTasks = 0;

Task tasks[MAX];


// Function to add collaborators
void AddPartner(Task *newT) {
    int n;
    printf("How many collaborators would you like to add?\n");
    scanf("%d", &n);

    newT->collaborators = malloc(n * sizeof(User));
    newT->numCollaborators = n;

    for (i = 0; i < n; i++) {
        printf("---------------------------------\n");
        printf("Enter the collaborator's full name:\n");
        scanf(" %[^\n]", newT->collaborators[i].fullName);
        printf("Enter their email:\n");
        scanf(" %[^\n]", newT->collaborators[i].email);
    }
}

// Function to add a new task
void Addnew() {
    Task newT;
    newT.ID = numTasks + 1;

    printf("--------------------------------\n");
    printf("Enter the task title:\n");
    scanf(" %[^\n]", newT.title);
    printf("Enter the task description:\n");
    scanf(" %[^\n]", newT.description);
    strcpy(newT.status, "TO DO");
    printf("Enter the task deadline (format: DD-MM-YYYY):\n");
    scanf(" %[^\n]", newT.deadline);

    AddPartner(&newT);

    time_t now = time(NULL);
    struct tm *local_time = localtime(&now);
    strftime(newT.creationDate, sizeof(newT.creationDate), "%c", local_time);

    tasks[numTasks++] = newT;
}

// Function to add more tasks
void Addmore() {
    int more;
    printf("How many more tasks would you like to add?\n");
    scanf("%d", &more);
    for (i = 0; i < more; i++) {
        Addnew();
    }
}

// Function to display all tasks
void DisplayAll() {
    if(numTasks > 0) {
        printf("\nAll available tasks:\n");
        printf("---------------------------------\n");
        for (i = 0; i < numTasks; i++) {
            printf("[%d]. %s\n", tasks[i].ID, tasks[i].title);
            printf("Description: %s\n", tasks[i].description);
            printf("Deadline: %s\n", tasks[i].deadline);
            printf("Status: %s\n", tasks[i].status);
            printf("Created on: %s\n", tasks[i].creationDate);
            if (strcmp(tasks[i].status, "DONE") == 0) {
                printf("Collaborators for this task:\n");
                printf("--------------------------------\n");
                for (int j = 0; j < tasks[i].numCollaborators; j++) {
                    printf("[%d] %s\n", j+1, tasks[i].collaborators[j].fullName);
                    printf("Email: %s\n", tasks[i].collaborators[j].email);
                }
                printf("--------------------------------\n");
            }
        }
    } else {
        printf("You have no tasks TO DO.\n");
    }
}

// Function to edit a task
void Edit() {
    int id;
    printf("Enter task ID: ");
    scanf("%d", &id);
    if (id <= 0 || id > numTasks) {
        printf("Invalid task ID.\n");
        return;
    }
    printf("1. Edit task description\n");
    printf("2. Edit task status\n");
    printf("3. Edit task deadline\n");
    printf("Select an option from the list above: ");
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            printf("Enter new description: ");
            scanf(" %[^\n]", tasks[id - 1].description);
            break;
        case 2:
            printf("Enter new status (TO DO, DOING, DONE): ");
            scanf(" %[^\n]", tasks[id - 1].status);
            break;
        case 3:
            printf("Enter new deadline (format: YYYY-MM-DD): ");
            scanf(" %[^\n]", tasks[id - 1].deadline);
            break;
        default:
            printf("Invalid option.\n");
    }
    printf("Task updated successfully.\n");
}

// Function to delete a task
void Delete() {
    int id;
    printf("Enter task ID to delete: ");
    scanf("%d", &id);
    if (id <= 0 || id > numTasks) {
        printf("Invalid task ID.\n");
        return;
    }
    for (i = id - 1; i < numTasks - 1; i++) {
        tasks[i] = tasks[i + 1];
    }
    numTasks--;
    printf("Task deleted successfully.\n");
}

// Function to search tasks
void Search() {
    int id;
    char title[100];
    printf("1. Search by ID\n");
    printf("2. Search by title\n");
    printf("Select an option from the list above: ");
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            printf("Enter task ID: ");
            scanf("%d", &id);
            if (id <= 0 || id > numTasks) {
                printf("Invalid task ID.\n");
                return;
            }
            for (i = 0; i < numTasks; i++) {
                if (tasks[i].ID == id) {
                    printf("[%d]. %s\n", tasks[i].ID, tasks[i].title);
                    printf("Description: %s\n", tasks[i].description);
                    printf("Deadline: %s\n", tasks[i].deadline);
                    printf("Status: %s\n", tasks[i].status);
                    printf("---------------------------------\n");
                    return;
                }
            }
            printf("Task not found.\n");
            break;
        case 2:
            printf("Enter task title: ");
            scanf(" %[^\n]", title);
            for (i = 0; i < numTasks; i++) {
                if (strcmp(tasks[i].title, title) == 0) {
                    printf("[%d]. %s\n", tasks[i].ID, tasks[i].title);
                    printf("Description: %s\n", tasks[i].description);
                    printf("Deadline: %s\n", tasks[i].deadline);
                    printf("Status: %s\n", tasks[i].status);
                    printf("---------------------------------\n");
                    return;
                }
            }
            printf("Task not found.\n");
            break;
        default:
            printf("Invalid option.\n");
    }
}

// Function to calculate days remaining until deadline
double DaysRemaining(const char *deadline_str) {
    int day, month, year;
    sscanf(deadline_str, "%d-%d-%d", &day, &month, &year);

    struct tm deadline_tm = {0};
    deadline_tm.tm_mday = day;
    deadline_tm.tm_mon = month - 1;
    deadline_tm.tm_year = year - 1900;
    deadline_tm.tm_hour = 0;
    deadline_tm.tm_min = 0;
    deadline_tm.tm_sec = 0;
    deadline_tm.tm_isdst = -1; // Let mktime determine if daylight saving time is in effect

    time_t deadline_date = mktime(&deadline_tm);
    time_t now = time(NULL);

    if (deadline_date != -1) {
        return difftime(deadline_date, now) / (60 * 60 * 24);
    } else {
        return -1; // Invalid deadline
    }
}

// Function to sort tasks by title or deadline
int compareByTitle(const void *a, const void *b) {
    return strcmp(((Task *)a)->title, ((Task *)b)->title);
}

int compareByDeadline(const void *a, const void *b) {
    return strcmp(((Task *)b)->deadline, ((Task *)a)->deadline);
}

void Sort() {
    printf("1. Sort by title\n");
    printf("2. Sort by deadline\n");
    printf("3. Display tasks with 3 days remaining until deadline\n");
    printf("Select an option from the list above: ");
    scanf("%d", &choice);
    switch(choice) {
        case 1:
            qsort(tasks, numTasks, sizeof(Task), compareByTitle);
            break;
        case 2:
            qsort(tasks, numTasks, sizeof(Task), compareByDeadline);
            break;
        case 3:
            printf("\nAll tasks with 3 days or less remaining:\n");
            for(i = 0; i < numTasks; i++) {
                if(DaysRemaining(tasks[i].deadline) <= 3) {
                    printf("---------------------------------\n");
                    printf("[%d]. %s\n", tasks[i].ID, tasks[i].title);
                    printf("Description: %s\n", tasks[i].description);
                    printf("Deadline: %s\n", tasks[i].deadline);
                    printf("Status: %s\n", tasks[i].status);
                    printf("---------------------------------\n");
                }
            }
            break;
        default:
            printf("Invalid option.\n");
            return;
    }
    printf("Tasks sorted successfully.\n");
}


// Function to display statistics
void Statistics() {
    int total = 0;
    int done = 0;
    int still = 0;

    for (i = 0; i < numTasks; i++) {
        total++;
        if (strcmp(tasks[i].status, "DONE") == 0) {
            done++;
        } else if (strcmp(tasks[i].status, "TO DO") == 0 || strcmp(tasks[i].status, "DOING") == 0) {
            still++;
        }

        double days_remaining = DaysRemaining(tasks[i].deadline);
        if (days_remaining != -1) {
            printf("Task [%d] has %.0f days remaining until deadline.\n", tasks[i].ID, days_remaining);
        } else {
            printf("Task [%d] has an invalid deadline.\n", tasks[i].ID);
        }
    }

    printf("The total number of tasks is: %d\n", total);
    printf("The total number of tasks completed is: %d\n", done);
    printf("The total number of tasks incomplete is: %d\n", still);
}

int main() {
    do {
        printf("--------------MENU--------------\n");
        printf("1. ADD A NEW TASK\n");
        printf("2. ADD MORE TASKS\n");
        printf("3. DISPLAY ALL TASKS\n");
        printf("4. EDIT A TASK\n");
        printf("5. DELETE TASK BY ID\n");
        printf("6. SEARCH FOR A TASK\n");
        printf("7. SORT TASKS\n");
        printf("8. STATISTICS\n");
        printf("0. EXIT\n");
        printf("--------------------------------\n");
        printf("Select an option from the list above:\n");
        scanf("%d", &choice);
        switch(choice) {
            case 1:
                Addnew();
                break;
            case 2:
                Addmore();
                break;
            case 3:
                DisplayAll();
                break;
            case 4:
                Edit();
                break;
            case 5:
                Delete();
                break;
            case 6:
                Search();
                break;
            case 7:
                Sort();
                break;
            case 8:
                Statistics();
                break;
            case 0:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option.\n");
                break;
        }
    } while (choice != 0);

    return 0;
}
