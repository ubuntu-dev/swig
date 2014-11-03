%module mysql
%insert("include")
%{
#include <mysql/mysql.h>
%}

%include <mysql.h>
