#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
#endif

struct employee {
    int id;
    char name[50];
    int age;
    float salary;
};

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void wait(int seconds) {
    #ifdef _WIN32
        Sleep(seconds * 1000);  
    #else
        sleep(seconds);       
    #endif
}

bool isOptionSelectValid(const char* userInput) {
    if (!isdigit(userInput[0]) || strlen(userInput) != 1) {
        return false;
    }

    int userInputInt = atoi(userInput);

    return userInputInt >= 1 && userInputInt <= 6;
}

int getUserOption() {
    char userOption[1];
    int userOptionInt;

    for (;;) {
        printf("Please enter your option (1-6): ");
        scanf("%1s", userOption);

        clearInputBuffer();

        if (isOptionSelectValid(userOption)) {
            userOptionInt = atoi(userOption);
            return userOptionInt;
        } else {
            printf("Invalid input. Please enter a number between 1 and 6.\n");
        }
    }
}

struct employee getNewEmployee() {
    struct employee newEmployee;

    printf("Enter Employee ID (number): ");
    while (scanf("%d", &newEmployee.id) != 1) {
        printf("Invalid ID. Please enter an integer: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter Employee Name: ");
    fgets(newEmployee.name, sizeof(newEmployee.name), stdin);
    newEmployee.name[strcspn(newEmployee.name, "\n")] = '\0'; 

    printf("Enter Employee Age: ");
    while (scanf("%d", &newEmployee.age) != 1 || newEmployee.age < 3 || newEmployee.age > 100) {
        printf("Invalid age. Please enter an integer between 3 and 100 (child labour and strong eldery people are supported): ");
        clearInputBuffer();
    }
    clearInputBuffer();

    printf("Enter Employee Salary: ");
    while (scanf("%f", &newEmployee.salary) != 1 || newEmployee.salary < 0) {
        printf("Invalid salary. Please enter a positive number: ");
        clearInputBuffer();
    }
    clearInputBuffer();

    return newEmployee;
}


int main() {
    struct employee *employees = NULL;
    int employeeCount = 0;
    
    for(;;) {
        printf("Employee Management System\n");
        printf("1. Add Employee\n2. Display Employees\n3. Search Employees by ID\n");
        printf("4. Save Employees to File\n5. Load Employees from File\n6. Exit\n");

        int userOption = getUserOption();

        switch (userOption) {
            case 1: {
                struct employee newEmployee = getNewEmployee();
                struct employee *temp = realloc(employees, (employeeCount + 1) * sizeof(struct employee));
                if (!temp) {
                    fprintf(stderr, "Memory allocation failed\n");
                    free(employees);
                    return 1;
                }
                employees = temp;

                employees[employeeCount] = newEmployee;
                employeeCount++;

                printf("New employee added: %d, %s, %d, %.2f\n", newEmployee.id, newEmployee.name, newEmployee.age, newEmployee.salary);
                break;
            }
            case 2:
                if (employeeCount == 0) {
                    printf("No employees found\n");
                    break;
                }
                printf("\nAll Employees:\n");
                for (int i = 0; i < employeeCount; i++) {
                    printf("Employee %d:\n", i + 1);
                    printf("ID: %d\n", employees[i].id);
                    printf("Name: %s\n", employees[i].name);
                    printf("Age: %d\n", employees[i].age);
                    printf("Salary: %.2f\n\n", employees[i].salary);
                }
                break;
            case 3: {
                int searchId;
                printf("Enter Employee ID to search: ");
                while (scanf("%d", &searchId) != 1) {
                    printf("Invalid ID. Please enter an integer: ");
                    clearInputBuffer();
                }
                clearInputBuffer();

                bool found = false;
                for (int i = 0; i < employeeCount; i++) {
                    if (employees[i].id == searchId) {
                        if (!found) {
                            printf("Employees found:\n");
                        }
                        printf("ID: %d\n", employees[i].id);
                        printf("Name: %s\n", employees[i].name);
                        printf("Age: %d\n", employees[i].age);
                        printf("Salary: %.2f\n\n", employees[i].salary);

                        found = true;
                    }
                }

                if (!found) {
                    printf("No employees with ID %d found.\n", searchId);
                }

                break;
            }
            case 4:
                printf("Save employees to file\n");
                break;
            case 5:
                printf("Load employees from file\n");
                break;
            case 6:
                printf("Exiting program\n");
                return 0;
        }
        wait(2);
    }
    
    return 0;
}