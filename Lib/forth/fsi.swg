/*****************************************************************************

	fsi.swg

	Function:	Forth extension module for swig

	Started:	22.11.2010
	Finished:	x

	Copyright 2010 by Gerald Wodni

	This file is part of Forth-Swig-Extension.

	Forth-Swig-Extension is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	Forth-Swig-Extension is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *****************************************************************************/


#define SWIG_FORTH_FSI_PARAMETER		"%{type}"
#define SWIG_FORTH_FSI_FUNCTION			"c-function %{c-name} %{forth-name} %{inputs} -- %{output}"
#define SWIG_FORTH_FSI_COMMENT			"\\ %{value}"
#define SWIG_FORTH_FSI_OUTPUT			"printf( \"%{value}\\n\" );\n"

#define SWIG_FORTH_STACKEFFECT_COMMENT	        "\t( %{arguments}%{spacer}-- )"

