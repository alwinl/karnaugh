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

find_program( COMPILE_LANGUAGE NAMES msgfmt REQUIRED )

SET( DOC_LANGS en nl hr )

foreach( DOC_LANG ${DOC_LANGS} )

SET( THE_INPUT ${CMAKE_CURRENT_SOURCE_DIR}/${DOC_LANG}.po )
SET( THE_OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/../src/${DOC_LANG}/karnaugh.mo )

	add_custom_command(
		OUTPUT ${THE_OUTPUT}
		DEPENDS ${THE_INPUT}
		COMMAND ${COMPILE_LANGUAGE} ${THE_INPUT} -o ${THE_OUTPUT}
	)

	list( APPEND DOC_LANG_RESULT ${THE_OUTPUT} )

endforeach()

add_custom_target( karnaugh_translations ALL DEPENDS ${DOC_LANG_RESULT})