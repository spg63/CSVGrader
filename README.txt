Author: Sean Grimes
Contact: spg63@cs.drexel.edu

License: No license, use it however you want. It'd be nice if you left my name on the files I wrote though. 

Warranty: No. 

Changes & Bug reports: Pull requests on git, or send me an email.

Bugs: None. -_-. I'm sure there is a bug or 2...I didn't find any issues when comparing results manually for my lab section but that's a fairly small sample area. 

Tested: OS X and Tux. Requires a compiler that supports C++11. 

*********************************************************************

Build: make

clean: make clean 
  - Removes CSVGrader and results.csv

To Run:
  Run: ./CSVGrader codelabs.csv problem_list.csv bblearn.csv -f
  Alternate Run: ./CSVGrader codelabs.csv problem_list.csv user_list.{csv, txt}

Providing a user_list file instead of the bblearn file will simply search for those users in the codelabs.csv and report the results. 

A more simple way of using it is to provide the bblearn.csv download, no need to put together a user list. The program will search for users in the codelabs.csv based on first and last name since some users are registered with multiple email addresses (user IDs). As this was put together rather quickly I didn't take the time to go through and remove the reported results for the unused email addresses, so users that are registered multiple times will have multiple results returned.

The results are written to a csv file "results.csv" after the searching is completed. The program should take 1-2 seconds to run against a user list, and an additional 1-2 seconds if it has to search for users from the bblearn file. Results are returned for all users in the bblearn file. No tabulating of data is returned, however the results.csv file is quite easy to read.

Creating a problem list
Put the first problem in the first cell of the first row, put subsequent problems in the rows below the first. Don't put anything outside of the first column. Should work for .csv or .txt files, only .csv was tested. 

*********************************************************************

Please note: these instructions have been taken from:
Amndeep Singh Mann: https://github.com/Amndeep7/DrexelCSDepartmentTuringscraftAnalysisTool

How to get BBLearn roster
Go to the class page
Go to the Grade Center
Hover over the "Work Offline" menu and click "Download"
Make sure the delimiter type is "Comma"
Fill out other options as desired
Click "Submit"
Click "Download"
Save the file where you want it
How to get Turingscraft roster

Go to the course page
Click on "Status"
Hover over the menu button in the new window that appears and click "Mail Roster"
Fill out the form as desired
Click submit
Acquire file however you desired it and save it where you want


