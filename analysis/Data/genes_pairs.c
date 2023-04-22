#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 4096
#define INITIAL_TABLE_SIZE 10000
#define GROW_FACTOR 2

//################################################################################################################################################

typedef struct data1   //unique list
{
  int chr_num;
  int loci;
  char gene[200];
} loci_gene;

//####################################################################################################################################################

typedef struct node {
    char *key;
    char *value;
    struct node *next;
} node_t;

typedef struct {
    node_t **table;
    size_t size;
    size_t count;
} hash_table_t;

unsigned int hash(const char *key) {
    unsigned int hash = 5381;
    int c;
    while ((c = *key++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

node_t *create_node(const char *key, const char *value) {
    node_t *node = malloc(sizeof(node_t));
    if (!node) {
        return NULL;
    }
    node->key = strdup(key);
    node->value = strdup(value);
    node->next = NULL;
    return node;
}

void destroy_node(node_t *node) {
    if (node) {
        free(node->key);
        free(node->value);
        free(node);
    }
}

hash_table_t *create_table(size_t size) {
    hash_table_t *table = malloc(sizeof(hash_table_t));
    if (!table) {
        return NULL;
    }
    table->table = calloc(size, sizeof(node_t *));
    if (!table->table) {
        free(table);
        return NULL;
    }
    table->size = size;
    table->count = 0;
    return table;
}

void destroy_table(hash_table_t *table) {
    if (!table) {
        return;
    }
    for (size_t i = 0; i < table->size; i++) {
        node_t *node = table->table[i];
        while (node) {
            node_t *next = node->next;
            destroy_node(node);
            node = next;
        }
    }
    free(table->table);
    free(table);
}

void resize_table(hash_table_t *table) {
    size_t old_size = table->size;
    size_t new_size = old_size * GROW_FACTOR;
    node_t **new_table = calloc(new_size, sizeof(node_t *));
    if (!new_table) {
        return;
    }
    for (size_t i = 0; i < old_size; i++) {
        node_t *node = table->table[i];
        while (node) {
            node_t *next = node->next;
            size_t index = hash(node->key) % new_size;
            node->next = new_table[index];
            new_table[index] = node;
            node = next;
        }
    }
    free(table->table);
    table->table = new_table;
    table->size = new_size;
}

int add(hash_table_t *table, const char *key, const char *value) {
    if (!table) {
        return 0;
    }
    if ((double)table->count / table->size > 0.75) {
        resize_table(table);
    }
    size_t index = hash(key) % table->size;
    node_t *node = table->table[index];
    while (node) {
        if (strcmp(node->key, key) == 0) {
            free(node->value);
            node->value = strdup(value);
            return 1;
        }
        node = node->next;
    }
    node = create_node(key, value);
    if (!node) {
        return 0;
    }
    node->next = table->table[index];
    table->table[index] = node;

    table->count++;

    return 1;
}

int delete(hash_table_t *table, const char *key){
    // compute the index in the hash table
    size_t index = hash(key) % table->size;

    // find the node with the given key in the linked list at the index
    struct node *prev = NULL;
    struct node *curr = table->table[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            // remove the node from the linked list
            if (prev == NULL) {
                table->table[index] = curr->next;
            } else {
                prev->next = curr->next;
            }

            // free the memory for the node's key, value, and itself
            free(curr->key);
            free(curr->value);
            free(curr);

            // decrement the count of nodes in the table
            table->count--;

            return 1;
        }

        prev = curr;
        curr = curr->next;
    }

    // the key was not found
    return 0;
}

const char *lookup(hash_table_t *table, const char *key){
    // compute the index in the hash table
    size_t index = hash(key) % table->size;

    // find the node with the given key in the linked list at the index
    struct node *curr = table->table[index];
    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            // the key was found, return the associated value
            return curr->value;
        }

        curr = curr->next;
    }

    // the key was not found
    return NULL;
}

//###################################################################################################################################################################################

// void correl(hash_table_t *table) {
//     FILE *file;
//     char line[400];
//     char *gene, *corr;

//     file = fopen("/home/chaimae/final_senior/analysis/new_gene_corr_list.txt", "r");
//     if (file == NULL) {
//         printf("Error opening file3.\n");
//         return;
//     }


//     while (fgets(line, sizeof(line), file)) {
//         // Tokenize the line
//         gene = strtok(line, ",");
//         corr = strtok(NULL, "\n");

//         // Remove quotes from the gene and correlation strings
//         gene = strtok(gene, "\"");
//         corr = strtok(corr, "\"");

//         // Add the gene and correlation to the hash table
//         if (add(table, gene, corr) == 0) {
//             printf("Error adding gene %s and correlation %s to hash table.\n", gene, corr);
//             return;
//         }
//     }

//     fclose(file);
// }

int loci_ge(struct data1 *loci_gene)
{
	FILE *file;

	char str[100];
	sprintf(str, "/home/chaimae/final_senior/loci_offset/loci_test2_500.txt");
	file = fopen(str, "r"); 
  
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
                "%d,%d,%s\n",
                &loci_gene[records].chr_num, 
                &loci_gene[records].loci, 
                loci_gene[records].gene); 
    
    if (read == 3) records++;

    if (read != 3 && !feof(file))
    {
    	printf("File format incorrect1.%d\n",records);
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

//#####################################################################################################################################################################################


char** find_gene(char *chr,char *pos,struct data1 *loci_gene,int len_loci_gene)
{
    int loci_pos = atoi(pos);
    int chr_num = atoi(chr);
    char** genes = malloc(100 * sizeof(char*)); // Allocate memory for 100 strings
    int j = 0;

    for (int i = 0; i < len_loci_gene; i++)
    {
        if (loci_gene[i].chr_num == chr_num && loci_gene[i].loci == loci_pos)
        {
            if (j < 100) {
                genes[j] = malloc((strlen(loci_gene[i].gene) + 1) * sizeof(char)); // Allocate memory for each string
                strcpy(genes[j], loci_gene[i].gene);
                j++;
            } else {
                break; // Stop copying genes when the limit is reached
            }
        }
    }

    for (int i = j; i < 100; i++){
        genes[i] = NULL; // Set remaining pointers to NULL
    }

    if (genes[0] == NULL)
    {
        genes[0] = NULL;
        return genes;
    }else
        return genes;
}

void free_genes(char** genes) {
    for (int i = 0; i < 100; i++) {
        if (genes[i] != NULL) {
            free(genes[i]);
        }
    }
    free(genes);
}


int array_len(char** array)
{
    int i = 0;
    while(array[i] != NULL)
        i++;
    return i;
}

// double calculate_corre(char *gene1,char *gene2,hash_table_t *table,FILE *fd)
// {
//     char str[500];
//     sprintf(str,"%s_%s",gene1,gene2);
//     const char* value = lookup(table,str);
//     if (value == NULL){
//         return 0;
//     }
//     fprintf(fd,"%s_%s,1\n",gene1,gene2);
//     return atof(value);
// }

double find_correl(char** loci_gene_1, char** loci_gene_2,hash_table_t *table,FILE *fd)
{
    int i,j;
    double sum;

    i = 0;
    j = 0;

    while (i < array_len(loci_gene_1))
    {
        while (j < array_len(loci_gene_2))
        {
            fprintf(fd,"%s_%s,1\n",loci_gene_1[i],loci_gene_2[j]);
            j++;
        }
        j = 0;
        i++; 
    }
}

void programe_run(struct data1 *loci_gene,hash_table_t *table,int len_loci_gene,FILE *fd)
{
    FILE *file;
    char line[400];
    char *chrm1,*pos1,*chrm2,*pos2,*freq,*num1,*num2;

    file = fopen("/home/chaimae/final_senior/analysis/Data/List.csv", "r");
    if (file == NULL) {
        printf("Error opening file2.\n");
        return;
    }
	while (fgets(line, sizeof(line), file)) {

	// Tokenize the line
		chrm1 = strtok(line, ",");
		pos1 = strtok(NULL, ",");
		chrm2 = strtok(NULL, ",");
		pos2 = strtok(NULL, ",");
		freq = strtok(NULL, ",");
		num1 = strtok(NULL, ",");
		num2 = strtok(NULL, "\n");


        if (atoi(freq) >= FREQ)
        {
            // printf("found\n");

            char** loci_gene_1 = find_gene(chrm1,pos1,loci_gene,len_loci_gene);
            char** loci_gene_2 = find_gene(chrm2,pos2,loci_gene,len_loci_gene);


            if (loci_gene_1[0] != NULL && loci_gene_2[0] != NULL)
            {
                double avg_correlation = find_correl(loci_gene_1,loci_gene_2,table,fd);
            }

            free_genes(loci_gene_1);
            free_genes(loci_gene_2);
        }
    }

	fclose(file);
}

int main()
{
    FILE *fd;
    char str[500];
    sprintf(str,"/home/chaimae/final_senior/analysis/genes_pairs_test2_%d.csv",FREQ);
    fd = fopen(str,"w");
    if (fd == NULL) {
        printf("Error opening file1111.\n");
        return 0 ;
    }

	struct data1 *loci_gene = (struct data1*) malloc(60000 * sizeof(struct data1));
	hash_table_t *table = create_table(INITIAL_TABLE_SIZE);
	if (!table) {
        printf("Failed to create hashtable.\n");
        return 1;
    }
	// correl(table);

	int len_loci_gene = loci_ge(loci_gene);
	programe_run(loci_gene,table,len_loci_gene,fd);

	destroy_table(table);
	free(loci_gene);
    fclose(fd);
}