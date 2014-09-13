DIR_STAY = -1
DIR_UP = 0
DIR_RIGHT_UP = 1
DIR_RIGHT = 2
DIR_RIGHT_DOWN = 3
DIR_DOWN = 4
DIR_LEFT_DOWN = 5
DIR_LEFT = 6
DIR_LEFT_UP = 7

Entity = extends BaseEntity {
	__object = {
		level = null,
		time = 0,
		aimTime = 0,
		aimInverse = false,
		dirIndex = DIR_RIGHT_UP,
		dirChangeTime = 0,
		animSpeed = -1,
		animUpdateHandle = null,
		path = null,
		isRunning = false,
	},
	
	__construct = function(level){
		super()
		@level = level
	},
	
	/* __set@x = function(p){
		super(math.round(p))
	},
	__set@y = function(p){
		super(math.round(p))
	},
	__set@pos = function(p){
		super(vec2(math.round(p.x), math.round(p.y)))
	}, */
	
	playAnim = function(dt, animsMap){
		var animNum, count = 0, #animsMap
		@resAnimFrameNum = animsMap[0]
		@animUpdateHandle && @removeUpdate(@animUpdateHandle)
		@animUpdateHandle = @addUpdate(dt, function(ev){
			animNum = (animNum + 1) % count
			@resAnimFrameNum = animsMap[animNum]
		}.bind(this))
	},
	stopAnim = function(){
		@animUpdateHandle && @removeUpdate(@animUpdateHandle)
		@animUpdateHandle = null
	},

	dirToIndex = function(p){
		var edge = 0.5
		switch((p.x < -edge ? (1<<0) : p.x > edge ? (1<<1) : 0) 
				| (p.y < -edge ? (1<<2) : p.y > edge ? (1<<3) : 0)){
		// case 0: return DIR_DOWN;
		case (1<<0): return DIR_LEFT
		case (1<<1): return DIR_RIGHT
		case (1<<2): return DIR_UP
		case (1<<3): return DIR_DOWN
		case (1<<0)|(1<<2): return DIR_LEFT_UP
		case (1<<1)|(1<<2): return DIR_RIGHT_UP
		case (1<<0)|(1<<3): return DIR_LEFT_DOWN
		case (1<<1)|(1<<3): return DIR_RIGHT_DOWN
		}
		return DIR_STAY
	},
	
	/* updateAnimToDir = function(dir){
		// dir = dir.clone().normalize()
		var dirIndex = @dirToIndex(dir || @linearVelocity.normalize())
		if(dirIndex == DIR_STAY){
			// return
		}
		var maxAnimSpeed = @physics.maxSpeed
		var animSpeed = math.min(#@linearVelocity, maxAnimSpeed) / maxAnimSpeed
		// print "speed: ${@speed}, ${animSpeed}"
		
		var dirChanged = @dirIndex != dirIndex
		if(!dirChanged && @animSpeed == animSpeed){
			return
		}
		@dirIndex, @animSpeed = dirIndex, animSpeed
		var dt = math.min(0.2, 0.1 / animSpeed)
		if(dirChanged){
			@playAnim(dt, [dirIndex*2+0, dirIndex*2+1])
		}else{
			@animUpdateHandle.interval = dt
		}
	}, */
	
	updateSprite: function(){
		var level = @level
		if(level.paused){
			return
		}
		
		var speedEdge = 4
		var speed = #@linearVelocity
		var isRunning = speed >= 2*speedEdge && @isAwake
		// print "speed: ${speed}, isRunning: ${isRunning}"
		
		var dirIndex = DIR_STAY
		/* if(this == level.player && @isShooting){
			dirIndex = cm.dirToIndex(level.aimData.targetDir)
			@dirChangeTime = 0
		}else if(!isRunning){
			// dirIndex = cm.DIR_DOWN;
		}else{
			// dirIndex = cm.dirToIndex(p);
		} */
		if(dirIndex == DIR_STAY){
			if(this != level.player && !@path && @time - @aimTime < 0.6){ // actor.desc.physics.aimMoveOnly){ // && speed < actor.desc.physics.maxSpeed * 0.7){
				print("[aimMoveOnly] speed: ${speed}, max: ${@physics.maxSpeed}");
				var dir = (level.player.pos - @pos).normalize()
				if(@aimInverse){
					dir = -dir
				}
				dirIndex = @dirToIndex(dir)
				@dirChangeTime = 0
			}else{
				var dir = @linearVelocity
				if(speed > 0){
					dir = dir * (1.0 / speed)
				}
				dirIndex = @dirToIndex(dir)
				// print("[dir] ${math.round(dir.x, 3)} ${math.round(dir.y, 3)}, new: ${dirIndex}, speed: ${speed}");
				switch(dirIndex){
				case DIR_UP:
					if(speed <= 3*speedEdge){
						dirIndex = DIR_LEFT_DOWN
						// print "dir updated: DIR_LEFT_DOWN: ${speed} <= ${3*speedEdge}"
					}else if(speed <= 5*speedEdge){
						dirIndex = DIR_LEFT
						// print "dir updated: DIR_LEFT: ${speed} <= ${5*speedEdge}"
					}else if(speed <= 10*speedEdge){
						dirIndex = DIR_LEFT_UP
						// print "dir updated: DIR_LEFT_UP: ${speed} <= ${10*speedEdge}"
					}
					break;
					
				case DIR_LEFT_UP:
					if(speed <= 5*speedEdge){
						dirIndex = DIR_LEFT_DOWN
						// print "dir updated: DIR_LEFT_DOWN: ${speed} <= ${5*speedEdge}"
					}
					break;
					
				case DIR_RIGHT_UP:
					if(speed <= 5*speedEdge){
						dirIndex = DIR_RIGHT_DOWN
						// print "dir updated: DIR_RIGHT_DOWN: ${speed} <= ${5*speedEdge}"
					}
					break;
				}
			}
		}
		/* if(actor.dirIndex === undefined){
			actor.dirIndex = cm.DIR_DOWN;
			actor.isRunning = null;
		} */
		if(this == level.player){
			switch(dirIndex){
			case DIR_LEFT:
				dirIndex = DIR_LEFT_DOWN
				break
			case DIR_RIGHT:
				dirIndex = DIR_RIGHT_DOWN
				break
			}
		}
		if(dirIndex == DIR_STAY || @time - @dirChangeTime < 0.2){
			// print "use cur dirIndex: ${@dirIndex}, time: ${@time}, dirChangeTime: ${@dirChangeTime}"
			dirIndex = @dirIndex
		}
		if(@dirIndex != dirIndex || @isRunning != isRunning){
			/* if(actor == level.player && actor.dirIndex != dirIndex){
				// cm.log("new dir: "+dirIndex);
			} */
			var diff = dirIndex - @dirIndex
			if(math.abs(diff) > 1){
				var savePreIndex = dirIndex
				if(diff > 0){
					var diff2 = diff - 8
				}else{
					var diff2 = diff + 8
				}
				if(math.abs(diff) <= math.abs(diff2)){
					dirIndex = @dirIndex + (diff < 0 ? -1 : 1)
				}else{
					dirIndex = @dirIndex + (diff2 < 0 ? -1 : 1)
				}
				dirIndex = (dirIndex + 8) % 8
			}
			
			@dirChangeTime = @time
			@dirIndex = dirIndex
			@isRunning = isRunning
			
			if(isRunning){
				// print "play new dirIndex: ${dirIndex}"
				@playAnim(0.2, [dirIndex*2+0, dirIndex*2+1])
			}else{
				// print "play new dirIndex: ${dirIndex}"
				@stopAnim()
				// @resAnimFrameNum = dirIndex*2 + math.round(math.random())
			}
		}
	},	
}
