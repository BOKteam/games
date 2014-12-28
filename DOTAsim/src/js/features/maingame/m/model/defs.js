/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 18/02/14
 * Time: 14:55
 * To change this template use File | Settings | File Templates.
 */
goog.provide('dotasim.features.maingame.m.model.defs');

dotasim.model = {};

/**
 * @enumerate
 * */
dotasim.model.State = {
    ACTIVE: 'active',
    DEAD: 'dead',
    WAITING: 'waiting'
};


dotasim.model.UnitBase = function(id, type, x, y)
{
    this.id = id;
    this.type = type;
    this.action = {type:'IDLE', target:null};
    this.pos = {x:x, y:y};
    this.atkRange = 0;
    this.atk = 0;
    this.atkCD = 1000;
    this.hp = 0;
    this.speed = 0;
    this.exp = 0;
    this.level = 1;
};

dotasim.model.HQ = function(party, pos)
{
    dotasim.model.UnitBase.call(this, 'base_'+party, 'HQ', pos.x, pos.y);

    this.party = party;
    this.hp = dotasim.GamePlayData.HQ.HP;
};
BOK.inherits(dotasim.model.HQ, dotasim.model.UnitBase);

dotasim.model.Tower = function(party, pos)
{
    if(this.constructor.itemCounter_ == undefined)
        this.constructor.itemCounter_ = 0;
    var count = this.constructor.itemCounter_;

    dotasim.model.UnitBase.call(this, 'tower_'+party+'_'+count, 'TOWER', pos.x, pos.y);

    var towerData = dotasim.GamePlayData.TOWER;

    this.party = party;
    this.atkRange = towerData.ATK_RANGE;
    this.atk = towerData.ATK;
    this.atkCD = towerData.ATK_CD;
    this.hp = towerData.HP;

    this.constructor.itemCounter_++;
};
BOK.inherits(dotasim.model.Tower, dotasim.model.UnitBase);

/**
 * @param {Number} lane The lane that this creep walks on, can only be 0,1,2 from a dota game
 * */
dotasim.model.Creep = function(party, pos, lane)
{
    if(dotasim.model.Creep.itemCounter_ == undefined)
        dotasim.model.Creep.itemCounter_ = 0;
    var count = dotasim.model.Creep.itemCounter_;

    dotasim.model.UnitBase.call(this, 'creep_'+party+'_'+count, 'CREEP', pos.x, pos.y);

    var creepData = dotasim.GamePlayData.CREEP;

    this.lane = lane;
    this.party = party;
    this.atkRange = creepData.ATK_RANGE;
    this.atk = creepData.ATK;
    this.atkCD = creepData.ATK_CD;
    this.hp = creepData.HP;
    this.speed = creepData.SPD;

    dotasim.model.Creep.itemCounter_++;
};
BOK.inherits(dotasim.model.Creep, dotasim.model.UnitBase);

/**
 * @param {Number} lane The lane that this creep walks on, can only be 0,1,2 from a dota game
 * */
dotasim.model.RangeCreep = function(party, pos, lane)
{
    dotasim.model.Creep.call(this, party, pos, lane);

    //overwrite unit type
    this.type = 'RANGE_CREEP';

    this.atkRange = dotasim.GamePlayData.RANGE_CREEP.ATK_RANGE;
    this.hp = dotasim.GamePlayData.RANGE_CREEP.HP;
};
BOK.inherits(dotasim.model.RangeCreep, dotasim.model.Creep);

/**
 * */
dotasim.model.Hero = function(party, name)
{
    dotasim.model.UnitBase.call(this, 'hero_'+name, 'HERO', 0, 0);

    var heroData = dotasim.GamePlayData.HERO.Aryan;

    this.name = name;
    this.party = party;
    this.atkRange = heroData.ATK_RANGE;
    this.atk = heroData.ATK;
    this.atkCD = heroData.ATK_CD;
    this.hp = heroData.HP;
    this.speed = heroData.SPD;
    this.state = dotasim.model.State.WAITING;
};
BOK.inherits(dotasim.model.Hero, dotasim.model.UnitBase);

dotasim.model.RangeHero = function(party, name)
{
    dotasim.model.Hero.call(this, party, name);

    var heroData = dotasim.GamePlayData.HERO.Angi;

    //overwrite unit type
    this.type = 'RANGE_HERO';

    this.atkRange = heroData.ATK_RANGE;
    this.atk = heroData.ATK;
    this.atkCD = heroData.ATK_CD;
    this.hp = heroData.HP;
    this.speed = heroData.SPD;
};
BOK.inherits(dotasim.model.RangeHero, dotasim.model.Hero);


//TODO: this dude is test use only, or use it if you really hate the world.
dotasim.model.IMBAHero = function(party, name)
{
    dotasim.model.Hero.call(this, party, name);

    this.atk = 300;
    this.hp = 10000;
    this.speed = 100;
    this.atkCD = 100;
};
BOK.inherits(dotasim.model.IMBAHero, dotasim.model.Hero);

