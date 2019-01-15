## Homework Notes

## General Notes
Read the assignment carefully, including what files to include.
Don't assume limitations unless they are explicitly stated.
Treat provided examples as just that, not exhaustive list of cases that should work.
When in doubt regarding what needs to be done, ask. Another option is test it in the real
UNIX operating system. Does it behave the same way?
TEST your solutions, make sure they work. It's obvious when you didn't test the code.

## Rubric
Since we had some issues before on homework 1. Here are some of the things we know we will
test, but these are not the only things we will test. Therefore make sure to test your program
thoroughly and thoughtfully.
Total: 100 points
-10: Can't specify number of lines when input is from a pipe
-10: No exit() at the end of hello.c
-10: Does not handle long lines (more than 512 characters)
-20: head does not allow specifying nu
$ make qemu
QEMU should appear and show the xv6 command prompt, where you can run programs inside
xv6. It will look something like:
You can play around with running commands such as ls, cat, etc. by typing them into the
QEMU window; for example, this is what it looks like when you run ls in xv6:
## Part 1: Hello World (20 points)
Write a program for xv6 that, when run, prints "Hello world" to the xv6 console. This can be
broken up into a few steps:
1. Create a file in the xv6 directory named hello.c
2. Put code you need to implement printing "Hello world" into hello.c
3. Edit the file Makefile, find the section UPROGS (which contains a list of programs to be
built), and add a line to tell it to build your Hello World program. When you're done that
portion of the Makefile should look like:
UPROGS=\
_cat\
_echo\
_forktest\
_grep\
_init\
_kill\
_ln\
_ls\
_mkdir\
_rm\
_sh\
_stressfs\
_usertests\
_wc\
_zombie\
_hello\
4. Run make to build xv6, including your new program (repeating steps 2 and 4 until you
have compiling code)
5. Run make qemu to launch xv6, and then type hello in the QEMU window. You should
see "Hello world" be printed out.
Of course step 2 is where the bulk of the work lies. You will find that many things are subtly
different from the programming environments you've used before; for example, the printf
function takes an extra argument that specifies where it should print to. This is because you're
writing programs for a new operating system, and it doesn't have to follow the conventions of
anything you've used before. To get a feel for how programs look in xv6, and how various APIs
should be called, you can look at the source code for other utilities: echo.c, cat.c, wc.c, ls.c.
## Hints:
1. In places where something asks for a file descriptor, you can use either an actual file
descriptor (i.e., the return value of the open function), or one of the standard I/O
descriptors: 0 is "standard input", 1 is "standard output", and 2 is "standard error".
Writing to either 1 or 2 will result in something being printed to the screen.
2. The standard header files used by xv6 programs are "types.h" (to define some standard
data types) and "user.h" (to declare some common functions). You can look at these
files to see what code they contain and what functions they define.
A brief digression on IDEs and text editors
I do not have strong preferences as to how you create source code. I personally prefer to use a
traditional text editor that can be run at the command line such as pico. Although vim and emacs
are great as well and there are plenty of alternatives out there. On macOS, some may prefer to
use Xcode, others may prefer to use something like TextMate or Sublime Text. In the Linux VM
I have provided, pico works fine. As long as you get a plain text file out of it with valid C
syntax, you can choose whatever you like.
How you compile the code is another matter. The xv6 OS is set up to be built using make, which
uses the rules defined in Makefile to compile the various pieces of xv6, and to allow you to run
the code. The simplest way to build and run it is to use this system. Trying to coerce an IDE such
as Xcode into building xv6 is far more trouble than it's worth.
## Part 2: Implementing the ‘head’ command (50 points)
Write a program that prints the first 10 lines of its input. If a filename is provided on the
command line (i.e., head FILE) then head should open it, read and print the first 10 lines, and
then close it. If no filename is provided, head should read from standard input.
$ head README
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's
Unix
Version 6 (v6). xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI
C.
## ACKNOWLEDGMENTS
xv6 is inspired by John Lions's Commentary on UNIX 6th Edition (Peer
to Peer Communications; ISBN: 1-57398-013-7; 1st edition (June 14,
2000)). See also http://pdos.csail.mit.edu/6.828/2014/xv6.html, which
provides pointers to on-line resources for v6.
You should also be able to invoke it without a file, and have it read from standard input. For
example, you can use a pipe to direct the output of another xv6 command into head :
$ grep the README | head
Version 6 (v6). xv6 loosely follows the structure and style of v6,
xv6 borrows code from the following sources:
JOS (asm.h, elf.h, mmu.h, bootasm.S, ide.c, console.c, and
others)
Plan 9 (entryother.S, mp.h, mp.c, lapic.c)
In addition, we are grateful for the bug reports and patches
contributed by
The code in the files that constitute xv6 is
To run xv6, install the QEMU PC simulators. To run in QEMU, run
"make qemu".
To create a typeset version of the code, run "make xv6.pdf". This
requires the "mpage" utility. See http://www.mesa.nl/pub/mpage/.
The above command searches for all instances of the word the in the file README , and then
prints the first 10 matching lines.

## Hints:
1. Many aspects of this are similar to the wc program: both can read from standard input if
no arguments are passed or read from a file if one is given on the command line. Reading
its code will help you if you get stuck.
Part 3: Extending ‘head’ (30 points)
The traditional UNIX head utility can print out a configurable number of lines from the start of a
file. Implement this behavior in your version of head . The number of lines to be printed should
be specified via a command line argument as head -NUM FILE, for example head -3 README to
print the first 3 lines of the file README. The expected output of that command is:
$ head -3 README
xv6 is a re-implementation of Dennis Ritchie's and Ken Thompson's
Unix
Version 6 (v6). xv6 loosely follows the structure and style of v6,
but is implemented for a modern x86-based multiprocessor using ANSI
C.
If the number of lines is not given (i.e., if the first argument does not start with - ), the number of
lines to be printed should default to 10 as in the previous part.

## Hints:
1. You can convert a string to an integer with the atoi function.
2. You may want to use pointer arithmetic (discussed in class in Lecture 2) to get a string
suitable for passing to atoi.
