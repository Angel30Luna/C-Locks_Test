# C-Locks_Test
The presented source code is used to visualize and better understand how multi-threaded processes work and how locks and semaphores can help us avoid interlocks within a process. 

File Summary:
  main.c: This file is responsible for instantiating the threads of the process and making these threads do something, which in this case is simulating whether these threads are reading or writing to file.
  readerwritter.c: This file contains the definition for the locks that will be used by the threads of the process. This is achieved by making use of the semaphore library.
  scenarioGenerator: This CPP file can be used to generate a certain amount of arbitrary cases which define what each of the threads will be doing. For example, it could generate a line which looks like "rrwrwwrr", then the executable from the readerwritter.c would use this text file and assign 5 reading threads and 3 writing threads.
  makefile: Defines how to compile and link the object files. Use this to easily attain the executable by typing "make" on the terminal.
  
NOTE: THE RESULTING EXECUTABLE IS INTENDED TO RUN IN A UNIX/LINUX ENVIRONMENT ONLY
