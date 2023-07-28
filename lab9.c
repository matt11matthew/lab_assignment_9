#include <stdio.h>
#include <malloc.h>

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType structure using a hash table with chaining (linked lists)
struct HashType
{
    int size;         // Size of the hash table
    struct Node** table; // Pointer to the array of linked lists (the hash table itself)
};

int next_prime(int num) {
    int prime_candidate = num;
    int is_prime = 0;

    while (!is_prime) {
        prime_candidate++;
        is_prime = 1;

        for (int i = 2; i * i <= prime_candidate; i++) {
            if (prime_candidate % i == 0) {
                is_prime = 0;
                break;
            }
        }
    }

    return prime_candidate;
}
// Function to initialize the HashType structure
struct HashType* create_hash_table(int size)
{
    int prime_size = next_prime(size);
    struct HashType* hash_table = (struct HashType*)malloc(sizeof(struct HashType));
    hash_table->size = prime_size;
    hash_table->table = (struct Node**)calloc(prime_size, sizeof(struct Node*));
    return hash_table;
}

// Compute the hash function
int hash1(int x, int table_size)
{
    int hash_value = abs(x) % table_size;
    return hash_value;
}

// Function to find the greatest common divisor (GCD) using Euclid's algorithm
int gcd(int a, int b)
{
    if (b == 0)
        return a;
    return gcd(b, a % b);
}

// Second hash function for quadratic probing
int hash2(int x, int table_size)
{
    // Choose a fixed prime number smaller than the table size
    const int fixed_prime = 31;

    // Find a step size that is a relative prime to the table size using Euclid's algorithm
    int step = fixed_prime % table_size;
    while (gcd(step, table_size) != 1)
    {
        step++;
    }

    return step;
}

void insert(struct HashType* hash_table, struct RecordType record)
{
    int index = hash1(record.id, hash_table->size);
    int step = hash2(record.id, hash_table->size);

    // Find an empty slot using quadratic probing
    int count = 0;
    while (hash_table->table[index] != NULL)
    {
        // If the current slot has a record with the same key, update its values and return
        if (hash_table->table[index]->record.id == record.id)
        {
            hash_table->table[index]->record = record;
            return;
        }

        // Quadratic probing: increment the index in a quadratic manner
        index = (index + count * step) % hash_table->size;

        // Increment the count to move to the next quadratic offset
        count++;
    }

    // If the slot is empty, create a new node and insert the record
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->record = record;
    new_node->next = NULL;
    hash_table->table[index] = new_node;
}

int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
        struct Node* current = pHashArray->table[i];
        if (current != NULL)
        {
            printf("Index %d -> ", i);
            while (current != NULL)
            {
                printf("%d %c %d -> ", current->record.id, current->record.name, current->record.order);
                current = current->next;
            }
            printf("NULL\n");
        }
	}
    printf("\n");
}

void destroy_hash_table(struct HashType* hash_table)
{
    if (hash_table == NULL)
        return;

    for (int i = 0; i < hash_table->size; i++)
    {
        struct Node* current = hash_table->table[i];
        while (current != NULL)
        {
            struct Node* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(hash_table->table);
    free(hash_table);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

//	recordSz = parseData("input.txt", &pRecords);
	recordSz = parseData("C:\\Users\\matt1\\CLionProjects\\lab_assignment_9\\input.txt", &pRecords);
//	recordSz = parseData("C:\\Users\\Matthew Eisenberg\\Documents\\DREnhancedMod\\Projects\\DRMod\\lab_assignment_9\\input.txt", &pRecords);
//	recordSz = parseData("C:\\Users\\Matthew Eisenberg\\Documents\\DREnhancedMod\\Projects\\DRMod\\lab_assignment_9\\input.txt", &pRecords);
	printRecords(pRecords, recordSz);

    struct HashType* hash_table = create_hash_table(recordSz);

    // Insert the records into the hash table
    for (int i = 0; i < recordSz; i++)
    {
        insert(hash_table, pRecords[i]);
    }

    // Display the records stored in the hash table
    displayRecordsInHash(hash_table, recordSz);

    // Clean up and free memory
    destroy_hash_table(hash_table);
    free(pRecords);
	// Your hash implementation
}