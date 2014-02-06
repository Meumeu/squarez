include(m4_config_file)dnl
changecom()dnl
<?xml version="1.0" standalone="no"?>
<!DOCTYPE svg PUBLIC "-//W3C//DTD SVG 20010904//EN" "http://www.w3.org/TR/2001/REC-SVG-20010904/DTD/svg10.dtd">
<svg xmlns="http://www.w3.org/2000/svg" xmlns:xlink="http://www.w3.org/1999/xlink" width="90" height="90">
<defs>
	<rect id="box" width="90" height="90" rx="20" ry="20"/>
	m4_shape
	<clipPath id="shape-clip">
		<use xlink:href="#shape" />
	</clipPath>
</defs>

ifdef(`selected',
`<use id="border" xlink:href="#box" m4_border_attr />
<use id="front" xlink:href="#box" transform="translate(45, 45) scale(0.85) translate(-45, -45)" m4_front_attr />',
`<use id="front" xlink:href="#box" m4_front_attr />')

<use id="shadow" xlink:href="#shape" m4_shadow_attr />
<g clip-path="url(#shape-clip)" >
	<use id="shape-back" xlink:href="#shape" transform="translate(5,5)" m4_back_attr />
</g>
<use id="contour" xlink:href="#shape" m4_contour_attr />
</svg>
