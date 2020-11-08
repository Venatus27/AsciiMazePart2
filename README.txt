GitHub link (may be private): https://github.com/Venatus27/AsciiMazePart2

Known issues:
	> There's a memory leak issue with the A* implementation I've managed to track down to getAdjacent squares, getLowestFValue and possibly it calling itself multiple times, 
	  while I've attempted to fix this no attempt has been successful and I unfortunately ran out of time. Most program functions work fine so long as many many things arent done in one session.
	  Getting Average player moves however will cause issues if run on too high a grid and/or too many players. 
	  I've attempted to manually free what i thought was culprit memory and also turn it into a namespace which just threw errors. (any feedback on how this could have been fixed would be appreciated for future reference)

The UI starts with only 4 options since no map is loaded, after the first non exit option is picked a map is loaded and more options open up.

There are 2 text files submitted with this:

mapFile.txt is the file used with default file calls (option 2 and 6) in the secondary option menu, it currently contains a simple maze to solve.
PartSolvable.txt has a partially solvable simple maze in it. 1 player can finish while the other 2 are stuck behind walls.

The 100 random mazes requirement in the spec returns the average moves a player makes in the given configuration.



