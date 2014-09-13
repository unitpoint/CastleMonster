print "--"
print "[start] ${DateTime.now()}"

GAME_SIZE = vec2(960, 540)

var displaySize = stage.size
var scale = displaySize / GAME_SIZE
// scale = math.max(scale.x, scale.y)
scale = math.min(scale.x, scale.y)
stage.size = displaySize / scale
stage.scale = scale

// dump(_G)

function getTimeSec(){
	return DateTime.now().comtime
}

function clamp(a, min, max){
	return a < min ? min : a > max ? max : a
}

function randItem(items){
	if(arrayOf(items)){
		return items[math.random(#items)]
	}
	if(objectOf(items)){
		var keys = items.keys
		return items[keys[math.random(#keys)]]
	}
	return items
}

function PhysBlock.valueOf(){
	return {type = @type, pos = @pos, size = @size}.valueOf()
}

function extend(a, b, clone_result){
	if(b === null){
		return a.deepClone()
	}
	if(!!objectOf(b) != !!objectOf(a)){
		return b.deepClone()
	}
	if(clone_result !== false){
		a = a.deepClone()
	}
	for(var key, item in b){
		if(objectOf(item)){
			var val
			if((val = a[key]) && objectOf(val)){
				a[key] = extend(val, item, false)
			}else{
				a[key] = item.deepClone()
			}
		}else{
			a[key] = item
		}
	}
	return a
}

TEST_LEVEL_INVASION = 3
INVASION_COUNT = 30
DAY_COUNT = 30

GameLevel(0, 1, 1).attachTo(stage)

