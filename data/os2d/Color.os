local function clamp(a){
	// if(a < 0) return 0
	// if(a > 1) return 1
	return a
}

Color = extends Object {
	__construct = function(r, g, b, a){
		@r = clamp(r || 0)
		@g = clamp(g || 0)
		@b = clamp(b || 0)
		@a = clamp(a || 1)
	},
	
	__cmp = function(b){
		b is Color || throw "Color required"
		var i = @r <=> b.r 
		if(i != 0) return i
		i = @g <=> b.g 
		if(i != 0) return i
		i = @b <=> b.b 
		if(i != 0) return i
		return @a <=> b.a
	},
	
	__add = function(b){
		b is Color && return Color(@r + b.r, @g + b.g, @b + b.b, @a + b.a)
		b = numberOf(b) || throw "number or Color required"
		return Color(@r + b, @g + b, @b + b, @a + b)
	},
	
	__sub = function(b){
		// print "Color ${this} sub ${b}"
		b is Color && return Color(@r - b.r, @g - b.g, @b - b.b, @a - b.a)
		b = numberOf(b) || throw "number or Color required"
		return Color(@r - b, @g - b, @b - b, @a - b)
	},
	
	__mul = function(b){
		// print "Color ${this} mul ${b}"
		b is Color && return Color(@r * b.r, @g * b.g, @b * b.b, @a * b.a)
		b = numberOf(b) || throw "number or Color required"
		return Color(@r * b, @g * b, @b * b, @a * b)
	},
	
	__div = function(b){
		b is Color && return Color(@r / b.r, @g / b.g, @b / b.b, @a / b.a)
		b = numberOf(b) || throw "number or Color required"
		return Color(@r / b, @g / b, @b / b, @a / b)
	},
}

Color.RED = Color(1, 0, 0)
Color.GREEN = Color(0, 1, 0)
Color.BLUE = Color(0, 0, 1)
Color.WHITE = Color(1, 1, 1)
Color.BLACK = Color(0, 0, 0)

