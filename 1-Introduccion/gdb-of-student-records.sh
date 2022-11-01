#!/usr/bin/env bash
DB_NAME='database'
EXEC_NAME='student-records'

gdb_with_args(){
	echo -e "\e[34m$EXEC_NAME -f $DB_NAME $1\e[m"
	[ "$SKIP" != 'X' ] \
		&& gdb $EXEC_NAME -tui -ex 'b main' -ex "run -f $DB_NAME $1" -ex 'p *argv@argc' \
		|| ./student-records -f "$DB_NAME" $1
	return 0
}
[ -e $DB_NAME ] && rm $DB_NAME
gcc -o student-records -Wall -g student-records.c \
&& SKIP='X' gdb_with_args '-c 27:67659034X:Chris:Rock 34:78675903J:Antonio:Banderas' \
&& SKIP='X' gdb_with_args '-a 3:58943056J:Santiago:Segura 4:6345239G:Penelope:Cruz'  \
&& SKIP='X' gdb_with_args '-a 3:58943056J:Antonio:Segura '                           \
&& SKIP='X' gdb_with_args '-l'                                                       \
&& SKIP='X' gdb_with_args '-q -i 7'                                                  \
&& SKIP='X' gdb_with_args '-q -i 34'													\
&& echo -e "\e[33mWinner winner, chicken dinner!\e[m"
