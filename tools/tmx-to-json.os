require "std"

print "============================="
// print process.argv

function usage(){
	print "Usage:"
	print "${process.argv[1]} filename.tmx"
	terminate()
}

var filename = process.argv[2]
// if(!Regexp(`/\.tmx$/`).test(filename)){
if(path.extname(filename) != ".tmx"){
	usage()
}

function extractAttributes(str){
	var m = Regexp(`/([\w\d_]+)=\"([^\"]*)\"/g`).exec(str)
	var r = {}
	for(var i, v in m[1]){
		r[v] = m[2][i]
	}
	return r
}

var contents = File.readContents(filename)
var m = Regexp(`/<map\s+(.+?)>/`).exec(contents)
var map = extractAttributes(m[1])

map.tilesets = []
var m = Regexp(`#<tileset\s+([^>]+)>(.*?)</tileset>#sg`).exec(contents)
for(var i, v in m[1]){
	var tileset = extractAttributes(v)
	if(tileset.name != "physics-tiles"){
		continue
	}
	if(tileset.source){
		throw "tileset.source is not implemented: ${tileset}"
	}
	map.tilesets[] = tileset
}

map.layers = []
var m = Regexp(`#<layer\s+([^>]+)>(.*?)</layer>#sg`).exec(contents)
for(var i, v in m[1]){
	var layer = extractAttributes(v)
	if(layer.name != "physics"){
		continue
	}
	
	var dm = Regexp(`#<data encoding="base64" compression="zlib">(.*?)</data>#s`).exec(m[2][i])
	// print dm[1].trim()
	var memdata = base64.decode(dm[1].trim())
	// print "memdata: ${memdata}"
	memdata = zlib.gzuncompress(memdata)
	// print "memdata2: ${memdata}"
	
	var width = toNumber(layer.width)
	var height = toNumber(layer.height)
	var data, offs = {}, 0
	for(var y = 0; y < height; y++){
		for(var x = 0; x < width; x++){
			gid = memdata.sub(offs, 4).unpack("V")
			gid != 0 && (data[x] || data[x] = {})[y] = gid
			offs += 4
		}
	}
	layer.map = data
	
	map.layers[] = layer
}

// print map
var filename = "../data/"..path.basename(filename).replace(".tmx", ".json")
File.writeContents(filename, json.encode(map))
print "${filename} saved, layers: ${#map.layers}, tilesets: ${#map.tilesets}\n"

