/* ---------------------------------------------------------------- LIBARIEIS ---------------------------------------------------------------- */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <dirent.h>

/* ---------------------------------------------------------------- DEFINES ---------------------------------------------------------------- */
#define CHAR_LENGTH 55
#define BELBINS_ROLES 9
#define FALSE 0
#define TRUE 1
#define ASCII_DIGIT_ONE '1'
#define NO_USERINPUT TRUE

/* ---------------------------------------------------------------- STRUCTS ---------------------------------------------------------------- */
typedef struct student
{
    char name[CHAR_LENGTH];
    int  role[BELBINS_ROLES];
}structStudent;

typedef struct
{
    char name[100];
} structfile;

typedef struct
{
    int sum;
    int role;
} structAmountRole;

typedef struct
{
    int *indexStudent;
    int amountOfMembers;
    int roleCovered[BELBINS_ROLES];
    int roleSum;
} structGroup;


/*---------------------------------------------------------------- PROTOTYPES --------------------------------------------------------------------*/

/* -------------------------------------------------  main  ------------------------------------------------- */
int userInputGroups(int numberOfStudents);
void freeAll(structStudent *student, structGroup *groupResult, int amountGroup);

/* -------------------------------------------------  input  ------------------------------------------------- */
structStudent *loadStructStudent(int *numberOfStudentsOutput);
structStudent loadStudent(FILE *fileHandle);
structStudent *expandStructStudent(structStudent *inputPointer, int size);
void initializeRoleStudent(structStudent *tempStruct, char *tempString);
structfile *userSelectFile();

/*  -------------------------------------------------  algoritme -------------------------------------------------  */
structGroup *algoritme(structStudent *student, int numberOfStudents, int amountGroup);
int calcAverageRolePrGroup(structStudent *student, int numberOfStudents, int amountGroup, structAmountRole *roleSumAndIndex);
int calculateSum(structStudent *student, int numberOfStudents, int amountGroup, structAmountRole *roleSumAndIndex);
void sortRoleFrequency(structStudent *student, int size, structAmountRole *roleSumAndIndex, int numberOfStudents);
int compareSumRole(const void *a, const void *b);
structGroup *assignStudentToGroup(structStudent *student, int numberOfStudents, int amountGroup, int averageRoleAmount, structAmountRole *roleSumAndIndex);
void initializeRoleFalseGroup(structGroup *group, int amountGroup);
int *groupsWithMaxPoints(structGroup *group, int amountGroup, int numberOfStudents, structStudent student, int *gainedPointsInGroup, int *countOfMaxPoints, int averageRoleAmount);
int lowestScoreGroupCalc(structGroup *group, int amountGroup, int averageRoleAmount, int maxMemberCount, int *gainedPoints);
void insertStudentInGroup(int i, int j, structStudent *student, structGroup *group);
void groupWithLeastStudents(structGroup *group, structStudent *student, int **groupIndex, int groupIndexSize, int amountGroup, int numberOfStudents, int studentId, int leastStudent, int averageRoleAmount);
int checkRareRole(structStudent student, structGroup group, int amountGroup, structAmountRole *roleSumAndIndex);

/* -------------------------------------------------  Output  ------------------------------------------------- */
void switchForOutput(structStudent *student, structGroup *group, int amountGroup);
void printGroupsToFile(structStudent *student, structGroup *group, int amountGroup);
void printGroupsToTerminal(structStudent *student, structGroup *group, int amountGroup);
