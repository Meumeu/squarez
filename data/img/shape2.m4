changecom()dnl
define(`m4_shape', `<rect id="shape" width="52" height="52" x="-26" y="-26" rx="4" ry="4" transform="translate(45,45) rotate(45)"/>')dnl
define(`m4_front_attr', 
	ifdef(`selected', 
		`fill="#0000CC"',dnl hsl(240, 100%, 40%)
		`fill="#0000FF"'))dnl hsl(240, 100%, 50%)
define(`m4_shadow_attr',
	ifdef(`selected', 
		`fill="#000099"',dnl hsl(240, 100%, 30%)
		`fill="#0000CC"'))dnl hsl(240, 100%, 40%)
define(`m4_back_attr',
	ifdef(`selected', 
		`fill="#0000B2"',dnl hsl(240, 100%, 35%)
		`fill="#0000E5"'))dnl hsl(240, 100%, 45%)
define(`m4_border_attr', `fill="#B2B2E5"')dnl hsl(240, 50, 80)
define(`m4_contour_attr',
	ifdef(`selected', 
		`fill="none" stroke="#000066" stroke-width="2"',dnl hsl(240, 100%, 20%)
		`fill="none" stroke="#000099" stroke-width="2"'))dnl hsl(240, 100%, 30%)
