changecom()dnl
define(`m4_shape', `<path id="shape" d="m -0.08928486,-31.961687 c 3.39534766,0 29.47182286,18.945669 30.52104286,22.1748366 1.04922,3.2291675 -8.911108,33.8839034 -11.658002,35.8796384 -2.746894,1.995735 -34.979191,1.995734 -37.726085,-10e-7 -2.746894,-1.995735 -12.70722,-32.6504716 -11.658,-35.8796391 1.04922,-3.2291679 27.1256965,-22.1748359 30.52104414,-22.1748349 z" transform="translate(45,45)" />')dnl
define(`m4_front_attr', 
	ifdef(`selected', 
		`fill="#CCCC00"',dnl hsl(60, 100%, 40%)
		`fill="#FFFF00"'))dnl hsl(60, 100%, 50%)
define(`m4_shadow_attr',
	ifdef(`selected', 
		`fill="#999900"',dnl hsl(60, 100%, 30%)
		`fill="#CCCC00"'))dnl hsl(60, 100%, 40%)
define(`m4_back_attr',
	ifdef(`selected', 
		`fill="#B2B200"',dnl hsl(60, 100%, 35%)
		`fill="#E5E500"'))dnl hsl(60, 100%, 45%)
define(`m4_border_attr', `fill="#E5E5B2"')dnl hsl(60, 50, 80)
define(`m4_contour_attr',
	ifdef(`selected', 
		`fill="none" stroke="#666600" stroke-width="2"',dnl hsl(60, 100%, 20%)
		`fill="none" stroke="#999900" stroke-width="2"'))dnl hsl(60, 100%, 30%)
