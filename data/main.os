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

function clamp(a, min, max){
	return a < min ? min : a > max ? max : a
}

TEST_LEVEL_INVASION = 3
INVASION_COUNT = 30
DAY_COUNT = 30

GameLevel(3, 1, 1).attachTo(stage)

