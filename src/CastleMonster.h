#ifndef __CASTLEMONSTER_H__

#include "oxygine-framework.h"
#include <ox-binder.h>
#include <Box2D\Box2D.h>
#include "Box2DDebugDraw.h"

enum
{
	PHYS_CAT_BIT_PLAYER			= 1<<0,
	PHYS_CAT_BIT_MONSTER		= 1<<1,
	PHYS_CAT_BIT_PLAYER_FIRE	= 1<<2,
	PHYS_CAT_BIT_POWERUP		= 1<<3,
	PHYS_CAT_BIT_STATIC			= 1<<4,
	PHYS_CAT_BIT_WATER			= 1<<5,
	PHYS_CAT_BIT_MONSTER_AREA	= 1<<6,
	PHYS_CAT_BIT_ENVIRONMENT	= 1<<7,
	PHYS_CAT_BIT_MONSTER_FIRE	= 1<<8,
	PHYS_CAT_BIT_BLOOD			= 1<<9,
			
	PHYS_CAT_BIT_MONSTER_SPAWN	= 1<<15,
	PHYS_CAT_BIT_PLAYER_SPAWN	= 1<<14,

	PHYS_CAT_BIT_ALL			= 0xFFFF
};

enum EPhysCellType
{
	PHYS_WATER,
	PHYS_SOLID,
	PHYS_PLAYER_SPAWN,
	PHYS_MONSTER_SPAWN,
	PHYS_UNKNOWN
};

#define PHYS_DEF_FIXED_ROTATION		true
#define PHYS_DEF_LINEAR_DAMPING		0.02f
#define PHYS_DEF_ANGULAR_DAMPING	0.02f
#define PHYS_DEF_DENSITY			1.0f
#define PHYS_DEF_RESTITUTION		0.0f
#define PHYS_DEF_FRICTION			0.02f

struct PhysCell
{
	EPhysCellType type;
	bool parsed;

	PhysCell()
	{
		type = PHYS_UNKNOWN;
		parsed = false;
	}
};

DECLARE_SMART(PhysBlock, spPhysBlock);
class PhysBlock: public Object
{
public:
	OS_DECLARE_CLASSINFO(PhysBlock);

	Vector2 pos;
	Vector2 size;
	EPhysCellType type;

	PhysBlock(): pos(0, 0), size(0, 0)
	{
		type = PHYS_UNKNOWN;
	}

	Vector2 getPos() const { return pos; }
	Vector2 getSize() const { return size; }
	EPhysCellType getType() const { return type; }
};

class BaseEntity;

DECLARE_SMART(PhysContact, spPhysContact);
class PhysContact: public Object
{
public:
	OS_DECLARE_CLASSINFO(PhysContact);

	b2Contact * contact;

	PhysContact(b2Contact * c){ contact = c; }
	void reset(){ contact = NULL; }

	int getCategoryBits(int i) const;
	BaseEntity * getEntity(int i) const;
};

float toPhysValue(float a);
float fromPhysValue(float a);
b2Vec2 toPhysVec(const Vector2 &pos);
Vector2 fromPhysVec(const b2Vec2 &pos);

class BaseGameLevel;

Actor * getOSActor(Actor * actor, const char * name);
float getOSActorFloat(Actor * actor, const char * name, float def);
float getOSActorPhysicsFloat(Actor * actor, const char * name, float def);
int getOSActorPhysicsInt(Actor * actor, const char * name, int def);
bool getOSActorPhysicsBool(Actor * actor, const char * name, bool def);

DECLARE_SMART(BaseEntity, spBaseEntity);
class BaseEntity: public oxygine::Sprite
{
public:
	OS_DECLARE_CLASSINFO(BaseEntity);

	BaseGameLevel * game;
	b2Body * body;

	BaseEntity();
	~BaseEntity();

	void applyForce(const Vector2& force, int paramsValueId);

	Vector2 getLinearVelocity() const
	{
		return body ? fromPhysVec(body->GetLinearVelocity()) : Vector2(0, 0);
	}
	void setLinearVelocity(const Vector2& a)
	{
		if(body){
			body->SetLinearVelocity(toPhysVec(a));
		}
	}

	bool getIsAwake() const 
	{
		return body ? body->IsAwake() : false;
	}

	void setLinearDamping(float a)
	{
		if(body) body->SetLinearDamping(a);
	}

	void setAngularDamping(float a)
	{
		if(body) body->SetAngularDamping(a);
	}

	float getPhysicsFloat(const char * name, float def)
	{
		return getOSActorPhysicsFloat(this, name, def);
	}
	int getPhysicsInt(const char * name, int def)
	{
		return getOSActorPhysicsInt(this, name, def);
	}
	bool getPhysicsBool(const char * name, bool def)
	{
		return getOSActorPhysicsBool(this, name, def);
	}
};

class BaseGameLevel: public oxygine::Actor, protected b2DestructionListener
{
public:
	OS_DECLARE_CLASSINFO(BaseGameLevel);

	BaseGameLevel();
	~BaseGameLevel();

	void createPhysicsWorld(const b2Vec2& size);
	void drawPhysics();

	void setPhysSize(int width, int height);
	void setPhysCell(int x, int y, EPhysCellType type);

	Actor * getView();

	void initEntityPhysics(BaseEntity * ent);
	void addEntityPhysicsShapes(BaseEntity * ent);

	void destroyEntityPhysics(BaseEntity * ent);

	void updatePhysics(float dt);

	bool getDebugDraw() const;
	void setDebugDraw(bool value);

	int getPhysBlockCount() const;
	spPhysBlock getPhysBlock(int i) const;

protected:

	spBox2DDraw debugDraw;

	float accumTimeSec;
	b2World * physWorld;
	PhysCell * physCells;
	int physCellWidth;
	int physCellHeight;

	std::vector<spPhysBlock> physBlocks;
	// std::vector<spBaseEntity> entities;
	std::vector<b2Body*> waitBodiesToDestroy;

	int platfromEventId;
	void onPlatformEvent(Event*);

	void destroyWaitBodies();
	void destroyAllBodies();

	void drawPhysShape(b2Fixture* fixture, const b2Transform& xf, const b2Color& color);
	void drawPhysJoint(b2Joint* joint);

	PhysCell * getPhysCell(int x, int y);
	void initPhysBlocks();

	/// Called when any joint is about to be destroyed due
	/// to the destruction of one of its attached bodies.
	void SayGoodbye(b2Joint* joint);

	/// Called when any fixture is about to be destroyed due
	/// to the destruction of its parent body.
	void SayGoodbye(b2Fixture* fixture);
};

class KeyboardEvent: public Event
{
public:
	OS_DECLARE_CLASSINFO(KeyboardEvent);

	enum {
		DOWN = makefourcc('K', 'E', 'D', 'O'),
		UP = makefourcc('K', 'E', 'U', 'P'),
		TEXTEDITING = makefourcc('K', 'E', 'T', 'E'),
		TEXTINPUT = makefourcc('K', 'E', 'T', 'I')
	};

	SDL_KeyboardEvent key;
	char str[5];

	KeyboardEvent(int type);

	SDL_Scancode getScancode() const { return key.keysym.scancode; }
	SDL_Keycode getSym() const { return key.keysym.sym; }
	int getMod() const { return key.keysym.mod; }
	
	void makeStr();
	const char * getStr() const { return str; }
};

namespace ObjectScript {

OS_DECL_CTYPE_NAME(b2Vec2, "vec2");
// OS_DECL_CTYPE_NAME(const b2Vec2&, "vec2");

template <>
struct CtypeValue<b2Vec2>
{
	typedef b2Vec2 type;

	static bool isValid(const type&){ return true; }

	static type def(ObjectScript::OS * os){ return type(0, 0); }
	static type getArg(ObjectScript::OS * os, int offs)
	{
		if(os->isObject(offs)){
			float x = (os->getProperty(offs, "x"), os->popFloat());
			float y = (os->getProperty(offs, "y"), os->popFloat());
			return type(x, y);
		}
		OS_NUMBER v;
		if(os->isNumber(offs, &v)){
			return type((float)v, (float)v);
		}
		os->setException("vec2 or number required");
		return type(0, 0);
	}

	static void push(ObjectScript::OS * os, const type& p)
	{
		os->getGlobal("vec2");
		OX_ASSERT(os->isObject());
		os->pushGlobals();
		os->pushNumber(p.x);
		os->pushNumber(p.y);
		os->call(2, 1);
		os->handleException();
	}
};

OS_DECL_CTYPE_ENUM(EPhysCellType);
OS_DECL_CTYPE_ENUM(SDL_Scancode);
// OS_DECL_CTYPE_ENUM(SDL_Keycode);
OS_DECL_OX_CLASS(PhysBlock);
OS_DECL_OX_CLASS(PhysContact);
OS_DECL_OX_CLASS(BaseGameLevel);
OS_DECL_OX_CLASS(BaseEntity);
OS_DECL_OX_CLASS(KeyboardEvent);

} // namespace ObjectScript

#endif //__CASTLEMONSTER_H__
