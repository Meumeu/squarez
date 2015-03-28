file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/img)

foreach(i shape0 shape1 shape2)
	add_custom_command(OUTPUT img/${i}.svg
		COMMAND m4 -Dm4_config_file=${i}.m4 shape.svg.m4 > ${CMAKE_CURRENT_BINARY_DIR}/img/${i}.svg
		DEPENDS img/${i}.m4 img/shape.svg.m4
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/img)
	add_custom_command(OUTPUT img/${i}-selected.svg
		COMMAND m4 -Dm4_config_file=${i}.m4 -Dselected shape.svg.m4 > ${CMAKE_CURRENT_BINARY_DIR}/img/${i}-selected.svg
		DEPENDS img/${i}.m4 img/shape.svg.m4
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/img)
endforeach()

configure_file(generated_resources.qrc generated_resources.qrc COPYONLY)

add_custom_target(shapes ALL DEPENDS img/shape0.svg img/shape1.svg img/shape2.svg img/shape0-selected.svg img/shape1-selected.svg img/shape2-selected.svg)