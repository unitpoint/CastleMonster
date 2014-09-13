Monster = extends Entity {
	__construct = function(level, params){
		super(level)
		
		params = extend({
			pos = { x = level.view.width * math.random(0.2, 0.8), y = level.view.height * math.random(0.2, 0.8) },
			image = {
				size = [ 20, 1 ],
				ms = 200
			},
			sounds = {
				pain = [ "unknown" ], // "temp/paind", "temp/painb", "temp/giant1", "temp/giant2", "temp/giant3", "temp/giant4" ],
				death = [ "unknown" ], // "temp/deathe", "temp/deathb", "temp/deathd" ],
				idle = [ "unknown" ]
			},
			health = 10,
			fire = {
				day = 1,
				weapon = 5,
				frequency = 1000.0 / 500.0,
				speed = 100,
				damage = 10,
				density = 0.5
			},
			physics = {
				radiusScale = 1,
				density = 1.0,
				minSpeed = 20,
				maxSpeed = 100,
				linearDamping = 0,
				// friction = 0.5,
				restitution = 0.2,
				forcePower = 1000,
				inversePower = 2000,
				stopPercent = 10,
				stopDurationMS = [1.0, 3.0],
				aimOnDamage = "inverse",
				aimIntervalSec = 3.0,
				aimDurationSec = [2.0, 4.0],
				pathWalkDurationSec = [20.0, 30.0],
				inverseDurationSec = [2.0, 4.0],
				categoryBits = PHYS_CAT_BIT_MONSTER,
				ignoreBits = PHYS_CAT_BIT_POWERUP | PHYS_CAT_BIT_BLOOD
					| PHYS_CAT_BIT_MONSTER_SPAWN | PHYS_CAT_BIT_MONSTER_AREA 
					| PHYS_CAT_BIT_PLAYER_SPAWN,
			}
		}, params)
		
		print "spawnMonster: ${params}"
		
		@attrs {
			resAnim = res.getResAnim(params.image.id),
			parent = level.layers[LAYER.MONSTERS],
			pos = params.pos,
			pivot = vec2(0.5, 0.5),
			physics = params.physics,
		}
		
		level.initEntityPhysics(this)
	},
}
