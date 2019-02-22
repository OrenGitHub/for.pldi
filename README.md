# Getting started

1. Get the docker image [here](https://drive.google.com/open?id=1dJmyqz5CdfQ3RK9tHvCK-xCaKYKncMoq)
1. Install docker, for debian the install instructions are [here](https://docs.docker.com/install/linux/docker-ce/debian/)
2. Load the docker image: `docker load --input paper288_artefact.tgz`
3. Run the docker image: `docker run --rm -it paper288 bash`


The docker image loads you into `/artefact/loops` directory where most of the
relevant code is present. The folder has many subcomponents and we will walk
through them in this README.


To quickly evaluate the main points of the paper go to the Synthesis heading of
the detailed description. 


# Detailed description


## Extracting loops

This section focuses on how to extract loops from the programs. The section is
skippable as the extracted loops are already present in the directory.

The repository contains LLVM 6.0 bitcode binaries (`*.bc` files) of the programs
we used for our evaluation.  We provide the binaries for convenience, interested
parties can compile the appropriate versions of the programs to obtain them.

The automatic part of the extraction process starts with the
`LoopFinder/LoopFind/findLoops.sh` script, that is a wrapper to the LLVM pass
that analyses the code and outputs raw data. The script can be invoked as:

```
LoopFinder/LoopFind/findLoops.sh make.bc
```
which outputs
```
...
Function: hash_delete_items
hash.c:197:3 call  A write loop!
Function: hash_free
Function: hash_map
hash.c:227:3 loop with a pointer call
...
```

Which tells us function `hash_delete_items` has a loop in file `hash.c` at like
197 that is disqualified due to being a write loop. Similarly function
`hash_map` has a loop at line 227 that was disqualified due to having a function
call with pointers.


To run the script for all the programs we provide a `make` target. Running `make
all` should create a `.loopinfo` file containing the raw loop information for
each of the programs. `./summarizeLoopInfo.sh` can then be used to summarize the
data into the one presented in Table 2:

```
make all
ls *.loopinfo | xargs -L1 ./summarizeLoopInfo.sh
```

Which should output:

```
bash   1085   944   506   174  45
diff   186   140   80   20  14
gawk   608   502   292   105  17
...
```

The second column is the total number of loops, the third one is the number of
loops without an inner loop, the fourth is the number of loops with calls to
functions with pointers. Column 5 is the number of loops with writes, the final
column is the number of loops we will consider in the manual filtering phase.
Note the semantics of the columns of this script are slightly different from
Table 2, but Table 2 can be obtained by simple arithmetic.

### Manual phase

Loops where then extracted manually and all the extracted loops were put in
the `FoundLoops.c` file. The file is structured as a sequence of comments which
indicate the loop location and the code snippet of the loop.

For the first loop in `FoundLoops.c` we would first `grep`:

```
grep BINGO m4.loopinfo
```

giving:

```
builtin.c:1458:7 call  BINGO!  loop reads from 1 pointers
builtin.c:1412:3 call  BINGO!  loop reads from 1 pointers
...
```

We would then go to `m4` source code to `builtin.c` file and etract the first
loop of `FoundLoops.c`:

```
//m4-1.4.18/src/builtin.c:1458:7

int len = strlen (str);
for (i = len; i > 1; i--)
    if (str[i - 1] != 'X')
       break;


//m4-1.4.18/src/builtin.c:1412:3
...
```

The loops are then given a driver and separated into files with the `extract-loops.py` script:

```
cat FoundLoops.c | ./extract-loops.py
```

This creates a `loopFiles` directory with all the loops there. The file names
follow the convention `<utility-name>_<source file of the loop>_<source line of
the loop>.c`. The files in the `loopFiles` directory where then split into
loops that match our criteria and put into `gdLoops` file and into other loops
put into one of the  `badLoops` subfolders, coresponding to the reason they
were filetered out.


```
cd badLoops 
./summarize.sh
```

Which gives the data presented in section 4.1.2.

## Synthesis

The 115 loops we evaluate our synthesis on are found in the `gdLoops/` folder.
In addition `gdLoops/` folder contains a Makefile, which is the entry point for
the synthesis experiments. We assume `gdLoops/` is your working directory for
this section.

To synthesize a program invoke make with `*.prog` target. It links the loop with
the code in `synthesize/` direcotry and runs it with KLEE. When KLEE terminates,
it outputs the synthesized program. The synthesized program is then extracted
from KLEE-s output into the .prog file. We recomend using `hexdump -C` to view
the synthesized programs. For example to synthesize and view the loop from git,
file commit.c at line 628, run:

```
make git_commit_628.prog
hexdump -C git_commit_628.prog
```

There are 4 variables you can tweak through make to influence the synthesis: The
length of the synthesized program, number of characters to show eqvivalnce with,
timeout and the gadgets to synthesize with. For example:

```
make -B TIME=10 PROGRAM_SIZE=5 EXAMPLE_SIZE=3 GADGETS='X P F' git_commit_628.prog 
```

Will synthesize a program for the same loop, with a synthesized program size of
5, proving eqvivalnce up to 3 characters, using only 3 gatdets (X P F) and with
timeout of 10 seconds.


To run the synthesis over all loops use:

```
make synth
```

Note that it might take a while, depending on the timeout you use. The variables
described above still apply. To count the number of successfully synthesized
loops you can use:

```
ls *.prog | wc -l
```

We provide some convenience scripts. `./genResults.sh` creates results.html that
does best effort to show a highlighted snippet of the loop as well as the hexdump
of the synthesized program below. `./printTodo.sh` Outputs all the loops that
weren't synthesized.


### Cross-check original and synthesized loop

The original loop and the synthesized program can then be cross-checked using 
the `*.verify` target of the makefile. This can serve as a sanity check that
synthesis ran correctly. You can also change the bound up to which to check to
with `EXAMPLE_SIZE`, this can be larger in this case than in synthesis as it is
a cheaper run.

```
make EXAMPLE_SIZE=5 git_commit_628.verify
```


If KLEE doesn't terminate due to assertion failure, the run showed equivalence up
to the bound. 

`make ver` can be used to perform this crosscheck for all the programs
synthesized in the `gdLoops` directory.

### Evaluate loop summaries for symbolic execution (Section 4.3)

Here we run the loop summaries with two version of KLEE. One is the current
master version (`klee` in the docker image), which gathers the constraints in
theory of bitvectors and arrays and has not special use for loop summaries. That
is it executes the loop as is.  The second version is our modification of KLEE,
that gathers the constraints in theory of strings. Therefore it makes use of
loop summaries in that it doesn't execute the loop of the string function but
directly translates them into constraints. The binary of this version of KLEE is
located at `/artefact/str.KLEE/bin/klee-str`.


To perform the comparison use the `*.compare` target of the makefile. For our
rolling example:

```
make git_commit_628.compare
cat git_commit_628.compare
```

which outputs:

```
git_commit_628.nostr 0.12
git_commit_628.str 0.03
```

Meaning the vanilla version terminated in 0.12s whereas the string version
terminated in 0.03s. `EXAMPLE_SIZE` can again be used to change the lengths of
the strings to do the comparison on.

`make comp` can be used to perform the comparison on all the synthesized
programs in the `gdLoops` directory. This can be used to get the date for Figure 4 .
We used `cat *.compare` to get all the data, the pasted it into google sheets
to pivot and plot it.

The data for Figure 3, can be obtained by running the `./runCompareExp.sh`
script. It calls the `make comp` with different values for EXAMPLE_SIZE and puts
them in `compare-[number].cexp` files. These can then be again put in a
spreadsheet, pivoted and plotted. Note that `klee-str` fails on small fraction
of the loops, due to implementation bugs. We ignore those.


### Evaluate loop summaries for optimization (Section 4.4)

The `*.native.run` make target provides the facility to compare the summarized
loop with the original implementation. For example:

```
make git_commit_628.native.run
cat git_commit_628.native.run
```

Outputs:
```
Loop function took 0.421490 seconds
Native took 0.205331 seconds
Speedup 2.052734
summary 0.421490 0.205331 2.052734
```

Which means, the original function took 0.42s, the synthesized program took
0.20, with speedup of 2x. `make native_comp` can be used to run this for all the
synthesized programs. The plots can be obtained by copying the concatenated data
into a spreadsheet.


### Evaluate loop summaries for refactoring (Section 4.5)

To evaluate the refactoring potential the synthesized programs can be inspected
as described previously. We provide links to the patches:

 - [libosip](http://git.savannah.gnu.org/cgit/osip.git/commit/?id=8e788b73bc7c4d10e2cc679edd3519d2b762475a)
 - [wget](http://git.savannah.gnu.org/cgit/wget.git/commit/?id=6d7cd9313c692b4d2736e94a6fec3290f1dbde06)
 - [patch](http://lists.gnu.org/archive/html/bug-patch/2018-10/msg00002.html)


## Iterative deepening

To perform iterative deepening first synthesize some programs in the `gdLoops/`
directory as described above (ie. with `make synth`). Then go to
`iterativeDeepening/` directory. 

At a high level iterative deepening works, by copying the `gdLoops` directory
and then performing synthesis there. The `iterativeDeepining/Makefile` performs
this automatically with the `make do` target. This will run the iterative
deepening with 1 hour timeout and create `1h.exp` directory. The `1h.exp`
directory has `1.dir 2.dir 3.dir ...` subfolders, corresponding to the maximum
program size the synthesis was performed in that subfolder.

After running `make do`, `make 1h.dat` can be run the extract the number of
synthesized loops per maximum program size. To get the data for the plot in
Figure 2, `runAll.sh` script can be used, followed by `make 30s.dat 3min.dat
10min.dat 1h.dat`, to get the data files.


## Optimizing the vocabulary

Optimizing the vocabulary is performed in the `gadgets/` directory by running
`./optimize.py` script. It performs the gaussian optimization, creating
direcotries of the form `exp-[vocabulary used]-[timeout]-[program size]`. 

For example it could create `exp-PBXE-300s-7` directory, which is a copy of the
`gdLoops` folder, where synthesis was performed using only the P B X E eleemnts
of the vocabulary with 300s timeout and program size of 7.

The `printResults.sh` script prints the name of each of this folder alongside
the number of synthesized programs in that folder, giving the data for Table 4.
Note that there is some randomness in the gaussian optimization so your results
might not be exaxtly the same.


## Running the full set of experiments

So far we presented the individual components of our experiments and how you can
play around with them. Running `cd FullExp && make` will run all the experiments
we show in the paper with the configuration we used.

It will create copies of the above folders and run the experiments there, which
you can inspect with the methods presented above.

# Verification of Memoryless loops

- This step takes the 115 loops and classifies those which are memoryless.
- It does so by checking the loop's increments, comparisons, read values, phi values etc.
- Every C loop is compiled with `clang-3.8.0` to produce a (non optimized) bitcode.
- The resulting bitcode is instrumented and then fed to KLEE's symbolic execution engine.
  The instrumentation contains code the turns a status flag on whenever a loop is *not* memoryless.
  KLEE then checks this (symbolic) status flag, if there exists an execution path which turns it on.
  If KLEE finds such an execution path, it will fire an assert.

