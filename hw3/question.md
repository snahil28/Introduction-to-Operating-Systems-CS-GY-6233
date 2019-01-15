## Homework Notes

## General Notes
Read the assignment carefully, including what files to include.
Don't assume limitations unless they are explicitly stated.
Treat provided examples as just that, not exhaustive list of cases that should work.
When in doubt regarding what needs to be done, ask. Another option is test it in the real
UNIX operating system. Does it behaves the same way?
TEST your solutions, make sure they work. It's obvious when you didn't test the code.
Due Date: April 9

## Preliminaries
In this assignment, we will go back to using xv6.
Start by getting a modified version of the xv6 source code from github.
$ git clone https://github.com/gussand/xv6-public.git
Cloning into 'xv6-public'...
remote: Counting objects: 4484, done.
remote: Compressing objects: 100% (36/36), done.
remote: Total 4484 (delta 11), reused 0 (delta 0), pack-reused 4448
Receiving objects: 100% (4484/4484), 11.69 MiB | 3.79 MiB/s, done.
Resolving deltas: 100% (1779/1779), done.
Checking connectivity... done.
Switch to the homework branch:
$ cd xv6-public
$ git checkout hw4
Branch hw4 set up to track remote branch hw4 from origin.
Switched to a new branch 'hw4'
The modified version has some new features:
1. It contains a random number generator. You can use it by including rand.h in a source
file and then calling the function
long random_at_most(long max)
to get a random number between 0 and max, inclusive.
2. It contains a new system call
int gettime(struct rtcdate *date)
which retrieves the current date and time from the CMOS RTC and stores it in an
rtcdate structure (you can see how struct rtcdate is defined by looking at
date.h).
3. It contains a program designed to test the lottery scheduler you will develop, in the source
file lotterytest.c.

## Important Note
For simplicity in testing, you should run the tests for this assignment in QEMU with just one
CPU. You can do this by invoking the Makefile as:
make qemu CPUS=1
Problem : Lottery Scheduling
For this assignment, you will implement and test lottery scheduling, a randomized algorithm
(discussed in class and in the reading) that allows processes to receive a proportional share of the
CPU without explicitly tracking how long each process has been run.
Specifically, you should modify xv6 so that:
1. Each struct proc has an additional field, tickets, that tracks how many tickets it
has.
2. New processes are assigned 10 lottery tickets when they are created.
3. When the scheduler runs, it picks a random number between 0 and the total number of
tickets. It then uses the algorithm described in class to loop over runnable processes and
pick the one with the winning ticket.
4. User processes have a new system call, settickets, that allows a process to specify
how many lottery tickets it wants. Normally this would be a bad idea, since it would let a
process hog the CPU by specifying an arbitrary number of tickets -- but xv6 has no
security anyway, so this is not that big a deal.
In addition to Tannenbaum's description of lottery scheduling in Modern Operating Systems, you
may find it helpful to read over Operating Systems: Three Easy Pieces, Chapter 9: Scheduling:
Proportional Share.
Of course, once you have implemented this, you will want to test that the scheduler works. Aside
from basic tests that the system is still functioning, you should verify that allocating more tickets
to a process does increase its share of the CPU allotted by the scheduler.
Included in the source distribution is a new program, lotterytest, which forks two children,
sets their priorities, and runs a CPU-intensive task in each, timing the results. The test assigns 80
tickets to one and 20 to the other, so the first should be scheduled 4 times as often as the second
(an 80% / 20% split).
To build lotterytest, just add it to the UPROGS list in the Makefile (taking care to use a
tab character for indentation rather than 4 spaces). Note that it will not compile until you have
added the settickets system call. The relevant portion of the Makefile is shown below:
.PRECIOUS: %.o
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
_hackbench\
fs.img: mkfs README $(UPROGS)
./mkfs fs.img README $(UPROGS)
Given two processes started at the same time, that do the same amount of work, we expect that
they will finish at the same time if they have the same number of tickets. We can predict how
long they should take when the CPU is divided between them at various ratios. Suppose that we
have two tasks T1
 and T2
, each of which takes 5 seconds to run. Regardless of the share of the
CPU each has, they will take 10 seconds total for both to finish. Running them with an equal
share of the CPU will cause them both to finish at the same time. We can calculate the expected
runtime of T1
 when given a certain fraction of the CPU as:
R(T1, 1.0) = 5 seconds
R(T1, 0.5) = 10 seconds
R(T1, x) = 1/x * R(T1, 1.0)
So from this we can compute that if T1
is given an 80% share of the CPU, it should finish in 6.25
seconds; T2 will still finish in 10 seconds (remember, the scheduler is just redistributing the work
to be done, so running the two tasks still takes 10 seconds no matter what).
By running lotterytest a few times, you should be able to verify that the process with 80
tickets finishes sooner than the process with only 20. For example, here's the output I get:
$ make qemu CPUS=1
qemu-system-i386 -nographic -hdb fs.img xv6.img -smp 1 -m 512
WARNING: Image format was not specified for 'fs.img' and probing
guessed raw.
Automatically detecting the format is dangerous for raw images, write
operations on block 0 will be restricted.
Specify the 'raw' format explicitly to remove the restrictions.
WARNING: Image format was not specified for 'xv6.img' and probing
guessed raw.
Automatically detecting the format is dangerous for raw images, write
operations on block 0 will be restricted.
Specify the 'raw' format explicitly to remove the restrictions.
xv6...
cpu0: starting sb: size 1000 nblocks 941 ninodes 200 nlog 30 logstart
2 inodestart 32 bmap start 58
init: starting sh
$ lotterytest
starting test at 17 hours 22 minutes 22 seconds
spin with 80 tickets ended at 17 hours 22 minutes 28 seconds
spin with 20 tickets ended at 17 hours 22 minutes 32 seconds
You can see that the process with 80 tickets finished after 6 seconds, while the process with 20
tickets finished after 10 seconds. You can also play with the numbers in lotterytest to see
the effect of varying the number of tickets.

## Hints:
1. One possible way to track the total number of tickets is by carefully finding each place
where the process state changes, and adding or subtracting tickets from a global as
appropriate. However, this is actually rather tricky to get right. An alternative is to simply
compute the number of tickets each time we enter the scheduler loop, before choosing the
winning ticket.
2. Right now, the scheduler loop picks up where it left off after scheduling a process -- so if
it finds a runnable process at index i, the next time the scheduler runs it will keep going
through the list at index i+1. In order to make the lottery algorithm work, you will need
to restructure the scheduler loop so that it starts back at the beginning of the list every
time we return to the scheduler.
3. No changes are needed to the process list itself; you can continue to use the
ptable.proc array (i.e., you don't have to implement a process queue).
4. You can consult the slides to find out how to add a new system call when adding
settickets .
5. The use of gdb to debug your code is highly encouraged.
6. If you want to see the list of running processes at any time, you can type Control-p at
the xv6 console and it will print out a list of running processes. You may also want to
modify this function (procdump in proc.c) to print out the number of tickets each
process has so that you can debug your settickets call.
Submitting
You will use git to create a patch that contains your changes. First, tell git who you are:
$ git config --global user.email "you@example.com"
$ git config --global user.name "Your Name"
Then, commit your changes:
$ git commit --all --message="Implement lottery scheduling"
[hw4 94b0cf7] Implement lottery scheduling
7 files changed, 60 insertions(+), 7 deletions(-)
(Note: if you added any new files, you will also have to use git add <filename> before you
run git commit.)
Finally, create a patch file. The command takes an argument that specifies what code we're
comparing against to make the patch; in this case I have created a git tag that refers to the
unmodified Homework 3 called hw3.unmodified, so you should run:
$git format-patch hw3.unmodified
0001-Implement-lottery-scheduling.patch
The command creates a file, 0001-Implement-lottery-scheduling.patch, containing
the changes you've made. Submit this file on NYU Classes, along with the usual partner.txt
listing your partner (if any).
Submission Note
Don't try to edit the patch file after creating it. Doing so will most likely corrupt the patch file
and make it impossible to apply. Instead, change the original file, commit your changes, and run
git format-patch again:
$ git commit --all --message="Description of your change here"
[hw4 7cc4977] Description of your change here
1 file changed, 1 insertion(+), 1 deletion(-)
$ git format-patch hw3.unmodified
0001-Implement-lottery-scheduling.patch
0002-Description-of-your-change-here.patch
