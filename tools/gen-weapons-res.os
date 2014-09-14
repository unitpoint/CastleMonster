require "std"

print "============================"

function getId(filename){
	var id = "weapons/"..path.basename(filename).replace(Regexp(`/\..*$/`), "")
}

var dir = fs.readdir("..\data_debug\images\weapons")
// print dir
for(var i, filename in dir){
	print "<image id=\"${getId(filename)}\" file=\"data_debug/images/weapons/${filename}\" cols=\"2\" />"
}