#version 120

attribute vec2 coords;
attribute vec2 cell_coords;

invariant gl_Position;
varying vec2 var_cell_coords;

void main(void)
{
	gl_Position = vec4(coords, 0.0, 1.0);
	
	var_cell_coords = cell_coords;
}
