#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define MAX_LINE_LEN 4096
#define INITIAL_TABLE_SIZE 10000
#define GROW_FACTOR 2

//################################################################################################################################################

typedef struct data
{
  char name[200];
  char name2[200];
  char ID[200];
  char Letter[200];
  char oper[1000];
  char Go[200];
  char orf[200];
} go_slim_mapping;


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


//####################################################################################################################################################

int Go_terms(struct data *go_slim_mapping)
{
	FILE *file;

	char str[100];
	sprintf(str, "/home/chaimae/final_senior/analysis/Data/go_slim_mapping.csv");
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
                "%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^\n]\n",
                go_slim_mapping[records].name, 
                go_slim_mapping[records].name2, 
                go_slim_mapping[records].ID,
                go_slim_mapping[records].Letter,
                go_slim_mapping[records].oper,
                go_slim_mapping[records].Go,
                go_slim_mapping[records].orf); 

    
    if (read == 7) records++;

    if (read != 7 && !feof(file))
    {
    	printf("File format incorrect %d\n",records);
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

void genes_pairs(hash_table_t *table) {
    FILE *file;
    char line[400];
    char *gene, *value;

    char str[500];
    sprintf(str,"/home/chaimae/final_senior/analysis/Data/genes_pairs_test_%d.csv",FREQ);
    file = fopen(str,"r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }


    while (fgets(line, sizeof(line), file)) {
        // Tokenize the line
        gene = strtok(line, ",");
        value = strtok(NULL, "\n");

        // Remove quotes from the gene and correlation strings
        gene = strtok(gene, "\"");
        value = strtok(value, "\"");

        // Add the gene and correlation to the hash table
        if (add(table, gene, value) == 0) {
            printf("Error adding gene %s and value %s to hash table.\n", gene, value);
            return;
        }
    }

    fclose(file);
}


//###################################################################################################################################################################################

char** find_gene(char *term,int go_slim,struct data *go_slim_mapping)
{
    char** genes = malloc(5000 * sizeof(char*)); 
    int j = 0;

    for (int i = 0; i < go_slim; i++)
    {
        if (strcmp(term,go_slim_mapping[i].Go) == 0)
		{
			genes[j] = malloc((strlen(go_slim_mapping[i].name) + 1) * sizeof(char));
			strcpy(genes[j],go_slim_mapping[i].name);
			j++;
		}
    }
	for (int i = j; i < 5000; i++){
		genes[i] = NULL;
    }
    return genes;
}

int array_len(char** array)
{
    int i = 0;
    while(array[i] != NULL)
        i++;
    return i;
}

void free_genes(char** genes) {
    for (int i = 0; i < 5000; i++) {
        if (genes[i] != NULL) {
            free(genes[i]);
        }
    }
    free(genes);
}

int pairs_found(char *gene1,char *gene2,hash_table_t *table)
{
    char str[500];
	char str2[500];
    sprintf(str,"%s_%s",gene1,gene2);
	sprintf(str2,"%s_%s",gene2,gene1);
    const char* value1 = lookup(table,str);
	const char* value2 = lookup(table,str2);
	
    if (value1 == NULL && value2 == NULL ){
        return 0;
    }
    return 1;
}

int num_links(char** genes, hash_table_t *table)
{
    int i,j;
	int links;

    i = 0;
    j = 0;
    links = 0;
    while (i < array_len(genes))
    {
		j = i+1;
        while (j < array_len(genes))
        {
            links += pairs_found(genes[i],genes[j],table);
            j++;
        }
        i++; 
    }
	return (links);
} 

double* simulation(struct data *go_slim_mapping,char* term, int num_genes,hash_table_t *table)
{
    double *arr = malloc(3 * sizeof(double));
    int iter,rand_num,links;
    long double sum = 0, sum_squared = 0, stnd_div = 0;

    iter = 100;
    srand(time(NULL));
    links = 0;

    for (int i = 0; i < iter; i++)
    {
        
        // printf("%d",i);
        char** genes = malloc((num_genes + 1)* sizeof(char*)); 
        for (int j = 0; j < num_genes; j++)
        {
            rand_num = (rand() % 33675) ;
            // printf("%d\n",rand_num);
		    genes[j] = malloc((strlen(go_slim_mapping[rand_num].name) + 1) * sizeof(char));
			strcpy(genes[j],go_slim_mapping[rand_num].name);
        }
        
        genes[num_genes] = NULL;
        int links = num_links(genes,table);
        sum += links;
        sum_squared += links*links;
        for (int n = 0; n < num_genes ; n++) {
            if (genes[n] != NULL) {
                free(genes[n]);
            }
        }
        free(genes);
    }

    sum /= iter;
    sum_squared /= iter;
    stnd_div = sqrt(sum_squared - sum * sum );
    arr[0] = sum;
    arr[1] = sum_squared;
    arr[2] = stnd_div;

    return (arr);
}

void programe_run(struct data *go_slim_mapping,hash_table_t *table,int mapping,FILE *fd)
{
    FILE *fp;
    char buffer[100];

    fp = fopen("/home/chaimae/final_senior/analysis/Data/go.dat", "r");
    if (fp == NULL) {
        printf("Error opening file.\n");
        return ;
    }

    printf("Go_term,num_genes,exp_link,sim_link,stan_div\n");
    fprintf(fd,"Go_term,num_genes,exp_link,sim_link,stan_div\n");


    while (fgets(buffer, 100, fp) != NULL) {
		buffer[strcspn(buffer, "\n")] = '\0';
        char **genes = find_gene(buffer,mapping,go_slim_mapping);

		int links = num_links(genes,table);

        double *sim = simulation(go_slim_mapping,buffer,array_len(genes),table);
		printf("%s,%d,%d,%f,%f\n",buffer,array_len(genes),links,sim[0],sim[2]);

        fprintf(fd,"%s,%d,%d,%f,%f\n",buffer,array_len(genes),links,sim[0],sim[2]);

        free(sim);
		free_genes(genes);
    }

    fclose(fp);
}

int main ()
{
    FILE *fd;
    char str[500];
    sprintf(str,"/home/chaimae/final_senior/analysis/number_links_test_%d.csv",FREQ);
    fd = fopen(str,"w");
    if (fd == NULL) {
        printf("Error opening file2.\n");
        return 0 ;
    }
    // fprintf(fd,"Go_term\tnum_genes\tnum_link\tmean\tstan_div\n");


	hash_table_t *table = create_table(INITIAL_TABLE_SIZE);
	if (!table) {
        printf("Failed to create hashtable.\n");
        return 1;
    }

	struct data *go_slim_mapping = (struct data*) malloc(33700* sizeof(struct data));

	genes_pairs(table);

	int mapping = Go_terms(go_slim_mapping);
	programe_run(go_slim_mapping,table,mapping,fd);

	destroy_table(table);
	free(go_slim_mapping);
	fclose(fd);
}
