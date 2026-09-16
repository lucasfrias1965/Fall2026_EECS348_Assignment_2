## Generative AI Analysis For Assignment 1

In order to complete this assignment to both the best of my ability and to have more interesting and thorough analysis, I used two especially unique models relative to the more commonly used AI assistants (ChatGPT, Claude, CoPilot).

My first model was Apple's new Siri App (or Apple Intellegence), which is technically running off of Google's Gemini models. It will become a native app and feature to Apple Silicon Macs in Mac OS X this fall with the Mac OS 27 release. I was able to access it by having an Apple Silicon Macbook Pro, and by being enrolled in the Apple Developer Beta program in order to test out this new AI feature. 

The steps in order to replicate and access this model exactly is to have a ARM-64 Macbook (in my case a 2020 M1 MBPro) and also be enrolled in Apple's Beta dev program (which I have from writing XCode for a Flutter/Dart application) and then installing the latest beta of Mac OS 27. This will also be released sometime not so soon for anyone with an Apple device past 2023 this September when it releases to the public.

On the other end, because I have Apple Silicon, I ran gemma4b from Ollama. Meta (Facebook) releases local parameter models, and gemma4b is the best one that I can run in a reasonable amount of time on my M1 Macbook. The output is relatively fast (~ 1:24 seconds).

In order to access this model, you must goto the Ollama website ("https://www.ollama.com") and downloading their app by hitting the "Download" button is simple enough. I actually used the CLI interface as specified here: ("https://docs.ollama.com/cli") but the prompts output the same, just in a different place.

The prompt I gave was the exact same as the assignment prompt. Here it is as follows

```

Objective
Write a C program that picks a secret number between 1 and 10.
The user gets up to 3 tries to guess the number.
Requirements
• Use a fixed secret number (for example, 7).
• Use printf to prompt the user and scanf to read their
guess.
• Use a while or for loop to let the user guess up to 3 times.
• Use if-else statements to check if the guess is too high,
too low, or correct.
• Stop the loop immediately if the user guesses the correct
number.
• Print a final win or lose message.
Example Output
2. 3. 4. 5. text
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: 5
Too low! Try again.
Attempt 2/3. Enter your guess: 8
Too high! Try again.
Attempt 3/3. Enter your guess: 7
Correct! You win!

```

This prompt had a couple of interesting side effects. First, here's the code that Apple Intellegence outputed with the prompt above:


```c
#include <stdio.h>

int main() {
    int secret_number = 7;
    int guess; 
    int attempts = 3;
    int won = 0; 

    printf("Guess a number between 1 and 10.\n");
    for (int i = 1; i <= attempts; i++) {
        printf("Attempt %d/%d. Enter your guess: ", i, attempts);
        scanf("%d", &guess);

        if (guess == secret_number) {
            printf("Correct! You win\n");
            won = 1;
            break; 
        } else if (guess < secret_number) {
            printf("Too low! Try again.\n");
        } else {
            printf("Too high! Try again.\n");
            
        }
    }
    if (!won) {
        printf("Sorry, you lose! The secret number was %d.\n", secret_number);
    }

    return 0;
}
```

And here is the Ollama implementation (note that the comments in this code are not my own)

```c
#include <stdio.h>
#include <stdlib.h> // For system("cls") if desired, but not strictly needed here

// Function prototypes (optional but good practice)
void startGame();

int main() {
    printf("***************************************\n");
    printf("*       The Number Guessing Game      *\n");
    printf("***************************************\n");
    startGame();
    return 0;
}

/**
 * Implements the core game logic: guessing a fixed secret number.
 */
void startGame() {
    // Fixed secret number (Requirement)
    const int SECRET_NUMBER = 7;
    int user_guess;
    int attempts_left = 3;
    int i;
    int won = 0;

    printf("\nGuess a number between 1 and 10.\n");

    // Use a for loop to limit the attempts (Requirement)
    for (i = 1; i <= attempts_left; i++) {

        // Check if the player has already won in a previous iteration
        if (won) {
            break; // Stop looping immediately upon winning
        }

        printf("\nAttempt %d/%d. Enter your guess: ", i, attempts_left);

        // Read user input
        if (scanf("%d", &user_guess) != 1) {
            // Handle invalid input (e.g., non-numeric characters)
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer to prevent infinite loop on bad input
            while (getchar() != '\n');
            i--; // Do not count this failed attempt
            continue;
        }

        // Check the guess against the secret number using if-else statements
        if (user_guess == SECRET_NUMBER) {
            printf("\nCorrect! You guessed the number %d!\n", SECRET_NUMBER);
            won = 1; // Set the win flag
            break;   // Stop the loop immediately
        } else if (user_guess < SECRET_NUMBER) {
            printf("Too low! Try again.\n");
        } else { // user_guess > SECRET_NUMBER
            printf("Too high! Try again.\n");
        }
    }

    // Final win or lose message check
    if (won) {
        printf("\n*** CONGRATULATIONS! You WIN the game! ***\n");
    } else {
        printf("\n=========================================\n");
        printf("Game Over. You ran out of attempts.\n");
        printf("The secret number was %d.\n", SECRET_NUMBER);
        printf("Better luck next time!\n");
    }
}
```

Here's the stuff in order:

## "Correctness"

In terms of functionality, the programs are *nearly* identical. First two quick weird things:
  1) Gemma decided to add a lot more extra output. I don't know why the AI thought to do this.
  
  2) Secondly, that because of the influence of the recommendation, both AIs decided to use the number 7. They really wanted to adhere to the guidelines

Now here's the crazy thing: generally, LLMs such as Gemini (or Apple Intellegence) are stronger as cloud based models. They usually preform better at making code. However, Gemma's output accounts for a non-integer or invalid character string. It checks the value of scanf("%d) to see if there's any errors! 

```c
        if (scanf("%d", &user_guess) != 1) {
            // Handle invalid input (e.g., non-numeric characters)
            printf("Invalid input. Please enter a number.\n");
            // Clear the input buffer to prevent infinite loop on bad input
            while (getchar() != '\n');
            i--; // Do not count this failed attempt
            continue;
        }
```

It does it a little clunkily (we could implement this same behavior with a while loop and not have this deincrementing i and calling continue situation) but it does it. I suspect that when it read that the guess must be evaluated using an if/else block, it also assumed it would have to do that with handling incorrect input.


There is also another problem with both of these: integer under and overflow checking. If the user were to (for some reason) guess a number that is **above** 10 or **below** 1 (something crazy high) the integer value will wrap back around and overflow.

```
Guess a number between 1 and 10.
Attempt 1/3 Enter your guess: 2147483647
Too high! Try again
Attempt 2/3 Enter your guess: 2147483648
Too low! Try again!
```


Here the integer value wrapped back around. This is unexpected behavior, and causes something strange to happen:

```
Guess a number between 1 and 10.
Attempt 1/3. Enter your guess: 4294967303
Correct! You win
```

I got this number by taking 7 and adding the total possibilities an int can represent (2^32) which causes it to wrap all the way back around to 7. Clearly, my intended number was not 4294967303, but because of the lack of input checking it worked.
C



Apple AI does not have any scanf check, and typing in a non numeric character will cause it to announce your defeat. Meta 1, Apple 0.

## Execution Time

Both have a theoretical time complexity of O(n) where N is the number of guesses. This isn't very complicated of a program, so there's not too much to say here.
The only difference I could measure benchmarking them is a slight CPU increase because of the constant printf calls Gemma made.

As we continue in the semester, I will show the downsides of different N inputs in algorithms designed by LLMs. Here's the output I got for each:

```
./apple  0.05s user 0.02s system 1% cpu 4.051 total
./gemma  0.05s user 0.02s system 2% cpu 3.555 total
```

To clarify, the 4.051 and 3.555 total is the total amount of runtime, including user delay typing. The important metric is the 0.02 s system time but the slight CPU boost in usage for gemma.



## Space Complexity

Also not too much to say except a weird quirk: both Gemma and Apple ended up using integers as a boolean value. I discuss this a little on my maintainability section. There is no real good reason to use an integer instead of stdbool except for that in the end, it doesn't really matter or make any difference except for readability.

The program has an O(1) space complexity, as all of the used variables don't mutate in size after the initial call for N number of guesses. 

### Apple Intellegence Program Memory Statistics
```
            6803456  maximum resident set size
                   0  average shared memory size
                   0  average unshared data size
                   0  average unshared stack size
                2767  page reclaims
                   9  page faults
                   0  swaps
                   0  block input operations
                   0  block output operations
                   0  messages sent
                   0  messages received
                   0  signals received
                   3  voluntary context switches
                  13  involuntary context switches
           373788298  instructions retired
           214753902  cycles elapsed
             1425408  peak memory footprin
```
### Gemma 4B Program Memory Statistics
```
    6803456  maximum resident set size
        0  average shared memory size
        0  average unshared data size
        0  average unshared stack size
      2768  page reclaims
        9  page faults
        0  swaps
        0  block input operations
        0  block output operations
        0  messages sent
        0  messages received
        0  signals received
        3  voluntary context switches
        19  involuntary context switches
    373593336  instructions retired
    217778774  cycles elapsed
    1425408  peak memory footprint
```

The fact that there is little difference here shows that in the end, the compilier takes ownership of your code. Indeed, by doing some zsh magic:

```
lucasfrias@Frias-iMac 3195413_Frias_Assignment_1 % file1=./apple   
file2=./gemma   

size1=$(wc -c < "$file1")
size2=$(wc -c < "$file2")

common=$(( size1 < size2 ? size1 : size2 ))
diff_common=$(cmp -l "$file1" "$file2" 2>/dev/null | wc -l)
extra=$(( size1 > size2 ? size1 - size2 : size2 - size1 ))
total=$(( size1 > size2 ? size1 : size2 ))
different=$(( diff_common + extra ))

awk -v d="$different" -v t="$total" \
  'BEGIN { printf "%d of %d bytes differ (%.2f%%)\n", d, t, 100*d/t }'

1126 of 8568 bytes differ (13.14%)
```

**N.B This zsh script was generated by DuckDuckGo's Duck AI (ChatGPT Luna 5.6 Masked) to quickly test space code**

While I will be very critical of certain practices introduced by the LLMs in the next section, it's good to know that the actual implementation does not majorily differ.

Addended - Most of GCC's program files contain large amounts of zeroes within the middle section (which may be due to alignment? https://stackoverflow.com/questions/59026218/gcc-is-generating-binaries-filled-with-zeroes). Don't know if that's germane, but take this whole previous statement with a grain of salt.

## Maintainability


There is a lot to analyze here. Let's start with variable declaration conventions.

In ANSI C (or C89, the ISO C standard), the variable declaration is different from modern C. Variable declarations are only permited at the top of the function. If there's a line past this, that declares a new variable not referenced previously, then ANSI C compiliers may not be able to compile it at all.

The reason for this is the same reason that most bad programming design choices are made: memory. When working with a couple of kilobytes of buffer space for compiling, it is more memory safe (and often faster, avoiding recursion) to have all variables within the function scope declared in the beginning of the function. 


```c
#include <stdio.h>

/*An example of the variable declaration */
int main(){
    int a = 1; //all good
    int b = 2;
    printf("%d + %d = ", a, b);
    int c = 3; //not so fast!! this wouldn't compile back in dr j's day :(
    printf("%d", c);
}
```

This convention *only* exists because of the limitations of early computers compiling C. There is little to no good reason to always declare **every** single variable at the top of the current scope. Gemma illustrates why this sucks fantastically

```c

    const int SECRET_NUMBER = 7;
    int user_guess;
    int attempts_left = 3;
    int i;
    int won = 0;

    printf("\nGuess a number between 1 and 10.\n");

    // Use a for loop to limit the attempts (Requirement)
    for (i = 1; i <= attempts_left; i++) {
```

Notice how confusing it is that we don't declare i's type, unless we scroll up? We can assume int, but syntactically it's very strange to not declare the variable 
*within* the for loop. Not only that, but this means that the lifetime of the variable ```i``` will persist past the for scope (which doesn't really matter, but doesn't need to happen anyways). Having a for loop with the ```for(int i = 0; i<5; i++)``` is something that's on the second chapter of every C book written since C99. Why did Gemma decide to write this way? No way to tell.

(Small note, I also believe this is why Gemma used an int as a boolean value instead of <stdbool.h>)


Another bone to pick with Gemma: the const int
```c
    const int SECRET_NUMBER = 7;
```

I feel fair in criticizing this for a convention that I think matters a lot for scalability: compile time versus runtime constant variable declaration. 

If you want to declare a constant variable that's assigned once during its duration during the scope and never mutated after it's assigned, the const keyword is the perfect keyword for you. This helps us make sure our program manages constant memory well

```c
const unsigned char = fgetc(my_fptr); //the first char depends on what file the user gives us
```

In fact, some people like this so much (cough, Rust) that they will make all variables immutable by declaration unless using the ```mut``` keyword.

But what if your value is predetermined, or how the assignment says it

```
• Use a fixed secret number (for example, 7).
```

This is fixed, and not mutable by the user at runtime. Therefore, we can use a macro.

A C macro, without getting too technical, allows us to replace the instance of a namespace with the value we assign it. 

Using Godbolt in x86 gcc 16.2 (the default) here's a table demonstrating the difference between using a macro and const int within the scope

| With C macro | With `const int` |
|---|---|
| **C code**<br><br>`#define B 2`<br><br>`int do_some_math() {`<br>&nbsp;&nbsp;&nbsp;&nbsp;`int a = 1;`<br>&nbsp;&nbsp;&nbsp;&nbsp;`return a + B;`<br>`}`<br><br>**Assembly**<br><br>`"do_some_math":`<br>&nbsp;&nbsp;&nbsp;&nbsp;`push rbp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov rbp, rsp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov DWORD PTR [rbp-4], 1`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov eax, DWORD PTR [rbp-4]`<br>&nbsp;&nbsp;&nbsp;&nbsp;`add eax, 2`<br>&nbsp;&nbsp;&nbsp;&nbsp;`pop rbp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`ret` | **C code**<br><br>`int do_some_math() {`<br>&nbsp;&nbsp;&nbsp;&nbsp;`int a = 1;`<br>&nbsp;&nbsp;&nbsp;&nbsp;`const int B = 2;`<br>&nbsp;&nbsp;&nbsp;&nbsp;`return a + B;`<br>`}`<br><br>**Assembly**<br><br>`"do_some_math":`<br>&nbsp;&nbsp;&nbsp;&nbsp;`push rbp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov rbp, rsp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov DWORD PTR [rbp-4], 1`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov DWORD PTR [rbp-8], 2`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov edx, DWORD PTR [rbp-4]`<br>&nbsp;&nbsp;&nbsp;&nbsp;`mov eax, DWORD PTR [rbp-8]`<br>&nbsp;&nbsp;&nbsp;&nbsp;`add eax, edx`<br>&nbsp;&nbsp;&nbsp;&nbsp;`pop rbp`<br>&nbsp;&nbsp;&nbsp;&nbsp;`ret` |

**This table MD was AI generated (ChatGPT Luna 5.6 Masked) because i didn't want to write inline HTML to prove my point. The contents of the table are from Godbolt.org, but should more or less arise if compiling using GNU 16 without libc and the -S flag**

As you can see, we do extra work in actually moving the variable. Here's where you might think I'm being really overdramatic, because it does not necessarily *really* matter, right? Compilier optimization gets rid of this, right?

Only if you use the -02 flag in GCC. Otherwise, the compilier will do this work, every single time. I'm not saying that using a const int in this scope is a bad idea, but I want to point out that, especially when doing large projects, there can be severe slowdowns that could be optimized at compile time instead of runtime. This does matter, because unless you very specifically know what's happening under the hood in this situation, there would be no ovicous solution why there's a preformance cost unless you look under the hood and really understand what your LLM is spitting out.

Now let's look at the for loop:

```c
    for (i = 1; i <= attempts_left; i++) {

        // Check if the player has already won in a previous iteration
        if (won) {
            break; // Stop looping immediately upon winning
        }

```

Why not have this as a boolean condition in the for loop? Such as

```c

for (i=1; i <= attempts_left && !won; i++)
```

There is no real good reason to check on the next iteration inside the for loop instead of in it's conditional. However, the AI is strictly following the instructions, which state:
• Use if-else statements to check if the guess is too high,
too low, or correct.

It seems like the LLMS are strictly following this instruction by using if statements to branch instead of the conditional. This doesn't need to happen, and it is erring too much on the side of caution by making more unreadable code. 

This may seem like a lot to criticise on the LLM. But it is because this technology is so impressive and starting to become imbued in our day to day lives that I sincerely believe that this criticism matters, more than ever. If we want to implement LLMs in fields such as biology, thoroughness and well-thought consideration matters, and these two models make strange, robotic choices when generating code.


## Human Improvement

I will be selecting Apple Intellegence's version, mostly because it is less verbose. Most of my criticisms past functionality were directed at Gemma, so it should be easier to modify to how I think this program ought to be made. It's mostly because I'm lazy. Below is the contents of lucas.c:

```c
/* Code generated with help of Apple Intellegence and Gemma 4B, modified by Lucas Frias, 2026
 * MIT License 2.0
 * Make cool stuff!
 */


#include <stdio.h> //standard C library for input output
#include <stdbool.h> //using real booleans instead of integers like C89 does
#define SECRET_NUMBER 3 //macro definition for fixed number, used and evaluated at compile time

int main() {
    //set variables - guess should be zero because the user hasn't guessed yet.
    int guess = 0;
    bool won = false; //use boolean instead of int

    printf("Guess a number between 1 and 10.\n");//display the prompt to the user
    //use non-ANSI compliant int i which modern C utilizes to loop.
    /* this loop has two conditions. we continue this loop only as long as
     i<= the attempts (we haven't done too many attempts) and as long as we haven't won yet
     (!bool). otherwise, we iterate i by one. 
    for (int i = 3; i > 0 && !won; i--){
        //conditional contains win condition which avoids a if break
        // this helps make the code more legible and clear, and also
        // avoids adding extra unecessary code

        printf("Attempt %d/3. Enter your guess: ", i);//print the attempt numbers formatted as an integer
        //if the guess's result is not 1
        if (scanf("%d", &guess) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); //here, we keep cycling through the extra characters.
            //scanf stops at the nth char that doesn't match the value, so we have to clear the
            //rest of the input buffer by moving the pointer until we get to the return value
            i++; //at the end of that, do not count the incorrect attempt as an attempt
            continue; //jump back to the top of the for loop
        }
        if (guess < 1 || guess > 10){
	  printf("Invalid input. Please enter a number between 1-10\n");
	  i++;
	  continue;
        }
 
        //match each guess with an if, as specificed on the rubric
        if (guess == SECRET_NUMBER) {
            won = true;//will stop execution next iteration and jump to result
            //code below handles incorrect cases
        } else if (guess < SECRET_NUMBER) {
            printf("Too low! Try again.\n");
        } else {
            //where g is our guess and x is our input if not g == x and not g > x then must be g < x
            printf("Too high! Try again.\n");
        }
    }
    //handles two outcomes in one inline if statement
    if (won)  printf("Correct! You win\n");
    if (!won) printf("Sorry, you lose! The secret number was %d.\n", SECRET_NUMBER);
    //return exit code
    return 0;
}
```

I would like to say that I vastly improved on the C code provided by Gemma and Apple. While Gemma's was verbose, the input scanning trick was clever, so I implemented that. The rest was Apple Intellegence, with a couple of changes for the if drawbacks and weird program control flow that I discussed in Maintainability.

If I had to criticise myself and my changes, it may be using the C language features (macros, inline if statements) a little too strongly, especially for a beginner program. True, this is a valid criticism. However, I except that generative AI, as it progresses and I progress in this class, must be able to understand these language features. And especially because of the antiquated C89 usage, I worry that scalability on a C program. Especially where syntax and linking multiple headers and libraries can be tricky, it may not serve a user well.


## Conclusion

This assignment allowed me to compare the output, time, space, and maintainibility of a simple C program. Both Apple Intellegence and Meta's Gemma 4B were able to make outputs that worked, although Apple's had no error handling for non-integer or invalid number strings. Meta's had several bizarre syntactical choices, which Apple Intellegence also drew inspiration from. Gemma also added more text than necessary, which isn't necessarily wrong but was not the expected output.

The outputs both struggled on maintainibility, but the output's space time were both O(1) and O(N) respectively for each models' C code.



While this is in PDF format, in the Github there is also a markdown version. I wrote in Markdown because I'm more comfortable with it, and since it's plain text it's safer. You are welcome to read both, they have the same content, but as per the grading rubric the PDF is the definitive version of the document.
