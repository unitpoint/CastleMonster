Player = extends Entity {
	__object = {
		originSpeed = 120,
		stopDampingUpdated = true,
		playerSpeedScale = null,
		isShooting = false,
	},

	__construct = function(level){
		super(level)
		@attrs {
			resAnim = res.getResAnim("player-1"),
			parent = level.view,
			pos = vec2(300, 370),
			pivot = vec2(0.5, 0.5),
		}
		@physics = {
			// radiusScale = 0.8,
			// density = 1.0,
			restitution = 0.2,
			// friction = 1.0,
			linearDamping = 0.08,
			stopLinearDamping = 0.03,
			// angularDamping = 1.0,
			categoryBits = PHYS_CAT_BIT_PLAYER,
			ignoreBits = PHYS_CAT_BIT_PLAYER_FIRE | PHYS_CAT_BIT_BLOOD 
				| PHYS_CAT_BIT_MONSTER_SPAWN | PHYS_CAT_BIT_MONSTER_AREA 
				| PHYS_CAT_BIT_PLAYER_SPAWN,
			
			minSpeed: 0,
			maxSpeed: @originSpeed,
			forcePower: 10000 * 3.0,
			
			shapes: [ {						
				radiusScale: 0.8
				// widthScale: 0.6,
				// heightScale: 0.9
			}, {
				radiusScale: 2.5,
				sensor: true,
				density: 0,
				ignoreBits: PHYS_CAT_BIT_ALL & ~PHYS_CAT_BIT_POWERUP
			} /*, {
				radiusScale: 8,
				sensor: true,
				density: 0,
				ignoreBits: cm.physics.CAT_BIT_ALL & ~cm.physics.CAT_BIT_MONSTER
			}*/ ]
		}
		print "ignoreBits: "..@physics.ignoreBits
		level.initEntityPhysics(this)
		@setMaxSpeed(130)
	},
	
	setMaxSpeed = function(value){
		value || throw "setMaxSpeed with null"
		var keys = {
			120 = {
				linearDamping = 0.04,
				forcePower = 10000 * 2.0,
			},
			300 = {
				linearDamping = 0.08,
				forcePower = 10000 * 3.0,
			}
		}
		var selectedKeys = []
		for(var speed, params in keys){
			if(speed <= value && (selectedKeys[0] === null || selectedKeys[0].speed < speed)){
				selectedKeys[0] = {
					speed = speed, 
					linearDamping = params.linearDamping,
					forcePower = params.forcePower,
				}
			}
			if(speed >= value && (selectedKeys[1] === null || selectedKeys[1].speed > speed)){
				selectedKeys[1] = {
					speed = speed, 
					linearDamping = params.linearDamping,
					forcePower = params.forcePower,
				}
			}
		}
		var result = {}
		if(selectedKeys[0] !== null && selectedKeys[1] !== null){
			var a, b = selectedKeys[0], selectedKeys[1]
			if(a.speed == b.speed){
				result = a
			}else{
				var t = (value - a.speed) * 1.0 / (b.speed - a.speed)
				for(var i in a){
					result[i] = a[i] + (b[i] - a[i]) * t
				}
			}
		}else if(selectedKeys[0] !== null){
			result = selectedKeys[0]
		}else{
			result = selectedKeys[1]
		}
		result.speed = value
		print('set player max speed', value, selectedKeys, result, t)
		
		@physics.linearDamping = result.linearDamping
		@physics.maxSpeed = result.speed
		@physics.forcePower = result.forcePower
		
		@linearDamping = 1 - @physics.linearDamping
		@stopDampingUpdated = false
	},
	
	update = function(ev){
		@time = ev.time
		if(@level.moveJoystick.active){
			var dir = @level.moveJoystick.dir
		}else{
			var dx, dy = 0, 0
			if(@level.keyPressed.left) dx--
			if(@level.keyPressed.right) dx++
			if(@level.keyPressed.up) dy--
			if(@level.keyPressed.down) dy++
			if(dx != 0 || dy != 0){
				var dir = vec2(dx, dy).normalize() * 0.8
			}
		}
		if(dir.x != 0 || dir.y != 0){
			if(@stopDampingUpdated){
				@linearDamping = 1 - @physics.linearDamping
				@angularDamping = 1 - @physics.angularDamping
				@stopDampingUpdated = false
			}
			
			if(@playerSpeedScale === null || @playerSpeedScale != @level.playerData.effects.scale.playerSpeed){
				@playerSpeedScale = @level.playerData.effects.scale.playerSpeed
				@setMaxSpeed(@originSpeed * @playerSpeedScale)
				print('player max speed changed', @physics.maxSpeed)
			}
			
			@applyForce(dir * @physics.forcePower)
		}else{
			if(!@stopDampingUpdated){
				@linearDamping = 1 - (@physics.stopLinearDamping || 0.04)
				@angularDamping = 1 - (@physics.stopAngularDamping || 0.04)
				@stopDampingUpdated = true
			}
		}
		// cm.applyActorForce(this, cm.keysToDir().multiply(this.desc.physics.forcePower), undefined, true);
		// cm.log("[player move] force "+this.desc.physics.forcePower+" "+dir.x+" "+dir.y);
		if(#@linearVelocity > 5){				
			@playFootstepSound()
		}
		
		@updateSprite()
	},
	
	playFootstepSound = function(){
	
	},
}
