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
		DEF_CONST(PHYS_CAT_BIT_PLAYER),
		DEF_CONST(PHYS_CAT_BIT_MONSTER),
		DEF_CONST(PHYS_CAT_BIT_PLAYER_FIRE),
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
bool __registerBaseGameGlobals = addRegFunc(registerBaseGameGlobals);

// =====================================================================

static void registerBaseGameLevel(OS * os)
{
	struct Lib {
		static BaseGameLevel * __newinstance()
		{
			return new BaseGameLevel();
		}
	};

	OS::FuncDef funcs[] = {
		def("__newinstance", &Lib::__newinstance),
		def("createPhysicsWorld", &BaseGameLevel::createPhysicsWorld),
		def("updatePhysics", &BaseGameLevel::updatePhysics),
		def("setPhysSize", &BaseGameLevel::setPhysSize),
		def("setPhysCell", &BaseGameLevel::setPhysCell),
		def("initEntityPhysics", &BaseGameLevel::initEntityPhysics),
		def("destroyEntityPhysics", &BaseGameLevel::destroyEntityPhysics),
		DEF_PROP(debugDraw, BaseGameLevel, DebugDraw),
		DEF_GET(physBlockCount, BaseGameLevel, PhysBlockCount),
		def("getPhysBlock", &BaseGameLevel::getPhysBlock),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<BaseGameLevel, Actor>(os, funcs, nums, true OS_DBG_FILEPOS);
}
bool __registerBaseGameLevel = addRegFunc(registerBaseGameLevel);

// =====================================================================

static void registerPhysBlock(OS * os)
{
	struct Lib {
		static PhysBlock * __newinstance()
		{
			return new PhysBlock();
		}
	};

	OS::FuncDef funcs[] = {
		// def("__newinstance", &Lib::__newinstance),
		DEF_GET(pos, PhysBlock, Pos),
		DEF_GET(size, PhysBlock, Size),
		DEF_GET(type, PhysBlock, Type),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<PhysBlock, Object>(os, funcs, nums, true OS_DBG_FILEPOS);
}
bool __registerPhysBlock = addRegFunc(registerPhysBlock);

// =====================================================================

static void registerPhysContact(OS * os)
{
	struct Lib {
		static PhysContact * __newinstance()
		{
			return new PhysContact(NULL);
		}
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
bool __registerPhysContact = addRegFunc(registerPhysContact);

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
		DEF_PROP(linearVelocity, BaseEntity, LinearVelocity),
		DEF_GET(isAwake, BaseEntity, IsAwake),
		DEF_SET(linearDamping, BaseEntity, LinearDamping),
		DEF_SET(angularDamping, BaseEntity, AngularDamping),
		{}
	};
	OS::NumberDef nums[] = {
		{}
	};
	registerOXClass<BaseEntity, Sprite>(os, funcs, nums, true OS_DBG_FILEPOS);
}
bool __registerBaseEntity = addRegFunc(registerBaseEntity);

// =====================================================================

static void registerKeyboardEvent(OS * os)
{
	struct Lib {
		static KeyboardEvent * __newinstance()
		{
			return new KeyboardEvent(0);
		}
	};

	OS::FuncDef funcs[] = {
		def("__newinstance", &Lib::__newinstance),
		DEF_GET(scancode, KeyboardEvent, Scancode),
		DEF_GET(sym, KeyboardEvent, Sym),
		DEF_GET(mod, KeyboardEvent, Mod),
		DEF_GET(str, KeyboardEvent, Str),
		{}
	};
	OS::NumberDef nums[] = {
		{"DOWN", KeyboardEvent::DOWN},
		{"UP", KeyboardEvent::UP},
		{"TEXTEDITING", KeyboardEvent::TEXTEDITING},
		{"TEXTINPUT", KeyboardEvent::TEXTINPUT},
#include "KeyboardEventCodes.inc"
		{}
	};
	registerOXClass<KeyboardEvent, Event>(os, funcs, nums, true OS_DBG_FILEPOS);
}
bool __registerKeyboardEvent = addRegFunc(registerKeyboardEvent);

// =====================================================================

}; // namespace ObjectScript

// =====================================================================

KeyboardEvent::KeyboardEvent(int type): Event(type)
{
}

void KeyboardEvent::makeStr()
{
	int code = key.keysym.sym;
	str[0] = (char)(code & 0xff);
	str[1] = (char)((code>>8) & 0xff);
	str[2] = (char)((code>>16) & 0xff);
	str[3] = (char)((code>>24) & 0xff);
	str[4] = '\0';
}

// =====================================================================
// =====================================================================
// =====================================================================

// =====================================================================
// =====================================================================
// =====================================================================

int PhysContact::getCategoryBits(int i) const 
{
	if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return fixture->GetFilterData().categoryBits;
	}
	return 0;
}

BaseEntity * PhysContact::getEntity(int i) const
{
	if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return dynamic_cast<BaseEntity*>((BaseEntity*)fixture->GetBody()->GetUserData());
	}
	return 0;
}

bool PhysContact::getIsSensor(int i) const
{
	if(contact){
		b2Fixture * fixture = i ? contact->GetFixtureB() : contact->GetFixtureA();
		return fixture->IsSensor();
	}
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
	accumTimeSec = 0;
	physWorld = NULL;
	physCells = NULL;
	platfromEventId = Input::instance.addEventListener(Input::event_platform, CLOSURE(this, &BaseGameLevel::onPlatformEvent));
}

BaseGameLevel::~BaseGameLevel()
{
	debugDraw = NULL;
	destroyAllBodies();
	Input::instance.removeEventListener(platfromEventId);
	delete physCells;
	delete physWorld;
}

void BaseGameLevel::onPlatformEvent(Event * p_ev)
{
	SDL_Event * ev = (SDL_Event*)p_ev->userData;
	KeyboardEvent keyEvent(0);
	switch(ev->type){
	case SDL_KEYDOWN:
		keyEvent.type = KeyboardEvent::DOWN;
		keyEvent.key = ev->key;
		break;

	case SDL_KEYUP:
		keyEvent.type = KeyboardEvent::UP;
		keyEvent.key = ev->key;
		break;

	case SDL_TEXTEDITING:
		keyEvent.type = KeyboardEvent::TEXTEDITING;
		keyEvent.key = ev->key;
		break;

	case SDL_TEXTINPUT:
		keyEvent.type = KeyboardEvent::TEXTINPUT;
		keyEvent.key = ev->key;
		break;

	default:					
		return;
	}
	keyEvent.makeStr();
	dispatchEvent(&keyEvent);
}

void BaseGameLevel::setPhysSize(int width, int height)
{
	OX_ASSERT(!physCells);
	physCellWidth = width;
	physCellHeight = height;
	physCells = new PhysCell[width * height];
}

void BaseGameLevel::setPhysCell(int x, int y, EPhysCellType type)
{
	getPhysCell(x, y)->type = type;
}

PhysCell * BaseGameLevel::getPhysCell(int x, int y)
{
	OX_ASSERT(physCells);
	OX_ASSERT(x >= 0 && x < physCellWidth);
	OX_ASSERT(y >= 0 && y < physCellHeight);
	if(physCells && x >= 0 && x < physCellWidth && y >= 0 && y < physCellHeight){
		return physCells + (y*physCellWidth + x);
	}
	return NULL;
}

const float PHYS_SCALE = 1.0f / 100.0f;

float toPhysValue(float a)
{
	return a * PHYS_SCALE;
}

float fromPhysValue(float a)
{
	return a / PHYS_SCALE;
}

b2Vec2 toPhysVec(const Vector2 &pos)
{
	return b2Vec2(toPhysValue(pos.x), toPhysValue(pos.y));
}

Vector2 fromPhysVec(const b2Vec2 &pos)
{
	return Vector2(fromPhysValue(pos.x), fromPhysValue(pos.y));
}

int BaseGameLevel::getPhysBlockCount() const
{
	return physBlocks.size();
}

spPhysBlock BaseGameLevel::getPhysBlock(int i) const
{
	return physBlocks.at(i);
}

void BaseGameLevel::initPhysBlocks()
{
	OX_ASSERT(physBlocks.size() == 0);
	int cellWidth = 16, cellHeight = 16;
	for(int y = 0; y < physCellHeight; y++){
		for(int x = 0; x < physCellWidth; x++){
			PhysCell * cell = getPhysCell(x, y);
			if(cell->type == PHYS_UNKNOWN || cell->parsed){
				continue;
			}
			int max_x = physCellWidth-1, max_y = physCellHeight-1;
			for(int ax = x+1; ax <= max_x; ax++){
				PhysCell * ac = getPhysCell(ax, y);
				if(ac->parsed || ac->type != cell->type){
					max_x = ax-1;
					break;
				}
			}
			for(int ay = y+1; ay <= max_y; ay++){
				bool isOk = true;
				for(int ax = x; ax <= max_x; ax++){
					PhysCell * ac = getPhysCell(ax, ay);
					if(ac->parsed || ac->type != cell->type){
						isOk = false;
						break;
					}
				}
				if(!isOk){
					max_y = ay-1;
					break;
				}
			}
			spPhysBlock block = new PhysBlock();
			block->type = cell->type;
			block->size = Vector2((float)(max_x - x + 1) * cellWidth, (float)(max_y - y + 1) * cellHeight);
			block->pos = Vector2((float)x * cellWidth, (float)y * cellHeight);
			physBlocks.push_back(block);
			
			for(int ax = x; ax <= max_x; ax++){
				for(int ay = y; ay <= max_y; ay++){
					getPhysCell(ax, ay)->parsed = true;
				}
			}
		}
	}
	std::vector<spPhysBlock>::iterator it = physBlocks.begin();
	for(; it != physBlocks.end(); ++it){
		spPhysBlock physBlock = *it;
		
		b2BodyDef bodyDef;
		bodyDef.type = b2_staticBody;
		bodyDef.position = toPhysVec(physBlock->pos + physBlock->size / 2);

		b2Body * body = physWorld->CreateBody(&bodyDef);

		b2PolygonShape polyShape;
		b2Vec2 halfSize = toPhysVec(physBlock->size / 2);
		polyShape.SetAsBox(halfSize.x, halfSize.y);

		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polyShape;

		// b2Fixture * fixture = body->CreateFixture(&polyShape, 0);
		
		// b2Filter filter = fixture->GetFilterData();
		switch(physBlock->type){
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
		fixtureDef.density = (os->getProperty(-1, "density"), os->popFloat(PHYS_DEF_DENSITY));
		fixtureDef.restitution = (os->getProperty(-1, "restitution"), os->popFloat(PHYS_DEF_RESTITUTION));
		fixtureDef.friction = (os->getProperty(-1, "friction"), os->popFloat(PHYS_DEF_FRICTION));
		fixtureDef.isSensor = (os->getProperty(-1, "sensor"), os->popBool(false));
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
		accumTimeSec -= dt;
	}

	b2Body * body = physWorld->GetBodyList();
	for(; body; body = body->GetNext()){
		BaseEntity * ent = dynamic_cast<BaseEntity*>((BaseEntity*)body->GetUserData());
		if(ent){
			OX_ASSERT(ent->body == body);
			ent->setPosition(fromPhysVec(body->GetPosition()));
			ent->setRotation(body->GetAngle());
		}
	}

	ObjectScript::OS * os = ObjectScript::os;
	b2Contact * c = physWorld->GetContactList();
	for(; c; c = c->GetNext()){
		spPhysContact physContact;
		BaseEntity * ent = dynamic_cast<BaseEntity*>((BaseEntity*)c->GetFixtureA()->GetBody()->GetUserData());
		if(ent){
			ObjectScript::pushCtypeValue(os, ent);
			os->getProperty("onPhysicsContact");
			OX_ASSERT(os->isFunction());
			ObjectScript::pushCtypeValue(os, ent); // this
			physContact = new PhysContact(c);
			ObjectScript::pushCtypeValue(os, physContact);
			os->pushNumber(0);
			os->call(2, 1);
			if(os->popBool()){
				continue;
			}
		}
		ent = dynamic_cast<BaseEntity*>((BaseEntity*)c->GetFixtureB()->GetBody()->GetUserData());
		if(ent){
			ObjectScript::pushCtypeValue(os, ent);
			os->getProperty("onPhysicsContact");
			OX_ASSERT(os->isFunction());
			ObjectScript::pushCtypeValue(os, ent); // this
			if(!physContact) physContact = new PhysContact(c);
			ObjectScript::pushCtypeValue(os, physContact);
			os->pushNumber(1);
			os->call(2);
		}
		if(physContact){
			physContact->reset();
		}
	}
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

	initPhysBlocks();
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
