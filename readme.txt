Dev notes:

After populating the name field (or any other one), we need a way to get back to the beginning of the file.



User notes:


This is a beta, so you'll have to run the program from terminal. If running on windows, type ".\optimizer <NAME_OF_THE_CSV_FILE>
Do NOT merge cells (like in general, not only for this optimizer purposes)
The names of the parameters are assumed to be a, b, c, cpm and universe. If you wanna name them differently, please pass them as arguments in that order.
The names must match your arguments to the actual names of the csv.
Download the file as a CSV in the sheet where the parameters are.
THe cells containing the parameter names must only contain the name and nothing else.
If another cell somewhere in the sheet has also a parameter name in it (for instance, two cells with the word "Universo" in them), the program will ingest by default the one located upper in the document.
You can move around the columns where the parameters's values are, but their relative position with their names must remain equal (just to be safe, keep it all tight in a single table as originally portraited)
