
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILE_NAME "complaints.dat"

typedef struct {
    int id;
    char name[100];
    char language[30];
    char location[200];
    char description[500];
    char category[100];
    char department[150];
    char status[50];
} Complaint;

/* Read input safely */
void readInput(const char *message, char *input, int size) {
    int ch;

    printf("%s", message);

    if (fgets(input, size, stdin) == NULL) {
        exit(0);
    }

    if (strchr(input, '\n') == NULL) {
        while ((ch = getchar()) != '\n' && ch != EOF);
    }

    input[strcspn(input, "\n")] = '\0';
}

/* Read numerical input */
int readNumber(const char *message) {
    char input[30];
    char extra;
    int number;

    readInput(message, input, sizeof(input));

    if (sscanf(input, "%d %c", &number, &extra) == 1)
        return number;

    return -1;
}

/* Convert text to lowercase */
void toLowerCase(char *text) {
    for (int i = 0; text[i] != '\0'; i++) {
        text[i] = tolower((unsigned char)text[i]);
    }
}

/* Select preferred language */
void selectLanguage(Complaint *c) {
    printf("\nSELECT PREFERRED LANGUAGE\n");
    printf("1. English\n");
    printf("2. Kannada\n");
    printf("3. Telugu\n");
    printf("4. Hindi\n");
    printf("5. Tamil\n");

    int choice = readNumber("Enter choice: ");

    switch (choice) {
        case 2:
            strcpy(c->language, "Kannada");
            break;
        case 3:
            strcpy(c->language, "Telugu");
            break;
        case 4:
            strcpy(c->language, "Hindi");
            break;
        case 5:
            strcpy(c->language, "Tamil");
            break;
        default:
            strcpy(c->language, "English");
    }
}

/* Assign category and department */
void assignDepartment(Complaint *c, int type) {
    switch (type) {
        case 1:
            strcpy(c->category, "Water Leakage");
            strcpy(c->department, "Local Water Supply Board");
            break;

        case 2:
            strcpy(c->category, "Road Damage");
            strcpy(c->department, "Road Maintenance Department");
            break;

        case 3:
            strcpy(c->category, "Garbage");
            strcpy(c->department, "Solid Waste Management Department");
            break;

        case 4:
            strcpy(c->category, "Streetlight");
            strcpy(c->department, "Municipal Streetlight Department");
            break;

        case 5:
            strcpy(c->category, "Drainage");
            strcpy(c->department, "Drainage Department");
            break;

        default:
            strcpy(c->category, "Other Issue");
            strcpy(c->department, "Local Municipal Helpdesk");
    }
}

/* Intelligent rule-based issue classification */
void classifyIssue(Complaint *c) {
    char text[500];

    strcpy(text, c->description);
    toLowerCase(text);

    int water = strstr(text, "water") != NULL ||
                strstr(text, "leak") != NULL ||
                strstr(text, "pipe") != NULL;

    int road = strstr(text, "road") != NULL ||
               strstr(text, "pothole") != NULL;

    int drain = strstr(text, "drain") != NULL ||
                strstr(text, "sewage") != NULL;

    if (water && (road || drain)) {
        printf("\nCivicConnect Assistant:\n");
        printf("What is the main source of the problem?\n");
        printf("1. Leaking water pipe\n");
        printf("2. Damaged road\n");
        printf("3. Overflowing drain\n");
        printf("4. Not sure\n");

        int choice = readNumber("Enter choice: ");

        if (choice == 1)
            assignDepartment(c, 1);
        else if (choice == 2)
            assignDepartment(c, 2);
        else if (choice == 3)
            assignDepartment(c, 5);
        else
            assignDepartment(c, 0);
    }

    else if (drain) {
        assignDepartment(c, 5);
    }

    else if (water) {
        assignDepartment(c, 1);
    }

    else if (road) {
        assignDepartment(c, 2);
    }

    else if (strstr(text, "garbage") ||
             strstr(text, "waste") ||
             strstr(text, "dustbin")) {
        assignDepartment(c, 3);
    }

    else if (strstr(text, "streetlight") ||
             strstr(text, "street light") ||
             strstr(text, "lamp")) {
        assignDepartment(c, 4);
    }

    else {
        printf("\nPlease select the issue category:\n");
        printf("1. Water leakage\n");
        printf("2. Road damage\n");
        printf("3. Garbage\n");
        printf("4. Streetlight\n");
        printf("5. Drainage\n");
        printf("6. Other\n");

        assignDepartment(c,
            readNumber("Enter choice: "));
    }
}

/* Generate unique complaint ID */
int generateID(void) {
    FILE *fp = fopen(FILE_NAME, "rb");
    Complaint c;
    int maxID = 1000;

    if (fp == NULL)
        return 1001;

    while (fread(&c, sizeof(Complaint), 1, fp) == 1) {
        if (c.id > maxID)
            maxID = c.id;
    }

    fclose(fp);
    return maxID + 1;
}

/* Display complaint details */
void displayComplaint(Complaint c) {
    printf("\n====================================\n");
    printf("       CIVICCONNECT COMPLAINT\n");
    printf("====================================\n");

    printf("Complaint ID : CC%d\n", c.id);
    printf("Name         : %s\n", c.name);
    printf("Language     : %s\n", c.language);
    printf("Location     : %s\n", c.location);
    printf("Issue        : %s\n", c.description);
    printf("Category     : %s\n", c.category);
    printf("Department   : %s\n", c.department);
    printf("Status       : %s\n", c.status);

    printf("====================================\n");
}

/* Generate formal complaint */
void generateComplaint(Complaint c) {
    printf("\nGENERATED COMPLAINT LETTER\n");
    printf("------------------------------------\n");

    printf("To: %s\n", c.department);
    printf("Subject: Complaint regarding %s\n", c.category);

    printf("\nRespected Sir/Madam,\n");

    printf("I would like to report the following issue:\n");
    printf("%s\n", c.description);

    printf("\nLocation: %s\n", c.location);

    printf("Kindly inspect the issue and take ");
    printf("necessary action.\n");

    printf("\nThank you.\n");
    printf("Regards,\n%s\n", c.name);

    printf("------------------------------------\n");
}

/* Register a new complaint */
void registerComplaint(void) {
    Complaint c = {0};
    char confirm[10];

    c.id = generateID();

    printf("\n--- REGISTER NEW COMPLAINT ---\n");

    selectLanguage(&c);

    readInput("Enter name: ",
              c.name, sizeof(c.name));

    readInput("Enter location or landmark: ",
              c.location, sizeof(c.location));

    readInput("Describe the problem: ",
              c.description, sizeof(c.description));

    if (strlen(c.name) == 0 ||
        strlen(c.location) == 0 ||
        strlen(c.description) == 0) {
        printf("\nAll fields are required.\n");
        return;
    }

    classifyIssue(&c);

    strcpy(c.status, "Pending confirmation");

    displayComplaint(c);
    generateComplaint(c);

    readInput("\nIs the suggested department correct? (y/n): ",
              confirm, sizeof(confirm));

    if (confirm[0] == 'n' || confirm[0] == 'N') {
        printf("\n1. Water leakage\n");
        printf("2. Road damage\n");
        printf("3. Garbage\n");
        printf("4. Streetlight\n");
        printf("5. Drainage\n");
        printf("6. Other\n");

        assignDepartment(&c,
            readNumber("Select correct category: "));

        generateComplaint(c);
    }

    readInput("\nSave complaint locally? (y/n): ",
              confirm, sizeof(confirm));

    if (confirm[0] == 'y' || confirm[0] == 'Y') {
        FILE *fp = fopen(FILE_NAME, "ab");

        if (fp == NULL) {
            printf("Error opening complaint file.\n");
            return;
        }

        strcpy(c.status, "Saved - Not officially submitted");

        if (fwrite(&c, sizeof(Complaint), 1, fp) != 1) {
            printf("Error saving complaint.\n");
            fclose(fp);
            return;
        }

        if (fclose(fp) != 0) {
            printf("Error closing complaint file.\n");
            return;
        }

        printf("\nComplaint saved successfully!\n");
        printf("Your Complaint ID: CC%d\n", c.id);
        printf("Suggested Department: %s\n", c.department);
        printf("Keep your complaint ID for tracking.\n");
    }
    else {
        printf("\nComplaint cancelled.\n");
    }
}

/* Track complaint using ID */
void trackComplaint(void) {
    Complaint c;
    int id;
    char input[30];
    char extra;

    readInput("\nEnter complaint ID (example CC1001): ",
              input, sizeof(input));

    if (sscanf(input, "CC%d %c", &id, &extra) != 1 &&
        sscanf(input, "%d %c", &id, &extra) != 1) {
        printf("Invalid complaint ID.\n");
        return;
    }

    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("No complaints found.\n");
        return;
    }

    while (fread(&c, sizeof(Complaint), 1, fp) == 1) {
        if (c.id == id) {
            displayComplaint(c);
            fclose(fp);
            return;
        }
    }

    printf("\nComplaint ID not found.\n");
    fclose(fp);
}

/* Display all locally saved complaints */
void viewComplaints(void) {
    Complaint c;
    FILE *fp = fopen(FILE_NAME, "rb");

    if (fp == NULL) {
        printf("\nNo complaints available.\n");
        return;
    }

    int count = 0;

    while (fread(&c, sizeof(Complaint), 1, fp) == 1) {
        displayComplaint(c);
        count++;
    }

    printf("\nTotal complaints: %d\n", count);
    fclose(fp);
}

/* Display department information */
void departmentDirectory(void) {
    printf("\n--- DEPARTMENT DIRECTORY ---\n");

    printf("Water leakage : Local Water Supply Board\n");
    printf("Road damage   : Road Maintenance Department\n");
    printf("Garbage       : Solid Waste Management\n");
    printf("Streetlights  : Municipal Streetlight Department\n");
    printf("Drainage      : Drainage Department\n");
    printf("Other issues  : Local Municipal Helpdesk\n");

    printf("\nVerify the responsible authority ");
    printf("using your city's official portal.\n");
}

/* Main function */
int main(void) {
    int choice;

    do {
        printf("\n====================================\n");
        printf("           CIVICCONNECT\n");
        printf("    Smart Civic Complaint System\n");
        printf("====================================\n");

        printf("1. Report a new problem\n");
        printf("2. Track a complaint\n");
        printf("3. View saved complaints\n");
        printf("4. Department directory\n");
        printf("5. Exit\n");

        printf("====================================\n");

        choice = readNumber("Enter your choice: ");

        switch (choice) {
            case 1:
                registerComplaint();
                break;

            case 2:
                trackComplaint();
                break;

            case 3:
                viewComplaints();
                break;

            case 4:
                departmentDirectory();
                break;

            case 5:
                printf("\nThank you for using CivicConnect!\n");
                break;

            default:
                printf("\nInvalid choice. Try again.\n");
        }

    } while (choice != 5);

    return 0;
}
