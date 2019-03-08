#include "allIncludes.h"

/* ---------------------- Goes through the algoritmes steps, returns divided groups ---------------------- */
structGroup *algoritme (structStudent *student, int numberOfStudents, int amountGroup)
{
    //Hvor mange roller der skal væære i hver gruppe
    int averageRoleAmount;
    structAmountRole roleSumAndIndex[BELBINS_ROLES];

    //Udregn hvor mange roller der skal være i hver gruppe
    averageRoleAmount = calcAverageRolePrGroup(student, numberOfStudents, amountGroup, roleSumAndIndex);
    sortRoleFrequency(student, numberOfStudents, roleSumAndIndex, numberOfStudents);

    return assignStudentToGroup(student, numberOfStudents, amountGroup, averageRoleAmount, roleSumAndIndex);
}

/* ---------------------- Calculates the average role pr. group ---------------------- */

int calcAverageRolePrGroup(structStudent *student, int numberOfStudents, int amountGroup, structAmountRole *roleSumAndIndex)
{               //Udregn sum
    int sum = calculateSum(student, numberOfStudents, amountGroup, roleSumAndIndex);
    //Dividere med amountgroup, for at få gennemsnittet af roller.
    return sum / amountGroup;
}

/* ---------------------- Calculates excess roles and returns the sum of avaliable roles ---------------------- */
int calculateSum(structStudent *student, int numberOfStudents, int amountGroup, structAmountRole *roleSumAndIndex)
{
    int sumRole = 0, 
        i, 
        j, 
        excessRole;

    for (i = 0; i < BELBINS_ROLES; i++)
    {   //Overskydende af hver rolle
        excessRole = 0;

        for (j = 0; j < numberOfStudents; j++)
        {                   //Er enten true eller false, og tæller op for hver enkelte rolle
            excessRole += student[j].role[i];
        }
        
        roleSumAndIndex[i].sum = excessRole;
        roleSumAndIndex[i].role = i;

        
        if (excessRole >= amountGroup)
        {
            sumRole += amountGroup;
        }
        else
        {   //Hvis der er færre roller end antallat af grupper, så kan alle grupper ikke få hver rolle. 
            sumRole += excessRole;
        }

    }
    
    return sumRole;
}

/* ---------------------- Sorts the roles by frequency. The least common role will be first  ---------------------- */
void sortRoleFrequency (structStudent *student, int size, structAmountRole *roleSumAndIndex, int numberOfStudents)
{
    structStudent tmp;
    int i=0,
        j=0,
        studentCounter = 0;
            //Er bare et struct med roller og sum, altså hvor mange af der af hver rolle.
    qsort(roleSumAndIndex, BELBINS_ROLES, sizeof(structAmountRole), compareSumRole);

    //Starter fra rolle 0
    for(i = 0; i < BELBINS_ROLES; i++)
    {
        //Starter fra første studerende
        for(j = 0; j < numberOfStudents; j++)
        {
            //Hvis studerendeX har rolle 
            if (student[j].role[roleSumAndIndex[i].role])
            {
                //Har jeg sorteret ham i forvejen?
                if (j > studentCounter)
                {
                    //Vi bytter rundt på to elever
                    tmp = student[studentCounter];
                    student[studentCounter++] = student[j];
                    student[j] = tmp;
                }
                else if (j == studentCounter)
                {
                    //Hvis eleven står på den rigtige plads så lader vi ham være, men tæller studentcounter op så vi sammenligner med den næste elev.
                    studentCounter++;
                }
                 
            }
        }    
    }    
}

/* ---------------------- Compare function for qsort ---------------------- */
int compareSumRole(const void *a, const void *b)
{   //Qsort funktion
    const structAmountRole *role2 = a;
    const structAmountRole *role1 = b;

    if (role2->sum > role1->sum)
    {
        return 1;
    }
    else if (role2->sum < role1->sum)
    {
        return -1;
    }
    else 
    {
        return role2->role - role1->role;
    }
}

/* ---------------------- Calculate in which group, a student is most benificial. Returns the divided groups ---------------------- */
structGroup *assignStudentToGroup(structStudent *student, int numberOfStudents, int amountGroup, int averageRoleAmount, structAmountRole *roleSumAndIndex)
{
    int* groupIndex = NULL,
         i,
         *gainedPointsInGroup, 
         maxMemberCount, 
         completedGroups, 
         countOfMaxPoints = 0;

    structGroup *group;
    group = (structGroup *)malloc(sizeof(structGroup) * amountGroup);
    //Sætter alle roller i gruppen til at være nul.
    initializeRoleFalseGroup(group, amountGroup);
    //Hvor mange points der er i hver gruppe
    gainedPointsInGroup = (int *)malloc(sizeof(int) * amountGroup);
    //Hvor mange mennesker må der maks være i en gruppe. 
    maxMemberCount = (int) ceil((double)numberOfStudents / (double)amountGroup);
    
    for(i = 0; i < numberOfStudents; i++)
    {
        //Finder de grupper eleven kan give flest point til.
        groupIndex = groupsWithMaxPoints(group, amountGroup, numberOfStudents, student[i], gainedPointsInGroup, &countOfMaxPoints, averageRoleAmount);
        //Finder den gruppe med færrest point, som eleven kan færdiggøre
        completedGroups = lowestScoreGroupCalc(group, amountGroup, averageRoleAmount, maxMemberCount, gainedPointsInGroup);
        //Hvis der fandtes en gruppe som eleven kunne fuldføre, og hvis eleven IKKE har en 'rare' role, så vil if være sand. 
        if(completedGroups >= 0 && checkRareRole(student[i], group[completedGroups], amountGroup, roleSumAndIndex))
        {
            insertStudentInGroup(i, completedGroups, student, group);
        }
        else
        {
            groupWithLeastStudents(group, student, &groupIndex, countOfMaxPoints, amountGroup, numberOfStudents, i, maxMemberCount, averageRoleAmount);
        }
         free(groupIndex);
         groupIndex = NULL;
    }
   
    free (gainedPointsInGroup);
    
    return group;
}

/* ---------------------- Initialize all groups to a zeroStruct ---------------------- */
void initializeRoleFalseGroup(structGroup *group, int amountGroup)
{
    int i;
    structGroup zeroStruct = {NULL, 0, {0, 0, 0, 0, 0, 0, 0, 0, 0}, 0};
    
    for (i = 0; i < amountGroup; i++)
    {
        group[i] = zeroStruct;
    }
}

/* ---------------------- Finds in which groups, the student gives most points to and returns the result ---------------------- */
int *groupsWithMaxPoints(structGroup *group, int amountGroup, int numberOfStudents, structStudent student, int *gainedPointsInGroup, int *countOfMaxPoints, int averageRoleAmount)
{
    int i,
        j,
        maxPoints = 0,
        *groupIndex = NULL,
        index = 0;

    //Gør plads til en gruppe
    groupIndex = (int *)malloc(sizeof(int));

    for (i = 0; i < amountGroup; i++)
    {
        gainedPointsInGroup[i] = 0;
        for (j = 0; j < BELBINS_ROLES; j++)
        {
            //Hvis gruppen IKKE har rollen, og eleven har rollen samt at gruppen ikke overstiger gennemsnitlige rollesum.
            if (group[i].roleCovered[j] == 0 && student.role[j] == 1 && group[i].roleSum < averageRoleAmount)
            {
                gainedPointsInGroup[i]++;
            }
        }

        if ((gainedPointsInGroup[i]) > maxPoints)
        {
            index = 0;
           
                groupIndex = (int *) realloc(groupIndex, sizeof(int));
            
            
            maxPoints = gainedPointsInGroup[i];
            groupIndex[index] = i;
        }
        else if (gainedPointsInGroup[i] == maxPoints && maxPoints > 0)
        {
            index++;
            groupIndex = (int *)realloc(groupIndex, (sizeof(int) * (index + 1)));
            groupIndex[index] = i;
        }
    }
    *countOfMaxPoints = index;
    
    return groupIndex;
}

/* ---------------------- Checks is a student completes the group. Returns the group, with currently lowest score, if the student completes one ---------------------- */
int lowestScoreGroupCalc(structGroup *group, int amountGroup, int averageRoleAmount, int maxMemberCount, int *gainedPointsInGroup)
{
    int lowestScore = BELBINS_ROLES,
        completedGroups = -1,
        i;
    
    for (i = 0; i < amountGroup; i++)
    {
        if (group[i].roleSum != averageRoleAmount && group[i].roleSum + gainedPointsInGroup[i] == averageRoleAmount && group[i].amountOfMembers < maxMemberCount - 1 && group[i].roleSum < lowestScore)
        {
            lowestScore = group[i].roleSum;
            completedGroups = i;
        }       
    }

    return completedGroups;
}

/* ---------------------- Function to insert a stundet in a group, and updates group info ---------------------- */
void insertStudentInGroup(int i, int j, structStudent *student, structGroup *group)
{
    int k;
    
    if (group[j].amountOfMembers == 0)
    {
        group[j].indexStudent = (int *)malloc(sizeof(int));
    }
    else
    {
        group[j].indexStudent = (int *)realloc(group[j].indexStudent, sizeof(int) * (group[j].amountOfMembers + 1));
    }
    //For at finde de tidligere medlemmer, og indsætter eleven efter dem der er der i forvejen
    //Så Philip er fx studerende nummer 10, og det nummer kommer ind i gruppens index. Sådan at gruppemedlem 4, har index 10, sådan ved vi at gruppemedlem 10 er Philip.
    group[j].indexStudent[group[j].amountOfMembers] = i;

    group[j].amountOfMembers++;

    
    for (k = 0; k < BELBINS_ROLES; k++)
    {
        if (student[i].role[k] == TRUE && group[j].roleCovered[k] == 0)
        {
            group[j].roleCovered[k] = TRUE;
            group[j].roleSum++;
        }
    }
    
}

/* ---------------------- Function to determine which gruop contains the least amount of students. Only compares in groups the student benefits the most  ---------------------- */
void groupWithLeastStudents(structGroup *group, structStudent *student, int **groupIndex, int groupIndexSize, int amountGroup, int numberOfStudents, int studentId, int leastStudent, int averageRoleAmount)
{
    int i,
        result = -1,
        leastPoint = BELBINS_ROLES;


    for (i = 0; i <= groupIndexSize; i++)
    {
        if (group[(*groupIndex)[i]].roleSum < leastPoint && group[(*groupIndex)[i]].amountOfMembers < leastStudent && group[(*groupIndex)[i]].roleSum < averageRoleAmount)
        {
            leastPoint = group[(*groupIndex)[i]].roleSum;
            result = (*groupIndex)[i];
        }
    }

    /*free(*groupIndex);*/
    /**groupIndex = NULL;*/
    if (result == -1)
    {
        for (i = 0; i < amountGroup; i++)
        {
            if (group[i].amountOfMembers < leastStudent)
            {
                leastStudent = group[i].amountOfMembers;
                result = i;
            }
        }
    }
                    //Hvilken elev, hvilken gruppe, array af elever, array af grupper
    insertStudentInGroup(studentId, result, student, group);
}

/* ---------------------- Checks if a student have more than one role, where the frequency is less than amount of groups, and the role is allready covered i the specified group ---------------------- */
int checkRareRole(structStudent student, structGroup group, int amountGroup, structAmountRole *roleSumAndIndex)
{
    int i;

    for (i = 0; i < BELBINS_ROLES; i++)
    {
        if (student.role[roleSumAndIndex[i].role] && roleSumAndIndex[i].sum <= amountGroup && group.roleCovered[roleSumAndIndex[i].role])
        {
            return 0;
        }
    }

    return 1;
}
