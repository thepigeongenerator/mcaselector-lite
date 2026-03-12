.. Copyright (C)2026 mcxedit
.. Licensed under GPL-2.0-only. For further information,
.. view `git log`, and the COPYING and CONTRIBUTORS files
.. at www.github.com/thepigeongenerator/mcxedit.

.. rst2man shows "Name", rather than "NAME", annoying, but can live with it.

=======
MCXEDIT
=======
----------------------------------------------------------------
A utility for processing *Minecraft: Java Edition* Region files.
----------------------------------------------------------------
:Version: mcxedit @@VERSION@@
:Date: @@DATE@@
:Manual section: 1
:Manual group: User Commands

SYNOPSIS
========
**mcxedit** [*OPTIONS*] *FILE*...

DESCRIPTION
===========
Mcxedit is a tool used to edit region files for your
Minecraft: Java Edition worlds. This is useful for reducing world size,
general world management, copying chunks from one world to another,
or various other administrative tasks.

OPTIONS
=======
Options are read as "flags", thus the order does not matter
(it will generally perform actions in the order given here.)
The order in which the flags are given is only relevant if it affects
whether another option is set, or if stated otherwise.

By default, with no flags given, the programme performs some high-level
file size based checks on the file, for more thorough checking
**-c** can be specified.

-c
Performs the same checks as **-R**, but instead just reports these errors instead
of repairing them.

-R:
Attempts to repair the file to the best of its ability.
Invalid chunk entries in the table will be zeroed out, leaving the previously
allocated sectors unused, you may wish to combine with **-G**
to delete these as well. If data is present, an attempt will be made to recover
whatever is there, though Minecraft itself may be better at this job.

-G
Performs defragmentation upon the file, updating the table & deleting unused
sectors.
Fails if the file's size does not match the expected size, indicating corruption.
You may wish to combine with **-R**, if this happens.

-q:
Silences non-fatal errors.
Disables **-v**.

-v:
TODO: Scope not defined.
Disables **-q**.

-V:
Outputs version information & exits.

-h:
Shows a brief explanation on the options available for the programme.
Adding potentially useful mnemonics.

EXIT STATUS
===========
0 on success. 1 if any of *FILE*... had an issue, in which case
an error is (usually) printed to STDERR.

EXAMPLES
========
TODO: W.I.P.

SEE ALSO
========

* https://minecraft.wiki/w/Java_Edition
* https://minecraft.wiki/w/Region_file_format
* https://minecraft.wiki/w/Anvil_file_format
* https://minecraft.wiki/w/Chunk
