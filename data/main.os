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

function getCurTime(){
	return DateTime.now().comtime
}

GameLevel().attachTo(stage)

