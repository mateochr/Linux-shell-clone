# Linux-shell-like
A very simple Linux shell-like program that executes basic commands such as:

* ls
* ls -l
* sort –u < in.txt > out.txt (read from in.txt, sort and then write result in out.txt)
* ls -l /home/user/Downloads | sort –u > listing.txt (single piping)
* ls -l /home/user/Downloads | sort –u | wc –l > count.txt (multiple piping)

Properties:

* Ends with EOF(Ctrl+D).
* Maximum number of input characters is 255.
* Using `fork()` to operate different processes.
* No zombie processes.
* Running only external commands that aren't built into the Linux shell.


## HowTo

Open a cmd in the folder that contains the files and type the commands below.

`gcc mysh.c -o mysh`

`mysh`
