vec2 = extends Object {
	__construct = function(x, y){
		@x = x || 0 // numberOf(x) || throw "x number required"
		@y = y || @x // numberOf(y || x) || throw "y number required"
	},
	
	clone = function(){
		return vec2(@x, @y)
	},
	
	round = function(p){
		return vec2(math.round(@x, p), math.round(@y, p))
	},
	
	ceil = function(){
		return vec2(math.ceil(@x), math.ceil(@y))
	},
	
	floor = function(){
		return vec2(math.floor(@x), math.floor(@y))
	},
	
	normalize = function(){
		var len = #this
		if(len > 0){
			@x /= len
			@y /= len
		}
		return this
	},
	
	normalizeTo = function(newLen){
		var len = #this
		if(len > 0){
			var scale = newLen / len
			@x *= scale
			@y *= scale
		}
		return this
	},
	
	normalizeToMax = function(maxLen){
		var len = #this
		if(len > maxLen){
			var scale = maxLen / len
			@x *= scale
			@y *= scale
		}
		return this
	},
	
	__get@rotation = function(){
		return math.atan2(@y, @x)
	},
	__get@angle = function(){
		return math.deg(@rotation)
	},
	
	fromRotation = function(rot){
		return vec2(math.cos(rot), math.sin(rot))
	},
	fromAngle = function(angle){
		return @fromRotation(math.rad(angle))
	},
	
	__len = function(){
		return math.sqrt(@x*@x + @y*@y)
	},
	
	__cmp = function(b){
		b is vec2 || b = vec2(b) // throw "vec2 required"
		var i = @x <=> b.x 
		if(i != 0) return i
		return @y <=> b.y 
	},
	
	__minus = function(){
		return vec2(-@x, -@y)
	},

	__add = function(b){
		b is vec2 && return vec2(@x + b.x, @y + b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x + b, @y + b)
	},
	
	__sub = function(b){
		b is vec2 && return vec2(@x - b.x, @y - b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x - b, @y - b)
	},
	
	__rsub = function(a){
		a = numberOf(a) || throw "number required"
		return vec2(a - @x, a - @y)
	},
	
	__mul = function(b){
		b is vec2 && return vec2(@x * b.x, @y * b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x * b, @y * b)
	},
	
	__div = function(b){
		b is vec2 && return vec2(@x / b.x, @y / b.y)
		b = numberOf(b) || throw "number or vec2 required"
		return vec2(@x / b, @y / b)
	},
	
	__get = function(i){
		if(typeOf(i) === "number"){
			i |= 0 // fast way to convert to int
			return i == 0 ? @x : i == 1 ? @y : throw "error index ${i}, 0 or 1 required"
		}
		return super(i)
	},
	
	__set = function(i, value){
		if(typeOf(i) === "number"){
			i |= 0 // fast way to convert to int
			i == 0 ? @x = value : i == 1 ? @y = value : throw "error index ${i}, 0 or 1 required"
			return
		}
		super(i, value)
	},
}