I faced many challenges when making this program, particularly because I had never used C before this. However, I believe that I was able to fulfill all the requirements quite well. For example, the user input and all of its edge cases work properly. I take in a value from 1-6, then subtract it to put it into index form. The AI works properly as well. It chases the highest value points and never collides with the player. The game itself runs out of a recursive function called turn(). 

Upon opening my code file, you'll find I used many functions. This kept everything segmented so I wasn't confused when I was coding. Some of these functions are printScore(), isValidMove(), and printBoard(). 

In another attempt to minimize confusion, I obsessively commented everything into the code file. In some functions (like turn or isValidMove), there are full lists of requirements for the function or comments on each if statement comparison. I originally implemented this project using structs but on the last day I found out I had to change it all back to ints... that was kind of a nightmare but it worked out in the end. 

I also had a segmentation fault error that I just couldn't get rid of and nearly gave up. I managed to fix it by running gdb on the file and locating the source of the error (line 265, if you're curious-- I even named the fix variable 'seg'). The error happened because I was attempting to get an undefined index of an array.

-M