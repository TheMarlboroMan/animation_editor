# editor_animaciones
##v.0.1

This tool generates animation markup files from the output of "editor_sprites".

#Building

This tool depends on:

- libdansdl2 -> branch classic.
- herramientas_proyecto -> branch master-es

Any other branches and the code will not compile... In case these dependencies cannot be satisfied, I bundled them in the "dependencies" directory, where they can be relocated, configured and compiled.

#Use

Parameters are:

	./a.out img=path_to_image hoja=path_to_editor_sprites_out out=path_to_result [res=800x600]

Such as in:

	./a.out img=ejemplos/MiniKnightSet.png hoja=ejemplos/frames.dat out=ejemplos/result.dat

No in-application help. These are the commands:

	Cursor keys: Move through lists.
	Space: Access animation / select frame.
	Enter: Change animation name (will appear as comment in output file).
	Insert: New animation / frame.
	Tab: Switch animation / frame with the previous (basic reorder)
	Delete: Delete animation / frame.
	Escape: Back / exit
	s: Save (only in animation menu)

./a.out img=ejemplos/MiniKnightSet.png hoja=ejemplos/frames.dat out=ejemplos/result.dat

#History

- As of July 2017 the dependencies are bundled and this thing will not be updated much.

#Bugs

There are many bugs lying around, but the tool is as functional as I need it to be.

