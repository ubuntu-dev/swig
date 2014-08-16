/*****************************************************************************

	forth.swg

	Function:	Forth extension module for swig

	Started:	25.02.2008
	Finished:	x

	Copyright 2008-2010 by Gerald Wodni

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

/* Primitive types */
%typemap(forth) bool,			const bool &			"n"
%typemap(forth) char,			const char &			"n"
%typemap(forth) signed char,		const signed char &		"n"
%typemap(forth) unsigned char,		const unsigned char &		"n"
%typemap(forth) short,			const short &			"n"
%typemap(forth) unsigned short,		const unsigned short &		"n"
%typemap(forth) int,			const int &			"n"
%typemap(forth) unsigned int,		const unsigned int &		"n"
%typemap(forth) long,			const long &			"n"
%typemap(forth) unsigned long,		const unsigned long &		"n"
%typemap(forth) long long,		const long long &		"d"
%typemap(forth) unsigned long long,	const unsigned long long &	"d"
%typemap(forth) float,			const float &			"r"
%typemap(forth) double,			const double &			"r"
%typemap(forth) long double,		const long double &		"r"
%typemap(forth) void							"void"

/* instead of creatiung a typemap for every pointer and array e.g.:
	%typemap(forth) char *, char[ANY], char[]			"a"
   all pointers are mapped to "a" (see documentation 10.3.3)	
 */
%typemap(forth) SWIGTYPE *						"a"
%typemap(forth) SWIGTYPE []						"a"

/* enums are "n" */
%typemap(forth) enum SWIGTYPE						"n"

/* common templates */
%include "sections.swg"
%include "structs.swg"

/* vendor specific output */
%include "gforth.swg"
%include "swiftForth.swg"
%include "vfx.swg"

/* file header */
%insert("include") "include.swg"
%insert("header") "header.swg"

%insert("footer") "footer.swg"

