#!/usr/bin/env bash
FROM_FILE=`[ -z "$1" ] && echo "lorem" || echo "$1"`
TO_FILE=`[ -z "$2" ] && echo "lorem_copy" || echo "$2"`

[ -f "$FROM_FILE" ] || lorem > "$FROM_FILE"

[ "$FROM_FILE" = "$TO_FILE" ] \
	&& echo "Undefined behaviour when source and target files have the same path" && exit 1

if gcc -g -Wall -o copy copy.c; then
	./copy $FROM_FILE 2> /dev/null \
		&& (echo "copy should fail when called with only one argument" && exit 1)
	./copy alaldsfqpoewrlsaldsjf $TO_FILE 2> /dev/null \
		&& (echo "copy should fail when source file doesn't exist" && exit 1)
	./copy $FROM_FILE $TO_FILE \
		|| (echo "copy shouldn't fail with valid arguments" && exit 1)
	diff $FROM_FILE $TO_FILE \
		|| (echo "copy should copy the contents from the source file to the target file " && exit 1)
	./copy $FROM_FILE $TO_FILE \
		|| (echo "copy shouldn't fail with valid arguments" && exit 1)
	diff $FROM_FILE $TO_FILE \
		|| (echo "copy should truncate when the target file already exists" && exit 1)
	echo -e "\e[32mSuccess!\e[m"
fi

lorem(){
	echo > "$FROM_FILE" << EOF
Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras non cursus purus. Vestibulum sit amet suscipit diam. Cras eu sem lorem. Donec semper aliquam eros, in molestie magna egestas in. Sed maximus sagittis velit, eu fringilla velit feugiat sit amet. Nunc vehicula quam id justo ullamcorper accumsan. Cras vel ipsum id arcu mollis molestie placerat lobortis turpis. In hac habitasse platea dictumst. Ut vulputate neque nunc, ac vehicula justo aliquet at. Sed volutpat ac nibh vitae egestas. Donec vitae nulla volutpat, fermentum nisi et, feugiat velit. Sed imperdiet nisi id lacus tempor, at cursus tellus volutpat. Donec maximus augue mi. Cras mollis ut sem in sollicitudin.
Morbi felis orci, rhoncus nec sem ut, congue sollicitudin magna. Ut id neque libero. Nullam vel ipsum scelerisque, venenatis lacus posuere, faucibus purus. Praesent sem odio, dignissim non orci eu, mattis lacinia nunc. Donec augue ipsum, faucibus nec sapien feugiat, ultrices fringilla diam. Pellentesque sed ex metus. Quisque consectetur, est a venenatis viverra, nibh nisl ullamcorper odio, eget lobortis nisi nisl eget tortor. Pellentesque vitae quam quam. Donec eu interdum mauris.
Morbi nec tristique enim. Proin ullamcorper orci eu lacus pellentesque euismod. Curabitur diam risus, semper sed ipsum nec, scelerisque facilisis diam. Vivamus eu erat ligula. Maecenas faucibus volutpat auctor. Donec suscipit turpis magna, eu laoreet nibh porta vitae. Phasellus varius risus nec convallis lacinia. Maecenas urna nibh, porttitor id tempus interdum, dignissim at magna. Fusce ornare malesuada magna, sed semper felis volutpat quis. Aenean scelerisque est nec quam porta, at convallis ligula feugiat. Ut mattis placerat laoreet.
In metus odio, ultricies et justo sit amet, dignissim consectetur ipsum. Vestibulum lobortis convallis porta. Fusce elementum pharetra dictum. Curabitur non dictum dolor, eget dictum nunc. Aliquam felis risus, pellentesque hendrerit vulputate et, aliquam eget nulla. Pellentesque iaculis massa nec purus tristique sodales. Duis placerat dignissim elit. Duis interdum eros vel massa iaculis ullamcorper. Curabitur semper fringilla urna, non lacinia lacus scelerisque id. Nullam in felis ante. Etiam blandit dictum orci a suscipit. Donec lobortis elementum arcu quis rutrum. Nam at tortor elementum lacus pulvinar pretium. Ut commodo felis vel quam tempus, eu consectetur leo accumsan. Proin quis hendrerit neque.
Suspendisse iaculis dui elit, eleifend bibendum turpis malesuada eu. Morbi sit amet sodales orci, et tempor metus. Duis ultricies consequat erat. Sed at tellus velit. Praesent elit urna, congue a rutrum quis, rutrum ut ipsum. Proin luctus eros nec magna iaculis viverra. Morbi eget magna nec sem feugiat posuere vitae in lectus. Ut sit amet suscipit enim. Sed felis massa, efficitur non sapien vitae, mattis pulvinar urna. Proin scelerisque id ante et tempor. Donec venenatis fringilla tincidunt. Nullam sed massa bibendum, accumsan sem vitae, vestibulum turpis.
EOF
}
