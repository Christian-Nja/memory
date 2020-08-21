# Memory

## DESCRIPTION

Memory is a program to memorize unix commands. Given a command you can type some example and a comment about the usage.
Once you have a personal command database you can query it to show commands, add a new one or delete unuseful.

Example:

`$ memory show`

```
[*] displaying all commands
cmake
dpkg
git
mount
umount
vscode
```

`$ memory show cmake`

```

 cmake

27    cmake -S <path-to-source> -B <path-to-build>      prepare building files
28    cmake --build <dir-to-build>                      build the program

```

Nothing can stop you to name a command _todo_ and hack the program to memorize notes.

## USAGE

`$ memory db` Initialize the db

`$ memory new` Asks for a command to insert, an example and a comment

`$ memory new <command>` Asks for an example and a comment to type

`$ memory show` Lists all the available commands

`$ memory show <command>` Lists all the examples for the command

`$ memory show -k <keyword>` Lists all the commands and examples matching the query in the example or in the comment

## INSTALLATION

You'll need to work a bit to install the program as it is in a raw version yet.

1. Clone the project: `$ git clone <repo>`
2. `$ cd <cloned_repo>`
3. Prepare building files `$ cmake -S . -B <path-to-build>`
4. cd to building directory and make `$ cd <path-to-build> && cmake --build .`

You will find the executable inside the folder:
run `$ ./memory --help`

Alternatively you can create a debian package running inside the build folder
`$ cpack`
and installing it with `$ dpkg -i <memory_deb_package>`

** ATTENTION **
You still need to manually change the path to command db.
You will find it inside the db.c file at line 12.

```
/*** PATH TO DB ***/
const char *DB_PATH = "/home/<username>/.memory/commands.db";
```

You need to manually create .memory folder.

## CONTRIBUTION

If you like the tool feel free to fork and pr
Any help will be appreciated !
