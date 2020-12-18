# animation_editor

## TODO:
- main controller
	- display animations as vertical_list [name, total time, preview]
		- the preview, how will it resize as to not collide with the others?
			- it will calculate how to resize the highest frame of each animation to the static list height (run through the frame list, get the highest and calculate a factor.
				- and it will reload each time the list changes, of course.
	- rename animation
	- change animation id [the number]
		- check if it does not exist...
- preview controller
	- shows on top of the list
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

## credits

MiniKnightset by Master484 (http://m484games.ucoz.com/)
Bebas Neue by Ryoichi Tsunekawa [Dharma Type] info@dharmatype.com
