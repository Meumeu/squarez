changecom()dnl
define(`m4_shape', `<circle id="shape" r="32" cx="45" cy="45"/>')dnl
define(`m4_front_attr', 
	ifdef(`selected', 
		`fill="#CC0000"',dnl hsl(0, 100%, 40%)
		`fill="#FF0000"'))dnl hsl(0, 100%, 50%)
define(`m4_shadow_attr',
	ifdef(`selected', 
		`fill="#990000"',dnl hsl(0, 100%, 30%)
		`fill="#CC0000"'))dnl hsl(0, 100%, 40%)
define(`m4_back_attr',
	ifdef(`selected', 
		`fill="#B20000"',dnl hsl(0, 100%, 35%)
		`fill="#E50000"'))dnl hsl(0, 100%, 45%)
define(`m4_border_attr', `fill="#E5B2B2"')dnl hsl(0, 50, 80)
define(`m4_contour_attr',
	ifdef(`selected', 
		`fill="none" stroke="#660000" stroke-width="2"',dnl hsl(0, 100%, 20%)
		`fill="none" stroke="#990000" stroke-width="2"'))dnl hsl(0, 100%, 30%)
