# MULTIFX
Audio FX C Framework using Linux OSS ( Codeblocks project)

This simple project aims to develop audio algorithms and test them with a real audio source.
The application control is not GUI based but uses a simple shell menu using ncurses lib.
The algorithm interface is structured as BufferIn->DSP->BufferOut where DSP function is a function pointer
that can be substituted by any custom implementation.

The audio interface is based on old Linux Open Sound System.

