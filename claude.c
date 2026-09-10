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
