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

		paused = false,
	},
	
	__construct = function(){
		super()
		@size = stage.size
		
		@levelName = "level-3"
		@view = Sprite().attrs {
			// name = "view",
			priority = 0,
			resAnim = resLevels.getResAnim(@levelName),
			parent = this,
			pos = vec2(0, 0),
			pivot = vec2(0, 0),
			startContentOffs = vec2(0, 0),
		}
		@initLevelPhysics("level-3")
		
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
		var function keyboardEvent(ev){
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
	},
	
	initLevelPhysics = function(name){
		var tiledMap = json.decode(File.readContents(name..".json"))
		// print "tiledMap: "..tiledMap
		
		tiledMap.physics = {}
		for(var i = 0; i < #tiledMap.tilesets; i++){
			var tileset = tiledMap.tilesets[i]
			if(tileset.name == "physics-tiles"){
				tiledMap.physics.tileset = tileset
				tiledMap.physics.firstGid = tileset.firstgid
				// cm.log("[firstPhysGid] "+tiledMap.physics.firstGid);
				break;
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
		var move = (idealPos - pos) * (ev.dt / 0.6)
		// move.x, move.y = math.round(move.x), math.round(move.y)
		
		pos += move
		// pos = cm.roundPoint(pos);
		
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

		// pos.x = math.round(pos.x) // * @view.scaleX);
		// pos.y = math.round(pos.y) // * @view.scaleY);
		
		@view.pos = pos
	},
	
	update = function(ev){
		@updatePhysics(ev.dt)
		@updateCamera(ev)
		@player.update(ev)
	},
}
