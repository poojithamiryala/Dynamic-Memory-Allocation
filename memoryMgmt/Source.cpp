#include<stdio.h>
#include<stdlib.h>
#define PAGE_SIZE 1024

struct freeblock
{
	void* start;
	int size;//end-start
	struct freeblock* next;
};
/*
struct table
{
	struct block* head;
};
*/
struct freeblock* head;


void* navmalloc(int size)
{
	
	struct freeblock* temp = head;

	struct freeblock* prev = head;


	while ((temp->size)<(size + sizeof(int)))
	{
		prev = temp;
		temp = temp->next;
	}

	//inital node

	if (temp == prev)
	{
		if ((size + sizeof(int)) > ((temp->size)))
		{
			//no free space available

			return NULL;
		}

		else if ((size + sizeof(int)) == ((temp->size)))
		{

			temp = temp->next;

			head = temp;

			//allocate space

			//storing size in 1st 4 bytes
			
			*((int*)(temp->start)) = size;

			printf("Memory allocated Succesufully %p \n", (int*)temp->start + 1);
			
			return (int*)temp->start + 1;
			
			//if temp is null then get one free one page

		}

		else
		{
			//(temp->size)>(size+2)

			// move temp to length size + 2;
			
			*((int*)(temp->start)) = size;

			void* ptr = temp->start;

			for (int i = 0; i < size + sizeof(int); i++)
			{
				temp->start=((char*)temp->start)+1;
			}

			temp->size = temp->size - size - sizeof(int);

			ptr = (int*)ptr + 1;


			printf("Memory allocated Succesufully %p \n", ptr);


			return ptr;
		}
	}

	else  if (temp!=NULL)
	{
		 
		if ((size + sizeof(int)) == ((temp->size)))
		{

			prev->next = temp->next;

			//free-temp free block

			*((int*)(temp->start)) = size;


			printf("Memory allocated Succesufully %p \n", (int*)temp->start + 1);


			return (int*)temp->start + 1;
		}

		else if (((size + sizeof(int)) < ((temp->size))))
		{
			//return temp->start+2
				
			*((int*)(temp->start)) = size;

			void* ptr = temp;

			for (int i = 0; i < size + sizeof(int); i++)
			{
				temp->start = ((char*)temp->start) + 1;
			}

			temp->size = temp->size - size - sizeof(int);


			printf("Memory allocated Succesufully %p \n", (int*)ptr + 1);

			return (int*)ptr + 1;
		}

	}
	
	else
	{
			//get one free one page

	}
}
int ptrcmp(void* ptr1, void* ptr2)
{
	if (ptr1 < ptr2)
	{
		return 1;
	}
	else if (ptr1 == ptr2)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}
void navfree(void* ptr)
{
	void* ptr1 = ptr;

	ptr1=((int*)ptr1)-1;

	int space = *((int*)ptr1);
	
	struct freeblock* newBlock = (struct freeblock*)malloc(sizeof(struct freeblock*));

	newBlock->size = space+sizeof(int);

	newBlock->start = ptr1;

	newBlock->next = NULL;

	if (head == NULL)
	{
		//there is nothing to deallocate
	}
	else
	{
		struct freeblock* temp = head;

		struct freeblock* prev = head;

		//insering at first node
		if (temp == prev && ptrcmp(prev->start, newBlock->start) == -1)//> here represents -1
		{
			void* checkCond = (newBlock->start);
			checkCond = ((char*)checkCond) + space+sizeof(int);
			if (ptrcmp(checkCond, prev->start) == 0)
			{
				newBlock->size = newBlock->size + temp->size;
				newBlock->next = temp->next;
				//free temp
				head = newBlock;

				printf("Freed Successfully\n");
			}
			else
			{
				newBlock->next = temp;
				head = newBlock;
				printf("Freed Successfully\n");
			}
		}
		else
		{
			temp = prev->next;
			while (temp != NULL)
			{
				if (ptrcmp(temp->start, newBlock->start) == -1)
				{
					void* checkCond = (prev->start);
					checkCond = ((char*)checkCond) + prev->size;
					if (ptrcmp(checkCond, newBlock->start) == 0)
					{
						void* checkCond2 = (newBlock->start);
						checkCond = ((char*)checkCond) + space+sizeof(int);
						if (ptrcmp(checkCond, temp->start) == 0)
						{
							prev->size = temp->size + newBlock->size + prev->size;
							prev->next = temp->next;
							printf("Freed Successfully\n");
							break;
							//free temp and newBlock
						}
						else
						{
							prev->size = newBlock->size + prev->size;
							prev->next = temp;
							//free newblock
							printf("Freed Successfully\n");
							break;
						}
					}

					else
					{
						newBlock->next = temp;
						prev->next = newBlock;
						printf("Freed Successfully\n");
						break;
					}
				}
				prev = temp;
				temp = temp->next;
			}
		}
		if (temp == NULL)
		{
			if (ptrcmp(prev->start, newBlock->start) == 1)
			{
				void* checkCond = (prev->start);
				checkCond = ((char*)checkCond) + prev->size;
				if (ptrcmp(checkCond, newBlock->start) == 0)
				{
					prev->size = prev->size + newBlock->size;
					prev->next = NULL;
					printf("Freed Successfully\n");
					//free temp && newBlock
				}
				else
				{
					prev->next = newBlock;
					newBlock->next = NULL;
					printf("Freed Successfully\n");
				}
			}
		}
	}
}

void debug()
{
	printf("Available Free blocks\n");

	printf("Pointer Address  Space\n");

	struct freeblock* ptr = head;

	while (ptr!=NULL)
	{
		printf("%p \t %d \n", ptr->start, ptr->size);
		ptr = ptr->next;
	}
	printf("-----------------------------------------------------------------------------------\n");
}
void createFreeMemPage()
{

	void* memory = malloc(PAGE_SIZE);

	struct freeblock* ptr = (struct freeblock*)malloc(sizeof(struct freeblock*));

	ptr->next = NULL;

	ptr->size = PAGE_SIZE;

	ptr->start = memory;

	if (head == NULL)
		head = ptr;

	else
	{ 
		struct freeblock* temp = head;
		
		while (temp->next != NULL)
		{
			temp = temp->next;
		}

		temp->next = ptr;
	}

}

int main()
{
	
	createFreeMemPage();

	debug();

	int* ptr = (int*)navmalloc(sizeof(int));

	debug();

	int* ptr1 = (int*)navmalloc(sizeof(int));

	int* ptr2 = (int*)navmalloc(sizeof(int));

	navfree(ptr);

	debug();

	navfree(ptr1);

	debug();

	return 0;
}