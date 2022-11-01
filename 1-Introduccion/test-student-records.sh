### CONSTANTS ###
EXEC_NAME="student-records"
REC_FILE="records.txt"
DB_NAME="database"
exec="./$EXEC_NAME -f $DB_NAME"

### ASSERTIONS ### 
[ -x $EXEC_NAME ] || (echo "Couldn't find the executable $EXEC_NAME"   && exit 1)
[ -f $REC_FILE ]  || (echo "Couldn't find the regular file $REC_FILE"  && exit 1)

### MAIN ###
# Create database file
records=`cat $REC_FILE | sed 's/\n/ /g'`
for record in $records; do 
	if [ -z $first_iteration ]; then
		$exec -c $record || exit 1
		first_iteration="no"
	else
		$exec -a $record || exit 1
	fi
done

# Dump contents of database
$exec -l || exit 1
xdd $DB_NAME

# Test the records were included properly
for record in $records; do
	nif=`echo $record | cut -d: -f2 record`
	$exec -q -n $nif || exit 1
done
