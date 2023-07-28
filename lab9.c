#include <stdio.h>
#include <malloc.h>
#include <string.h>

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

// Function to initialize the HashType structure
struct HashType* create_hash_table(int size)
{
//    int prime_size = next_prime(size);
    struct HashType* hash_table = (struct HashType*)malloc(sizeof(struct HashType));
    hash_table->size = 15;
    hash_table->table = (struct Node**)calloc(15, sizeof(struct Node*));
    return hash_table;
}

int sizeOfNode(struct Node *pNode);
int hashFunction(struct HashType* mp, int key)
{
    const double A = 0.6180339887;
    double scaledKey = key * A;
    scaledKey -= (int)scaledKey;

    int bucketIndex = (int)(mp->size * scaledKey);

    return bucketIndex;
}
//int hashFunction(struct HashType* mp, int key)
//{
//    int bucketIndex;
//    int sum = 0, factor = 15;
//    for (int i = 0; i <  key; i++) {
//        sum = ((sum % mp->size)
//               + (((int)key) * factor) % mp->size)
//              % mp->size;
//
//        factor = ((factor % __INT16_MAX__)
//                  * (31 % __INT16_MAX__))
//                 % __INT16_MAX__;
//    }
//
//    bucketIndex = sum;
//    if (bucketIndex>14)bucketIndex=14;
//    return bucketIndex;
//}


// Compute the hash function
int hash(int x, int table_size)
{
    int hash_value = x % table_size;
    return hash_value;
}
// Function to insert a record into the hash table with linear probing to find the next available bucket


// Function to insert a record into the hash table with a maximum of 2 records per table (bucket)
void insert(struct HashType* mp, struct RecordType record)
{
    int bucketIndex = hashFunction(mp, record.id);

    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->record = record;
    new_node->next = NULL;

    // Bucket index is empty....no collision
    if (mp->table[bucketIndex] == NULL) {
        mp->table[bucketIndex] = new_node;
    }

        // Bucket index already has records, check if there are less than 2 records in the chain
    else {
        // Find the last node in the chain
        struct Node* current = mp->table[bucketIndex];
        int recordCount = 1; // We already have one record in the chain

        while (current->next != NULL) {
            current = current->next;
            recordCount++;
        }

        current->next = new_node;

    }
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

	for (i=0;i<15;++i) // FIXED BROKEN FUNCTION
	{
//        if (pHashArray!=NULL && pHashArray->table[i]!=NULL) {}
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
            free(temp); // Free each node in the linked list
        }
    }

    free(hash_table->table); // Free the array of linked lists (the hash table)
    free(hash_table); // Free the HashType structure itself
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