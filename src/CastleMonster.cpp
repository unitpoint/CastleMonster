#include "CastleMonster.h"

// =====================================================================

namespace ObjectScript
{

// =====================================================================

static void registerBaseGameGlobals(OS * os)
{
	struct Lib {
	};

	OS::FuncDef funcs[] = {
		{}
	};

	OS::NumberDef nums[] = {
		DEF_CONST(TO_PHYS_SCALE),
		DEF_CONST(PHYS_CAT_BIT_PLAYER),
		DEF_CONST(PHYS_CAT_BIT_MONSTER),
		DEF_CONST(PHYS_CAT_BIT_PLAYER_FIRE),
		DEF_CONST(PHYS_CAT_BIT_PLAYER_SENSOR),
		DEF_CONST(PHYS_CAT_BIT_POWERUP),
		DEF_CONST(PHYS_CAT_BIT_STATIC),
		DEF_CONST(PHYS_CAT_BIT_WATER),
		DEF_CONST(PHYS_CAT_BIT_MONSTER_AREA),
		DEF_CONST(PHYS_CAT_BIT_ENVIRONMENT),
		DEF_CONST(PHYS_CAT_BIT_MONSTER_FIRE),
		DEF_CONST(PHYS_CAT_BIT_BLOOD),
		DEF_CONST(PHYS_CAT_BIT_MONSTER_SPAWN),
		DEF_CONST(PHYS_CAT_BIT_PLAYER_SPAWN),
		DEF_CONST(PHYS_CAT_BIT_ALL),
		DEF_CONST(PHYS_WATER),
		DEF_CONST(PHYS_SOLID),
		DEF_CONST(PHYS_PLAYER_SPAWN),
		DEF_CONST(PHYS_MONSTER_SPAWN),
		DEF_CONST(PHYS_DEF_FIXED_ROTATION),
		DEF_CONST(PHYS_DEF_LINEAR_DAMPING),
		DEF_CONST(PHYS_DEF_ANGULAR_DAMPING),
		DEF_CONST(PHYS_DEF_DENSITY),
		DEF_CONST(PHYS_DEF_RESTITUTION),
		DEF_CONST(PHYS_DEF_FRICTION),
		{}
	};
	os->pushGlobals();
	os->setFuncs(funcs);
	os->setNumbers(nums);
	os->pop();
}
static bool __registerBaseGameGlobals = addRegFunc(registerBaseGameGlobals);

// =====================================================================

static void registerBaseGameLevel(OS * os)
{
	struct Lib {
		static BaseGameLevel * __newinstance()
		{
			return new BaseGameLevel();
		}

		static int findPath(OS * os, int params, int, int, void*)
		{
			OS_GET_SELF(BaseGameLevel*);
			if(params < 7){
				os->setException("7 arguments required");
				os->handleException();
				return 0;
			}
			if(!os->isFunction(-params+6)){
				os->setException("seven argument must be function");
				os->handleException();
				return 0;
			}
			// Vector2 p1 = ObjectScript::CtypeValue<Vector2>::getArg(os, -params+0);
			// Vector2 p2 = ObjectScript::CtypeValue<Vector2>::getArg(os, -params+1);
			// BaseEntity * ent = ObjectScript::CtypeValue<BaseEntity*>::getArg(os, -params+2);
			int x1 = os->toInt(-params+0);
			int y1 = os->toInt(-params+1);
			int x2 = os->toInt(-params+2);
			int y2 = os->toInt(-params+3);
			bool fly = os->toBool(-params+4);
			bool allowNotFinishedPath = os->toBool(-params+5);
			int callbackOSValueId = os->getValueId(-params+6);
			
			if(os->isExceptionSet()){ // check for error arguments
				os->handleException();
				return 0;
			}
			self->findPath(x1, y1, x2, y2, fly, allowNotFinishedPath, callbackOSValueId);
			return 0;
		}
		
		static int entityPosToTile(OS * os, int params, int, int need_ret_values, void*)
		{
			OS_GET_SELF(BaseGameLevel*);
			int x, y;
			Vector2 pos = ObjectScript::CtypeValue<Vector2>::getArg(os, -params+0);
			self->entityPosToTile(pos, x, y);
			if(need_ret_values < 2){
				os->setException("two return values required");
				return 0;
			}
			os->pushNumber(x);
			os->pushNumber(y);
			return 2;
		}
	};

	OS::FuncDef funcs[] = {
		def("__newinstance", &Lib::__newinstance),
		def("createPhysicsWorld", &BaseGameLevel::createPhysicsWorld),
		def("updatePhysics", &BaseGameLevel::updatePhysics),
		def("setTileWorldSize", &BaseGameLevel::setTileWorldSize),
		def("setTile", &BaseGameLevel::setTile),
		def("initEntityPhysics", &BaseGameLevel::initEntityPhysics),
		def("destroyEntityPhysics", &BaseGameLevel::destroyEntityPhysics),
		DEF_PROP("debugDraw", BaseGameLevel, DebugDraw),
		DEF_GET("tileAreaCount", BaseGameLevel, TileAreaCount),
		def("getTileArea", &BaseGameLevel::getTileArea),
		DEF_GET("findPathInProgress", BaseGameLevel, FindPathInProgress),
		def("updatePath", &BaseGameLevel::updatePath),
		{"findPath", &Lib::findPath},
		{"entityPosToTile", &Lib::entityPosToTile},
		def("tileToEntityPos", &BaseGameLevel::tileToEntityPos),
		def("traceEntities", &BaseGameLevel::traceEntities),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<BaseGameLevel, Actor>(os, funcs, nums, true OS_DBG_FILEPOS);
}
static bool __registerBaseGameLevel = addRegFunc(registerBaseGameLevel);

// =====================================================================

static void registerTileArea(OS * os)
{
	struct Lib {
		static TileArea * __newinstance()
		{
			return new TileArea();
		}
	};

	OS::FuncDef funcs[] = {
		// def("__newinstance", &Lib::__newinstance),
		DEF_GET("pos", TileArea, Pos),
		DEF_GET("size", TileArea, Size),
		DEF_GET("type", TileArea, Type),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<TileArea, Object>(os, funcs, nums, true OS_DBG_FILEPOS);
}
static bool __registerTileArea = addRegFunc(registerTileArea);

// =====================================================================

static void registerPhysContact(OS * os)
{
	struct Lib {
		/* static PhysContact * __newinstance()
		{
			return new PhysContact();
		} */
	};

	OS::FuncDef funcs[] = {
		// def("__newinstance", &Lib::__newinstance),
		def("getCategoryBits", &PhysContact::getCategoryBits),
		def("getEntity", &PhysContact::getEntity),
		def("getIsSensor", &PhysContact::getIsSensor),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<PhysContact, Object>(os, funcs, nums, true OS_DBG_FILEPOS);
}
static bool __registerPhysContact = addRegFunc(registerPhysContact);

// =====================================================================

static void registerBaseEntity(OS * os)
{
	struct Lib {
		static BaseEntity * __newinstance()
		{
			return new BaseEntity();
		}

		static int applyForce(OS * os, int params, int, int, void*)
		{
			OS_GET_SELF(BaseEntity*);
			if(params < 1){
				os->setException("vec2 argument required");
				os->handleException();
				return 0;
			}
			Vector2 force = CtypeValue<Vector2>::getArg(os, -params+0);
			self->applyForce(force, params >= 2 ? os->getValueId(-params+1) : 0);
			return 0;
		}
	};

	OS::FuncDef funcs[] = {
		def("__newinstance", &Lib::__newinstance),
		{"applyForce", &Lib::applyForce},
		DEF_PROP("linearVelocity", BaseEntity, LinearVelocity),
		DEF_GET("isAwake", BaseEntity, IsAwake),
		DEF_SET("linearDamping", BaseEntity, LinearDamping),
		DEF_SET("angularDamping", BaseEntity, AngularDamping),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<BaseEntity, Sprite>(os, funcs, nums, true OS_DBG_FILEPOS);
}
static bool __registerBaseEntity = addRegFunc(registerBaseEntity);

// =====================================================================

}; // namespace ObjectScript

// =====================================================================

// =====================================================================
// =====================================================================
// =====================================================================

void PhysContact::Data::reset()
{
	memset(this, 0, sizeof(*this));
}

PhysContact * PhysContact::with(const Data& d)
{
		data = d;
		return this;
}

int PhysContact::getCategoryBits(int i) const 
{
	OS_ASSERT(i >= 0 && i < 2);
	if(i >= 0 && i < 2){
		return data.filter[i].categoryBits;
	}
	/* if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return fixture->GetFilterData().categoryBits;
	} */
	return 0;
}

BaseEntity * PhysContact::getEntity(int i) const
{
	OS_ASSERT(i >= 0 && i < 2);
	if(i >= 0 && i < 2){
		return data.ent[i];
	}
	/* if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return dynamic_cast<BaseEntity*>((BaseEntity*)fixture->GetBody()->GetUserData());
	} */
	return NULL;
}

bool PhysContact::getIsSensor(int i) const
{
	OS_ASSERT(i >= 0 && i < 2);
	if(i >= 0 && i < 2){
		return data.isSensor[i];
	}
	/* if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return fixture->IsSensor();
	} */
	return false;
}

// =====================================================================
// =====================================================================
// =====================================================================

BaseEntity::BaseEntity()
{
	game = NULL;
	body = NULL;
}

BaseEntity::~BaseEntity()
{
	OX_ASSERT(!game && !body);
}

void BaseEntity::applyForce(const Vector2& viewForce, int paramsValueId)
{
	if(!body){
		return;
	}
	if(viewForce.x || viewForce.y){
		ObjectScript::SaveStackSize saveStackSize;
		ObjectScript::OS * os = ObjectScript::os;

		b2Vec2 force = toPhysVec(viewForce);
		
		os->pushValueById(paramsValueId);
		bool noClipForce = (os->getProperty(-1, "noClipForce"), os->popBool(false));
		if(!noClipForce){
			float scalarSpeed = toPhysValue((os->getProperty(-1, "maxSpeed"), os->popFloat(getOSActorPhysicsFloat(this, "maxSpeed", 120.0f))));
			scalarSpeed *= (os->getProperty(-1, "speedScale"), os->popFloat(1.0f));
			
			b2Vec2 destSpeed = force;
			destSpeed.Normalize();
			destSpeed *= scalarSpeed;
			
			b2Vec2 speed = body->GetLinearVelocity();
			float clip_edge = 0.5f, src, dest;
			for(int i = 0; i < 2; i++){
				if(i == 0){
					src = speed.x;
					dest = destSpeed.x;
				}else{
					src = speed.y;
					dest = destSpeed.y;
				}
				float t = src / (dest ? dest : 0.00001f);
				if(t >= clip_edge){
					t = 1 - (t - clip_edge) / (1 - clip_edge);
					
					if(t < -1) t = -1;
					else if(t > 1) t = 1;
					// t = t*t * (t < 0 ? -1.0f : 1.0f);
					// t = (1 - (1-t)*(1-t)) * (t < 0 ? -1.0f : 1.0f);
					
					if(i == 0){
						force.x *= t;
					}else{
						force.y *= t;
					}
				}
			}
		}
		if(force.x || force.y){
			body->ApplyForce(force, body->GetWorldCenter(), true);
		}
	}
}

// =====================================================================
// =====================================================================
// =====================================================================

BaseGameLevel::BaseGameLevel()
{
	pathFindThread.level = this;
	accumTimeSec = 0;
	physWorld = NULL;
	tiles = NULL;
	tilesMipmap = 0;
	tileWorldWidth = 0;
	tileWorldHeight = 0;
	tileWidth = 0;
	tileHeight = 0;
	physContactShare = new PhysContact;
}

BaseGameLevel::~BaseGameLevel()
{
	pathFindThread.exit();
	debugDraw = NULL;
	destroyAllBodies();
	delete [] tiles;
	delete physWorld;
}

void BaseGameLevel::setTileWorldSize(int width, int height)
{
	OX_ASSERT(!tiles);
	tileWorldWidth = width;
	tileWorldHeight = height;
	tiles = new Tile[width * height];
}

void BaseGameLevel::setTile(int x, int y, EPhysType type)
{
	getTile(x, y)->type = type;
}

Tile * BaseGameLevel::getTile(int x, int y)
{
	OX_ASSERT(tiles);
	// OX_ASSERT(x >= 0 && x < tileWorldWidth);
	// OX_ASSERT(y >= 0 && y < tileWorldHeight);
	if(tiles && x >= 0 && x < tileWorldWidth && y >= 0 && y < tileWorldHeight){
		return tiles + (y*tileWorldWidth + x);
	}
	return NULL;
}

void BaseGameLevel::createTilesMipmap()
{
	int newTileWorldWidth = tileWorldWidth / 2;
	int newTileWorldHeight = tileWorldHeight / 2;
	Tile * newTiles = new Tile[newTileWorldWidth * newTileWorldHeight];
	for(int x = 0; x < newTileWorldWidth; x++){
		for(int y = 0; y < newTileWorldHeight; y++){
			Tile * cur = newTiles + (y*newTileWorldWidth + x);
			bool waterFound = false, playerSpawnFound = false, monsterSpawnFound = false;
			for(int dx = 0; dx < 2 && cur->type == PHYS_EMPTY; dx++){
				for(int dy = 0; dy < 2 && cur->type == PHYS_EMPTY; dy++){
					int px = x*2 + dx;
					int py = y*2 + dy;
					Tile * tile = getTile(px, py);
					if(tile){
						switch(tile->type){
						case PHYS_WATER:
							waterFound = true;
							break;

						case PHYS_PLAYER_SPAWN:
							playerSpawnFound = true;
							break;

						case PHYS_MONSTER_SPAWN:
							monsterSpawnFound = true;
							break;

						case PHYS_SOLID:
							cur->type = PHYS_SOLID;
							break;
						}
					}
				}
			}
			if(cur->type == PHYS_EMPTY){
				if(waterFound){
					cur->type = PHYS_WATER;
				}else if(playerSpawnFound){
					cur->type = PHYS_PLAYER_SPAWN;
				}else if(monsterSpawnFound){
					cur->type = PHYS_MONSTER_SPAWN;
				}
			}
		}
	}
	delete [] tiles;
	tiles = newTiles;
	tilesMipmap++;
	tileWorldWidth /= 2;
	tileWorldHeight /= 2;
	tileWidth *= 2;
	tileHeight *= 2;
}

bool BaseGameLevel::isTileBlocked(int x, int y, int x1, int y1, int x2, int y2, bool fly)
{
	if(tilesMipmap > 0){
		Tile * tile = getTile(x, y);
		if(tile){
			switch(tile->type){
			case PHYS_WATER:
				return !fly;
								
			case PHYS_SOLID:
				return true;
			}
		}
		return false;
	}

	if( (x - 2-1 <= x2 && x2 <= x + 2+1 && y - 2-1 <= y2 && y2 <= y + 2+1) 
			|| (x - 2-1 <= x1 && x1 <= x + 2+1 && y - 2-1 <= y1 && y1 <= y + 2+1) )
	{
		Tile * tile = getTile(x, y);
		if(tile){
			switch(tile->type){
			case PHYS_WATER:
				return !fly;
								
			case PHYS_SOLID:
				return true;
			}
		}
		return false;
	}

	Tile * tile = getTile(x, y);
	if(tile->mipmapCalculated[fly]){
		return tile->mipmapBlocked[fly];
	}
	tile->mipmapCalculated[fly] = true;

	int startX = x-2, endX = x+2;
	int startY = y-2, endY = y+2;
	for(int px = startX; px <= endX; px++){
		for(int py = startY; py <= endY; py++){
			Tile * curCell = getTile(px, py);
			if(curCell){
				switch(curCell->type){
				case PHYS_WATER:
					return tile->mipmapBlocked[fly] = !fly;
									
				case PHYS_SOLID:
					return tile->mipmapBlocked[fly] = true;
				}
			}
		}
	}
	return tile->mipmapBlocked[fly] = false;
}

int BaseGameLevel::getTileId(int x, int y)
{
	return y * tileWorldHeight + x;
}

static float dist(int x, int y)
{
	if(x < 0) x = -x;
	if(y < 0) y = -y;
	if(x > y){
		return x + y*0.5f;
	}
	return y + x*0.5f;
}

PathFindThread::PathFindThread()
{
	struct Lib {
		static void * start(void * p)
		{
			PathFindThread * self = (PathFindThread*)p;
			self->threadFunc();
			return NULL;
		}
	};
	state = READY_FOR_NEW_TASK;
	level = NULL;
	x1 = y1 = x2 = y2 = 0;
	fly = allowNotFinishedPath = false;
	callbackOSValueId = 0;
	finishedPath = false;

	pthread_create(&thread, 0, Lib::start, this);
}

PathFindThread::~PathFindThread()
{
	exit();
}

void PathFindThread::exit()
{
	if(getState() != EXIT){
		setState(WAIT_EXIT);
		while(getState() != EXIT){
			sleep(10);
		}
	}
}

PathFindThread::EState PathFindThread::getState()
{
	MutexAutoLock autoLock(m);
	return state;
}

void PathFindThread::setState(EState a)
{
	MutexAutoLock autoLock(m);
	state = a;
}

void PathFindThread::threadFunc()
{
	for(;;){
		bool run = false;
		do{
			MutexAutoLock autoLock(m);
			if(state == WAIT_EXIT){
				state = EXIT;
				return;
			}
			if(state == FINDING || state == EXIT){
				OX_ASSERT(false);
			}
			if(state == WAIT_BREAK_FINDING){
				state = READY_FOR_NEW_TASK;
				reversePath.clear();
				nodes.clear();
				openNodes.clear();
				finishedPath = false;
				break;
			}
			if(state == READY_FOR_NEW_TASK){
				break;
			}
			if(state != NEW_TASK){
				break;
			}
			state = FINDING;
			reversePath.clear();
			nodes.clear();
			openNodes.clear();
			finishedPath = false;
			run = true;
		}while(false);
		if(!run){
			sleep(50);
			continue;
		}

		spPathNode node = new PathNode();
		node->x = x1;
		node->y = y1;
		node->closed = true;
		node->id = level->getTileId(node->x, node->y);
		node->weightH = dist(x2 - node->x, y2 - node->y);
		node->weight = node->weightG + node->weightH;
				
		nodes[node->id] = node;
				
		int endNodeId = level->getTileId(x2, y2);
		spPathNode curNode = node;

		int maxSteps = 10000;
		for(int steps = 0; getState() == FINDING; steps++, sleep(0)){
			if(curNode->id == endNodeId || steps >= maxSteps){
				MutexAutoLock autoLock(m);
				if(curNode->id != endNodeId && !allowNotFinishedPath){
					state = FINISHED;
					finishedPath = false;
					break;
				}
				reversePath.push_back(Vector2((float)curNode->x, (float)curNode->y));
				while(curNode->parent){
					curNode = curNode->parent; 
					reversePath.push_back(Vector2((float)curNode->x, (float)curNode->y));
				}
				state = FINISHED;
				finishedPath = true;
				break;
			}
			spPathNode bestNode = NULL;
			float bestWeight = FLT_MAX;
						
			int curX = curNode->x;
			int curY = curNode->y;
						
			int startX = max(0, curX-1);
			int endX = min(level->tileWorldWidth-1, curX+1);
						
			int startY = max(0, curY-1);
			int endY = min(level->tileWorldHeight-1, curY+1);
						
			for(int px = startX; px <= endX; px++){
				int dx = curX - px;
				for(int py = startY; py <= endY; py++){
					int dy = curY - py;
					if(!(dx | dy)){
						continue;
					}
					int id = level->getTileId(px, py);
					if(nodes.find(id) == nodes.end()){
						if(level->isTileBlocked(px, py, x1, y1, x2, y2, fly)){
							continue;
						}
						spPathNode node = new PathNode();
						node->x = px;
						node->y = py;
						node->weightG = dist(dx, dy) + curNode->weightG;
						node->weightH = dist(x2 - px, y2 - py);
						node->weight = node->weightG + node->weightH;
						node->id = id;
						node->closed = false;
						node->parent = curNode.get();
									
						nodes[id] = node;
						openNodes[id] = node;
						// cm.log("[path] new node "+gid+", pos "+px+" "+py);
					}else{
						if(node->closed){
							continue;
						}
						float weightG = dist(dx, dy) + curNode->weightG;
						if(node->weightG > weightG){
							node->weightG = weightG;
							node->weight = weightG + node->weightH;
							node->parent = curNode.get();
							// node.closed = false;
							// cm.log("[path] node "+node.gid+" => parent "+gid);
						}							
					}
					if(bestWeight > node->weight){
						bestWeight = node->weight;
						bestNode = node;
					}
				}
			}
			if(bestNode){
				curNode = bestNode; 
				curNode->closed = true;
				// delete openNodes[ curNode.gid ];
				PathNodes::iterator it = openNodes.find(curNode->id);
				if(it != openNodes.end()) openNodes.erase(it);
				continue;
			}
			/* if(curNode->parent){
				int i = 0;
				do{
					curNode = curNode->parent;
				}while(curNode && ++i < 10);
				if(curNode){
					continue;
				}
			} */
			curNode = NULL;
			bestWeight = FLT_MAX;
			PathNodes::iterator it = openNodes.begin();
			for(; it != openNodes.end(); ++it){
				spPathNode node = it->second; // openNodes[i];
				if(bestWeight > node->weight){
					bestWeight = node->weight;
					curNode = node;
				}
			}
			if(!curNode){
				// cm.log("[path] not found");
				// self.findPathInProgress = false;
				// callback(false);
				MutexAutoLock autoLock(m);
				state = FINISHED;
				finishedPath = false;
				break;
			}
			curNode->closed = true;
			// delete openNodes[ curNode.gid ];
			it = openNodes.find(curNode->id);
			if(it != openNodes.end()) openNodes.erase(it);
		}
	}
}

bool BaseGameLevel::getFindPathInProgress()
{
	MutexAutoLock autoLock(pathFindThread.m);
	return pathFindThread.state != PathFindThread::READY_FOR_NEW_TASK;
}

void BaseGameLevel::updatePath(ObjectScript::UpdateEvent*)
{
	MutexAutoLock autoLock(pathFindThread.m);
	if(pathFindThread.state == PathFindThread::FINISHED){
		if(pathFindThread.callbackOSValueId){
			// pathFindThread.state = PathFindThread::PROCESSING;
			ObjectScript::OS * os = ObjectScript::os;
			os->pushNull(); // this
			os->pushValueById(pathFindThread.callbackOSValueId); // func
			OX_ASSERT(os->isFunction());
			if(pathFindThread.finishedPath){
				os->newArray(pathFindThread.reversePath.size());
				std::vector<Vector2>::reverse_iterator it = pathFindThread.reversePath.rbegin();
				for(; it != pathFindThread.reversePath.rend(); ++it){
					ObjectScript::pushCtypeValue(os, *it);
					os->addProperty(-2);
				}
				os->callTF(1);
			}else{
				os->callTF();
			}
			os->handleException();
			ObjectScript::unregisterCallback(this, "pathFindThread", pathFindThread.callbackOSValueId);
			pathFindThread.callbackOSValueId = 0;
		}
		pathFindThread.state = PathFindThread::READY_FOR_NEW_TASK;
	}
}

void BaseGameLevel::findPath(int x1, int y1, int x2, int y2, bool fly, bool allowNotFinishedPath, int callbackOSValueId)
{
	// int x1 = (int)p1.x / tileWidth, y1 = (int)p1.y / tileHeight;
	// int x2 = (int)p2.x / tileWidth, y2 = (int)p2.y / tileHeight;
	OX_ASSERT(x1 >= 0 && x1 < tileWorldWidth && y1 >= 0 && y1 < tileWorldHeight);
	OX_ASSERT(x2 >= 0 && x2 < tileWorldWidth && y2 >= 0 && y2 < tileWorldHeight);
	if(!callbackOSValueId){
		OX_ASSERT(false);
		return;
	}
	
	ObjectScript::OS * os = ObjectScript::os;
	MutexAutoLock autoLock(pathFindThread.m);
	if(pathFindThread.state == PathFindThread::READY_FOR_NEW_TASK){
		pathFindThread.x1 = x1;
		pathFindThread.y1 = y1;
		pathFindThread.x2 = x2;
		pathFindThread.y2 = y2;
		pathFindThread.fly = fly;
		pathFindThread.allowNotFinishedPath = allowNotFinishedPath;
		pathFindThread.callbackOSValueId = callbackOSValueId;
		pathFindThread.state = PathFindThread::NEW_TASK;
		ObjectScript::registerCallback(this, "pathFindThread", pathFindThread.callbackOSValueId);
		return;
	}
	os->pushNull(); // this
	os->pushValueById(callbackOSValueId); // func
	OX_ASSERT(os->isFunction());
	os->callTF();
	os->handleException();
}

void BaseGameLevel::entityPosToTile(const Vector2& pos, int& x, int& y)
{
	x = (int)pos.x / tileWidth;
	y = (int)pos.y / tileHeight;
}

Vector2 BaseGameLevel::tileToEntityPos(int x, int y)
{
	return Vector2(
		(float)(x * tileWidth + tileWidth/2),
		(float)(y * tileHeight + tileHeight/2)
		);
}

bool BaseGameLevel::traceEntities(BaseEntity * ent1, BaseEntity * ent2, bool fly)
{
	Vector2 _p1 = ent1->getPosition();
	Vector2 _p2 = ent2->getPosition();
	float x1 = floorf(_p1.x / tileWidth), y1 = floorf(_p1.y / tileHeight);
	float x2 = floorf(_p2.x / tileWidth), y2 = floorf(_p2.y / tileHeight);

	float dx = x2 - x1;
	float dy = y2 - y1;
	// cm.log("[trace] "+dx+" "+dy);
	// return false;
	if(dx == 0 && dy == 0){
		return false;
	}
	float dxAbs = fabs(dx);
	float dyAbs = fabs(dy);
	int count = 0;  
	if(dxAbs >= dyAbs){
		dx = dx >= 0 ? 1.0f : -1.0f;
		dy /= dxAbs;
		count = (int)dxAbs;
	}else{
		dx /= dyAbs;
		dy = dy >= 0 ? 1.0f : -1.0f;
		count = (int)dyAbs;
	}
	float x = x1, y = y1;
	for(; count > 0; count--){
		x += dx;
		y += dy;
		Tile * tile = getTile((int)x, (int)y);
		if(tile){
			switch(tile->type){
			case PHYS_WATER:
				if(fly){
					break;
				}
				// no break
							
			case PHYS_SOLID:
				return false;
			}
		}
	}
	return true;
}

float toPhysValue(float a)
{
	return a * TO_PHYS_SCALE;
}

float fromPhysValue(float a)
{
	return a / TO_PHYS_SCALE;
}

b2Vec2 toPhysVec(const Vector2 &pos)
{
	return b2Vec2(toPhysValue(pos.x), toPhysValue(pos.y));
}

Vector2 fromPhysVec(const b2Vec2 &pos)
{
	return Vector2(fromPhysValue(pos.x), fromPhysValue(pos.y));
}

int BaseGameLevel::getTileAreaCount() const
{
	return tileAreas.size();
}

spTileArea BaseGameLevel::getTileArea(int i) const
{
	return tileAreas.at(i);
}

void BaseGameLevel::initTileAreas()
{
	OX_ASSERT(tileAreas.size() == 0);
	// int cellWidth = 16, cellHeight = 16;
	for(int y = 0; y < tileWorldHeight; y++){
		for(int x = 0; x < tileWorldWidth; x++){
			Tile * tile = getTile(x, y);
			if(tile->type == PHYS_EMPTY || tile->parsed){
				continue;
			}
			int max_x = tileWorldWidth-1, max_y = tileWorldHeight-1;
			for(int ax = x+1; ax <= max_x; ax++){
				Tile * ac = getTile(ax, y);
				if(ac->parsed || ac->type != tile->type){
					max_x = ax-1;
					break;
				}
			}
			for(int ay = y+1; ay <= max_y; ay++){
				bool isOk = true;
				for(int ax = x; ax <= max_x; ax++){
					Tile * ac = getTile(ax, ay);
					if(ac->parsed || ac->type != tile->type){
						isOk = false;
						break;
					}
				}
				if(!isOk){
					max_y = ay-1;
					break;
				}
			}
			spTileArea block = new TileArea();
			block->type = tile->type;
			block->size = Vector2((float)(max_x - x + 1) * tileWidth, (float)(max_y - y + 1) * tileHeight);
			block->pos = Vector2((float)x * tileWidth, (float)y * tileHeight);
			tileAreas.push_back(block);
			
			for(int ax = x; ax <= max_x; ax++){
				for(int ay = y; ay <= max_y; ay++){
					getTile(ax, ay)->parsed = true;
				}
			}
		}
	}
	std::vector<spTileArea>::iterator it = tileAreas.begin();
	for(; it != tileAreas.end(); ++it){
		spTileArea tileArea = *it;
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position = toPhysVec(tileArea->pos + tileArea->size / 2);

		b2Body * body = physWorld->CreateBody(&bodyDef);

		b2PolygonShape polyShape;
		b2Vec2 halfSize = toPhysVec(tileArea->size / 2);
		polyShape.SetAsBox(halfSize.x, halfSize.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polyShape;

		// b2Fixture * fixture = body->CreateFixture(&polyShape, 0);
		
		// b2Filter filter = fixture->GetFilterData();
		switch(tileArea->type){
		case PHYS_WATER:
			fixtureDef.filter.categoryBits = PHYS_CAT_BIT_WATER;
			break;

		case PHYS_SOLID:
			fixtureDef.filter.categoryBits = PHYS_CAT_BIT_STATIC;
			break;

		case PHYS_PLAYER_SPAWN:
			fixtureDef.filter.categoryBits = PHYS_CAT_BIT_PLAYER_SPAWN;
			break;

		case PHYS_MONSTER_SPAWN:
			fixtureDef.filter.categoryBits = PHYS_CAT_BIT_MONSTER_AREA | PHYS_CAT_BIT_MONSTER_SPAWN;
			break;
		}
		// fixture->SetFilterData(filter);
		body->CreateFixture(&fixtureDef);
	}
}

Actor * getOSActor(Actor * actor, const char * name)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;
	ObjectScript::pushCtypeValue(os, actor);
	os->getProperty(-1, name);
	return ObjectScript::CtypeValue<Actor*>::getArg(os, -1);
}

float getOSActorFloat(Actor * actor, const char * name, float def)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;
	ObjectScript::pushCtypeValue(os, actor);
	os->getProperty(-1, name);
	return os->toFloat(-1, def);
}

float getOSActorPhysicsFloat(Actor * actor, const char * name, float def)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;
	ObjectScript::pushCtypeValue(os, actor);
	os->getProperty(-1, "physics");
	os->getProperty(-1, name);
	return os->toFloat(-1, def);
}

int getOSActorPhysicsInt(Actor * actor, const char * name, int def)
{
	return (int)getOSActorPhysicsFloat(actor, name, (float)def);
}

bool getOSActorPhysicsBool(Actor * actor, const char * name, bool def)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;
	ObjectScript::pushCtypeValue(os, actor);
	os->getProperty(-1, "physics");
	os->getProperty(-1, name);
	return os->toBool(-1, def);
}

Actor * BaseGameLevel::getView()
{
	return getOSActor(this, "view");
}

void BaseGameLevel::addEntityPhysicsShapes(BaseEntity * ent)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;

	OX_ASSERT(ent->game && ent->body);

	b2CircleShape circleShape;
	b2PolygonShape polyShape;
	b2FixtureDef fixtureDef;

	if(!os->isObject()){
		os->setException("object required");
		os->handleException();
		return;
	}

	os->getProperty(-1, "shapes");
	if(!os->isNull()){
		if(!os->isArray()){
			os->setException("array required for shapes");
			os->handleException();
			return;
		}
		while(os->nextIteratorStep()){
			addEntityPhysicsShapes(ent);
			os->pop(2);
		}
	}else{
		os->pop(); // pop shapes

		os->getProperty(-1, "radius");
		if(!os->isNull()){
			fixtureDef.shape = &circleShape;
			circleShape.m_radius = toPhysValue(os->popFloat());
		}else{
			os->pop(); // pop radius
			os->getProperty(-1, "radiusScale");
			if(!os->isNull()){
				fixtureDef.shape = &circleShape;
				Vector2 size = ent->getSize();
				circleShape.m_radius = toPhysValue((size.x > size.y ? size.x : size.y) / 2 * os->popFloat());
			}else{
				os->pop(); // pop radiusScale
				fixtureDef.shape = &polyShape;
				b2Vec2 halfSize = toPhysVec(ent->getSize() / 2);
				halfSize.x *= (os->getProperty(-1, "widthScale"), os->popFloat(1.0f));
				halfSize.y *= (os->getProperty(-1, "heightScale"), os->popFloat(1.0f));
				polyShape.SetAsBox(halfSize.x, halfSize.y);
			}
		}
		fixtureDef.density = (os->getProperty(-1, "density"), os->popFloat(ent->getPhysicsFloat("density", PHYS_DEF_DENSITY)));
		fixtureDef.restitution = (os->getProperty(-1, "restitution"), os->popFloat(ent->getPhysicsFloat("restitution", PHYS_DEF_RESTITUTION)));
		fixtureDef.friction = (os->getProperty(-1, "friction"), os->popFloat(ent->getPhysicsFloat("friction", PHYS_DEF_FRICTION)));
		fixtureDef.isSensor = (os->getProperty(-1, "sensor"), os->popBool(ent->getPhysicsBool("sensor", false)));
		fixtureDef.filter.categoryBits = (os->getProperty(-1, "categoryBits"), os->popInt(ent->getPhysicsInt("categoryBits", fixtureDef.filter.categoryBits)));
		fixtureDef.filter.maskBits = (os->getProperty(-1, "maskBits"), os->popInt(ent->getPhysicsInt("maskBits", fixtureDef.filter.maskBits)));
		
		int ignoreBits = (os->getProperty(-1, "ignoreBits"), os->popInt(ent->getPhysicsInt("ignoreBits", 0)));
		fixtureDef.filter.maskBits &= ~ ignoreBits;

		os->getProperty(-1, "fly");
		if(os->toBool()){
			fixtureDef.filter.maskBits &= ~ PHYS_CAT_BIT_WATER;
		}
		os->pop();

		ent->body->CreateFixture(&fixtureDef);
	}
}

void BaseGameLevel::initEntityPhysics(BaseEntity * ent)
{
	ObjectScript::SaveStackSize saveStackSize;
	ObjectScript::OS * os = ObjectScript::os;
	
	if(ent->game || ent->body){
		os->setException("entity physics is already initialized");
		os->handleException();
		return;
	}
	ent->game = this;
	// entities.push_back(ent);

	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = toPhysVec(ent->getPosition());
	bodyDef.angle = ent->getRotation();
	bodyDef.fixedRotation = getOSActorPhysicsBool(ent, "fixedRotation", PHYS_DEF_FIXED_ROTATION);
	bodyDef.linearDamping = getOSActorPhysicsFloat(ent, "linearDamping", PHYS_DEF_LINEAR_DAMPING);
	bodyDef.angularDamping = getOSActorPhysicsFloat(ent, "angularDamping", PHYS_DEF_ANGULAR_DAMPING);

	ent->body = physWorld->CreateBody(&bodyDef);
	ent->body->SetUserData(ent);

	ObjectScript::pushCtypeValue(os, ent);
	os->getProperty(-1, "physics");

	addEntityPhysicsShapes(ent);
}

void BaseGameLevel::destroyEntityPhysics(BaseEntity * ent)
{
	if(!ent->body){
		return;
	}
	OX_ASSERT(ent->game == this);
	OX_ASSERT(ent->body);
	
	b2Fixture * fixture = ent->body->GetFixtureList();
	while(fixture){
		ent->body->DestroyFixture(fixture);
		fixture = ent->body->GetFixtureList();
	}
	/* b2JointEdge * joint = ent->body->GetJointList();
	while(fixture){
		physWorld->DestroyJoint(joint);
		joint = ent->body->GetJointList();
	} */

	OX_ASSERT(std::find(waitBodiesToDestroy.begin(), waitBodiesToDestroy.end(), ent->body) == waitBodiesToDestroy.end());
	waitBodiesToDestroy.push_back(ent->body);

	ent->body->SetUserData(NULL);
	ent->body = NULL;
	ent->game = NULL;
}

void BaseGameLevel::destroyWaitBodies()
{
	if(!waitBodiesToDestroy.size()){
		return;
	}
	OX_ASSERT(physWorld);

	std::vector<b2Body*> waitBodiesToDestroy = this->waitBodiesToDestroy;
	this->waitBodiesToDestroy.clear();

	std::vector<b2Body*>::iterator it = waitBodiesToDestroy.begin();
	for(; it != waitBodiesToDestroy.end(); ++it){
		b2Body * body = *it;
		OX_ASSERT(!body->GetUserData());
		physWorld->DestroyBody(body);
	}
}

void BaseGameLevel::destroyAllBodies()
{
	if(!physWorld){
		return;
	}
	b2Body * body = physWorld->GetBodyList(), * next = NULL;
	for(; body; body = next){
		next = body->GetNext();
		BaseEntity * ent = dynamic_cast<BaseEntity*>((BaseEntity*)body->GetUserData());
		if(ent){
			OX_ASSERT(ent->game == this);
			OX_ASSERT(ent->body == body);
			ent->body->SetUserData(NULL);
			ent->body = NULL;
			ent->game = NULL;
		}
		physWorld->DestroyBody(body);
	}
}

void BaseGameLevel::updatePhysics(float dt)
{
	if(!physWorld){
		return;
	}
	destroyWaitBodies();

	accumTimeSec += dt;
	dt = 1.0f/30.0f;
	while(accumTimeSec >= dt){ 
		physWorld->Step(dt, 6, 2);
		dispatchContacts();
		accumTimeSec -= dt;
	}

	b2Body * body = physWorld->GetBodyList();
	for(; body; body = body->GetNext()){
		BaseEntity * ent = dynamic_cast<BaseEntity*>((BaseEntity*)body->GetUserData());
		if(ent){
			OX_ASSERT(ent->body == body && ent->game == this);
			ent->setPosition(fromPhysVec(body->GetPosition()));
			ent->setRotation(body->GetAngle());
		}
	}
}

void BaseGameLevel::dispatchContacts()
{
	ObjectScript::OS * os = ObjectScript::os;
	std::vector<PhysContact::Data>::iterator it = physContacts.begin();
	for(; it != physContacts.end(); ++it){
		PhysContact::Data& c = *it;
		if(c.ent[0]){
			ObjectScript::pushCtypeValue(os, c.ent[0]); // this
			os->getProperty(-1, "onPhysicsContact"); // func
			OX_ASSERT(os->isFunction());
			ObjectScript::pushCtypeValue(os, physContactShare->with(c));
			os->pushNumber(0);
			os->callTF(2, 1);
			if(os->popBool()){
				return;
			}
		}
		if(c.ent[1]){
			ObjectScript::pushCtypeValue(os, c.ent[1]); // this
			os->getProperty(-1, "onPhysicsContact"); // func
			OX_ASSERT(os->isFunction());
			ObjectScript::pushCtypeValue(os, physContactShare->with(c));
			os->pushNumber(1);
			os->callTF(2);
		}
	}
	physContactShare->data.reset();
	physContacts.clear();
}

void BaseGameLevel::BeginContact(b2Contact* c)
{
	PhysContact::Data data;
	for(int i = 0; i < 2; i++){
		b2Fixture * fixture = i ? c->GetFixtureB() : c->GetFixtureA();
		data.ent[i] = dynamic_cast<BaseEntity*>((BaseEntity*)fixture->GetBody()->GetUserData());
		data.filter[i] = fixture->GetFilterData();
		data.isSensor[i] = fixture->IsSensor();
	}
	physContacts.push_back(data);
}

void BaseGameLevel::EndContact(b2Contact* contact)
{
}

void BaseGameLevel::createPhysicsWorld(const b2Vec2& size)
{
	OX_ASSERT(!physWorld);
	// b2AABB aabb;
	// aabb.lowerBound = b2Vec2(-1000, -1000);
	// aabb.upperBound = b2Vec2(size.x + 1000, size.y + 1000);
	b2Vec2 gravity = b2Vec2(0, 0);
	// bool doSleep = true;
	physWorld = new b2World(gravity);
	physWorld->SetAllowSleeping(true);
	physWorld->SetDestructionListener(this);
	physWorld->SetContactListener(this);

	tileWidth = (int)size.x / tileWorldWidth;
	tileHeight = (int)size.x / tileWorldHeight;

	initTileAreas();
	// createTilesMipmap();
	// createTilesMipmap();
}

void BaseGameLevel::destroyPhysicsWorld()
{
	tileAreas.clear();
	destroyAllBodies();
	delete physWorld; physWorld = NULL;
	delete [] tiles; tiles = NULL;
	accumTimeSec = 0;
	tilesMipmap = 0;
	tileWorldWidth = 0;
	tileWorldHeight = 0;
	tileWidth = 0;
	tileHeight = 0;
}

bool BaseGameLevel::getDebugDraw() const
{
	return debugDraw;
}

void BaseGameLevel::setDebugDraw(bool value)
{
	if((bool)debugDraw != value){
		if(value){
			debugDraw = new Box2DDraw(this);
			debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
			debugDraw->setPriority(10000);
			debugDraw->attachTo(getView());
			// debugDraw->setWorld(1/PHYS_SCALE, physWorld);
		}else{
			debugDraw->detach();
			debugDraw = NULL;
		}
	}
}

void BaseGameLevel::drawPhysShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color)
{
	switch (fixture->GetType())
	{
	case b2Shape::e_circle:
		{
			b2CircleShape* circle = (b2CircleShape*)fixture->GetShape();

			b2Vec2 center = b2Mul(xf, circle->m_p);
			float32 radius = circle->m_radius;
			b2Vec2 axis = b2Mul(xf.q, b2Vec2(1.0f, 0.0f));

			debugDraw->DrawSolidCircle(center, radius, axis, color);
		}
		break;

	case b2Shape::e_edge:
		{
			b2EdgeShape* edge = (b2EdgeShape*)fixture->GetShape();
			b2Vec2 v1 = b2Mul(xf, edge->m_vertex1);
			b2Vec2 v2 = b2Mul(xf, edge->m_vertex2);
			debugDraw->DrawSegment(v1, v2, color);
		}
		break;

	case b2Shape::e_chain:
		{
			b2ChainShape* chain = (b2ChainShape*)fixture->GetShape();
			int32 count = chain->m_count;
			const b2Vec2* vertices = chain->m_vertices;

			b2Vec2 v1 = b2Mul(xf, vertices[0]);
			for (int32 i = 1; i < count; ++i)
			{
				b2Vec2 v2 = b2Mul(xf, vertices[i]);
				debugDraw->DrawSegment(v1, v2, color);
				debugDraw->DrawCircle(v1, 0.05f, color);
				v1 = v2;
			}
		}
		break;

	case b2Shape::e_polygon:
		{
			b2PolygonShape* poly = (b2PolygonShape*)fixture->GetShape();
			int32 vertexCount = poly->m_count;
			b2Assert(vertexCount <= b2_maxPolygonVertices);
			b2Vec2 vertices[b2_maxPolygonVertices];

			for (int32 i = 0; i < vertexCount; ++i)
			{
				vertices[i] = b2Mul(xf, poly->m_vertices[i]);
			}

			debugDraw->DrawSolidPolygon(vertices, vertexCount, color);
		}
		break;
            
    default:
        break;
	}
}

void BaseGameLevel::drawPhysJoint(b2Joint* joint)
{
	b2Body* bodyA = joint->GetBodyA();
	b2Body* bodyB = joint->GetBodyB();
	const b2Transform& xf1 = bodyA->GetTransform();
	const b2Transform& xf2 = bodyB->GetTransform();
	b2Vec2 x1 = xf1.p;
	b2Vec2 x2 = xf2.p;
	b2Vec2 p1 = joint->GetAnchorA();
	b2Vec2 p2 = joint->GetAnchorB();

	b2Color color(0.5f, 0.8f, 0.8f);

	switch (joint->GetType())
	{
	case e_distanceJoint:
		debugDraw->DrawSegment(p1, p2, color);
		break;

	case e_pulleyJoint:
		{
			b2PulleyJoint* pulley = (b2PulleyJoint*)joint;
			b2Vec2 s1 = pulley->GetGroundAnchorA();
			b2Vec2 s2 = pulley->GetGroundAnchorB();
			debugDraw->DrawSegment(s1, p1, color);
			debugDraw->DrawSegment(s2, p2, color);
			debugDraw->DrawSegment(s1, s2, color);
		}
		break;

	case e_mouseJoint:
		// don't draw this
		break;

	default:
		debugDraw->DrawSegment(x1, p1, color);
		debugDraw->DrawSegment(p1, p2, color);
		debugDraw->DrawSegment(x2, p2, color);
	}
}

void BaseGameLevel::drawPhysics()
{
	if(!debugDraw)
	{
		return;
	}

	uint32 flags = debugDraw->GetFlags();

	if (flags & b2Draw::e_shapeBit)
	{
		for (b2Body* b = physWorld->GetBodyList(); b; b = b->GetNext())
		{
			const b2Transform& xf = b->GetTransform();
			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				b2Filter filter = f->GetFilterData();
				if (b->IsActive() == false)
				{
					drawPhysShape(f, xf, b2Color(0.5f, 0.5f, 0.3f));
				}
				else if (b->GetType() == b2_staticBody)
				{
					b2Color color(0.6f, 0.65f, 0.6f);
					if(filter.categoryBits & PHYS_CAT_BIT_PLAYER_SPAWN){
						color = b2Color(0.5f, 0.8f, 0.5f);
					}else if(filter.categoryBits & PHYS_CAT_BIT_WATER){
						color = b2Color(0.6f, 0.6f, 0.9f);
					}else if(filter.categoryBits & PHYS_CAT_BIT_MONSTER_SPAWN){
						color = b2Color(0.8f, 0.6f, 0.6f);
					}
					drawPhysShape(f, xf, color);
				}
				else if (b->GetType() == b2_kinematicBody)
				{
					drawPhysShape(f, xf, b2Color(0.5f, 0.5f, 0.9f));
				}
				else if (b->IsAwake() == false)
				{
					drawPhysShape(f, xf, b2Color(0.6f, 0.6f, 0.6f));
				}
				else
				{
					drawPhysShape(f, xf, b2Color(0.9f, 0.7f, 0.7f));
				}
			}
		}
	}

	if (flags & b2Draw::e_jointBit)
	{
		for (b2Joint* j = physWorld->GetJointList(); j; j = j->GetNext())
		{
			drawPhysJoint(j);
		}
	}

	if (flags & b2Draw::e_pairBit)
	{
		b2Color color(0.3f, 0.9f, 0.9f);
		for (b2Contact* c = physWorld->GetContactList(); c; c = c->GetNext())
		{
			//b2Fixture* fixtureA = c->GetFixtureA();
			//b2Fixture* fixtureB = c->GetFixtureB();

			//b2Vec2 cA = fixtureA->GetAABB().GetCenter();
			//b2Vec2 cB = fixtureB->GetAABB().GetCenter();

			//debugDraw->DrawSegment(cA, cB, color);
		}
	}

	/* if (flags & b2Draw::e_aabbBit)
	{
		b2Color color(0.9f, 0.3f, 0.9f);
		b2BroadPhase* bp = &physWorld->GetContactManager().m_broadPhase;

		for (b2Body* b = physWorld->GetBodyList(); b; b = b->GetNext())
		{
			if (b->IsActive() == false)
			{
				continue;
			}

			for (b2Fixture* f = b->GetFixtureList(); f; f = f->GetNext())
			{
				for (int32 i = 0; i < f->m_proxyCount; ++i)
				{
					b2FixtureProxy* proxy = f->m_proxies + i;
					b2AABB aabb = bp->GetFatAABB(proxy->proxyId);
					b2Vec2 vs[4];
					vs[0].Set(aabb.lowerBound.x, aabb.lowerBound.y);
					vs[1].Set(aabb.upperBound.x, aabb.lowerBound.y);
					vs[2].Set(aabb.upperBound.x, aabb.upperBound.y);
					vs[3].Set(aabb.lowerBound.x, aabb.upperBound.y);

					debugDraw->DrawPolygon(vs, 4, color);
				}
			}
		}
	} */

	if (flags & b2Draw::e_centerOfMassBit)
	{
		for (b2Body* b = physWorld->GetBodyList(); b; b = b->GetNext())
		{
			b2Transform xf = b->GetTransform();
			xf.p = b->GetWorldCenter();
			debugDraw->DrawTransform(xf);
		}
	}
}


void BaseGameLevel::SayGoodbye(b2Joint* joint)
{
}

void BaseGameLevel::SayGoodbye(b2Fixture* fixture)
{
}
