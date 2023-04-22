#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ROWS 6041
#define COLS 1501


typedef struct data1
{
  char gene[100];
} genes;

int genes_p(struct data1 *genes)
{
	FILE *file;
	file = fopen("genes.csv", "r"); 
  
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
                "%s\n",
                genes[records].gene); 
    
    if (read == 1) records++;

    if (read != 1 && !feof(file))
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

int correlation(float **data,struct data1 *genes)
{
    FILE *fp;
    fp = fopen("correlation_RNA.csv", "w");

    int i = 0;
    int j =0;
    int counter;
    float sum;
    float *mean_values = malloc(sizeof(float) * ROWS);
    float *std_dev_values = malloc(sizeof(float) * ROWS);
    float **data_std = (float **)malloc(ROWS * sizeof(float *));
    for (int i = 0; i < ROWS; i++) {
        data_std[i] = (float *)malloc(COLS * sizeof(float));
    }
    while(i < ROWS)
    {
        // Calculate the mean of the row
        sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += data[i][j];
        }
        float mean = sum / COLS;
        mean_values[i] = mean;

        // Subtract the mean from each value in the row
        for (int j = 0; j < COLS; j++) {
            data_std[i][j] = data[i][j] - mean;
        }

        // Calculate the standard deviation of the row
        sum = 0;
        for (int j = 0; j < COLS; j++) {
            sum += data_std[i][j] * data_std[i][j];
        }
        float std_dev = sqrt(sum / (COLS-1));
        std_dev_values[i] = std_dev;

        // Divide each value in the row by the standard deviation
        for (int j = 0; j < COLS; j++) {
            data_std[i][j] /= std_dev;
        }

        i++;
    }

    // Calculate the correlations between rows in the standardized data
    float *correlation_values = malloc(sizeof(float) * (ROWS*ROWS));
    int n = 0;
    i = 0;
    while(i < ROWS)
    {
        j = i+1;
        while(j < ROWS)
        {
            counter = 0;
            sum = 0;
            while(counter < COLS)
            {
                sum += data_std[i][counter] * data_std[j][counter];
                counter++;
            }
            correlation_values[n] = sum/(COLS-1);
            n++;
            j++;
        }
        i++;
    }

    int a,b,c;
    a = b = c = 0;
    while(a < ROWS)
    {
        b = a + 1;
        while(b < ROWS)
        {
            fprintf(fp,"%s_%s,%lf\n",genes[a].gene,genes[b].gene,correlation_values[c]);
            c++;
            b++;
        }
        a++;
    }

    free(correlation_values);
    free(mean_values);
    free(std_dev_values);
    for (int i = 0; i < ROWS; i++) {
        free(data_std[i]);
    }
    free(data_std);

    fclose(fp);

    return 0;
}


int read_allTPM(struct data1 *genes) {
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    size_t read;
    int row = 0, col = 0;

    float **data = (float **)malloc(ROWS * sizeof(float *));
    for (int i = 0; i < ROWS; i++) {
        data[i] = (float *)malloc(COLS * sizeof(float));
    }

    fp = fopen("normlized.csv", "r");
    if (fp == NULL)
        exit(EXIT_FAILURE);

    while ((read = getline(&line, &len, fp)) != -1) {
        char *token = strtok(line, ",");
        col = 0;
        while (token != NULL) {
            data[row][col] = atof(token); // convert token to int and store in array
            token = strtok(NULL, ",");
            col++;
        }
        row++;
    }
	correlation(data,genes);


    free(line);
    fclose(fp);

    for (int i = 0; i < ROWS; i++) {
        free(data[i]);
    }
    free(data);
    exit(EXIT_SUCCESS);
}

int main()
{
	struct data1 *genes =  malloc(6042 * sizeof(struct data1));
	genes_p(genes);
    read_allTPM(genes);
}
