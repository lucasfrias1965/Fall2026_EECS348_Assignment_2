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
