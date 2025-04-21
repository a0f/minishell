MAKES SENSE
```
minishell> echo eChoe | echo a
created pipe, 5 6
RUNNING
program: echo
input fd: 7
output fd: 6
args: [echo, eChoe, ]
-- pipe into --
program: echo
input fd: 5
output fd: 8
args: [echo, a, ]

->

a
````

DOESNT MAKE SENSE
```
minishell> echo eChoe | echo a < ./minishell_tester/test_files/infile
created pipe, 5 6
RUNNING
program: echo
input fd: 7
output fd: 8
args: [echo, eChoe, ]
-- pipe into --
program: echo
input fd: 10
output fd: 9
args: [echo, a, ]
in_files: [file ./minishell_tester/test_files/infile, ]

Echoing to 8
eChoe

->

eChoe
a
````