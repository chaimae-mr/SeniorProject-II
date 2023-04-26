#include <stdio.h>
#include <stdlib.h>

typedef struct data
{
  int index;
  char gene1[50];
  char gene2[50];
  char gene3[50];
  int value1;
  int value2;
  int value3;
} probes;


typedef struct data2
{
	int index1;
	int index2;
	double correl;
}corre;

int prob_list(struct data prob[10000])
{
	FILE *file;

	file = fopen("/home/chaimae/seniorProject/exp data/probes.txt", "r"); 
  
	if (file == NULL)
	{
		
		printf("Error opening file.\n");
    	return 1;
	}

	int read = 0;
	int records = 0;

	do 
	{
	read = fscanf(file,
                "%d,%49[^,],%49[^,],%49[^,],%d,%d,%d\n",
                &prob[records].index, 
				prob[records].gene1,
				prob[records].gene2,
				prob[records].gene3,
                &prob[records].value1,
				&prob[records].value2,
                &prob[records].value3); 
    
    if (read == 7) records++;

    if (read != 7 && !feof(file))
    {
    	printf("File format incorrect.\n");
    	return 1;
    }
    
    if (ferror(file))
    {
    	printf("Error reading file.\n");
    	return 1;
    }

	} while (!feof(file));

	fclose(file);

	return records;
}

int correlation(struct data2 *corre)
{
	FILE *file;

	file = fopen("/home/chaimae/seniorProject/exp data/corr.txt", "r"); 
  
	if (file == NULL)
	{
		
		printf("Error opening file.\n");
    	return 1;
	}

	int read = 0;
	int records = 0;

	do 
	{
	read = fscanf(file,
                "%d\t%d\t%lf\n",
                &corre[records].index1, 
                &corre[records].index2,
				&corre[records].correl); 
    
    if (read == 3) records++;

    if (read != 3 && !feof(file))
    {
    	printf("File format incorrect.\n");
    	return 1;
    }
    
    if (ferror(file))
    {
    	printf("Error reading file.\n");
    	return 1;
    }

	} while (!feof(file));

	fclose(file);

	return records;
}

int main(void)
{
	FILE *fp;

	fp = fopen("/home/chaimae/seniorProject/exp data/new_gene_corr_list2.txt", "w");
	if (fp == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

	struct data2 *corre = malloc(50000000 * sizeof(struct data2));
	struct data prob[10000];
	correlation(corre);
	prob_list(prob);

	int i, j, counter;

	i = 0;
	counter = 0;
	while (i < 9335)
	{
		j = i + 1;
		while (j < 9335)
		{
			fprintf(fp,"%s_%s,%lf\n",prob[i].gene2,prob[j].gene2,corre[counter].correl);
			counter++;
			j++;
		}
		i++;
	}
	free(corre);
	fclose(fp);
}
