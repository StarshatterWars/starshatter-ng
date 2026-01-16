# Starshatter completion
# for use with bash-completions

_starshatter()
{
	local cur prev words cword
	_init_completion || return
	COMPREPLY=($(compgen -W '-nosplash -window -test -fps -dump -lan -d3d -server -dbg -filesys' -- "$cur"))
} &&
	complete -F _starshatter {,./}Starshatter{,.exe}
