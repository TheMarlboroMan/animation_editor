# animation_editor

## TODO:
- solve
	- animation line
		- double duration
		- double start time
		- tile frame id
	- animation
		- vector of [id + animation]
	- animation loader
		- roll your own, do not use animation_table
		- structure goes *title (starts new) -> !id -> duration[TAB]frame_id
	- animation saver
		- roll your own, do not use animation_table
		- structure goes *title (starts new) -> !id -> duration[TAB]frame_id

- main controller
	- display hud
		- X / total animations [total frames in sheet]
	- load file
	- save file
	- save file as
	- display animations as vertical_list [name, total time, preview]
	- move between animations with arrows
	- move up animation
	- move down animation
	- insert empty animation at current point with default name
	- delete animation
	- rename animation
	- change animation id [the number]
		- check if it does not exist...
- file picker controller
	- same as always
- animation controller
	- display hud
		- x / total frames in animation [total frames in sheet]
	- display frames as vertical list [preview, duration]
	- move between frames with arrows
	- move up frame
	- move down frame
	- insert empty frame
	- delete frame
	- change frame -> opens new controller
	- make frame longer
	- make frame shorter
- frame selection controller
	- show all frames as grid list of fixed size
	- pick one
- rename animation controller
- change animation id controller

## use

animation_editor -i path_to_image -s path_to_sheet [-f filename]

## build
