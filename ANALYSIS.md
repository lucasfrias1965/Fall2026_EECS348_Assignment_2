# Assignment 2 - EECS 348 (Sofware Engineering) by Lucas Frias

Welcome back, I'm excited for this assignment. Last time, we worked on the basic premise of the AI analysis of the code, and I chose two lesser models. When discussing this with my peers, they told me that Anthropic and OpenAI's latest models, in comparison to the attempts that the local Gwen 4B model and the novel Apple Intellegence, would be like apples to oranges. Both of these companies seem to show a great amount of investment and control in the modern tech landspace, and each are trying to compete to remain the dominant AI company of the future. I decided to do what I had avoided before, and use the two newer AI models. 

My access model was to visit the websites hosted by these AI companies, create an account, and enter in the prompt. I decided to use Sonnet 5 by Anthropic (on their website Claude.ai) and OpenAI's GPT 5.6 Luna (on the website chatgpt.com) and pitted them against one another in this project. 

I will be referring to Anthropic's model as Sonnet and OpenAI's as Luna throughout this document.


The prompt is modifed from the original assignment text, so here it is below:

```
Make this program in C.

•	The program will prioritize emails for a busy company CEO.  
•	You will use a MaxHeap as a means of implementing a priority queue. A priority queue is a queue where emails can shift towards the front of the queue based on a priority status. 
•	You must implement a MaxHeap using a list-based implementation. Then use that MaxHeap to handle all your email prioritizing for the CEO. 
•	You must create functions from scratch. Do not include pre-existing heap modules. 


Here's the file format:

-------------
EMAIL <sender category>, <subject line>, <date> - The emails in the CEO’s Inbox should be placed in queue based on their sender category and date. The sender categories and priority to be read are as follows:  
     •	Boss – read first   
     •	Subordinate – read next  
     •	Peer – read next   
     •	ImportantPerson – read next   
     •	OtherPerson – read last   
     If there is more than one from a sender, then the newest email (not the oldest) should be read first. I discovered this trick while a manager at Sprint.      EMAIL is followed by space. The rest of the fields are delimited.   Assume <sender category> is one of the five strings listed above.   Assume <subject line> is a string which may contain spaces, but not commas   Assume <date> is in the format: MM-DD-YYYY
--------------
NEXT - Next email for the CEO to read. Display the information on the terminal in the following format: 
     Sender: 
     Subject: 
     Date: 
------------
READ - CEO has read with the email and has dealt with it
-----------
COUNT - display current count of untracked files


Here's an example file:
EMAIL Peer,Can you help me on this?,12-01-2024 
EMAIL OtherPerson,Try our product,12-19-2024 
EMAIL Boss,Important,12-20-2024 
EMAIL Subordinate,How do I handle this?,12-25-2024 
EMAIL ImportantPerson,Health Insurance Enrollment,12-31-2024 
EMAIL Boss,Never Mind,01-03-2025 
COUNT 
NEXT 
READ 
NEXT 
READ 
COUNT 
Here is the output that the program should give
There are 6 emails to read. 
 
Next email: 
Sender: Boss 
Subject: Never Mind 
Date: 01-03-2025 
 
Next email: 
Sender: Boss 
Subject: Important 
Date: 12-20-2024 
 
There are 4 emails to read. 

Good luck!
```
The only modifcation that has been done are slight markdown changes to fromat into this document. You can see the original in PROMPT.md.

Below are the analysis pertaining to each category below. I hope this is informative and correct.

## OpenAI'S Luna Ouput:
```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_EMAILS 1000
#define MAX_SUBJECT 500
#define MAX_CATEGORY 30

/* 
   Higher number = higher priority.
   Boss is highest priority, OtherPerson is lowest.
*/
int getCategoryPriority(const char *category)
{
    if (strcmp(category, "Boss") == 0)
        return 5;
    else if (strcmp(category, "Subordinate") == 0)
        return 4;
    else if (strcmp(category, "Peer") == 0)
        return 3;
    else if (strcmp(category, "ImportantPerson") == 0)
        return 2;
    else if (strcmp(category, "OtherPerson") == 0)
        return 1;

    return 0;
}

/* Convert MM-DD-YYYY into YYYYMMDD for easy date comparison. */
int convertDate(const char *date)
{
    int month, day, year;

    sscanf(date, "%d-%d-%d", &month, &day, &year);

    return year * 10000 + month * 100 + day;
}

/* An email stored inside the heap. */
typedef struct
{
    char sender[MAX_CATEGORY];
    char subject[MAX_SUBJECT];
    char date[20];

    int categoryPriority;
    int dateValue;
} Email;

/* MaxHeap implemented using an array/list. */
typedef struct
{
    Email emails[MAX_EMAILS];
    int size;
} MaxHeap;


/*
   Returns 1 if email1 should appear before email2.
   
   Priority rules:
   1. Higher sender category priority comes first.
   2. If categories are equal, newer date comes first.
*/
int higherPriority(Email email1, Email email2)
{
    if (email1.categoryPriority > email2.categoryPriority)
        return 1;

    if (email1.categoryPriority < email2.categoryPriority)
        return 0;

    /* Same sender category: newest email comes first. */
    if (email1.dateValue > email2.dateValue)
        return 1;

    return 0;
}


/* Swap two emails in the heap. */
void swapEmails(Email *email1, Email *email2)
{
    Email temp = *email1;
    *email1 = *email2;
    *email2 = temp;
}


/*
   Move an email upward in the heap until the MaxHeap
   property is restored.
*/
void heapifyUp(MaxHeap *heap, int index)
{
    int parent;

    while (index > 0)
    {
        parent = (index - 1) / 2;

        if (higherPriority(heap->emails[index], heap->emails[parent]))
        {
            swapEmails(&heap->emails[index],
                       &heap->emails[parent]);

            index = parent;
        }
        else
        {
            break;
        }
    }
}


/*
   Move an email downward in the heap until the MaxHeap
   property is restored.
*/
void heapifyDown(MaxHeap *heap, int index)
{
    int left;
    int right;
    int largest;

    while (1)
    {
        left = 2 * index + 1;
        right = 2 * index + 2;
        largest = index;

        /* Check left child. */
        if (left < heap->size &&
            higherPriority(heap->emails[left],
                           heap->emails[largest]))
        {
            largest = left;
        }

        /* Check right child. */
        if (right < heap->size &&
            higherPriority(heap->emails[right],
                           heap->emails[largest]))
        {
            largest = right;
        }

        /* No child has higher priority. */
        if (largest == index)
            break;

        swapEmails(&heap->emails[index],
                   &heap->emails[largest]);

        index = largest;
    }
}


/* Initialize an empty heap. */
void initializeHeap(MaxHeap *heap)
{
    heap->size = 0;
}


/* Add an email to the MaxHeap. */
void insertEmail(MaxHeap *heap, Email email)
{
    if (heap->size >= MAX_EMAILS)
    {
        printf("Error: Maximum number of emails reached.\n");
        return;
    }

    /* Put the new email at the end of the list. */
    heap->emails[heap->size] = email;

    /* Move it upward into the correct position. */
    heapifyUp(heap, heap->size);

    heap->size++;
}


/*
   Remove and return the highest-priority email.
   This is the MaxHeap delete operation.
*/
Email removeMax(MaxHeap *heap)
{
    Email result;

    /* Safety value if heap is empty. */
    result.sender[0] = '\0';
    result.subject[0] = '\0';
    result.date[0] = '\0';

    if (heap->size == 0)
        return result;

    /* The root is the highest-priority email. */
    result = heap->emails[0];

    /* Move the last email to the root. */
    heap->size--;

    if (heap->size > 0)
    {
        heap->emails[0] = heap->emails[heap->size];

        /* Restore MaxHeap property. */
        heapifyDown(heap, 0);
    }

    return result;
}


/*
   Look at the highest-priority email without removing it.
*/
Email peekMax(MaxHeap *heap)
{
    Email result;

    result.sender[0] = '\0';
    result.subject[0] = '\0';
    result.date[0] = '\0';

    if (heap->size == 0)
        return result;

    return heap->emails[0];
}


/*
   Parse an EMAIL command.
   
   Example:
   EMAIL Peer,Can you help me on this?,12-01-2024
*/
Email parseEmail(char *line)
{
    Email email;

    char *data;
    char *sender;
    char *subject;
    char *date;

    /* Skip "EMAIL " */
    data = line + 6;

    /* Get sender category. */
    sender = strtok(data, ",");

    /* Get subject. */
    subject = strtok(NULL, ",");

    /* Get date. */
    date = strtok(NULL, "\n");

    if (sender == NULL)
        sender = "";

    if (subject == NULL)
        subject = "";

    if (date == NULL)
        date = "";

    /* Remove possible spaces around fields. */
    while (*sender == ' ')
        sender++;

    while (*subject == ' ')
        subject++;

    while (*date == ' ')
        date++;

    /* Copy information into the Email structure. */
    strncpy(email.sender, sender, MAX_CATEGORY - 1);
    email.sender[MAX_CATEGORY - 1] = '\0';

    strncpy(email.subject, subject, MAX_SUBJECT - 1);
    email.subject[MAX_SUBJECT - 1] = '\0';

    strncpy(email.date, date, sizeof(email.date) - 1);
    email.date[sizeof(email.date) - 1] = '\0';

    /* Remove trailing carriage return if the file uses Windows line endings. */
    email.date[strcspn(email.date, "\r\n")] = '\0';

    email.categoryPriority = getCategoryPriority(email.sender);
    email.dateValue = convertDate(email.date);

    return email;
}


/* Display the next email without removing it. */
void nextEmail(MaxHeap *heap)
{
    Email email;

    if (heap->size == 0)
    {
        printf("There are no emails to read.\n");
        return;
    }

    email = peekMax(heap);

    printf("Next email:\n");
    printf("Sender: %s\n", email.sender);
    printf("Subject: %s\n", email.subject);
    printf("Date: %s\n", email.date);
}


/* READ removes the highest-priority email. */
void readEmail(MaxHeap *heap)
{
    if (heap->size == 0)
    {
        printf("There are no emails to read.\n");
        return;
    }

    removeMax(heap);
}


/* Display the current number of emails in the heap. */
void countEmails(MaxHeap *heap)
{
    printf("There are %d emails to read.\n", heap->size);
}


int main(int argc, char *argv[])
{
    FILE *file;
    char line[600];

    MaxHeap heap;

    initializeHeap(&heap);

    /*
       The file name can be supplied when running the program:
       
       ./email_priority emails.txt
    */
    if (argc < 2)
    {
        printf("Usage: %s <input file>\n", argv[0]);
        return 1;
    }

    file = fopen(argv[1], "r");

    if (file == NULL)
    {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    /* Read each command from the input file. */
    while (fgets(line, sizeof(line), file) != NULL)
    {
        /* Remove newline characters. */
        line[strcspn(line, "\r\n")] = '\0';

        /* Ignore blank lines. */
        if (strlen(line) == 0)
            continue;

        /* EMAIL command */
        if (strncmp(line, "EMAIL ", 6) == 0)
        {
            Email email = parseEmail(line);
            insertEmail(&heap, email);
        }

        /* NEXT command */
        else if (strcmp(line, "NEXT") == 0)
        {
            nextEmail(&heap);
        }

        /* READ command */
        else if (strcmp(line, "READ") == 0)
        {
            readEmail(&heap);
        }

        /* COUNT command */
        else if (strcmp(line, "COUNT") == 0)
        {
            countEmails(&heap);
        }
    }

    fclose(file);

    return 0;
}
```

## Anthropic's Sonnet (Medium) Output:
```c
/* ============================================================
 * email_priority.c
 *
 * Prioritizes a busy CEO's inbox using a MaxHeap (array/list based
 * priority queue) implemented entirely from scratch -- no library
 * heap functions are used.
 *
 * Commands read from a script file (given as argv[1]) or stdin:
 *
 *   EMAIL <sender category>,<subject line>,<date MM-DD-YYYY>
 *   NEXT
 *   READ
 *   COUNT
 *
 * Sender category priority (highest to lowest):
 *   Boss > Subordinate > Peer > ImportantPerson > OtherPerson
 *
 * Within the same category, the NEWEST email (largest date) is
 * read first.
 * ============================================================ */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ------------------------------------------------------------
 * Constants
 * ------------------------------------------------------------ */
#define MAX_SUBJECT_LEN   256
#define MAX_CATEGORY_LEN  32
#define INITIAL_CAPACITY  16

/* ------------------------------------------------------------
 * Email struct
 * ------------------------------------------------------------ */
typedef struct {
    char category[MAX_CATEGORY_LEN];   /* Boss, Subordinate, Peer, ... */
    char subject[MAX_SUBJECT_LEN];     /* subject line                 */
    char date[11];                     /* MM-DD-YYYY (10 chars + NUL)  */
    int  categoryRank;                 /* numeric priority of category */
    long dateValue;                    /* YYYYMMDD, for easy comparison */
} Email;

/* ------------------------------------------------------------
 * MaxHeap struct (list/array based implementation)
 * ------------------------------------------------------------ */
typedef struct {
    Email *data;      /* dynamically allocated array of emails */
    int    size;      /* number of elements currently in heap  */
    int    capacity;  /* allocated capacity of the array       */
} MaxHeap;

/* ============================================================
 * Category priority lookup
 *
 * Higher number == higher priority (read sooner).
 * ============================================================ */
int categoryRank(const char *category) {
    if (strcmp(category, "Boss") == 0)            return 5;
    if (strcmp(category, "Subordinate") == 0)     return 4;
    if (strcmp(category, "Peer") == 0)            return 3;
    if (strcmp(category, "ImportantPerson") == 0) return 2;
    if (strcmp(category, "OtherPerson") == 0)     return 1;
    return 0; /* unknown category - lowest priority */
}

/* ============================================================
 * Convert MM-DD-YYYY into a comparable integer YYYYMMDD.
 * Larger value == newer date.
 * ============================================================ */
long dateToValue(const char *date) {
    int month = 0, day = 0, year = 0;
    if (sscanf(date, "%d-%d-%d", &month, &day, &year) != 3) {
        return 0; /* malformed date, treat as very old */
    }
    return ((long)year * 10000L) + (month * 100L) + day;
}

/* ============================================================
 * Compare two emails for max-heap ordering.
 * Returns 1 if "a" has strictly higher priority than "b", else 0.
 *
 * Priority rules:
 *   1) Higher categoryRank wins.
 *   2) If categories tie, newer date (larger dateValue) wins.
 * ============================================================ */
int emailHasHigherPriority(const Email *a, const Email *b) {
    if (a->categoryRank != b->categoryRank) {
        return a->categoryRank > b->categoryRank;
    }
    return a->dateValue > b->dateValue;
}

/* ============================================================
 * MaxHeap creation / destruction
 * ============================================================ */
MaxHeap *heapCreate(int initialCapacity) {
    MaxHeap *heap = (MaxHeap *)malloc(sizeof(MaxHeap));
    if (!heap) {
        fprintf(stderr, "Fatal: out of memory creating heap.\n");
        exit(EXIT_FAILURE);
    }
    heap->data = (Email *)malloc(sizeof(Email) * initialCapacity);
    if (!heap->data) {
        fprintf(stderr, "Fatal: out of memory allocating heap storage.\n");
        exit(EXIT_FAILURE);
    }
    heap->size = 0;
    heap->capacity = initialCapacity;
    return heap;
}

void heapDestroy(MaxHeap *heap) {
    if (!heap) return;
    free(heap->data);
    free(heap);
}

/* Double the backing array when it fills up. */
void heapEnsureCapacity(MaxHeap *heap) {
    if (heap->size < heap->capacity) return;
    int newCapacity = heap->capacity * 2;
    Email *newData = (Email *)realloc(heap->data, sizeof(Email) * newCapacity);
    if (!newData) {
        fprintf(stderr, "Fatal: out of memory growing heap.\n");
        exit(EXIT_FAILURE);
    }
    heap->data = newData;
    heap->capacity = newCapacity;
}

/* ------------------------------------------------------------
 * Index helpers for the array-based (list-based) heap layout.
 * For node at index i (0-based):
 *   parent  = (i - 1) / 2
 *   left    = 2*i + 1
 *   right   = 2*i + 2
 * ------------------------------------------------------------ */
static int parentIndex(int i) { return (i - 1) / 2; }
static int leftChildIndex(int i)  { return 2 * i + 1; }
static int rightChildIndex(int i) { return 2 * i + 2; }

static void swapEmails(Email *a, Email *b) {
    Email temp = *a;
    *a = *b;
    *b = temp;
}

/* ------------------------------------------------------------
 * Sift-up (bubble-up): restores heap property after insertion
 * at the end of the array.
 * ------------------------------------------------------------ */
void heapSiftUp(MaxHeap *heap, int index) {
    while (index > 0) {
        int parent = parentIndex(index);
        if (emailHasHigherPriority(&heap->data[index], &heap->data[parent])) {
            swapEmails(&heap->data[index], &heap->data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

/* ------------------------------------------------------------
 * Sift-down (bubble-down): restores heap property after the
 * root is replaced (e.g. during extraction).
 * ------------------------------------------------------------ */
void heapSiftDown(MaxHeap *heap, int index) {
    while (1) {
        int left  = leftChildIndex(index);
        int right = rightChildIndex(index);
        int largest = index;

        if (left < heap->size &&
            emailHasHigherPriority(&heap->data[left], &heap->data[largest])) {
            largest = left;
        }
        if (right < heap->size &&
            emailHasHigherPriority(&heap->data[right], &heap->data[largest])) {
            largest = right;
        }
        if (largest == index) break;

        swapEmails(&heap->data[index], &heap->data[largest]);
        index = largest;
    }
}

/* ------------------------------------------------------------
 * Insert a new email into the heap.
 * ------------------------------------------------------------ */
void heapInsert(MaxHeap *heap, Email email) {
    heapEnsureCapacity(heap);
    heap->data[heap->size] = email;
    heapSiftUp(heap, heap->size);
    heap->size++;
}

/* ------------------------------------------------------------
 * Peek at the highest-priority email without removing it.
 * Returns 1 on success, 0 if heap is empty.
 * ------------------------------------------------------------ */
int heapPeek(MaxHeap *heap, Email *outEmail) {
    if (heap->size == 0) return 0;
    *outEmail = heap->data[0];
    return 1;
}

/* ------------------------------------------------------------
 * Remove and return the highest-priority email.
 * Returns 1 on success, 0 if heap is empty.
 * ------------------------------------------------------------ */
int heapExtractMax(MaxHeap *heap, Email *outEmail) {
    if (heap->size == 0) return 0;

    *outEmail = heap->data[0];
    heap->size--;

    if (heap->size > 0) {
        heap->data[0] = heap->data[heap->size];
        heapSiftDown(heap, 0);
    }
    return 1;
}

int heapSize(MaxHeap *heap) {
    return heap->size;
}

/* ============================================================
 * String helpers
 * ============================================================ */

/* Strip trailing newline / carriage return characters. */
void trimNewline(char *s) {
    size_t len = strlen(s);
    while (len > 0 && (s[len - 1] == '\n' || s[len - 1] == '\r')) {
        s[len - 1] = '\0';
        len--;
    }
}

/* Remove leading/trailing whitespace in place. */
void trimWhitespace(char *s) {
    /* Trim trailing */
    int len = (int)strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[--len] = '\0';
    }
    /* Trim leading */
    int start = 0;
    while (s[start] != '\0' && isspace((unsigned char)s[start])) {
        start++;
    }
    if (start > 0) {
        memmove(s, s + start, strlen(s + start) + 1);
    }
}

/* ============================================================
 * Parse an "EMAIL <category>,<subject>,<date>" line (the text
 * AFTER the leading "EMAIL " token has already been stripped
 * off by the caller) into an Email struct.
 *
 * Returns 1 on success, 0 on a malformed line.
 * ============================================================ */
int parseEmailFields(char *fields, Email *outEmail) {
    /* fields looks like: "Boss,Never Mind,01-03-2025" */
    char *firstComma = strchr(fields, ',');
    if (!firstComma) return 0;
    *firstComma = '\0';
    char *category = fields;

    char *rest = firstComma + 1;
    char *secondComma = strrchr(rest, ','); /* subject can't have commas,
                                                so the LAST comma in the
                                                remainder separates the
                                                subject from the date */
    if (!secondComma) return 0;
    *secondComma = '\0';
    char *subject = rest;
    char *date = secondComma + 1;

    trimWhitespace(category);
    trimWhitespace(subject);
    trimWhitespace(date);

    if (strlen(category) == 0 || strlen(date) == 0) return 0;

    strncpy(outEmail->category, category, MAX_CATEGORY_LEN - 1);
    outEmail->category[MAX_CATEGORY_LEN - 1] = '\0';

    strncpy(outEmail->subject, subject, MAX_SUBJECT_LEN - 1);
    outEmail->subject[MAX_SUBJECT_LEN - 1] = '\0';

    strncpy(outEmail->date, date, sizeof(outEmail->date) - 1);
    outEmail->date[sizeof(outEmail->date) - 1] = '\0';

    outEmail->categoryRank = categoryRank(outEmail->category);
    outEmail->dateValue = dateToValue(outEmail->date);

    return 1;
}

/* ============================================================
 * Command handlers
 * ============================================================ */
void handleEmailCommand(MaxHeap *heap, char *line) {
    /* line begins with "EMAIL ", the rest is the field data */
    char *fields = line + strlen("EMAIL");
    while (*fields == ' ') fields++; /* skip the single space (and any extra) */

    Email email;
    if (!parseEmailFields(fields, &email)) {
        fprintf(stderr, "Warning: could not parse EMAIL line: %s\n", line);
        return;
    }
    heapInsert(heap, email);
}

void handleNextCommand(MaxHeap *heap) {
    Email top;
    if (!heapPeek(heap, &top)) {
        printf("Next email:\n");
        printf("There are no emails to read.\n\n");
        return;
    }
    printf("Next email:\n");
    printf("Sender: %s\n", top.category);
    printf("Subject: %s\n", top.subject);
    printf("Date: %s\n\n", top.date);
}

void handleReadCommand(MaxHeap *heap) {
    Email dequeued;
    if (!heapExtractMax(heap, &dequeued)) {
        printf("There is nothing to mark as read.\n\n");
    }
    /* Per spec, READ simply removes the current top email
       (the CEO dealt with it). No output is required. */
}

void handleCountCommand(MaxHeap *heap) {
    printf("There are %d emails to read.\n\n", heapSize(heap));
}

/* ============================================================
 * Main driver: reads commands line by line from the given file
 * (or stdin if no file argument is provided) and dispatches
 * them to the appropriate handler.
 * ============================================================ */
int main(int argc, char *argv[]) {
    FILE *input = stdin;

    if (argc > 1) {
        input = fopen(argv[1], "r");
        if (!input) {
            fprintf(stderr, "Error: could not open file '%s'\n", argv[1]);
            return EXIT_FAILURE;
        }
    }

    MaxHeap *heap = heapCreate(INITIAL_CAPACITY);

    char line[1024];
    while (fgets(line, sizeof(line), input)) {
        trimNewline(line);

        /* Skip blank lines */
        char *check = line;
        while (*check == ' ' || *check == '\t') check++;
        if (*check == '\0') continue;

        if (strncmp(line, "EMAIL", 5) == 0) {
            handleEmailCommand(heap, line);
        } else if (strncmp(line, "NEXT", 4) == 0) {
            handleNextCommand(heap);
        } else if (strncmp(line, "READ", 4) == 0) {
            handleReadCommand(heap);
        } else if (strncmp(line, "COUNT", 5) == 0) {
            handleCountCommand(heap);
        } else {
            fprintf(stderr, "Warning: unrecognized command: %s\n", line);
        }
    }

    if (input != stdin) {
        fclose(input);
    }
    heapDestroy(heap);

    return EXIT_SUCCESS;
}
```

# Correctness

Issues were immediately encountered here. OpenAI's Luna model was unable to produce functioning code, and as per the assignment instructions I cannot ask for further clarifcation. Nor am I going to cherrypick the best result given by OpenAI to try and get better results.

## The Spacing Problem
The mistake was a slight one. In the main program execution, after accepting a file as input, Luna's code reads the file correctly. It even is able to add elements to the MaxHeap:

```c
        ...
        /* EMAIL command */
        if (strncmp(line, "EMAIL ", 6) == 0) //correct spacing
        {
            Email email = parseEmail(line);
            insertEmail(&heap, email);
        }
        ...
```

There's two details important here. First is the spacing. Since we probably want to have spacing between our commands in the file formatting, it's important to include the spacing. However, Luna fails to do this and switches between two similar functions **strncmp** versus **strcmp**.


```c
else if (strcmp(line, "NEXT") == 0)
        {
            nextEmail(&heap);
        }

        /* READ command */
        else if (strcmp(line, "READ") == 0)
        {
            readEmail(&heap);
        }
```

According to Mr Bharati of StackOverflow fame (https://stackoverflow.com/questions/30190460/advantages-of-strncmp-over-strcmp#30190652) he has this to say about the difference:

```The problem with strcmp is that sometimes, if by mistake, arguments that are passed are not valid C-strings (meaning that p1 or p2 is not terminated with a null character i.e. not NULL-terminated String), then, strcmp continues comparing until it reaches non-accessible memory and crashes or sometimes results to an unexpected behaviour.
```


Is that what happens here? Yes, unfortunately. strcmp does not keep going until it sees the null terminator character, but it's very unsafe because the input file doesn't guarentee that the newline character is this. This causes every single strcmp to fail. Sonnet avoids this pitfail, but Luna fails with just a single space occuring after the line. strncmp would be much safer here because for each newline the promise is that there is at least some command such as COUNT, READ, NEXT. Just adding strncmp with the correct sizing fixes this bug simply, but Luna was unable to anticipate this, partly because of the file format but mostly because of using unsafe functions and trusting they work and ignoring new space.

The file format had padding on the actual prompt given (there is a space 31 0x0a character) and Luna just ignored it and believed that there would never be padding. Luna knows how to use strncmp as they do with the EMAIL portion, but fails to actually use it correctly for the other edge cases.

I took the liberty of fixing this bug and then testing Sonnet's and Luna's output with the test file. Both Sonnet and Luna corrected outputted fine, but Luna unpatched outputted nothing because it did not recognize any commands when there was one space at the end of the command. I will still be analyzing Luna's output in terms of time complexity, space complexity, and maintainability, because despite this error the implementation works for the test file. However, note that this analysis works only for when files are specially formatted for Luna's preferred formatting. This makes it so that Luna has less correct output than Sonnet.

Quick note: I believe Sonnet predicted this as it was able to test its code within a Unix-like shell given to it by Anthropic, anticipate the space, and program in edge cases.

## Heap VS Stack Prompting Analysis 

The prompt I gave, derived from the assignment prompt, specified
```
    You will use a MaxHeap as a means of implementing a priority queue
```
and then later stated
```
    You must implement a MaxHeap using a list-based implementation.
```
the key word being "list".

One very commonly discussed feature with C and other statically strictly typed languages is the fact that array sizes are known at run time, or when the array is created. It is not recommended and is UB to add extra elements to an array after it is created.

```
    char a[32] = {0}; //make a char array to do cstring stuff
    a[1] = 'a'; //fine and dandy
    a[33] = 12; //UB!! maybe this works, maybe we segfault, maybe we overwrite some other stack variable.
```

If we want to still do this in the stack, there's a couple of ways.
Firstly, we can just use a really big stack frame
```
    Email * email_max_heap[MAX_EMAIL_SIZE] = {0};
```
We do use a lot of stack that we don't probably need, but modern operating systems are smart with these things and actually don't really give you the memory unless you really need it. However, this might not be the best practice. There are other ways, because we don't have to set the max_heap_size at compile time.


If we are trying to, for example, track these emails from the file, we can't know the possible amount of emails before we read them. We could do something like this:
```c
    //Note: this is psuedo code, and I haven't actually tested it for unsafeness
    //just to illustrate my point. Also thank you to AProgrammer from StackOverflow
    //https://stackoverflow.com/questions/9206091/going-through-a-text-file-line-by-line-in-c
    FILE * fptr = open("EMAILS.TXT", "rb");//grab our fptr
    char cmd_buf[512] = {0};
    unsigned int email_count = 0;
    while (*fptr!=NULL){
        fgets(cmd_buf, sizeof(cmd_buf), fptr);
        if (! (strncmp(cmd_buf, "EMAIL") )){ //if strncmp doesn't return any differences
            email_count++; //increment the email count
        }
    }
    Email * email_max_heap_arr[email_count] = {0}; //make a new maxheap with the email array account. we can still malloc email structs, but use an array
```

This snippet of code I programmed up there would be smarter than just setting some max size, but also would take an extra pass before evaluation, possibly increasing the time complexity, and also could still have unused memory once an email is read and "de-heaped" from the array. This would contain extra unused memory.

If we instead want to have a completely dynamic implementation, we have to use the heap and allocate variables. We could do this two ways: with a linked list or an array.

However, when doing max heaps, just as a refresher, it would be better to use an array based implementation instead of a linked list because each malloc for a new node in a linked list will give it a completely unrelated virtual memory address. An array is the best way, but it because difficult. Look at this function below:
```c
void heapEnsureCapacity(MaxHeap *heap) {
    if (heap->size < heap->capacity) return;
    int newCapacity = heap->capacity * 2;
    Email *newData = (Email *)realloc(heap->data, sizeof(Email) * newCapacity);
    if (!newData) {
        fprintf(stderr, "Fatal: out of memory growing heap.\n");
        exit(EXIT_FAILURE);
    }
    heap->data = newData;
    heap->capacity = newCapacity;
}
```
This is a function used by Sonnet's implementation to ensure before an addition that the new size of the array is large enough to actually add the element. If not, it will double in size. However, we want to guarentee contigous (touching) memory, so we use realloc for the heap data and then pass the new capacity to the correct position. 

This is a very fancy solution, but it's kind of like bringing silverware to a McDonald's. A stack based implementation is possible without mallocing like this, but it is a pretty implementation too. The devil is in the details of whether or not we need to worry about massive amounts of emails. Let's do the math:


In 2009 (https://stackoverflow.com/questions/1825964/c-c-maximum-stack-size-of-program-on-mainstream-oses#1825996) the average stack frame size is roughly ~ 1 MB.

Our email struct is:

```c

typedef struct {
    char category[MAX_CATEGORY_LEN];   /* Boss, Subordinate, Peer, ... */
    char subject[MAX_SUBJECT_LEN];     /* subject line                 */
    char date[11];                     /* MM-DD-YYYY (10 chars + NUL)  */
    int  categoryRank;                 /* numeric priority of category */
    long dateValue;                    /* YYYYMMDD, for easy comparison */
} Email;

```

where:
```
#define MAX_SUBJECT_LEN   256
#define MAX_CATEGORY_LEN  32
#define INITIAL_CAPACITY  16
```

(Small note, it is strange that the date is hard-coded and not a macro, at least for readability)

Anyways, we can calculate roughly how much the struct's size ought to be.

char * 256 + char * 32 + char * 11 + int + long
256 + 32 + 11 + 4 + 8 //note here, i'm assuming modern implementations where int is 4 
256 + 32 + 11 + 4 + 8  = **311** bytes per email

Assume we have a pittance of 1 megabyte of stack frame. Let's calculate the worst case scenario (assume that the rest of our program uses 1 512 bytes for misc.)

(1,000,000 - 512) / 311 ~= 3,120 emails possible to be processed on one stack frame.


Being a KU student for a year and a half has given me 1,526 emails in my mailbox. I promise you, KU students get a ridiculous amount of emails. Maybe not as much as a CEO, but I think it's comparable

So, theoretically, if our boss was running his same email server on Windows Vista on a Pentium, he would have to clean his inbox once a year. Hopefully he's not. But since this is corporate software, expecting people to upgrade their systems and follow maintaince is a miracle. A heap based implementation probably is the way to go, and even though this is a lot of analysis, this is a fundemental decision choice. Heap isn't that much worse or better than stack, but it is interesting, because I belive that most human C programmers would probably do a heap implementation.

Can't say too much else about the MaxHeap implementation. AI has been programmed on thousands of maxheap implementations, and it chose a decent one for C.


# Time Complexity

There's a lot to analyze on this, but here's the flow of each program, generally


## General Program Flow:

* Read the file from argc
* Go through every character, and handle the command:
    * If the command == EMAIL, heapInsert
        * Then heapSiftUp
            * while loop to set the index at the right place
    * If the command == READ, then use HeapExtractMax
        * if the size is zero then return
        * else, heapSiftDown
            * while loop for every element that sorts everything

This is pretty much just a MaxHeap implementation. Since this is a type of complete Binary Tree, this has a time complexity of O(n * log(n)) complexity, and a space complexity of O(n) for every element. We'll touch on space later.


## Extra Malloc Time 

There is also not too much to say on the MaxHeap complexity, expect that because it's heap allocated, it is ever so slightly slower because of the heap. This is because unliked dynamically sized linked lists or binary search trees, the maxheap can generally be done in an array (remember above in Correctness the heap versus stack discussion as it relates to MaxHeap)

From Mr Delroy of StackOverflow:

(https://stackoverflow.com/questions/24057331/is-accessing-data-in-the-heap-faster-than-from-the-stack)

```
So, when using the stack programmers tend to do what they can with arrays, which are contiguous in memory, even if it means a little brute-force searching. The cache-efficiency may well make this better overall than heap based data containers where the elements are spread across more cache lines. Of course, stack usage doesn't scale to large numbers of elements, and - without at least a backup option of using heap - creates programs that stop working if given more data to process than expected.
```

P.S. I highly encourage you to read this post, it is insightful.

Exactly the point to illustrate, the heap allocation will just be a bit slower. We can call this delay in allocation *M*. Sonnet's default macro for the allocation size is 16, so we can say that if it passes through this, we need to reallocate the the memory, which doubles it's capacity. Therfore, the total delay *T* for a maximum occurance of reallocation N is:
```

    T = M + M*N
```

This does not overly slow down our program, unless the memory becomes quite large. This causes the M to adjust based on the amount of N.



# Space Complexity

Similar to the discussion over time complexity, the space complexity follows from the MaxHeap data structure, with the wrinkle of it being malloced. Generally, it is O(n). But there is a lot that can be change here in terms of optmization, which I will discuss below:

## Struct Padding
You might already know where I'm going with this, but let's look at Luna and Sonnet's structs, their layout and then discuss:

### Luna's Email Struct
```c
#define MAX_CATEGORY 30
#define MAX_SUBJECT 500
typedef struct
{
    char sender[MAX_CATEGORY];
    char subject[MAX_SUBJECT];
    char date[20];

    int categoryPriority;
    int dateValue;
} Email;
```

### Sonnet's Email Struct
```c
#define MAX_CATEGORY_LEN  32
#define MAX_SUBJECT_LEN   256

typedef struct {
    char category[MAX_CATEGORY_LEN];   /* Boss, Subordinate, Peer, ... */
    char subject[MAX_SUBJECT_LEN];     /* subject line                 */
    char date[11];                     /* MM-DD-YYYY (10 chars + NUL)  */
    int  categoryRank;                 /* numeric priority of category */
    long dateValue;                    /* YYYYMMDD, for easy comparison */
} Email;
```

Okay, first and probably most significant detail is the long versus int on the datevalue. Don't really know why Sonnet chose long here. An int represents (unless we're on really old C) 4 bytes, and 2^32 is way more than enough space to represent millenia to come. Long, however, is much larger and very unecessary. It makes larger Emails, which are totally unnecessary.

Sonnet does recognize that we only need 11 characters to represent the date, however, which gives its struct less size. When we build our own custom types, we want to implement the smallest byte size, especially when we have something that occurs a lot (like emails).

To also note, both Sonnet and Luna have ridiculously long email subject permissions. According to Mr. Petrotta from StackOverflow:

(https://stackoverflow.com/questions/1592291/what-is-the-email-subject-length-limit)

```
See RFC 2822, section 2.1.1 to start.

There are two limits that this standard places on the number of characters in a line. Each line of characters MUST be no more than 998 characters, and SHOULD be no more than 78 characters, excluding the CRLF.
```
He continues to say:

```
The recommendation for no more than 78 characters in the subject header sounds reasonable. No one wants to scroll to see the entire subject line, and something important might get cut off on the right.
```

Testing this on a modern email client (this post is from 2009, after all) it seems like KU's outlook has a limit of 256. This seems reasonable. 500 is too much, and makes every email twice as expensive. Less is even possible, although maybe not good to implement.

I made a quick little nonsense C program to check this:

```c
// Online C compiler to run C program online
#include <stdio.h>
/* Type your code here, or load an example. */
#define MAX_CATEGORY_LEN  32
#define MAX_SUBJECT_LEN   256
#define MAX_CATEGORY 30
#define MAX_SUBJECT 500


int main() {
    // Write C code here
typedef struct
{
    char sender[MAX_CATEGORY];
    char subject[MAX_SUBJECT];
    char date[20];

    int categoryPriority;
    int dateValue;
} Email;
    
typedef struct {
    char category[MAX_CATEGORY_LEN];   /* Boss, Subordinate, Peer, ... */
    char subject[MAX_SUBJECT_LEN];     /* subject line                 */
    char date[11];                     /* MM-DD-YYYY (10 chars + NUL)  */
    int  categoryRank;                 /* numeric priority of category */
    long dateValue;                    /* YYYYMMDD, for easy comparison */
} Email2;
    Email rest = {0,0,0,0,0};
    Email2 test = {0,0,0,0,0};
    printf("Sonnet is: %d\n", sizeof(test));
    printf("Luna is: %d", sizeof(rest));

    return 0;
}
```
And the result:

```
Sonnet is: 312
Luna is: 560
``` 

shows that Luna's is almost 80% more memory than Sonnet's.

Perhaps, the most important thing: struct ordering.

To refresh, when creating a struct in C, all the memory is contiguous. Becuse of how C structs operate, they need a certain amount of spacing for the memory. For example:

```c
typedef struct {
    int age
    char letter
} Guy;
```

We just made some Guy, and we would expect the size of this struct to be 4 + 1 to be five. However, actually getting sizeof,
we get 8. Why?

C will try and set the offsets of the table of accessable addresses the same width, generally speaking, for alignment. This can be platform dependent, but this example should generally hold true for almost all C compiliers. It is strange because this changes depending on exactly how the struct is ordered. For example:

```
typedef struct {
    int age
    char letter
    char smetter
    char better
    char netter
} Guy;
```
Also has 8 bytes of size, because the spacing is given for one char byte for each. It is a very strange result of the inner workings of C.

Struct alignment, when done right, saves a lot of space. The long's addition in Sonnet makes the padding larger just where it is itself, but the categoryRank can be a uint8_t (it doesn't have that much range). I suspect the AI is generating older C, again, and doesn't know about 1 byte types.

Recommended changes by me would defintely be to make a shorter subject line (let's say 256, which Sonnet does) and then also to implement smaller representations of the data. This is where I think a lot of problems with spacing and how many emails can be malloced onto the heap. Something like

```c
typedef struct {
    uint8_t categoryType;
    uint8_t senderType;
    int32_t dateValue;
    char subject[MAX_SUBJECT_LEN];
}
```
would be much more efficient, less verbose, and frankly better.

## Heap Space Complexity Benefits 

A brief statement is included here to laud the choice of using a heap allocated array instead of a stack array. While slower, it allows for significantly more emails and has much less potential to cause a stack overflow or other UB resulting from large amounts of data being processed. 

My criticism in Heap vs Stack in Correctness is just to note that humans would, generally, not program a heap allocation method, which makes it seem alien. While reviewing this code more, it makes me appreciate the implementation of using a heap based array for a MaxHeap with considerably large elements, even if it is more difficult to implement.

## String Storing 

I touched on this in the struct, but I would like to revisit the string storing in structs and how unnecessary it is and increases string sizing.

To start with the date, storing both the integer and string representation is wasteful, for every single email. In a program like this where space matters, that is 11 extra bytes for every single struct, best case scenario. Luna's is actually 20, for no specific reason. If the dateValue can be represented and displayed as an integer,
there is no reason to not implement a function to just get the dateValue info. Strings are harder to work with than integers and contain much more potential problems.

The only design reason to include the string is, if the string is constantly used by other parts of the program, it would be annoying to generate a string for it and then free it, because it would get difficult. But in the scope of this program, where we only need to display the date formatted from the integer, this point is moot and not really useful.

Also, because of the nature of this problem, we can represent the sender as a value in the range of a value between 0-4 (Boss, Subordinate, Peer, Important Person, Other Person) and using strncmp for the char * value seems wasteful and long, since the problem statement only has 5 statements. If this wasn't a "toy" problem, we of course would want to store the string value, but in the PROMPT.md given to the LLMs, the senders were specified to be finite values. 

Fixing the string storing in the struct is the biggest optmization in space that can be done to these structs
