#
# CMakeLists.txt Copyright 2024 Alwin Leerling dna.leerling@gmail.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
# MA 02110-1301, USA.
#

find_package( wxWidgets )

add_executable(
	karnaugh

	karnaughapp.cc
	karnaughapp.h
	karnaughconfig.cc
	karnaughconfig.h
	karnaughdata.cc
	karnaughdata.h
	karnaughwindow.cc
	karnaughwindow.h
	kmapgrid.cc
	kmapgrid.h
	solutionentry.cc
	solutionentry.h
	solutiontree.cc
	solutiontree.h
	truthtablegrid.cc
	truthtablegrid.h
)

target_include_directories( karnaugh PRIVATE ${wxWidgets_INCLUDE_DIRS} )
target_link_libraries( karnaugh PRIVATE ${wxWidgets_LIBRARIES} )
target_compile_options( karnaugh PRIVATE ${wxWidgets_CXX_FLAGS} )
target_compile_definitions( karnaugh PRIVATE ${wxWidgets_DEFINITIONS} )

add_custom_command(
        TARGET karnaugh POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_SOURCE_DIR}/wxwin.ico
			${CMAKE_CURRENT_BINARY_DIR}/../src/wxwin.ico
			)



