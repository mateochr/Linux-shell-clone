# DIY-Linux-shell
A very simple Linux like shell that executes basic commands such as:

* ls
* ls -l
* sort –u < in.txt > out.txt (read from in.txt, sort and then write result in out.txt)
* ls -l /home/user/Downloads | sort –u > listing.txt (single piping)
* ls -l /home/user/Downloads | sort –u | wc –l > count.txt (multiple piping)



## HowTo

Open a cmd in the folder that contains the files and type the commands below.

`gcc mysh.c -o mysh`

`mysh`
