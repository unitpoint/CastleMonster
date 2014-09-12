require "std"

var items = json.decode(File.readContents("originItems.json"))
// dump(items)
/*
define('ITEM_TYPE_WEAPON', 1);
define('ITEM_TYPE_ARTEFACT', 2);
define('ITEM_TYPE_RESOURCE', 3);
define('ITEM_TYPE_OBJECT', 4);
define('ITEM_TYPE_MONSTER', 5);
define('ITEM_TYPE_ACHIEVEMENT', 6);
define('ITEM_TYPE_ARMOR', 7);
define('ITEM_TYPE_MEDAL', 8);
*/

// extract only monsters
// items = items["5"] // monsters

var res = toString(items).replace{
[Regexp(`/0x[0-9a-fA-F]{2}/s`)] = function(m){
	var code = toNumber(m[0])
	return "c".pack(code)
},
"\"image_url\"" = "\"image\"",
[Regexp(`#"/images/([^/]+/[^\.]+).(png|jpg)(\?[\d\.]+)*"#s`)] = function(m){
	return '"'..m[1]..'"'
	/* print m; terminate()
	var code = toNumber(m[0])
	return "c".pack(code) */
},
}
File.writeContents("../data/items.json", res)

// then format json using http://jsonformat.com/#jsondataurllabel
