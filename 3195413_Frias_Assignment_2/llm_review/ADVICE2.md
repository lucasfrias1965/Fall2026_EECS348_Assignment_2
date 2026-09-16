# ADVICE 2 — Regrade after your fixes, against ADVICE_1 and the Assignment 2 instructions

I compiled `source/human.c`, ran it against the assignment's Sample Test File, and diffed the current `human.c` / `ANALYSIS.md` against every numbered item in `ADVICE_1.md`. Bottom line: you fixed the real bug, but a leftover line means the file **as committed right now does not compile as C at all**, which is a bigger problem than the one you fixed. Everything else moved forward significantly.

## Updated scorecard

| Criterion | Points possible | ADVICE_1 estimate | Now | Why it moved |
|---|---|---|---|---|
| C Program | 30 | 0–15 (wrong ordering) | **0 as committed / ~28-30 once the one-line fix below is applied** | Ordering logic is now correct against the sample file and the stated edge cases, but `#include <cstdlib>` makes the file fail to compile at all (see #1) |
| GenAI Program & Access | 4 | 4 | 4 | unchanged |
| Prompt & Raw Code | 6 | 6 | 6 | unchanged |
| Code Analysis | 10 | 9–10 | 9–10 | unchanged |
| Selection & Justification | 5 | 5 | 5 | unchanged |
| Code Improvement | 10 | 5 | **7–8** | Space/time now backed by real 500k-line benchmarks; correctness fix exists in code but is never narrated in the write-up, and one benchmark number is off (see #4) |
| Prolog Comments | 7 | 0 | **5–6** | Author, Collaborators, Created, Revised, Description, Output all present now; still missing an explicit "Inputs" line and a "Revisions" description of what changed (see #3) |
| In-Line Comments | 14 | ~7 | **13–14** | Nearly every line in every function now has a comment — this is genuinely in "Exceeds Expectations" territory now |
| Collaborators | 14 | 0 | **12–13** | Every function is tagged with its origin (Sonnet / Lucas / ZedAI); only gap is the StackOverflow URLs ADVICE_1 suggested pinning to the specific code they influenced (see #5) |
| **Total** | **100** | **~46–56** | **~65-70 as committed, ~93-96 once #1 is fixed** | |

The gap between those two "now" totals is one deleted line. Fix it before anything else.

---

## 1. CRITICAL (new, blocks everything): `human.c` does not compile as C

```
human.c:42:10: fatal error: 'cstdlib' file not found
   42 | #include <cstdlib>
```

`<cstdlib>` is a C++ header — it doesn't exist in a C toolchain. `human.c:42` includes it alongside `<stdlib.h>` (line 45, which is the one you actually need and already have). This isn't a portability nit, it's a hard compile failure: `gcc human.c` refuses to produce a binary on any machine, including the Cycle servers, which per the instructions are the **only** environment that counts ("Appeals for regrading based on the code working in your own environment will not be accepted"). A file that can't compile can't pass the Sample Test File, so right now the "C Program" line item — 30 of your 100 points — is a hard zero, for a completely different reason than last time.

**Fix:** delete line 42 (`#include <cstdlib>`). Nothing in the file needs it — `malloc`/`realloc`/`exit`/`EXIT_FAILURE`/`EXIT_SUCCESS` all come from `<stdlib.h>`, which is already included on line 45.

**I verified the rest of the logic is sound once that line is gone.** I copied `human.c` to a scratch location, removed only that include, compiled with `gcc -Wall -Wextra` (zero warnings), and ran it against:

- The assignment's exact Sample Test File — output matched expected exactly, including the two `NEXT`s and the final `COUNT`. This confirms **the categoryRank bug from ADVICE_1 #1 is genuinely fixed**: unifying `category`/`categoryRank` into a single `enum Sender` (ordered `Unknown < OtherPerson < ImportantPerson < Peer < Subordinate < Boss` so the ordinal *is* the priority) removed the uninitialized-garbage-field bug that broke ordering before.
- `NEXT`/`READ` on an empty heap, two `NEXT`s in a row with no `READ` between them, and two `READ`s in a row with no `NEXT` between them — all three edge cases the instructions explicitly call out (`Assignment 2 Instructions.md:59`) behaved correctly.

So the fix is literally one deleted line, not a redesign. Do this first — it's the same "highest-leverage fix available to you tonight" situation as last time, just a different line.

## 2. Still missing: Cycle-server executable (ADVICE_1 #5 — unresolved)

`source/a.out` is still a Mach-O 64-bit binary (macOS). No Linux/Cycle-server binary exists anywhere in the repo. This was flagged last round and hasn't moved. Per the instructions, the repo must include "Final cycle server executable file," and grading happens *only* on the Cycle servers/lab machines. Once #1 is fixed, `ssh` in, compile `human.c` there, and commit that binary under a clear name (e.g. `email_priority`) — not `a.out`.

Also still true from ADVICE_1: `source/anthropic.c` was formerly `claude.c`, but `ANALYSIS.md` (lines 1075, 1092, 1152) still shows commands run against a `./claude` binary. Either the analysis output needs regenerating against the current filenames, or add a one-line note that `claude` was the old binary name for what's now `anthropic.c`, so a grader isn't left hunting for a file that doesn't exist.

## 3. Prolog comments (7 pts) — much better, two items still missing

`human.c:1-40` now has: program name, description, an `Output:` line, `Author: Lucas Frias`, a `Collaborators:` line, `Created:`, and `Revised:`. That resolves most of what ADVICE_1 flagged as completely absent. Two rubric items are still not there:

- **Inputs.** The rubric explicitly lists this as a required prolog item, separate from Output. Add a line like `Inputs: a command script via argv[1] (EMAIL/NEXT/READ/COUNT)`.
- **Revisions** (what changed, not just when). Rubric wants something like "convert C code to C++" as the example — i.e., a one-line description of the actual edit. You have `Revised September 16th 2026` but nothing saying *what* was revised. Add: `Revisions: replaced string category + duplicate int categoryRank fields with a single enum Sender; date stored as uint32_t instead of char[]`.

Everything else in the prolog is solid now — don't touch it, just add these two lines.

## 4. Code Improvement (10 pts) — good benchmarks, but the write-up skips correctness

`ANALYSIS.md`'s "Improved Space and Time Complexity" section (near line 2283) has real, reproducible benchmarks on a 500,000-line generated file — actual `time` output, actual `time -l` memory numbers, a clear percentage improvement for both CPU time (~9%) and memory (~12%). That's genuinely strong evidence for the space/time portions of rubric item 2g.

Two things to fix:

- **Correctness is never discussed.** The assignment explicitly asks you to "explain what you did to improve it in terms of correctness, execution time, space complexity, and maintainability" (all four, explicitly). Your `human.c` fix *does* correct a real bug (the categoryRank issue ADVICE_1 caught), but nowhere in `ANALYSIS.md`'s "Human Output" section do you say so. Add a short paragraph: what was wrong (duplicate/uninitialized priority field), why it produced wrong ordering, and how collapsing it into one `enum Sender` fixed it. This is a two-minute add that closes a rubric-named gap.
- **One benchmark number doesn't match the current code.** `ANALYSIS.md` line 2317 states "Sonnet used 312 bytes as opposed to my 272 bytes." I compiled the current `Email` struct from `human.c` and got `sizeof(Email) == 264`, not 272. Likely this line is stale from an earlier version of the struct before you finished trimming it. Worth re-running `sizeof` on the current struct and updating the number so a grader who checks it (as I did) doesn't find a mismatch.

## 5. Collaborators (14 pts) — strong now, one nice-to-have left

Every function in `human.c` now carries an origin comment (`//generated by sonnet`, `//ADDED BY LUCAS`, `//generated by Sonnet, bool by Lucas`, etc.). This directly answers the academic-integrity concern ADVICE_1 raised — it's no longer just documented in `ANALYSIS.md`, it's in the source file itself, which is what the rubric asks for.

The one thing ADVICE_1 suggested that didn't make it in: dropping the StackOverflow URLs (Bharati's strncmp/strcmp answer, Delroy's stack-vs-heap answer) as comments next to the specific code they influenced. Right now there's no `stackoverflow.com` reference anywhere in `human.c` — the citations only live in `ANALYSIS.md`. This is a minor point next to the bigger items above, but the rubric phrase is "other sources for the code," and your prolog's `Collaborators:` line doesn't currently mention StackOverflow at all (only Sonnet and ZedAI). Consider adding it to the prolog's collaborators/sources line at minimum, even if you skip the inline URL comments.

## 6. New, not in ADVICE_1: repository structure doesn't match the naming convention

The instructions require: repo named `Fall2026_EECS348_Assignment_2` (✓, this is correct) **and inside it**, a folder named `StudentID_Lastname_Assignment_2` containing your submission. Right now `ANALYSIS.md`, `source/`, `resource/`, `rubric_prompt/`, and `llm_review/` sit directly at the repo root — there's no `StudentID_Lastname_Assignment_2` folder wrapping them. The instructions call this out explicitly: *"Failure to use the proper naming convention may result in a zero for the assignment."* Worth a folder rename/restructure before you push your final commit, independent of everything else in this document.

Also, per the instructions the repo needs a **PDF of your GenAI analysis** as a top-level deliverable. I didn't find one anywhere in the repo (only `ANALYSIS.md` and the course's own `rubric_prompt/Rubric 2.pdf`, which is not yours). `ANALYSIS.md`'s own closing line says "this is written in both MD and PDF format" — make sure that PDF actually gets generated and committed, since right now it isn't there.

## 7. Smaller / lower priority

- `git status` shows `resource/`, `rubric_prompt/`, `llm_review/`, and `source/` are all currently **untracked** — none of this is committed yet. Make sure everything you want graded actually gets `git add`ed and pushed; a public GitHub URL only helps if the files are actually in the repo.
- Stray files from ADVICE_1 are still present: `.DS_Store` (repo root, twice — once in the parent folder too), `resource/old_1.md`, `resource/test.py`, `source/a.out`. None of these belong in a graded submission; prune them or add them to `.gitignore`.
- Minor doc/code mismatch: `human.c`'s header comment (inherited from Sonnet, lines 28-33) still says commands are read "from a script file (given as argv[1]) or stdin," but `main` (line 662-672) now hard-requires exactly `argc == 2` and errors out otherwise — stdin is no longer supported. Not a rubric issue, but worth a one-line comment update so the header doesn't describe behavior the code no longer has.
