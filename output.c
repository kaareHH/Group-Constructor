
#include "allIncludes.h"

/* ---------------------- Prompt user for which output it wants ---------------------- */
void switchForOutput(structStudent *student, structGroup *group, int amountGroup)
{
    int input;
    
    printf("In which format do you want your output?\n");
    do
    {
        printf("Press 1 for terminal\n"
               "Press 2 for document\n"
               "Press 3 for both\n: ");
        scanf(" %d", &input);

    } while(input <= 1 && input >= 3);

    switch (input)
    {
        case 1: printGroupsToTerminal(student, group, amountGroup); break;
        case 2: printGroupsToFile(student, group, amountGroup); break;
        case 3: printGroupsToTerminal(student, group, amountGroup); printGroupsToFile(student, group, amountGroup); break;
    }
}

/* ---------------------- Creates a .txt file of the groups in the directory ---------------------- */
void printGroupsToFile(structStudent *student, structGroup *group, int amountGroup)
{
    int i, j;
    FILE* fileHandle = fopen("result_group.txt", "w");


    for(i = 0; i < amountGroup; i++)
    {
        fprintf(fileHandle, "Group #%d: ", i+1);
        fprintf(fileHandle, "roles covered [");

        for (j = 0; j < BELBINS_ROLES; j++)
        {
            fprintf(fileHandle, " %c", group[i].roleCovered[j] ? j + ASCII_DIGIT_ONE : '-');
        }
        fprintf(fileHandle, " ] Total [%d]\n-------------------------------------------------------\n", group[i].roleSum);

        for(j = 0; j < group[i].amountOfMembers; j++)
        {
            fprintf(fileHandle, "%s \n", student[group[i].indexStudent[j]].name);

        }
    }
    
    fprintf(fileHandle, "\nEnd of document");
    fclose(fileHandle);
}

/* ---------------------- Prints the groups in the terminal ---------------------- */
void printGroupsToTerminal(structStudent *student, structGroup *group, int amountGroup)
{
    int i, j;
    
    for(i = 0; i < amountGroup; i++)
    {
        printf("Group #%d: ", i+1);
        printf("roles covered [");

        for(j = 0; j < BELBINS_ROLES; j++)
        {
            printf(" %c", group[i].roleCovered[j] ? j + ASCII_DIGIT_ONE : '-');
        }
        printf(" ] Total [%d]\n-------------------------------------------------------\n", group[i].roleSum);

        for(j = 0; j < group[i].amountOfMembers; j++)
        {
            printf("%s \n", student[group[i].indexStudent[j]].name);
        }
        putchar('\n');
    }
}
