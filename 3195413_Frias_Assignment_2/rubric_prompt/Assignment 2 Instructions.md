Assignment No. 2 EECS 348 Software Engineering I

Due: 11:59 PM, Thursday, September 17, 2026

Deliverable (submitted to Canvas):

- 1. URL to your GitHub repository. Make sure the repository is public so the GTAs are able to access and grade it.

- Use the following naming convention for your GitHub repository:

- Repository: Fall2026_EECS348_Assignment_2

- Folder: StudentID_Lastname_Assignment_2

- Note: Failure to use the proper naming convention may result in a zero for the assignment.

- Your GitHub repository should include:

- Source code

- Final cycle server executable file

- PDF of your GenAI analysis

- The EECS Cycle servers (or the lab machines) are the only grading environment.

- Please note that the same code may produce different outputs in different environments, especially for C/C++ programs that require compilation.

- Since we cannot access your personal environment, grading will be based solely on the Cycle servers (or the lab machines).

- You must ensure that your submission works correctly in the allowed grading environment.

- Appeals for regrading based on the code working in your own environment will not be accepted.

- This policy applies to all labs and programming assignments for this course.

Assignment:

- 1. Use two GenAI’s to generate C code to implement the following requirements (Note: You may use either ChatGPT or CoPilot as one of your choices, but not both. They are essentially the same GenAI.):

- The program will prioritize emails for a busy company CEO.

- You will use a MaxHeap as a means of implementing a priority queue. A priority queue is a queue where emails can shift towards the front of the queue based on a priority status.

- You must implement a MaxHeap using a list-based implementation. Then use that MaxHeap to handle all your email prioritizing for the CEO.

- You must create functions from scratch. Do not include pre-existing heap modules.


- Test File format:

| Command | Description |
| --- | --- |
| EMAIL <sender category>, <subject line>, <date> | The emails in the CEO’s Inbox should be placed in queue based on their sender category and date. The sender categories and priority to be read are as follows: Boss  read first Subordinate  read next Peer – read next ImportantPerson  read next OtherPerson  read last If there is more than one from a sender, then the newest email (not the oldest) should be read first. I discovered this trick while a manager at Sprint. EMAIL is followed by space. The rest of the fields are delimited. Assume <sender category> is one of the five strings listed above. Assume <subject line> is a string which may contain spaces, but not commas Assume <date> is in the format: MM-DD-YYYY |
| NEXT | Next email for the CEO to read. Display the information on the terminal in the following format: Sender: Subject: Date: |
| READ | CEO has read email and dealt with it |
| COUNT | Display current count of unread emails |

- The grader will test your code with a different test file. The test file will begin with the emails in the CEOs Inbox, followed by the commands the CEO will use to read her Inbox. You can assume a given command will be formatted as shown, but the order and number of commands may vary. For example, be able to handle a file that does NEXT or READ when there are no emails, two NEXTs in a row without an intervening READ (output should be the same for both NEXTs), two READs in a row without an intervening NEXT (highest priority email deleted from queue without displaying it).


## Sample Test File:

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

- Your program should display this:

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

- 2. Analyze the two GenAI programs. The analysis must include:

- a. Name of two GenAI programs

- b. How you accessed them (e.g., Google query, CoPilot on my computer)

- c. Prompt used. (You must use the same prompt for both GenAIs.)

- d. Generated code from both GenAIs.

- e. Analyze the code in terms of correctness, execution time, space complexity, and maintainability as explained in the “GenAI Code Comparison” lecture.

- f. Select one to use as the basis for this assignment and justify why you picked it.

- g. Explain what you did to improve it in terms of correctness, execution time, space complexity, and maintainability.

- 3. Make the changes identified in 2g above.

- 4. Compile and debug your program.

- 5. Push both your source code, final cycle server executable file, and PDF of your GenAI analysis to your GitHub repository.


|   | Rubric for Program Comments |   |
| --- | --- | --- |
| Exceeds Expectations (90-100%) | Meets Expectations (80-89%) | Unsatisfactory (0-79%) |
| Software is adequately commented with prologue comments, comments summarizing major blocks of code, and comments on every line. | Prologue comments are present but missing some items or some major blocks of code are not commented or there are inadequate comments on each line. | Prologue comments are missing all together or there are no comments on major blocks of code or there are very few comments on each line. |

## Adequate Prologue Comments:

- Name of program contained in the file (e.g., EECS 348 Assignment 1)

- Brief description of the program, e.g.,

- o C program that picks a secret number between 1 and 10.

- o The user gets up to 3 tries to guess the number.

- Inputs e.g., none, for a function, it would be the parameters passed to it

- Output, e.g., terminal output between user and program

- All collaborators

- Other sources for the code ChatGPT, stackOverflow, etc.

- Authors full name

- Creation date: The date you first create the file, i.e., the date you write this comment

- Revision date: The date you revised the file

- Revisions: e.g, convert C code to C++

Adequate comments summarizing major blocks of code and comments on every line:

- Provide comments that explain what each line of code is doing.

- You may comment each line of code (e.g., using //) and/or provide a multi-line comment (e.g., using /* and */) that explains what a group of lines does.

- Multi-line comments should be detailed enough that it is clear what each line of code is doing.

- Each block of code must indicate whether you authored the code, you obtained it from one of the sources listed in the prolog, or one of your collaborators authored the code, or if it was a combination of all of these.

Collaboration and other sources for code:

- Failure to identify collaborators or other sources of code will not only result in a 0 on the assignment but will be considered an act of Academic Misconduct.

- Students who violate conduct policies will be subject to severe penalties, up through and including dismissal from the School of Engineering.
