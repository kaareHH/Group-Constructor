#include "allIncludes.h"

/* ---------------------- Opens file in readmode and returns students ---------------------- */
structStudent *loadStructStudent(int *numberOfStudentsOutput)
{
    int numberOfStudents = 0,
        choose = -1;
    //Et struct af studenter
    structStudent *result;
    //Pointer til hvor filen er
    FILE *fileHandle;
    //Er hvor filerne bliver lagt ud.
    structfile *fileArray;
    //Bliver init til funk kald. 
    fileArray = userSelectFile(&choose);
    //Vi åbner den givne fil i read mode
    fileHandle = fopen(fileArray[choose].name, "r");
    //Vi reallokkere vores resultat-struct til at have mere plads
    result = (structStudent *)malloc(sizeof(structStudent));
    //Vi kører indtil slut på filen
    while(!feof(fileHandle))
    {
        //Reallokere plads til studenter. En form for failsafe funktion
        result = expandStructStudent(result, numberOfStudents + 1);
        //resultat til den elev vi nu er kommet til, indlæses i result struct.
        result[numberOfStudents++] = loadStudent(fileHandle);
    }
    //Lukker fil og frier fileArray structen.
    fclose(fileHandle);
    free (fileArray);
    //Antallet af studenter bliver sat til den int vi har sendt med til at starte på.
    *numberOfStudentsOutput = numberOfStudents;

    return result;
}
/* ---------------------- prompt the user for an class, and returns the input file according to input ---------------------- */
structfile *userSelectFile(int *choose)
{
    int count = 0, strLn, condition = 0;
    //Pointer til det givne directory
    DIR *d;
    struct dirent *dir;
    //FIlearray bliver malloc allokeret så vi ved hvor mange filer der er
    structfile *fileArray;

    fileArray = (structfile *)malloc(sizeof(structfile));
    printf("Your input options are:\n");

    //Lokale sti? 
    d = opendir(".");
    if (d)
    {   //så længe den læser et directory hvor der er noget i, er det sand
        while ((dir = readdir(d)) != NULL)
        {   //navnet på d_name er navnet på den fil den pejer på.
            strLn = strlen(dir->d_name);                         
            //Hvis der ligger en masse andre filer, bliver de sorteret fra. Vi trækker 1 fra, da et array starter fra nul, og strlen er selve længden.                  Vi kontroller om denne fil eksistere, hvis den gør, vil vi ikke have den med.
            if (dir->d_name[strLn - 1] == 't' && dir->d_name[strLn - 2] == 'x' && dir->d_name[strLn - 3] == 't' && dir->d_name[strLn - 4] == '.' && strcmp(dir->d_name, "result_group.txt") != 0)
            {
                //Vi reallokere nu plads
                fileArray = (structfile *)realloc(fileArray, (sizeof(structfile) * (count + 1)));
                                        //Man kan ikke bare få den til at søge efter alting, så derfor er det en apostrof.
                sscanf(dir->d_name, " %[^'] ", fileArray[count++].name);
                
                printf("[%d] %s %s\n", count, count < 10 ? " |" : "|", fileArray[count - 1].name);
            }
        }
        //VI luker dir
        closedir(d);
        //Vi indsætter en newline
        putchar('\n');
    }

    //Indsæt et nummer fra 1 til x. osv
    printf("Insert a number from 1 to %d, according to the class you wish to use\n: ", count);
    do
    {
        if (condition)
        {
            printf("Invalid number, choose one between 1 and %d\n :", count);
        }
        scanf(" %d", choose);
        (*choose)--;
        //Sætter condition til at være false ved et rigtigt tal, eller true ved et forkert tal
        condition = *choose < 0 || *choose >= count;
    } while (condition);

    return fileArray;
}

/* ---------------------- reallocating memory for students, if unsucessful exits program  ---------------------- */
structStudent *expandStructStudent(structStudent *inputPointer, int size)
{
    structStudent *tmp;
    int byteSize = size * sizeof(structStudent);

    tmp = realloc(inputPointer, byteSize);
    if (tmp != NULL)
        return tmp;
    else
    {
        printf("Program failure, try again\n");
        exit(1);
    }
}

/* ---------------------- Scanning input file, for every line. returns the information of one student at a time ---------------------- */
structStudent loadStudent(FILE *fileHandle)
{
    //Indlæser bare fra dokumentet
    char tempString[CHAR_LENGTH];
    structStudent tempStruct;
                            //??
    fscanf(fileHandle, " %[^,],"
                       " %[0-9 ]"
                       " %*[;] ",
                       tempStruct.name, tempString);
    initializeRoleStudent(&tempStruct, tempString);

    return tempStruct;
}

/* ---------------------- intialize tempStruct roles accoring to file input ---------------------- */
void initializeRoleStudent(structStudent *tempStruct, char *tempString)
{
    int i,
        strLen = strlen(tempString);

    //Sætter alle roller til nul. 
    for (i = 0; i < BELBINS_ROLES; i++)
    {
        (*tempStruct).role[i] = FALSE;
    }

    //Hvis den finder en rolle (altså tal) så indsættes tallet til den givne rolle. 
    for (i = 0; i < strLen; i++)
    {
        if (isdigit(tempString[i]))
        {
            (*tempStruct).role[tempString[i] - ASCII_DIGIT_ONE] = TRUE;
        }
    }
}
