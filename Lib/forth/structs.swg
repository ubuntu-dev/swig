/*****************************************************************************

	structs.swg

	Function:	templates for struct handling according to Forth200x

	Started:	15.08.2012
	Finished:	x

	Copyright 2012 by Gerald Wodni

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

#define SWIG_FORTH_STRUCT_COMMENT               "%{c-name}\\n"

#define SWIG_FORTH_STRUCT_BEGIN	        	"\"begin-structure %{forth-name}\\n\""
#define SWIG_FORTH_STRUCT_END	        	"\"drop %d end-structure\\n\", sizeof( %{c-name} )"

#define SWIG_FORTH_STRUCT_FIELD                 "\"%{forth-name}\", offsetof( struct %{c-struct-name}, %{c-field-name} ), sizeof( %{c-type} )"

