// Lucas Battelle
// Asks user for URL and anayzes HTML source code assosiated with it
// Commands include:
// c = count of all src attributes
// t = prints all tags that have a src attribute in order
// u = prints all src urls in order
// f = prints in order of first found the tags that have src attributes and their frequency
// q = quit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define debugMode 1
#define SRC_TAG " src=\""

// Each SrcTag represents the data from one src tag
// It stores the src text inside the quotes
// It also stores the names of the tag which has the src attribute
struct SrcTag {
    char* name;
    char* src;

	// Stores the pointer to another SrcTag (linked-list)
    struct SrcTag* next;
};


// Creates a SrcTag struct from a pointer to the start of quote
struct SrcTag * createTag(char* quoteStart);

// Frees memory
void freeList(struct SrcTag* head);

int main(void){
    printf("Test1");
    
    // Asks user for URLName
    char URLName[200];
    printf("Test2");
    
    printf("URL:\n");
    fgets(URLName, 199, stdin);
    printf("Test-3");
    
    // gets rid of \n in URLName
    URLName[strlen(URLName)-1] = '\0';
   
    // Creates command used to access website source code text with url
    char command[208];
    char commandText[] = "curl -s ";
    strcat(command, commandText);
    strcat(command, URLName);
	
    // Creates FILE pointer for website and opens it
    FILE *fp;
    fp = popen(command, "r");
    if(!fp){
    	printf("Unable to connect to website");
    	return 0;
    }
    
    // Pointers used to navigate linked-list structure
    struct SrcTag* head;
    struct SrcTag* currentSrc;
    int numTags = 0;
    
    // Buffer for input from website and char pointer for finding src tags
    
    char buffer[BUFSIZ];
    
    // Holds whole page
    char *page;
   
    while(!feof(fp)){
    	
    	fgets(buffer, BUFSIZ, fp);
    	
    	char *temp = malloc(sizeof(char) * (strlen(page) + strlen(buffer)+1));
    	
    	
    	strcat(temp, page);
    	
    	strcat(temp,  buffer);
    	
    	free(page);
    	
    	page = temp;
    	
    }
    
    printf("%s", page);
    pclose(fp);
    
    char *searchResult;
    
    do {
        
        
        // Will be NULL if no srcTag is found
    	char * searchResult = strstr(searchResult, SRC_TAG);
        
        if(searchResult != NULL){
        	
        	if(debugMode){
        		printf("Src found\n");
        	}
        	
        	// Creates new SrcTag structure
        	struct SrcTag *tempSrc = createTag(searchResult);
        	numTags++;
        	
        	// Sets first SrcTag
        	if(head == NULL){
            	head = tempSrc;
            	currentSrc = tempSrc;
            	
        	} else {
        	// Adds new SrcTag to end of linked list
                currentSrc->next = tempSrc;
                currentSrc = tempSrc;
        	}
        	
        } 
            
    }  while(searchResult);
    pclose(fp);
    printf("\n");
    
    // Gets user input and performs commands
    char input[100];
    while(input[0] != 'q'){
    	printf("Ready\n"); 
    	fgets(input, 99, stdin);
    
    	if(input[0] == 'c'){
    	
    		//prints the count
    		printf("%d\n", numTags);
    	} else if(input[0] == 't'){
    	
    		// Prints all tagName's
    		struct SrcTag * temp = head;
    		while(temp){
    			printf("%s\n", temp->name);
    			temp = temp->next;
    		}
    	}
    	
    }
    
    
    freeList(head);
    if(debugMode){
    	printf("--------Mem Freed--------\n");
    }
    return EXIT_SUCCESS;
}



// Large function which creates a SrcTag struct from a pointer to 
struct SrcTag * createTag(char* quoteStart){
	
	// Find the start of tag/it's name
	char *tagNameStart = quoteStart;
	while( *(tagNameStart - 1) != '<'){
		tagNameStart--;
	}
	
	// Finds the end of tagName
	char *tagNameEnd = tagNameStart;
	while(*tagNameEnd != ' '){
		tagNameEnd++;
	}
	
	// Finds the start of the quote
	while(*(quoteStart) != '\"'){
		quoteStart++;
	}
	quoteStart++;
	
	
	// Finds the end of quote
	char *quoteEnd = quoteStart+1;
	while(*quoteEnd != '\"'){
		quoteEnd++;
	}
	
	// Copies tage name into new memory and adds end character
	int nameLen = tagNameEnd-tagNameStart;
	char *tagName = malloc(sizeof(char) * (nameLen + 1));
	
	// Copies the srcName into the src
	for(int i = 0; i < nameLen; i++){
		tagName[i] =  *(tagNameStart + i);
	}
	tagName[nameLen] = '\0';
	
	// Creates place to store quote
	int quoteLen = quoteEnd - quoteStart;
	char *quote = malloc(sizeof(char) * (quoteLen + 1));
	
	// Copies the information into the stored space
	for(int i = 0; i < quoteLen; i++){
		quote[i] = *(quoteStart + i);
	}
	quote[quoteLen] = '\0';
	
	if(debugMode){
		printf("tagName: %s - quote: %s\n", tagName, quote);
	}
	
	// Creates space for a SrcTag struct and saves data in it
	struct SrcTag * temp = malloc(sizeof(struct SrcTag));
	temp->name = tagName;
	temp->src = quote;
	temp->next = NULL;
	
	return temp;
}


// Frees all data previously malloc'd
void freeList(struct SrcTag* head){
	struct SrcTag* temp;
	
	while(head != NULL){
		temp = head;
		head = head->next;
		
		// Frees all the memory
		free(temp->name);
		free(temp->src);
		free(temp);
	}
}
