These are programs that can hurt your computer (sometimes in surprising ways).

jacked is a problem on at least OS X 10.7 due to the way the Terminal
application does 'Scrollback'.  It appears to keep track of all output
that occurs in the Terminal session in memory.  If this output gets
large, memory consumption will increase until the computer becomes
slow.  A few ways to avoid this:

-redirect large amounts of output to a file
-use "Command K" to clear the scrollback
-quit Terminal