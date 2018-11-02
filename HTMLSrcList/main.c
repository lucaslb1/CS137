// Lucas Battelle
// Asks user for URL and anayzes HTML source code assosiated with it
// Commands include:
// c = count of all src attributes
// t = prints all tags that have a src attribute in order
// u = prints all src urls in order
// f = prints in order of first found the tags that have src attributes and their frequency
// q = quit

// TestURL = http://wfs.sbcc.edu/staff/nfguebels/web/cs137/html_src_list/simple.html


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> //To check whitespace


// I decided to use this type of structure because I wanted to have some fun
// with the assignment and thought it would be nice if the program
// only had to look through the source code once.

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

// Prints the tag names in order
void printTagNames(struct SrcTag *start);

// Prints the tag src's in order
void printTagSrcs(struct SrcTag *start);

// Prints one of each tag in order and their frequency
void printFrequency(struct SrcTag *start);

int main(void){
	setvbuf(stdout, NULL, _IONBF, 0);

    // Asks user for URLName
	printf("URL:\n");
    char URLName[200];
    fgets(URLName, 199, stdin);

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
        return -1;
    }

    // Buffer and page; pages holds all data on page up to 25600 bytes
    char buffer[BUFSIZ];
    char pageSrc[256000];

    // Concatenate each line onto the end of page
    while(!feof(fp)){
        fgets(buffer, BUFSIZ, fp);
        strcat(pageSrc,  buffer);
    }
    pclose(fp);


    // Pointers used to navigate linked-list structure
    struct SrcTag* head;
    struct SrcTag* newestSrcAdded;
    int tagCount = 0;

    // Start the pointer at the beginning of the page
    char *currentTagPtr = pageSrc;
    while(1) {


        // Will be NULL if no srcTag is found and will break
        currentTagPtr = strstr(currentTagPtr, "src=\"");
        if(!currentTagPtr){
        	break;
        }

        // If src=" is found and the character before it is whitespace
        if(currentTagPtr != NULL && isspace(*(currentTagPtr-1))){

                // Creates new SrcTag structure
                struct SrcTag *tempSrc = createTag(currentTagPtr);

                // Tracks number of tags found and made
                tagCount++;

                if(head == NULL){
                	// Sets first SrcTag if no head yet
                	head = tempSrc;
                	newestSrcAdded = tempSrc;

                } else {
                	// Adds new SrcTag to end of linked list
                	newestSrcAdded->next = tempSrc;
                	newestSrcAdded = tempSrc;
                }

        }

        // Increments the pointer so that srcsrc() doesn't find the same src=" and infinitely loop
        currentTagPtr++;
    }


    // Gets user input and performs commands
    char input[100];
    while(input[0] != 'q'){

    	// Gets input from user
        printf("Ready\n");
        fgets(input, 100, stdin);

        // All commands
        if(input[0] == 'c'){
            printf("%d\n", tagCount);
        } else if(input[0] == 't'){
        	printTagNames(head);
        } else if(input[0] == 'u'){
        	printTagSrcs(head);
        } else if(input[0] == 'f'){
        	printFrequency(head);
        }

    }


    // Clearing Memory and exiting program
    freeList(head);
    printf("Complete\n");
    return 0;
}



// Prints one of each tag and their frequencies
void printFrequency(struct SrcTag *start){

	// array used to track which names have been counted
	// Needs to bee calloc'd because if just normal array using f multiple times would
	// return different outputs because array would already contain the tags when created
	char *allTags = calloc(200, '\0');

	// Used for navigation
    struct SrcTag * temp = start;
    char *currentNamePtr;
    while(temp){

          	currentNamePtr = temp->name;

          	// if that src name hasn't been checked yet
          	if(!strstr(allTags, currentNamePtr)){
          		printf("%s ",currentNamePtr);

          		// Count all src names that are the same as currentNamePtr
          		int count = 1;
          		struct SrcTag * secondStart = temp->next;
          		while(secondStart){

          			// Counts names that are equal to the current one
          			if(strcmp(secondStart->name, currentNamePtr) == 0){
          				count++;
          			}
          			secondStart = secondStart->next;
          		}
          		printf("%d\n", count);

          		// Put src name into list of already counted ones
          		strcat(allTags, currentNamePtr);
          	}

            temp = temp->next;
    }
    free(allTags);
}


// Prints all tagName's
void printTagNames(struct SrcTag *start){

	// continues until next is temp is NULL
    struct SrcTag * temp = start;
    while(temp){
            printf("%s\n", temp->name);
            temp = temp->next;
    }

}

// Prints all src's
void printTagSrcs(struct SrcTag *start){

	// continues until next is temp is NULL
    struct SrcTag * temp = start;
    while(temp){
            printf("%s\n", temp->src);
            temp = temp->next;
    }

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
        while(!isspace(*tagNameEnd)){
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

        // Copies tag name into new memory and adds end character
        int nameLen = tagNameEnd-tagNameStart;
        char *tagName = malloc(sizeof(char) * (nameLen + 1));

        // Copies the srcName into the src
        int i;
        for(i = 0; i < nameLen; i++){
                tagName[i] =  *(tagNameStart + i);
        }
        tagName[nameLen] = '\0';

        // Creates place to store quote
        int quoteLen = quoteEnd - quoteStart;
        char *quote = malloc(sizeof(char) * (quoteLen + 1));

        // Copies the information into the stored space
        for(i = 0; i < quoteLen; i++){
                quote[i] = *(quoteStart + i);
        }
        quote[quoteLen] = '\0';

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
