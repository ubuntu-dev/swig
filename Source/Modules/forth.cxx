/*****************************************************************************

	gforth.cxx

	Function:	Gforth extension module for swig

	Started:	25.02.2008
	Finished:	x

	Copyright 2008-2011 by Gerald Wodni

	This file is part of Forth-Swig-Extension.

	Forth-Swig-Extension is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	(at your option) any later version.

	Gforth-Swig-Extension is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.

 *****************************************************************************/

/*
 *	TODO:
 *		- SWIG_TYPE for mapping const to normal?
 *		- rename *n to *node
 *
 *	TODO: add typed stackeffect - comment
 *
 * */

#include "swigmod.h"

#define FORTH_DEFAULT_TYPE "n"

static const char *usage = (char *) "\
Forth Options (available with -forth)\n\
     -notrans-constants    - will not transform the constants ( \"#define FOO_bar 123\" becomes \"FOO_bar constant foo_BAR\" )\n\
     -full-crossplatform   - instead of resolving all types (standard setting), all types are left c-typed\n\
                             on the target platform call 'gcc -E' (headers required)\n\
     -defaulttype <type>   - specifies the forth-type to be used when no typemap was found (default is " FORTH_DEFAULT_TYPE " )\n\
     -forthifyfunctions    - change c-naming-convention into forth-names e.g. getAllStuff becomes get-all-stuff\n\
     -fsi-output           - generates an fsi(platform-independent) instead of an fs file, which needs to be compiled by gcc\n\
     -no-sectioncomments  - hides section comments in output file\n\
     \n\
     Forth Systems:\n\
     -gforth               - generate wrapper to Gforth (default)\n\
     -fsi                  - generate platform independent wrapper to GForth\n\
     -sf                   - generate wrapper to SwiftForth\n\
     -vfx                  - generate wrapper to VFX\n\
     \n\
     Documentation:\n\
     -enumcomments         - display enum name as comment over the enum-contants\n\
     -stackcomments        - display function-parameter-names in stack-notation over the c-function\n";


#define FORTH_DEBUG 0

class FORTH : public Language
{
	/* methods */
	public:
		virtual void main( int argc, char **argv );

		virtual int top( Node *n );

		void dumpSection( const char* sectionName, File *sectionFile );

		/* for enum mapping and typedef visualisation */
		virtual int enumDeclaration( Node *n);
  		virtual int typedefHandler( Node *n );

		/* struct-handling */
		virtual int constructorDeclaration( Node *n );

		/* shouts */
		virtual int functionHandler( Node *node ); 
		String *	functionWrapper( String *name, String *forthName, String *type, String *parmstr, const char *prefix, const char *functionTemplateName = "%s_FUNCTION", const char *cAction = "" );
		void	functionWrapper( File *file, String *name, String *forthName, ParmList *parms, String *returnType, const char *templateName, const char *functionName, const char *cAction = "" );
		/* wrappers */

		virtual int constantWrapper( Node *n );
		virtual int functionWrapper( Node *n );

		int	structMemberWrapper( Node *n );

		void	registerCallback( Node *n, String *name, SwigType *type, SwigType *returnType );

	private:
		void	printNewline( File *file );
		void	printSectionComment( File *file, const String *section ); 
		void	printComment( File *file, const char *comment ); 
		void	printComment( File *file, const String *comment ); 

		String	*ParmList_str_forthargs( ParmList *node, const char *attr_name );
		/* always_resolve ignores fullCrossPlatform-Mode (used for checking the type of constants) */
		String	*typeLookup( Node *node, bool always_resolve = false );
		String	*forthifyName( String *name );
		String	*templateInstace( const char *name );

		String	*toggleCase( String *name );
		void	uppercase( String *name );

		bool	itemExists( DOH *node, String *name );
	
		unsigned long base2dec( String *number, unsigned long base );

	/* members */
	protected:
		File *f_begin;
		File *f_runtime;
		File *f_header;
		File *f_footer;
		File *f_structs;
		File *f_functionPointers;
		File *f_callbacks;
		File *f_wrappers;

		File *f_include;
		File *f_enums;
		File *f_intConstants, *f_floatConstants, *f_stringConstants;
		File *f_functions;
		File *f_init;

	private:
		bool	fsiOutput;
		bool	useStackComments;
		bool	useEnumComments;
		bool	forthifyfunctions;
		bool	fullCrossPlatform;
		bool	noConstantsTransformation;
		bool	wrapFunction;			/* set by functionHandler to prevent swig from generating _set and _get fopr structs and alike */
		bool	containsVariableArguments;	/* set by typeLookup to handle special output in function wrapper */
		bool	sectionComments;
		String	*defaultType;
		List	*m_structs;
		Hash	*m_structFields;
		Hash	*m_templates;
};

void FORTH::main( int argc, char **argv )
{
	fsiOutput = false;
	useStackComments = false;
	useEnumComments = false;
	forthifyfunctions = false;
	fullCrossPlatform = false;
	noConstantsTransformation = false;
	defaultType = NULL;
	containsVariableArguments = false;
	sectionComments = true;

	/* treat arguments */
	for( int i = 1; i < argc; i++ ) 
	{
		if( argv[i] ) 
		{
			if( strcmp( argv[i], "-fsi-output" ) == 0)
			{
				fsiOutput = true;
				Preprocessor_define( "SWIGFORTH_FSI 1 ", 0);
				Swig_mark_arg(i);
			}
			else if( strcmp( argv[i], "-fullcrossplatform" ) == 0)
			{
				fullCrossPlatform = true;
				Swig_mark_arg(i);
			}
			else if( strcmp( argv[i], "-notrans-constants" ) == 0)
			{
				noConstantsTransformation = true;
				Swig_mark_arg(i);
			}
			else if( strcmp( argv[i], "-stackcomments" ) == 0)
			{
				useStackComments = true;
				Swig_mark_arg(i);
			}
			else if( strcmp( argv[i], "-enumcomments" ) == 0)
			{
				useEnumComments = true;
				Swig_mark_arg(i);
			}
			if( strcmp( argv[i], "-forthifyfunctions" ) == 0)
			{
				forthifyfunctions = true;
				Swig_mark_arg(i);
			}
			else if( strcmp( argv[i], "-defaulttype" ) == 0 )
			{
				if( i + 1 < argc && argv[i+1] )
				{
					defaultType = NewString("");
					Printf( defaultType, argv[i+1] );
					Swig_mark_arg( i );
					Swig_mark_arg( ++i );
				}
				else
					Swig_arg_error();
			}
			else if( strcmp( argv[i], "-help" ) == 0)
			{
				fputs( usage, stderr );
			}
			if( strcmp( argv[i], "-no-sectioncomments" ) == 0)
			{
				sectionComments = false;
				Swig_mark_arg(i);
			}
		}       
	}

	m_templates = NewHash();

	if( defaultType == NULL )
		defaultType = NewString( FORTH_DEFAULT_TYPE );

	/* Set language-specific subdirectory in SWIG library */
	SWIG_library_directory( "forth" );

	/* Set language-specific preprocessing symbol */
	Preprocessor_define( "SWIGFORTH 1 ", 0);

	/* Set target-system-specific configuration file */
	SWIG_config_file( "forth.swg" );

	/* Set typemap language ( historical ) */
	SWIG_typemap_lang( "forth" );
}

int FORTH::top( Node *n )
{
	/* Get the module name */
	/* currently not needed, create a new vocabulary? */
	//String *module = Getattr( n, "name" );

	/* Get the output file name */
	String *outfile = Getattr( n, "outfile" );

	/* Initialize I/O */
	f_begin = NewFile( outfile, "w", SWIG_output_files() );
	if ( !f_begin )
	{
		FileErrorDisplay( outfile );
		SWIG_exit( EXIT_FAILURE );
	}
	f_runtime = NewString( "" );
	f_init = NewString( "" );
	f_header = NewString( "" );
	f_footer = NewString( "" );
	f_structs = NewString( "" );
	f_functionPointers = NewString( "" );
	f_callbacks = NewString( "" );
	f_wrappers = NewString( "" );
	f_include = NewString( "" );
	f_enums = NewString( "" );
	f_intConstants = NewString( "" ); f_floatConstants = NewString( "" ); f_stringConstants = NewString( "" );
	f_functions = NewString( "" );
	m_structFields = NewHash();
	m_structs = NewList();

	/* Register file targets with the SWIG file handler */
	Swig_register_filebyname( "begin", f_begin );
	Swig_register_filebyname( "header", f_header );
	Swig_register_filebyname( "footer", f_footer );
	Swig_register_filebyname( "include", f_include );
	Swig_register_filebyname( "struct", f_structs );
	Swig_register_filebyname( "functionPointers", f_functionPointers );
	Swig_register_filebyname( "callbacks", f_callbacks );
	Swig_register_filebyname( "wrapper", f_wrappers );
	Swig_register_filebyname( "runtime", f_runtime );
	Swig_register_filebyname( "init", f_init );

	Swig_banner( f_begin );

	/* Emit code for children */
	Language::top( n );

	/* Write all to the file */

	/* user-includes */
	Dump( f_include, f_begin );

	/* Output module initialization code */
	Dump( f_header, f_begin );
	
	/* TODO: ifdefs for constants? */
	/* constants */
	dumpSection( "CONSTANTS_INT", f_intConstants );

	dumpSection( "CONSTANTS_FLOAT", f_floatConstants );

	dumpSection( "CONSTANTS_STRING", f_begin );

	/* enums */
	dumpSection( "ENUMS", f_enums );

	/* structs */
	dumpSection( "STRUCTS", f_structs );

	/* functionPointers */
	dumpSection( "FUNCTION_POINTERS", f_functionPointers );

	/* callbacks */
	dumpSection( "CALLBACKS", f_callbacks );

	/* functions */
	dumpSection( "FUNCTIONS", f_functions );

	/* footer */
	Dump( f_footer, f_begin );

	/* print to file */
	Wrapper_pretty_print( f_init, f_begin );

	/* Cleanup files */
	Delete( m_structs );
	Delete( f_header );
	Delete( f_footer );
	Delete( f_structs );
	Delete( f_callbacks );
	Delete( f_wrappers );
	Delete( f_init );
	Delete( f_runtime );

	Close( f_begin );
	Delete( f_begin );

	return SWIG_OK;
}

void FORTH::dumpSection( const char* sectionName, File *sectionFile )
{
	/* only print section if it has actial content */
	if( Len( sectionFile ) > 0 )
	{
		printSectionComment( f_begin, sectionName );
		Dump( sectionFile, f_begin );
	}
}

/* special handlers */

/* find structs */
int FORTH::constructorDeclaration( Node *n )
{
	/* append to list of known structs */
	String *name = Getattr( n, "sym:name" );
	Append( m_structs, name );

	/* starter-comment & begin */
	String *comment = templateInstace( "STRUCT_COMMENT" );
	Replace( comment, "%{c-name}", name, DOH_REPLACE_ANY );
	Replace( comment, "%{forth-name}", name, DOH_REPLACE_ANY );
	printComment( f_structs, comment );

	String *begin = templateInstace( "STRUCT_BEGIN" );
	Replace( begin, "%{c-name}", name, DOH_REPLACE_ANY );
	Replace( begin, "%{forth-name}", name, DOH_REPLACE_ANY );
	Printf( f_structs, "\tprintf( %s );\n", begin );

	/* fields */
	Hash *fieldHash = (Hash*) Getattr( m_structFields, name );
	if( fieldHash != NULL )
	{
		List *fieldKeys = Keys( fieldHash );
		for( int i = 0; i < Len( fieldKeys ); i++ )
		{
			String	*fieldName = (String*) Getitem( fieldKeys, i ),
				*fieldOutput = (String*) Getattr( fieldHash, fieldName );

			Printf( f_structs, "%s", fieldOutput );
		}
	}


	/* end */
	String *end = templateInstace( "STRUCT_END" );
	String *cName = NewStringf( "struct %s", name );
	Replace( end, "%{c-name}", cName, DOH_REPLACE_ANY );
	Replace( end, "%{forth-name}", name, DOH_REPLACE_ANY );
	Printf( f_structs, "\tprintf( %s );\n", end );

	return Language::constructorDeclaration( n );
}

/* only for visualisation */
int FORTH::typedefHandler( Node *n )
{
#if FORTH_DEBUG
	String *name = Getattr( n, "sym:name" );
	Printf( f_wrappers, "\\ typedef %s\n", name );
#endif

	return Language::typedefHandler( n );
}

/* context-sensive wrapping */
int FORTH::functionHandler( Node *node )
{
	/* enable function-wraping for this node */
	wrapFunction = true;

	this->Language::functionHandler( node );
	wrapFunction = false;

	return SWIG_OK;
}



/* wrappers */

int FORTH::constantWrapper(Node *n)
{
	String *name = Getattr( n, "sym:name" );
	String *cTypeName = SwigType_str( Getattr( n, "type" ), 0 );
	String *type = typeLookup( n, true );
	String *value = Getattr( n, "value" );

	/* check constant-type */
	if( Strncmp( cTypeName, "char *", 6) == 0 )
	{
		/* save template in hashtable */
		if( Strncmp( name, "SWIG_FORTH_", 11 ) == 0 )
		{
			const char *templateData = (const char *) Data( name );
			templateData += 11;
			String *templateName = NewString( templateData );
			Setattr( m_templates, templateName, value );
		}
		/* string found. create wrapper function */
		else
			Printf( f_stringConstants, "\t#ifdef %s\n\t\tswigStringConstant( %s, \"%s\" );\n\t#endif\n", name, name, name );
	}
	else
	{
		/* resolve type and create according constant */
		if( Strncmp( type, "n", 1 ) == 0 )
			Printf( f_intConstants, "\t#ifdef %s\n\t\tswigIntConstant( %s, \"%s\" );\n\t#endif\n", name, name, name );
		else if( Strncmp( type, "r", 1 ) == 0 )
			Printf( f_floatConstants, "\t#ifdef %s\n\t\tswigFloatConstant( %s, \"%s\" );\n\t#endif\n", name, name, name );
		else
			/* unable to find correct type */
			Swig_warning( WARN_FORTH_CONSTANT_TYPE_UNDEF, input_file, line_number, "No forth type found for c-type \"%s\", skipping constant \"%s\"\n", cTypeName, name );
	}

	return SWIG_OK;
}

int FORTH::enumDeclaration( Node *node )
{
	String		*name   = Getattr(node,"sym:name");

	if( useEnumComments )
	{
		String		*comment = NewStringf( "enum %s", name );
		const char	*commentData = (const char *) Data( comment);
		printComment( f_enums, commentData );
		Delete( comment );
	}

	DOH *item = firstChild( node );
	while( item )
	{
		String *itemName = Getattr( item, "sym:name" );
		
		/* make a constant of the enum-item */
		Printf( f_enums, "\tswigIntConstant( %s, \"%s\" );\n", itemName, itemName );

		item = nextSibling( item );
	}

	return SWIG_NOWRAP;
}

int FORTH::structMemberWrapper( Node *node )
{
#if 0
	List *keys = Keys( node );
	Printf( f_structs, "\n\nSTRUCT MEMBER WRAPPER:\n" );
	for( int i = 0; i < Len( keys ); i++ )
		Printf( f_structs, "\t\tstruct( %s ) = '%s'\n", (String *)Getitem( keys, i ), (String *)Getattr( node, (String*) Getitem( keys, i ) ) );
#endif

	/* only use member-gets, as we only need to create a +field offset/size pair */
	String *memberget = Getattr( node, "memberget" );
	if( memberget == NULL || Strcmp( memberget, "1" ) != 0 )
		return SWIG_OK;

	String	*structName = Getattr( parentNode( node ), "name" ),
		*fieldName = Getattr( node, "membervariableHandler:sym:name" ),
		*cName = NewStringf( "struct %s.%s", structName, fieldName ),
		*cType = NewString( "" ),
		*forthName = NewStringf( "%s-%s", structName, fieldName );

	/* pretty-print type */
	SwigType *type = Getattr( node, "membervariableHandler:type" );
	cType = SwigType_str( type, cType );

	//if( SwigType_isfunction( type ) )
	registerCallback( node, forthName, type, typeLookup( node ) );

	/* create/get hash for this struct's fields */
	Hash *structFields = Getattr( m_structFields, structName );
	if( structFields == NULL )
	{
		structFields = NewHash();
		Setattr( m_structFields, structName, structFields );
	}

	/* use template */
	String *fieldTemplate = templateInstace( "STRUCT_FIELD" ),
	       *output = NewString("");

	Replace( fieldTemplate, "%{c-name}", cName, DOH_REPLACE_ANY );
	Replace( fieldTemplate, "%{c-type}", cType, DOH_REPLACE_ANY );
	Replace( fieldTemplate, "%{forth-name}", forthName, DOH_REPLACE_ANY );
	Replace( fieldTemplate, "%{c-struct-name}", structName, DOH_REPLACE_ANY );
	Replace( fieldTemplate, "%{c-field-name}", fieldName, DOH_REPLACE_ANY );
	
	Printf( output, "\tswigStructField( %s );\n", fieldTemplate );

	Setattr( structFields, fieldName, output );

	Delete( cType );

	return SWIG_OK;
}

void	FORTH::registerCallback( Node *node, String *name, SwigType *type, SwigType *returnType )
{
	String	*cType = SwigType_str( type, NewString("") ),
		*functionType = NewString( type ),
		*poppedType;

	/* remove all prefix pointers */
	while( SwigType_ispointer( ( poppedType = SwigType_pop( functionType ) ) ) )
		Delete( poppedType );

	/* if this type isn't a callback, leave */
	if( SwigType_isfunction( poppedType ) == 0 )
		return;

	/* restore "pure" callback type */
	ParmList	*parms  = Getattr(node,"parms");
	String		*forthName = name;

	/* common function-pointer & callback */
	SwigType_push( functionType, poppedType );

	/* callback */
	functionWrapper( f_callbacks, name, forthName, parms, returnType, "CALLBACK", "swigCallback" );

	/* function pointer */
	String *action = NewString( Getattr( node, "wrap:action" ) );
	Replace( action, " ", "", DOH_REPLACE_ANY );
	Replace( action, "result=", "", DOH_REPLACE_FIRST );

	parms  = Getattr(node,"membervariableHandler:parms");

	functionWrapper( f_functionPointers, name, forthName, parms, returnType, "FUNCTION_POINTER", "swigFunctionPointer", Char( action ) );

	/* clean up */
	Delete( action );
	Delete( poppedType );
	Delete( functionType );
	Delete( cType );
}

/* wraps all available systems */
void FORTH::functionWrapper( File *file, String *name, String *forthName, ParmList *parms, String *returnType, const char *templateName, const char *functionName, const char *cAction)
{
	String		*parmstr= ParmList_str_forthargs( parms, "type" ),
			*parmSpacer = NewString(ParmList_len( parms ) ? " " : ""),
			*templateString = NewStringf( "%%s_%s", templateName ),
			*gforth =	functionWrapper( name, forthName, returnType, parmstr, "GFORTH", Char(templateString), cAction ),
			*swiftForth =	functionWrapper( name, forthName, returnType, parmstr, "SWIFTFORTH", Char(templateString), cAction ),
			*vfx =		functionWrapper( name, forthName, returnType, parmstr, "VFX", Char(templateString), cAction );


	String	*comment = templateInstace( "STACKEFFECT_COMMENT" );

	Replace( comment, "%{arguments}", ParmList_str_forthargs( parms, "name" ), DOH_REPLACE_ANY );
	Replace( comment, "%{spacer}", parmSpacer, DOH_REPLACE_ANY );

	Printf( file, "\t%s( \"%s\", \"%s\", \"%s\", \"%s\" );\n", functionName, gforth, swiftForth, vfx, comment  );

	Delete( templateString );
	Delete( gforth );
	Delete( swiftForth );
	Delete( vfx );
	Delete( comment );
}

/* wraps single system */
String *FORTH::functionWrapper(String *name, String *forthName, String *type, String *parmstr, const char *prefix, const char* functionTemplateName, const char *cAction)
{
	/* transform template declaration */
	String	*functionTemplate = NewStringf( functionTemplateName , prefix ),
		*callingConventionTemplate = NewStringf( "%s_CALLCONV", prefix ),
		*outputTemplate = NewStringf( "%s_OUTPUT", prefix ),
		*declaration = templateInstace( Char(functionTemplate) );

	Replace( declaration, "%{c-name}", name, DOH_REPLACE_ANY );
	Replace( declaration, "%{forth-name}", forthName, DOH_REPLACE_ANY );
	Replace( declaration, "%{inputs}", parmstr, DOH_REPLACE_ANY );
	Replace( declaration, "%{output}", type, DOH_REPLACE_ANY );
	Replace( declaration, "%{c-action}", cAction, DOH_REPLACE_ANY );

	/* take care of vfx's calling convention */
	Replace( declaration, "%{calling-convention}", (String*) Getattr(m_templates, callingConventionTemplate) , DOH_REPLACE_ANY );

	String *output = templateInstace( Char( outputTemplate ) );
	Replace( output, "%{value}", declaration, DOH_REPLACE_ANY );

	Delete( declaration );
	Delete( outputTemplate );
	Delete( callingConventionTemplate );
	Delete( functionTemplate );

	return output;
}

int FORTH::functionWrapper(Node *node)
{
	if( ! wrapFunction )	/* we only generate code for real functions */
		return structMemberWrapper( node );

	containsVariableArguments = false;

	/* Get some useful attributes of this function */
	String		*name   = Getattr(node,"sym:name");
	String		*forthName;
	String		*type;
	ParmList	*parms  = Getattr(node,"parms");
	
	type = typeLookup( node );

	/* TODO: handle variable arguments */
	/* TODO: omit static functions and enums */

	/* glBegin becomes gl-begin ( if enabled ) */
	if( forthifyfunctions )
		forthName = forthifyName( name );
	else
		forthName = name;

	functionWrapper( f_functions, name, forthName, parms, type, "FUNCTION", "swigFunction" );

	return SWIG_OK;
}

/* Helper Methods */
void	FORTH::printNewline( File *file )
{
	Printf( file, "\n\tswigNewline();\n" );
}

void	FORTH::printSectionComment( File *file, const String *section )
{
	String *sectionName = NewStringf( "SECTION_%s", section ),
	       *comment = (String*) Getattr(m_templates, sectionName );

	printNewline( file );
	printComment( file, comment );

	Delete( sectionName );
}

void	FORTH::printComment( File *file, const char *comment )
{
	Printf( file, "\n\tswigComment(\"%s\");\n", comment );
}

void	FORTH::printComment( File *file, const String *comment )
{
	Printf( file, "\n\tswigComment(\"%s\");\n", comment );
}

String *FORTH::ParmList_str_forthargs( ParmList *node, const char *attr_name )
{
	String *out = NewStringEmpty();
	while( node )
	{
		String *type;
		
		/* if type is requested, perform a lookup */
		if(!strncmp(attr_name, "type", 4))
			type = typeLookup( node );
		else
		{
			/* otherwise get the item */
			String *attr = Getattr( node, attr_name );
			/* prevent crashing on empty (or strange?) names */
			if( attr == NULL )
				type = NewString( "<noname>" );
			else
				type = SwigType_str( attr, NewStringEmpty() );
		}
		
		Append( out, type );

		node = nextSibling( node );
		if( node )
			Append( out, " " );
		Delete( type );	/* shouln't that string only be deleted if SwigType_str was used? */
	}

	return out;
}

String *FORTH::typeLookup( Node *node, bool always_resolve )
{
	String		*typeName;
	String		*resultType = NewString("");
	String		*cTypeName = SwigType_str( Getattr( node, "type" ) , 0);
	bool		foundType;

	/* Get return types */
	foundType = ( typeName = Swig_typemap_lookup( "forth", node, "", 0 ) );

	/* Type not found so far, check structs for an occurance of cTypeName */
	if( !foundType && itemExists( m_structs, cTypeName ) )
	{
		/* found, current type is struct */
		typeName = NewString( "struct" );
		foundType = true;
	}

	/* Type found, replace */
	if( foundType )
		Printf(resultType, "%s", typeName);
	/* Variable Argument List found */
	else if( ! Strcmp( cTypeName, "..." ) )
	{
		containsVariableArguments = true;
		Swig_warning( WARN_FORTH_VARIABLE_ARGUMENTS, input_file, line_number, "Variable Argument List detected ( \"%s\" ), using \"%s\"\n", cTypeName, defaultType );
	}
	else
	{

		/* Type not found, emit default-type and display warning */
		Printf( resultType, (char *)Data(defaultType) );
		Swig_warning( WARN_FORTH_TYPEMAP_UNDEF, input_file, line_number, "No forth typemap defined for \"%s\", using \"%s\"\n", cTypeName, defaultType );
	}

	Delete( typeName );
	Delete( cTypeName );

	return resultType;
}


String	*FORTH::toggleCase( String *name )
{
	String *toggled = NewString("");
	const char *nameData = (const char *) Data( name );

	for( int i = 0; i < Len( name ); i++ )
	{
		char currentChar = nameData[i];

		if( 'A' <= currentChar && currentChar <= 'Z' )
			currentChar += 'a' - 'A';
		else if( 'a' <= currentChar && currentChar <= 'z' )
			currentChar += 'A' - 'a';

		Printf( toggled, "%c", currentChar );
	}

	return toggled;
}

void	FORTH::uppercase( String *name )
{
	char *nameData = Char( name );

	for( int i = 0; i < Len( name ); i++ )
	{
		char currentChar = nameData[i];

		if( 'a' <= currentChar && currentChar <= 'z' )
			nameData[i] += 'A' - 'a';
	}
}

bool	FORTH::itemExists( DOH *node, String *name )
{
	for( int i = 0; i < Len( node ); i++ )
		if( ! Strcmp( name, Getitem( node, i ) ) )
			return true;

	return false;
}

String *FORTH::forthifyName( String *name )
{
	String *forthName = NewString("");
	String *functionName = Copy( name );
	int i = 0;
	char previousChar = 'x'; /* gcc-hint */

	while( Len( functionName ) )
	{
		const char *functionNameData = (const char *) Data( functionName );
		char currentChar = functionNameData[0];
		char currentLowercaseChar = ( 'A' <= currentChar && currentChar <= 'Z' ) ? currentChar - 'A' + 'a' : currentChar ;

		/* check if letter-group has changed */
		if( currentChar == '_' )
			/* convert _ to - */
			Printf( forthName, "-", currentLowercaseChar );
		else if( i > 0 && previousChar / 0x20 != currentChar / 0x20 )
		{
			/* group has changed, now take the right action */
			if( 'a' <= previousChar && previousChar <= 'z' && 'A' <= currentChar && currentChar <= 'Z' )
				/* lower->uppercase */
				Printf( forthName, "-%c", currentLowercaseChar );
			else if( previousChar / 0x20 == 1 || currentChar / 0x20 == 1 )
				/* number occured */
				Printf( forthName, "-%c", currentLowercaseChar );
			else
				/* other change (upper->lowercase) */
				Printf( forthName, "%c", currentLowercaseChar );
		}
		else
			/* no change, just add currentChar as is */
			Printf( forthName, "%c", currentLowercaseChar );

		Delslice( functionName, 0, 1 );
		previousChar = currentChar;
		i++;
	}

	Delete( functionName );

	return forthName;
}

String	*FORTH::templateInstace( const char *name )
{
	/* load template and transform some \-s */
	String *instance = NewString( (String*) Getattr( m_templates, name ) );

	/* preserve intended backspace */
	Replace( instance, "\\\\", "\\-", DOH_REPLACE_ANY );
	Replace( instance, "\\t", "\t", DOH_REPLACE_ANY );
	Replace( instance, "\\n", "\n", DOH_REPLACE_ANY );
	Replace( instance, "\\\"", "\"", DOH_REPLACE_ANY );

	/* transform back to intended backspace */
	Replace( instance, "\\-", "\\", DOH_REPLACE_ANY );

	return instance;
}

unsigned long FORTH::base2dec( String *number, unsigned long base )
{
	unsigned long result = 0, multiplier = 1;
	const char *numberData = (const char *) Data( number );

	for( int i = Len( number ) - 1; i >= 0; i-- )
	{
		result += multiplier * (unsigned long)( numberData[ i ] - '0' );
		multiplier *= base;
	}

	return result;
}

/* c-level access to class */
extern "C" Language *swig_forth( void )
{
	return new FORTH();
}

