# minishell

Codam [42 Network] project: the objective of this project is to create a simple shell
<br/><br/>

## Instructions
- run `make` to compile
- `./minishell` to launch program
<br/><br/>

## Project specifications
- show a prompt when waiting for a new command
- search and launch the right executable (based on the PATH variable or by using relative or absolute path) like in bash
- implement the builtins like in bash:
    - echo with option ’-n’
    - cd with only a relative or absolute path
    - pwd without any options
    - export without any options
    - unset without any options
    - env without any options and any arguments
    - exit without any options
- ; in the command should separate commands like in bash
- ’ and " should work like in bash except for multiline commands
- redirections < > “>>” should work like in bash except for file descriptor aggregation
- pipes | should work like in bash
- environment variables ($ followed by characters) should work like in bash
- $? should work like in bash
- ctrl-C, ctrl-D and ctrl-\ should have the same result as in bash
<br/><br/>

## Skills
- imperative programming
- Unix
<br/><br/>

## Objectives
- Unix logic  
<br/><br/>
