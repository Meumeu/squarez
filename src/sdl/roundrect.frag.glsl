#version 120

varying vec2 var_cell_coords;

uniform vec4 color;
uniform vec4 border_color;
uniform float border_size;
uniform float border_radius;

void main(void)
{
	float tmp = length(max(abs(var_cell_coords) + border_radius - 1.0, 0.0));
	
	if (tmp < border_radius - border_size)
		gl_FragColor = color;
	else if (tmp < border_radius)
		gl_FragColor = border_color;
	else
		gl_FragColor = vec4(0.0, 0.0, 0.0, 0.0);
}
