/*
 * C Program to Compare two Binary Files, Printing the First Byte 
 * Position where they Differ
 */
#include <stdio.h>
#include <string.h>

#define NUM_FRAME 4096
 
void compare_two_binary_files(FILE *,FILE *);
 
int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
 
    if (argc < 3)
    {
        printf("\nInsufficient Arguments: \n");
        printf("\nHelp:./executable <filename1> <filename2>\n");
        return 1;
    }
    else
    {
        fp1 = fopen(argv[1],  "r");
        if (fp1 == NULL)
        {
            printf("\nError in opening file %s", argv[1]);
            return 1;
        }
 
        fp2 = fopen(argv[2], "r");
 
        if (fp2 == NULL)
        {
            printf("\nError in opening file %s", argv[2]);
            return 1;
        }
 
        if ((fp1 != NULL) && (fp2 != NULL))
        {
            compare_two_binary_files(fp1, fp2);
        }
    }

    return 0; 
}
 
/*
 * compare two binary files character by character
 */
void compare_two_binary_files(FILE *fp1, FILE *fp2)
{
    int fpid1[NUM_FRAME];
    int fpid2[NUM_FRAME];

    int flag = 1;

    memset(fpid1, 0, sizeof(fpid1));
    memset(fpid2, 0, sizeof(fpid2));

    fread(fpid1, sizeof(int), NUM_FRAME, fp1);
    fread(fpid2, sizeof(int), NUM_FRAME, fp2);

    for (int i=0; i<NUM_FRAME; i++) {
        if (fpid1[i] != fpid2[i]) {
            flag = 0;
            printf("diff: %d, %d, %d \n", i, fpid1[i], fpid2[i]);
        }
    }
 
    if (flag == 0)
    {
        printf("Different");
    }
    else
    {
        printf("Equal");
    }
}