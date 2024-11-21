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
                // Add employee
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
            // List all employees
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
                // Search employees by ID
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
            case 4: {
                // Save employees to file
                if (employeeCount == 0) {
                    printf("No employees found\n");
                    break;
                }

                char fileName[50];

                printf("Enter file name: ");
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strcspn(fileName, "\n")] = '\0';

                FILE *fptr;

                fptr = fopen(fileName, "w");
            
                printf("\nWriting employees to file %s ...\n", fileName);

                for (int i = 0; i < employeeCount; i++) {
                    fprintf(fptr, "ID: %d\n", employees[i].id);
                    fprintf(fptr, "Name: %s\n", employees[i].name);
                    fprintf(fptr, "Age: %d\n", employees[i].age);
                    fprintf(fptr, "Salary: %.2f\n\n", employees[i].salary);
                }

                fclose(fptr); 
                break;
            }
            case 5: {
                // Load employees from file
                char fileName[50];

                printf("Enter file name: ");
                fgets(fileName, sizeof(fileName), stdin);
                fileName[strcspn(fileName, "\n")] = '\0';

                FILE *fptr = fopen(fileName, "r");
                if (!fptr) {
                    printf("There was an error while trying to open file %s\n", fileName);
                    break;
                }

                struct employee tempEmployee;
                int tempCount = 0;
                struct employee *tempEmployees = NULL;
                bool finished = false;
                bool broken = false;

                while (!finished) {
                    if (fscanf(fptr, "ID: %d\n", &tempEmployee.id) != 1) {
                        if (feof(fptr)) {       // end of file
                            finished = true;
                        } else {
                            printf("Cannot read file, invalid format.\n");
                            broken = true;
                            break;
                        }
                    } else {
                        fscanf(fptr, "Name: %49[^\n]\n", tempEmployee.name);
                        fscanf(fptr, "Age: %d\n", &tempEmployee.age);
                        fscanf(fptr, "Salary: %f\n\n", &tempEmployee.salary);

                        // print all the employee info
                        printf("ID: %d\n", tempEmployee.id);
                        printf("Name: %s\n", tempEmployee.name);
                        printf("Age: %d\n", tempEmployee.age);
                        printf("Salary: %.2f\n\n", tempEmployee.salary);

                        struct employee *newTemp = realloc(tempEmployees, (tempCount + 1) * sizeof(struct employee));
                        if (!newTemp) {
                            broken = true;
                            fprintf(stderr, "Memory allocation failed\n");
                            break;
                        }
                        tempEmployees = newTemp;
                        tempEmployees[tempCount] = tempEmployee;
                        tempCount++;
                    } 
                }

                if (broken) {
                    free(tempEmployees);
                    fclose(fptr);
                    break;
                }

                fclose(fptr);

                free(employees);
                employees = tempEmployees;
                employeeCount = tempCount;

                printf("Loaded %d employees from file %s\n", employeeCount, fileName);
                break;
            }
            case 6:
                printf("Exiting program\n");
                return 0;
        }
        wait(2);
    }
    
    return 0;
}