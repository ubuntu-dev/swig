\ (c) 2011 by Gerald Wodni
\ very small example for interfaceing with the c-api of mysql

\ library laden
s" mysqlclient" add-lib

\ funktionen und konstanten laden
require mysql.fs

\ 0-terminated string ausgeben
: .cstr ( addr -- )
	begin
		dup c@
	while
		dup c@ emit
		char+
	repeat drop ;

\ -- echtes programm --
\ connection element anlegen
0 mysql_init constant connection

\ verbinden
connection s\" localhost\0" drop s\" forth\0" drop s\" h4x0r\0"
drop s\" forth\0" drop 0 0 0 mysql_real_connect [if]
	." connection established" cr
[else]
	." connection error" cr bye
[then]

\ sql query absetzen
connection s\" SELECT `name` FROM `systems` \0" drop mysql_query [if]
	." Query-Error: " connection mysql_error .cstr cr bye
[then]

\ ergebnis ausgeben
: tab 9 emit ;
: show-result ( -- )
	\ result holen
	connection mysql_use_result
	begin
		dup mysql_fetch_row ?dup
	while
		tab @ .cstr cr
	repeat
	mysql_free_result ;
	
." Result:" cr
show-result

connection mysql_close
." connection closed" cr

bye

