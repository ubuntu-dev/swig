%module mysql
%insert("include")
%{
#include <mysql/mysql.h>
#include <mysql/manamana.h>
%}

%include <mysql.h>
