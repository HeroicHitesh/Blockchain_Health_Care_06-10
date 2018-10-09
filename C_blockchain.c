#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/crypto.h>
struct block
{
	
	unsigned char prevHash[100];
	int blockData;
	struct block *link;
}*head;
void addBlock(int);
void verifyChain();
void alterNthBlock(int,int);
void hackChain();
int hashCompare(unsigned char*,unsigned char*);
void hashPrinter();
unsigned char* toString(struct block);
void printBlock();
void printAllBlocks();

//SHA256(src,size,dest)
//src and dest are of type unsigned char *
void addBlock(int data)
{
	if(head==NULL)
	{
		head=malloc(sizeof(struct block));
		SHA256("",sizeof(""),head->prevHash);
		head->blockData=data;
		return;
	}
	struct block *currentBlock=head;
	while(currentBlock->link)
		currentBlock=currentBlock->link;
	struct block *newBlock=malloc(sizeof(struct block));
	currentBlock->link=newBlock;
	newBlock->blockData=data;
	SHA256(toString(*currentBlock),sizeof(*currentBlock),newBlock->prevHash);
}
void verifyChain()
{
	if(head==NULL)
	{
		printf("Blockchain is empty.Try adding some blocks to the blockchain.");
		return;
	}
	struct block *curr=head->link,*prev=head;
	int count=1;
	while(prev)
	{
		printf("%d\t[%d]\t",count++,curr->blockData);
		hashPrinter(SHA256(toString(*prev),sizeof(*prev),NULL),100);
		printf(" - ");
		hashPrinter(curr->prevHash,100);
		if(hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash))
			printf("Verified!\n");
		else
			printf("Verification Failed!\n");
		prev=curr;
		curr=curr->link;
	}
}
void alterNthBlock(int n,int newData)
{
	struct block *curr=head;
	int count=1;
	if(curr==NULL)
	{
		printf("Nth block does not exist!\n");
		return;
	}
	while(count!=n)
	{
		if(curr->link==NULL && count!=n)
		{
			printf("Nth block does not exist!\n");
			return;
		}
		else if(count==n)
			break;
		curr=curr->link;
		count++;
	}
	printf("Before: ");
		printBlock(curr);
	curr->blockData=newData;
	printf("\nAfter: ");
		printBlock(curr);
	printf("\n");	
}
void hackChain()
{
	struct block*curr=head,*prev;
	if(curr==NULL)
	{
		printf("Blockchain is empty!");
		return;
	}
	while(1)
	{
		prev=curr;
		curr=curr->link;
		if(curr==NULL)
			return;
		if(!hashCompare(SHA256(toString(*prev),sizeof(*prev),NULL),curr->prevHash))
		{
			hashPrinter(SHA256(toString(*prev),sizeof(*prev),curr->prevHash),100);
			printf("\n");
		}
	}
}
unsigned char *toString(struct block b)
{
    unsigned char *str=malloc(sizeof(unsigned char)*sizeof(b));
    memcpy(str,&b,sizeof(b));
    return str;
}
void hashPrinter(unsigned char hash[],int length)
{
    for(int i=0;i<length;i++)
        printf("%x",hash[i]);
}
int hashCompare(unsigned char *str1,unsigned char *str2)
{
    for(int i=0;i<10;i++)
        if(str1[i]!=str2[i])
            return 0;
    return 1;
}
void printBlock(struct block *b)
{
    printf("%d\t",b);
    hashPrinter(b->prevHash,sizeof(b->prevHash));
    printf("\t[%d]\t",b->blockData);
    printf("%d\n",b->link);
}
void printAllBlocks()
{
        struct block *curr=head;
        int count=0;
        while(curr)
        {
            printBlock(curr);
            curr=curr->link;
        }    
}
void main()
{
    int a,b,c,d,e,n,r;
    printf("1)Add Block\n 2)Add n random blocks\n 3)alterNthBlock\n 4)printAllBlocks\n 5)verifyChain\n 6)hackChain\n");
    while(1)
    {
        printf("Choice:");
        scanf("%d",&c);
        switch(c)
        {
            case 1 :printf("Enter data:\n");
                    scanf("%d",&n);
                    addBlock(n);
                    break;
            case 2 :printf("How many blocks to enter?");
                    scanf("%d",&e);
                    for(int i=0;i<e;i++)
                    {
                        d=rand()%(e*10);
                        printf("Entering: %d\n",d);
                        addBlock(d);
                    }
                    break;
            case 3 :printf("Which block to alter?:\n");
                    scanf("%d",&a);
                    printf("Enter value:");
                    scanf("%d",&b);
                    alterNthBlock(a,b);
                    break;
            case 4 :printAllBlocks();
                    break;
            case 5 :verifyChain();
                    break;
            case 6 :hackChain();
                    break;
            default:printf("Wrong Choice!\n");
                    break;
        }
    }
}
//gcc -o C_blockchain C_blockchain.c -lcrypto
//./C_blockchain.c