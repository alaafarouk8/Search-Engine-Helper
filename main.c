#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mpi.h"

int isSubstring(char s1[],char s2[] )
{
    int M = strlen(s1);
    int N = strlen(s2);

    /* A loop to slide pat[] one by one */
    int i ;
    for (i = 0; i <= N - M; i++)
    {
        int j;
        /* For current index i, check for pattern match */
        for (j = 0; j < M; j++)
            if (s2[i + j] != s1[j])
                break;

        if (j == M)
            return i;
    }

    return -1;
}
void clear_file(const char *filename)
{
    FILE *output = fopen(filename, "w");
    fclose(output);
}
int main(int argc, char * argv[])
{
    double starttime ;
    int my_rank;		/* rank of process	*/
    int p ;			    /* number of process	*/
    int source = 0;		/* rank of sender	*/
    int dest = 0 ;		/* rank of receiver	*/
    int tag = 0;		/* tag for messages	*/
    char message[100];	/* storage for message	*/
    MPI_Status status;	/* return status for 	*/
    int count = 0 ;
    int irecv[100] ;
    int isend[100] ;
    int start = 1 ;
    int end = 50;
    /* Start up MPI */
    MPI_Init( &argc, &argv );

    /* Find out process rank */
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    /* Find out number of process */
    MPI_Comm_size(MPI_COMM_WORLD, &p);
    int i, j ;

    char query[100] ;
    int sum = 0 ;
    int average=50/p;
    int remainder=50%p ;
    int tempp ;
    int input ;
    FILE * fp;
    fp = fopen ("output.txt","w");
    if(my_rank== 0)
    {
        starttime = MPI_Wtime();
        sum = 0 ;
        printf("Enter Your Query: ");
        scanf("%[^\n]%*c",query);
        tempp=0;
        for(i=0; i<p; i++)
        {
            {
                if (remainder<=0)
                {

                    input = average ;
                    printf("%d\n", input);
                }
                else if (remainder>0)
                {
                    input = average+1 ;
                    printf("%d\n", input);

                }
                isend[i]=tempp;
                tempp+=input;
                irecv[i]=tempp;
            }
        }

    }

     MPI_Bcast(&sum,1, MPI_CHAR, 0, MPI_COMM_WORLD);
//   sum += my_rank ;
     MPI_Bcast(&query,strlen(query), MPI_CHAR, 0, MPI_COMM_WORLD);
     MPI_Bcast(&count,1, MPI_CHAR, 0, MPI_COMM_WORLD);
     MPI_Scatter(&isend, 1, MPI_INT, &irecv, 1, MPI_INT, 0,MPI_COMM_WORLD);
    count = 0 ;
    const char s[2] = " ";
    int tokCounter = 0, wordsCounter = 0;
    int Result ;
    FILE *filePointer;
    char intt [10];
    char filename[100] = "Aristo-Mini-Corpus P-";
    char temp[100];
    strcpy(temp,filename);
    for (i = start ; i <= end ; i++ )
    {
        sprintf(intt,"%d",i);
        strcat(temp,intt);
        char FileName[100] = "Aristo-Mini-Corpus\\";
        strcat(FileName,temp);
        strcat(FileName,".txt");
        filePointer = fopen(FileName, "r");
        memset(temp,' ',100);
        strcpy(temp,filename);
        char dataToBeRead[1000];
        if(filePointer == NULL)
        {
            perror("Unable to open file!");
            return (-1)  ;
        }
        else
        {
            count=0;
            while (fgets(dataToBeRead, sizeof(dataToBeRead), filePointer) != NULL)
            {
                char tempquery[100];
                strcpy(tempquery, query);
                char * p = strtok(tempquery, s);
                tokCounter =0;
                wordsCounter =0;
                while( p != NULL)
                {
                    tokCounter++;
                    if (isSubstring(p, dataToBeRead) != -1)
                    {
                        wordsCounter++;
                    }
                    p = strtok(NULL, s);
                }
                if (wordsCounter == tokCounter)
                {
                    count++;
                    sum++ ;
                    fprintf (fp,"%s\n",dataToBeRead);
                }
                free(p);


            }

        }
        fclose(filePointer) ;
    }
    MPI_Reduce(&sum, &Result, 1, MPI_INT, MPI_SUM, 0,MPI_COMM_WORLD);
    if (my_rank==0)
    {
        printf("Search Results Found: %d\n",Result);
        double finish = MPI_Wtime();
        double exec = finish - starttime ;
        printf("Execution time of master = %f\n",exec);
    }
    fprintf (fp,"Search Results Found: %d\n",sum);
    fclose (fp);
    MPI_Finalize();

    return 0;

}
