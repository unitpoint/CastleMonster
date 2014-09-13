var enumVal = 0
LAYER = {
	UNDER_FLOOR 		= enumVal++, 
	FLOOR 				= enumVal++,
	MONSTER_SPAWN_AREA 	= enumVal++,
	PLAYER_SPAWN_AREA 	= enumVal++,
	FLOOR_DECALS 		= enumVal++,
	WALLS 				= enumVal++,
	PATH 				= enumVal++,
	BLOOD 				= enumVal++,
	POWERUPS 			= enumVal++,
	MONSTERS 			= enumVal++,
	PLAYER 				= enumVal++,
	MONSTER_BULLETS 	= enumVal++,
	EFFECTS 			= enumVal++,
	PHYSICS_DEBUG 		= enumVal++,
	COUNT 				= enumVal				
}

ITEM_TYPE_WEAPON = 1
ITEM_TYPE_ARTEFACT = 2
ITEM_TYPE_RESOURCE = 3
ITEM_TYPE_OBJECT = 4
ITEM_TYPE_MONSTER = 5
ITEM_TYPE_ACHIEVEMENT = 6
ITEM_TYPE_ARMOR = 7
ITEM_TYPE_MEDAL = 8
			
GameLevel = extends BaseGameLevel {
	__object = {
		playerData = {
			money = 0,
			meat = 0,
			enemyKilled = 0,
			
			health = 100,
			armor = 100,
			
			healthDamaged = 0,
			armorDamaged = 0,
			// damaged = 0,
			damagedTime = 0,
			
			healthRecovered = 0,
			armorRecovered = 0,

			healthRecoverMeatUsed = 0,
			armorRecoverMoneyUsed = 0,
			
			armorItem = null,
			defaultWeaponItem = null,
			
			// items = {},
			originItems = {},
			itemsById = {},
			itemsByNameId = {},
			itemsByTypeId = {},
			
			killedCountById = {},
			collectedCountById = {},
			usedCountById = {},
			
			startTimeSec = 0,
			playTimeSec = 0,
			daysCompleted = 0,
			
			activeItems = {},
			activeArtefacts = {},
			activeArmors = {},
			activeWeapons = {},
			
			effects = {
				scale = {
					weaponDamage = 1.0,
					weaponFrequency = 1.0,
					weaponSpeed = 1.0,
					weaponDensity = 1.0,
					playerArmor = 1.0,
					playerHealth = 1.0,
					playerSpeed = 1.0,
					monsterHealth = 1.0,
					monsterSpeed = 1.0
				},
				weaponFireType = 0
			}
		},
		wave: {
			time: 0,
			num: 0,
			phase: 0,
			maxAliveMonsters: 0,
			completed: false,
			params: null,
			phaseParams: null,
			phaseMonsters: 0,
			phaseMonstersSpawned: 0
		},
		wavePhaseMonstersSpawned = 0,
		time = 0,

		paused = false,
	},
	
	__construct = function(p_level, p_invasion, p_day){
		super()
		@size = stage.size
		
		@params = {
			// dayParams = dayParams,
			level = p_level,
			invasion = p_invasion,
			day = p_day,
		}
		
		@levelName = "level-"..@params.level
		@view = Sprite().attrs {
			// name = "view",
			priority = 0,
			resAnim = resLevels.getResAnim(@levelName),
			parent = this,
			pos = vec2(0, 0),
			pivot = vec2(0, 0),
			startContentOffs = vec2(0, 0),
		}
		@debugDraw = DEBUG // @view must be already created
		
		@layers = []
		for(var i = 0; i < LAYER.COUNT; i++){
			@layers[] = Actor().attrs {
				priority = i,
				parent = @view,
			}
		}
		
		@initLevelPhysics()
		
		@ui = Actor().attrs {
			priority = 10,
			parent = this,
			pos = vec2(0, 0),
			pivot = vec2(0, 0),
			size = @size
		}
		
		@moveJoystick = Joystick().attrs {
			// priority = 0,
			parent = @ui,
			pivot = vec2(0, 1),
			pos = vec2(0, @height),
		}
		
		@player = Player(this)
		
		// @player.playAnim(0.2, [0, 1])
		// @movePlayer(vec2(0.001, 0.001))
		
		@keyPressed = {}
		var keyboardEvent = function(ev){
			var pressed = ev.type == KeyboardEvent.DOWN
			if(ev.scancode == KeyboardEvent.SCANCODE_LEFT || ev.scancode == KeyboardEvent.SCANCODE_A){
				@keyPressed.left = pressed
				return
			}
			if(ev.scancode == KeyboardEvent.SCANCODE_RIGHT || ev.scancode == KeyboardEvent.SCANCODE_D){
				@keyPressed.right = pressed
				return
			}
			if(ev.scancode == KeyboardEvent.SCANCODE_UP || ev.scancode == KeyboardEvent.SCANCODE_W){
				@keyPressed.up = pressed
				return
			}
			if(ev.scancode == KeyboardEvent.SCANCODE_DOWN || ev.scancode == KeyboardEvent.SCANCODE_S){
				@keyPressed.down = pressed
				return
			}
		}
		@addEventListener(KeyboardEvent.DOWN, keyboardEvent.bind(this))
		@addEventListener(KeyboardEvent.UP, keyboardEvent.bind(this))
		
		@addUpdate(@update.bind(this))
		
		// @activateItem(@playerData.defaultWeaponItem)
		// @activateItem(@playerData.armorItem)
		
		@loadItems()
		
		var dayParams = @getDayParams(@params.level, @params.invasion, @params.day)
		print "loaded dayParams: "..dayParams
		@applyDayParams(dayParams)
		@startWave(@params.day, 0)
	},
	
	getDayParams = function(level, invasion, day){
		return DayParams(level, invasion, day).day_params
	},
	
	getMonsterByName = function(nameId){
		nameId = 'ITEM_MONSTER_'..nameId.upper()
		var item = @playerData.itemsByNameId[nameId]
		if(item && item.actorParams){
			print('getMonsterByName '..nameId..' - found')
			return item.actorParams
		}
		print('getMonsterByName '..nameId..' - NOT FOUND')
	},

	getItemByName = function(nameId){
		nameId = 'ITEM_'..nameId.upper()
		var item = cm.playerData.itemsByNameId[nameId]
		if(item && item.actorParams){
			print('getItemByName '..nameId..' - found')
			return item.actorParams
		}
		print('getItemByName '..nameId..' - NOT FOUND')
	},
	
	loadItems = function(){
		print "begin load items"
		@playerData.originItems = json.decode(File.readContents("items.json"))
		
		var playerData, f = @playerData, toNumber
		
		playerData.itemsById = {}
		playerData.itemsByNameId = {}
		playerData.itemsByTypeId = {}
		// playerData.usedItemsById = {}
		
		playerData.killedCountById = {}
		playerData.collectedCountById = {}
		playerData.usedCountById = {}
		
		// playerData.activeArmorItem = null
		// playerData.activeWeaponItem = null
		
		for(var type_id, originItems in playerData.originItems){
			for(var id, originItem in originItems){
				id = f(id)
				var item = {
					originItem = originItem,
					id = f( originItem['id'] ),
					typeId = f( originItem['type_id'] ),
					durationMS = f( originItem['action_time'] ) * 1000,
					nameId = originItem['name_id'],
					name = originItem['name'],
					desc = originItem['desc'],
					imageId = originItem['image'],
					data = originItem['data'] || {'sounds' = {}},
					count = f( originItem['count'] ),
					ingame = f( originItem['ingame'] ) * 1000
				}								
				playerData.itemsById[id] = item
				playerData.itemsByNameId[item.nameId] = item
				
				if(playerData.itemsByTypeId[item.typeId] === null){
					playerData.itemsByTypeId[item.typeId] = {}
				}
				playerData.itemsByTypeId[item.typeId][id] = item
				
				// loadImages.push({id:item.nameId, url:item.imageId})
				// cm.log('item', type_id, item['type_id'], cm.consts['ITEM_TYPE_MONSTER'], item['image_url'])
				
				var data = item.data
				/* for(var i, sounds in data['sounds']){
					cm.each(sounds, function(i, sound){
						loadSounds.push(sound)
					})
				} */
				
				if(type_id == ITEM_TYPE_MONSTER){
					item.actorParams = {
						itemId = item.id,
						nameId = item.nameId,
						image = {
							id =  item.imageId,
						},
						health = f( originItem['health'] ),
						fire = {
							weaponId = f( originItem['weapon_id'] ),
							damage = f( originItem['health'] ) / 10,
							density = f( originItem['density'] ) / 2,
							speed = f( originItem['speed'] ) * 1.5
							/*
							damage = f( originItem['weapon_damage'] ),
							density = f( originItem['weapon_density'] ),
							speed = f( originItem['weapon_speed'] ),
							*/
						},
						sounds = {
							pain = data['sounds']['pain'],
							death = data['sounds']['death'],
							idle = data['sounds']['idle']
						},
						physics = {
							maxSpeed = f( originItem['speed'] ),
							minSpeed = f( originItem['speed'] ) / 3,
							density = f( originItem['density'] ),
							forcePower = f( originItem['power'] ),
							inversePower = f( originItem['power'] ) * 1.5,
							fly = f( originItem['fly'] ) != 0,
						}
					}
					if(data['physics']){
						var physics = item.actorParams.physics
						if(data['physics']['radiusScale']){
							physics.radiusScale = f( data['physics']['radiusScale'] )
						}
						if(data['physics']['aimOnDamage']){
							physics.aimOnDamage = data['physics']['aimOnDamage']
						}
						if(data['physics']['aimIntervalSec']){
							physics.aimIntervalSec = data['physics']['aimIntervalSec']
						}
						if(data['physics']['aimDurationSec']){
							physics.aimDurationSec = data['physics']['aimDurationSec']
						}
						if(data['physics']['pathWalkDurationSec']){
							physics.pathWalkDurationSec = data['physics']['pathWalkDurationSec']
						}
						if(data['physics']['inverseDurationSec']){
							physics.inverseDurationSec = data['physics']['inverseDurationSec']
						}
					}
					// cm.log('SETUP MONSTER', item)
				}else if(type_id == ITEM_TYPE_ARTEFACT){
					item.weapon_damage_p = f( originItem['weapon_damage_p'] )
					item.weapon_frequency_p = f( originItem['weapon_frequency_p'] )
					item.weapon_speed_p = f( originItem['weapon_speed_p'] )
					item.weapon_density_p = f( originItem['weapon_density_p'] )
					item.weapon_fire_type = originItem['weapon_fire_type']
					item.player_armor_p = f( originItem['player_armor_p'] )
					item.player_health_p = f( originItem['player_health_p'] )
					item.player_speed_p = f( originItem['player_speed_p'] )
					item.monster_health_p = f( originItem['monster_health_p'] )
					item.monster_speed_p = f( originItem['monster_speed_p'] )
					item.actorParams = {
						itemId = item.id,
						nameId = item.nameId,
						image = {
							id =  item.imageId,
						}
					}
				}else if(type_id == ITEM_TYPE_WEAPON){
					item.frequency = f(originItem['frequency'] || 2)
					item.actorParams = {
						itemId = item.id,
						nameId = item.nameId,
						image = {
							id =  item.imageId,
						},
						damage = math.max(1, f( originItem['damage'] )),
						damageCount = math.max(1, f( originItem['damage_count'] )),
						traceCount = math.max(1, f( originItem['trace_count'] )),
						through = f( originItem['through'] ),
						coverPercentage = clamp( f( originItem['cover_p'] ), 2, 100 ),
						sounds = {
							shot = data['sounds']['shot']
						},
						physics = {
							speed = math.max(10, f( originItem['speed'] )),
							density = math.max(0.1, f( originItem['density'] ))
						}
					}
					
					/* if(!playerData.activeWeaponItem || playerData.activeWeaponItem['ingame'] < item['ingame']){
						playerData.activeWeaponItem = item
					} */
				}else if(type_id == ITEM_TYPE_ARMOR){
					item.player_armor_p = f( originItem['player_armor_p'] )
					item.player_speed_p = f( originItem['player_speed_p'] )
					/* if(!playerData.activeArmorItem || playerData.activeArmorItem.ingame < item.ingame){
						playerData.activeArmorItem = item
					} */
					item.actorParams = {
						itemId = item.id,
						nameId = item.nameId,
						image = {
							id =  item.imageId,
						}
					}
				}
			}
		}
		print('end load items', @playerData.itemsByTypeId)
		// this.nextStep()
	},
	
	_physBlocks = null,
	getPhysTypeBlocks = function(type){
		return @_physBlocks[type] || @{
			@_physBlocks && throw "there is no type: ${type}"
			@_physBlocks = {}
			var count = @physBlockCount
			for(var i = 0; i < count; i++){
				var p = @getPhysBlock(i)
				;(@_physBlocks[p.type] || @_physBlocks[p.type] = [])[] = p
			}
			print "loadPhysBlocks: ${@_physBlocks}"
			return @_physBlocks[type] || throw "there is no type: ${type}"
		}
	},
	
	excludedSpawnAreas = [],
	findBestSpawnArea = function(pos){
		pos || pos = @player.pos
		// cm.log("[findBestSpawnArea] pos "+pos.x+" "+pos.y);
		var bestSpawnArea = null
		var bestDist, bestNum = 999999999, -1
		var list = @getPhysTypeBlocks(PHYS_MONSTER_SPAWN)
		var count = #list
		var maxExcludedCount = math.ceil(math.min(count-1, math.max(2, @wave.day/5.0 + @params.invasion-1)))
		while(#@excludedSpawnAreas > maxExcludedCount){
			@excludedSpawnAreas.shift()
		}
		var excludedCount = #@excludedSpawnAreas
		for(var i = 0; i < count; i++){
			var area = list[i]
			
			var isExcludedArea = false;
			for(var j = 0; j < excludedCount; j++){
				if(area == @excludedSpawnAreas[j]){
					isExcludedArea = true
					break;
				}
			}
			if(isExcludedArea){
				continue
			}
			
			var points = [
						  area.pos + area.size/2,
						  /*
						  new cm.Point(area.x, area.y),
						  new cm.Point(area.x + area.width, area.y),
						  new cm.Point(area.x, area.y + area.height),
						  new cm.Point(area.x + area.width, area.y + area.height)
						  */
						  ];
			for(var j = 0; j < #points; j++){
				var p = points[j] - pos
				// cm.log("[findBestSpawnArea] pos: "+pos.x+" "+pos.y);
				var dist = p.x*p.x + p.y*p.y
				if(bestDist > dist){
					bestDist = dist
					bestNum = i
					bestSpawnArea = area
				}
				// cm.log("[findBestSpawnArea] i "+i+", p: "+p.x+" "+p.y+", dist "+dist+", best "+bestNum+", dist "+bestDist);
			}
		}
		print("[findBestSpawnArea] areas "..#list..", best "..bestNum..", dist "..bestDist)
		@excludedSpawnAreas[] = bestSpawnArea
		return bestSpawnArea
	},
	
	startWave = function(day, phase, maxAliveMonsters){
		print "startWave"
		@wave.time = @time
		@wave.day = day
		@wave.phase = phase
		@wave.completed = false
		@wave.phaseParams = @wave.params.phases[ phase % @wave.params.phases.length ]
		@wave.maxAliveMonsters = math.round(maxAliveMonsters || @wave.phaseParams.maxAliveMonsters 
			|| @wave.params.maxAliveMonsters || 10)
		@wave.phaseMonsters = math.round(@wave.phaseParams.count || 1)
		@wave.phaseMonstersSpawned = 0
		
		@spawnWaveMonsters()
		
		if(phase == 0){
			@playerData.startTimeSec = @time // getTimeSec()
			@playerData.playTimeSec = 0
			@playerData.killedCountById = {}
			@playerData.collectedCountById = {}
			@playerData.usedCountById = {}

			@playerData.healthDamaged = math.max(0, @playerData.healthDamaged - @playerData.healthRecovered)
			@playerData.healthRecovered = 0
			
			@playerData.armorDamaged = math.max(0, @playerData.armorDamaged - @playerData.armorRecovered)
			@playerData.armorRecovered = 0

			@playerData.meat = math.max(0, math.round(@playerData.meat - @playerData.healthRecoverMeatUsed))
			@playerData.healthRecoverMeatUsed = 0
			
			@playerData.money = math.max(0, math.round(@playerData.money - @playerData.armorRecoverMoneyUsed))
			@playerData.armorRecoverMoneyUsed = 0
			
			// cm.callbacks['dayStarted'](@params.level, @params.invasion, @wave.day)
		}
	},

	randAreaPos = function(area, edge){
		edge || edge = 0
		var x = math.random(area.pos.x - edge, area.pos.x + area.size.x + edge)
		var y = math.random(area.pos.y - edge, area.pos.y + area.size.y + edge)
		return vec2(x, y)
	},
	
	monsterSide = 0,
	useMonstersBattle = false,
	spawnMonster: function(params, spawnArea){
		spawnArea || spawnArea = @findBestSpawnArea()
		@useMonstersBattle && @monsterSide = (@monsterSide + 1) % 2
		// print "spawnMonster: ${params}, pos: ${@randAreaPos(spawnArea, -10)}"
		// return;
		
		var m = Monster(this, extend({
			pos = @randAreaPos(spawnArea, -10),
			battleSide = @useMonstersBattle ? @monsterSide : false
		}, params))
		/*
		cm.log("[spawnMonster] xy: "+x+" "+y
				+", m-xy:"+m.x+" "+m.y
				+", "+spawnArea.x+" "+spawnArea.y+" "+spawnArea.width+" "+spawnArea.height);
		*/
		// @layers[LAYER.MONSTERS].addChild(m)
	},
	
	spawnWaveMonsters = function(){
		if(@wave.completed){
			return false
		}
		var count = math.min(@wave.phaseMonsters - @wave.phaseMonstersSpawned, 
				@wave.maxAliveMonsters - #@layers[LAYER.MONSTERS])
		print "spawnWaveMonsters: ${count}, wave: ${@wave}"
		if(count > 0){				
			if(@wave.phaseMonsters >= 10 
				&& @wave.phaseMonsters - @wave.phaseMonstersSpawned - count <= 1)
			{
				count = @wave.phaseMonsters - @wave.phaseMonstersSpawned
			}
		
			var spawnArea = @findBestSpawnArea() // undefined, @prevSpawnArea)
			// @prevSpawnArea.push(spawnArea)
			
			// cm.log('[spawnWaveMonsters] clone @wave.phaseParams.monster')
			var monster
			var spawnRandMonster = @wave.phaseParams.monster[0] !== null
			count = math.min(5, count)
			for(var i = 0; i < count; i++){
				if(i == 0 || spawnRandMonster){
					if(spawnRandMonster){
						monster = randItem(@wave.phaseParams.monster).clone()
					}else{
						monster = @wave.phaseParams.monster.clone()
					}			
					
					monster.health *= @wave.params.monsterHealthScale || 1
					
					var speedScale = @wave.params.monsterSpeedScale || 1
					monster.physics.minSpeed = (monster.physics.minSpeed || 20) * speedScale
					monster.physics.maxSpeed = (monster.physics.maxSpeed || 100) * speedScale
					
					monster.fire.damage *= @wave.params.monsterFireDamageScale || 1
					monster.fire.speed *= @wave.params.monsterFireBulletSpeedScale || 1
					monster.fire.density *= @wave.params.monsterFireBulletDensityScale || 1
					
					if(@wave.params.monsterAimOnDamage){
						if((monster.physics.aimOnDamage = @wave.params.monsterAimOnDamage) === true){
							// delete monster.physics.inverseDurationSec
						}
					}
				}
				@spawnMonster(monster, spawnArea)
				@wavePhaseMonstersSpawned++
				// cm.log("[wave spawn step] spawned "+@wavePhaseMonstersSpawned)
			}
			@wave.phaseMonstersSpawned += count

			print("[wave spawn] "..@wave.day.." "..@wave.phase
					..", need "..@wave.phaseMonsters
					..", cur spawned "..count
					..", all spawned "..@wave.phaseMonstersSpawned
					..", exist "..#@layers[LAYER.MONSTERS]
					..", max "..@wave.maxAliveMonsters
					)
			
			@wave.completed = @wave.phaseMonstersSpawned >= @wave.phaseMonsters
			return count 
		}
		return 0
	},
	
	applyDayParams = function(dayParams){
		// if(!dayParams) dayParams = {}

		var params
		params = @wave.params = {} // @waveParams[0]
		params.meatPerHealth = math.max(1, dayParams['meat_per_health'] || 1)
		params.moneyPerArmor = math.max(1, dayParams['money_per_armor'] || 1)
		params.maxAliveMonsters =  dayParams['max_alive_monsters']
		params.monsterFireMaxBullets =  dayParams['monster_fire_max_bullets']
		params.monsterFireMinDistance =  dayParams['monster_fire_min_distance']
		params.monsterFireIntervalSec =  dayParams['monster_fire_interval_sec']
		params.monsterFireFrequencyScale =  dayParams['monster_fire_frequency_scale']
		params.monsterFireDamageScale =  dayParams['monster_fire_damage_scale']
		params.monsterFireBulletSpeedScale =  dayParams['monster_fire_bullet_speed_scale']
		params.monsterFireBulletDensityScale =  dayParams['monster_fire_bullet_density_scale']
		params.monsterSpeedScale =  dayParams['monster_speed_scale']
		params.monsterHealthScale =  dayParams['monster_health_scale']
		params.monsterAimOnDamage = dayParams['monster_aim_on_damage']
		
		params.phases = []
		for(local i, dayPhase in dayParams['phases']){
			var phase = {}
			
			phase.count = dayPhase['count']
			phase.maxAliveMonsters = dayPhase['max_alive_monsters']
			
			if(dayPhase['next']){
				phase.next = {
					delaySec = dayPhase['next']['delay_sec'],
					aliveMonsters = dayPhase['next']['alive_monsters']
				}					
			}
			
			if(dayPhase['monster'] is String){
				phase.monster = [ @getMonsterByName(dayPhase['monster']) ]
			}else{
				phase.monster = []
				for(local i, name in dayPhase['monster']){
					phase.monster[] = @getMonsterByName(name)
				}
			}
			params.phases[] = phase
		}

		print('day params ', params)
	},
	
	initLevelPhysics = function(name){
		var tiledMap = json.decode(File.readContents("level-${@params.level}.json"))
		// print "tiledMap: "..tiledMap
		
		tiledMap.physics = {}
		for(var i = 0; i < #tiledMap.tilesets; i++){
			var tileset = tiledMap.tilesets[i]
			if(tileset.name == "physics-tiles"){
				tiledMap.physics.tileset = tileset
				tiledMap.physics.firstGid = tileset.firstgid
				// cm.log("[firstPhysGid] "+tiledMap.physics.firstGid)
				break
			}
		}
		for(var i = 0; i < #tiledMap.layers; i++){
			var layer = tiledMap.layers[i]
			layer.name == "physics" || continue
			tiledMap.physics.layer = layer
			var width = layer.width
			var height = layer.height
			@setPhysSize(width, height)
			for(var x = 0; x < width; x++){
				for(var y = 0; y < height; y++){
					var gid = layer.map[x][y]
					if(gid){
						gid = gid - tiledMap.physics.firstGid
						/* switch(gid){
						case 0: // water
						case 1: // solid
						case 2: // player spawn
						case 3: // monster spawn
						} */ 
						@setPhysCell(x, y, gid)
					}
				}
			}
		}
		@createPhysicsWorld(@view.size)
	},
	
	updateCamera: function(ev){
		var idealPos = @size / 2 - @player.pos
		var pos = @view.pos
		var move = (idealPos - pos) * ev.dt
		// move.x, move.y = math.round(move.x), math.round(move.y)
		
		pos += move
		// pos = cm.roundPoint(pos)
		
		var maxOffs = @width * 0.05 // math.round(@width * 0.05)
		if(idealPos.x - pos.x > maxOffs){
			pos.x = idealPos.x - maxOffs
		}else if(idealPos.x - pos.x < -maxOffs){
			pos.x = idealPos.x + maxOffs
		}
		if(idealPos.y - pos.y > maxOffs){
			pos.y = idealPos.y - maxOffs
		}else if(idealPos.y - pos.y < -maxOffs){
			pos.y = idealPos.y + maxOffs
		}
		
		if(@view.width <= @width){
			pos.x = (@width - @view.width) / 2
		}else
		if(pos.x > -@view.startContentOffs.x){
			pos.x = -@view.startContentOffs.x
		}else if(pos.x + @view.width < @width){
			pos.x = @width - @view.width
		}
		if(@view.height <= @height){
			pos.y = (@height - @view.height) / 2
		}else 
		if(pos.y > -@view.startContentOffs.y){
			pos.y = -@view.startContentOffs.y
		}else if(pos.y + @view.height < @height){
			pos.y = @height - @view.height
		}

		// pos.x = math.round(pos.x) // * @view.scaleX)
		// pos.y = math.round(pos.y) // * @view.scaleY)
		
		@view.pos = pos // vec2(math.round(pos.x), math.round(pos.y))
	},
	
	update = function(ev){
		@updatePhysics(ev.dt)
		@updateCamera(ev)
		@player.update(ev)
	},
}
