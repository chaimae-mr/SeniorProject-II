#include <stdio.h>
#include <stdlib.h>

typedef struct data1
{
  // members for the chr_gene's chr_num, gene, min and max
  int chr_num;
  char gene[50];
  int min;
  int max;
} chr_genes;


typedef struct data2
{
  int chr_num2;
  int loci_pos;
} unique_loci;

int chr(struct data1 chr_genes[8000])
{
  // file pointer variable for accessing the file
	FILE *file;

	file = fopen("/home/chaimae/final_senior/loci_offset/exp data/chrm-gene.txt", "r"); 
  
	if (file == NULL)
	{
		printf("Error opening file1.\n");
    	return 1;
	}

	int read = 0;
	int records = 0;

	do 
	{
	read = fscanf(file,
                "%d,%49[^,],%d,%d\n",
                &chr_genes[records].chr_num, 
                chr_genes[records].gene, 
                &chr_genes[records].min, 
                &chr_genes[records].max); 
    
    if (read == 4) records++;

    if (read != 4 && !feof(file))
    {
    	printf("File format incorrect %d.\n",records);
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

int loci(struct data2 unique_loci[4100])
{
	FILE *file;

	file = fopen("/home/chaimae/final_senior/loci_offset/exp data/loci_uniq.txt", "r"); 

	if (file == NULL)
	{
		printf("Error opening file2.\n");
		return 1;
	}

	// unique_loci unique_loci[4100];
	
	int read = 0;

	int records = 0;

	do 
	{
		read = fscanf(file,
					"%d,%d\n",
					&unique_loci[records].chr_num2, 
					&unique_loci[records].loci_pos); 
		
		if (read == 2) records++;

		if (read != 2 && !feof(file))
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

int* flip(int min, int max)
{
    int* arr = malloc(2 * sizeof(int));  // allocate memory for the array
    if (min > max)
    {
        arr[0] = max;
        arr[1] = min;
    }
    else
    {
        arr[0] = min;
        arr[1] = max;
    }
    return arr;
}


int find_gene(int chrom,int pos, int offset,FILE *fp)
{
	struct data1 chr_gene[8000];
	int len ,i;
	len = chr(chr_gene);
	i = 0;

	while(i < len)
	{
		int *arr = flip(chr_gene[i].min,chr_gene[i].max);
		// if (chr_gene[i].chr_num == chrom && (pos >= (chr_gene[i].min - offset) && pos <= (chr_gene[i].max + offset) ))
		if (chr_gene[i].chr_num == chrom && (pos >= (arr[0] - offset) && pos <= (arr[1] + offset) ))
			fprintf(fp,"%d,%d,%s\n",chr_gene[i].chr_num, pos, chr_gene[i].gene);
		i++;
		free(arr);
	}
	return 0;
}

int main(void)
{
	FILE *fp;

	char str[1000];
	sprintf(str, "/home/chaimae/final_senior/loci_offset/loci_%d.txt",OFFSET);

	fp = fopen(str, "w");

	if (fp == NULL) {
        printf("Error opening file3!\n");
        return 1;
    }

	struct data2 unique_loci[4100];

	int len, i;
	len = loci(unique_loci);
	i = 0;

	while (i < len)
	{
		find_gene(unique_loci[i].chr_num2,unique_loci[i].loci_pos,OFFSET,fp);
		i++;
	}
	fclose(fp);

    return 0;
}