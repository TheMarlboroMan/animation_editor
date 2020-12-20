# animation_editor

## TODO:
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
- preview controller
	- shows on top of the list, shows the animation at its real size

## BUGS

- screen clear with rgba does not respect alpha.
- exiting on rename / change id fully resets the list, should keep the page
	and current selection.

## IDEAS

- The animation preview is a static 32x32... could be adjusted.

## use

animation_editor -i path_to_image -s path_to_sheet [-f filename]

## build

## credits

MiniKnightset by Master484 (http://m484games.ucoz.com/)
Bebas Neue by Ryoichi Tsunekawa [Dharma Type] info@dharmatype.com
