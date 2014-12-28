/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午9:09
 *
 * File over view.
 */

goog.provide("dotasim.features.maingame.m.UnitsProxy");
goog.require("dotasim.features.maingame.m.model.defs");
goog.require("bok.framework.core.BaseProxy");

/**
 * @param {Array} setupData
 * */
dotasim.features.maingame.m.UnitsProxy = function(setupData)
{
    BaseProxy.call(this, 'UnitsProxy');

    /** @type {Array} */
    this.baseData_ = setupData;
    this.heroData_ = [new dotasim.model.Hero(0, 'Nancy'),
        new dotasim.model.Hero(0, 'Aryan'),
        new dotasim.model.RangeHero(0, 'Flying'),
        new dotasim.model.Hero(1, '1'),
        new dotasim.model.Hero(1, '2'),
        new dotasim.model.Hero(1, '3')];


    /** @type {Array} */
    this.units_ = [];
};
BOK.inherits(dotasim.features.maingame.m.UnitsProxy, BaseProxy);

/**
 * this function will be called by main game Feature on its initialization.
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.initBaseUnits = function()
{
    //clear existing data
    this.units_ = [];
    this.resetHeroes_();

    var baseCopy = BOK.cloneObject(this.baseData_);

    BOK.each(baseCopy, function(unit){
        this.addUnit_(unit);
    }, this);

    //TODO: remove these test code
    //deploy TEST AI
    this.deployHero('hero_1', 0);
    this.deployHero('hero_2', 1);
    this.deployHero('hero_3', 2);
};

/**
 * @return {Array}
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.getUnitsData = function()
{
    return this.units_;
};

/**
 * @return {Array}
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.getHeroesData = function()
{
    return this.heroData_;
};

/**
 * Set hero back to ready waiting state and place in base
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.resetHero_ = function(hero)
{
    this.refreshHero_(hero);
    hero.action.type = 'IDLE';
    hero.state = dotasim.model.State.WAITING;
    hero.lane = undefined;
};
/**
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.resetHeroes_ = function()
{
    BOK.each(this.heroData_, function(hero){
        this.resetHero_(hero);
    }, this);
};

/**
 * @return {Object}
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.getHeroByID = function(id)
{
    var foundHero = null;
    BOK.each(this.heroData_, function(unit){
        if(unit.id == id)
        {
            foundHero = unit;
            return BOK.BREAK;
        }
    });

    return foundHero;
};

/**
 * @param id {String}
 * @return {Object}
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.getUnitByID = function(id)
{
    var foundUnit = null;
    BOK.each(this.units_, function(unit){
        if(unit.id == id)
        {
            foundUnit = unit;
            return BOK.BREAK;
        }
    });

    return foundUnit;
};

/**
 * Get all active heroes on map of a given party.
 * @param {Number} party
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.getActiveHeroes = function(party)
{
    var heroes = [];
    BOK.each(this.heroData_, function(hero){
        if(hero.state != dotasim.model.State.DEAD && hero.party == party)
            heroes.push(hero);
    });

    return heroes;
};

/**
 * @param id {String}
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.killUnitByID = function(id)
{
    var unit = this.removeUnitByID_(id);
    unit.state = dotasim.model.State.DEAD;
    this.sendNotification(MainGameFeatureNotes.getInternalNote('UNIT_KILLED'), unit);
};

/**
 * Add a unit to map
 * @param {String} heroID
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.callBackHero = function(heroID)
{
    this.removeUnitByID_(heroID);
    var hero = this.getHeroByID(heroID);
    hero.lane = undefined;
    if(dotasim.model.State.DEAD != hero.state)
        this.resetHero_(hero);
    this.sendNotification(MainGameFeatureNotes.getInternalNote('FADE_AWAY_HERO'), heroID);
};

/**
 * @param {String} id
 * @return {Object} full data of removed unit
 * @private
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.removeUnitByID_ = function(id)
{
    var removedUnit = null;
    BOK.each(this.units_, function(unit, index){
        if(unit.id == id)
        {
            removedUnit = unit;
            this.units_.splice(index, 1);
            return BOK.BREAK;
        }
    }, this);

    return removedUnit;
};
/**
 * Add a unit to map
 * @param {Object} unit
 * @private
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.addUnit_ = function(unit)
{
    this.units_.push(unit);
    this.sendNotification(MainGameFeatureNotes.getInternalNote('ADD_UNIT'), unit);
};

/**
 * deploy hero to one of the lanes
 * @param {String} heroID
 * @param {Number} lane
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.deployHero = function(heroID, lane)
{
    //cancel deployment if already deployed
    if(this.getUnitByID(heroID))
    {
        BOK.warn("dotasim.features.maingame.m.UnitsProxy.deployHero: unable to deploy Hero unit ["+heroID+"]," +
            "it's already deployed. call it back for redeployment.");
        return;
    }

    BOK.each(this.heroData_, function(hero){
        if(heroID == hero.id)
        {
            //always set hero lane dead or alive.
            hero.lane = lane;

            if(hero.hp > 0)
            {
                var basePos = dotasim.GameSettings.POS.HQ[hero.party];
                hero.pos.x = basePos.x + BOK.randN(6) - 3;
                hero.pos.y = basePos.y + BOK.randN(6) - 3;

                //hero deploy in IDLE state
                hero.action.type = 'IDLE';
                hero.state = dotasim.model.State.ACTIVE;

                this.addUnit_(hero);
            }
            return BOK.BREAK;
       }
    }, this);
};

/**
 * reborn hero after death
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.rebornHero = function(heroID)
{
    BOK.each(this.heroData_, function(hero){
        if(heroID == hero.id)
        {
            //if hero was assigned to lane, refresh and deploy straight away
            if(undefined != hero.lane)
            {
                this.refreshHero_(hero);
                this.deployHero(hero.id, hero.lane);
            }
            else
            {
                this.resetHero_(hero);
            }

            return BOK.BREAK;
        }
    }, this);
};


/**
 * restore hero to all green state
 * @param {Object} heroData
 * */
dotasim.features.maingame.m.UnitsProxy.prototype.refreshHero_ = function(heroData)
{
    //restore hero to full HP
    heroData.hp = dotasim.GamePlayData.HERO.Aryan.HP;
};

dotasim.features.maingame.m.UnitsProxy.prototype.spawnCreeps = function()
{
    var bp = new BOK2DGeo.Vec2(30, 240);
    this.addUnit_(new dotasim.model.RangeCreep(0, new BOK2DGeo.Vec2(bp.x - 10, bp.y - 5), 0));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x - 10, bp.y - 10), 0));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x, bp.y - 10),      0));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x + 10, bp.y - 10), 0));

    this.addUnit_(new dotasim.model.RangeCreep(0, new BOK2DGeo.Vec2(bp.x + 5, bp.y - 10), 1));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x + 10, bp.y - 10), 1));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x + 10, bp.y),      1));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x + 10, bp.y + 10), 1));

    this.addUnit_(new dotasim.model.RangeCreep(0, new BOK2DGeo.Vec2(bp.x - 10, bp.y + 5), 2));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x - 10, bp.y + 10), 2));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x, bp.y + 10),      2));
    this.addUnit_(new dotasim.model.Creep(0, new BOK2DGeo.Vec2(bp.x + 10, bp.y + 10), 2));

    bp = new BOK2DGeo.Vec2(610, 240);
    this.addUnit_(new dotasim.model.RangeCreep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y - 5), 0));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y - 10), 0));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x, bp.y - 10),      0));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x + 10, bp.y - 10), 0));

    this.addUnit_(new dotasim.model.RangeCreep(1, new BOK2DGeo.Vec2(bp.x - 5, bp.y - 10), 1));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y - 10), 1));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y),      1));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y + 10), 1));

    this.addUnit_(new dotasim.model.RangeCreep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y + 5), 2));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x - 10, bp.y + 10), 2));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x, bp.y + 10),      2));
    this.addUnit_(new dotasim.model.Creep(1, new BOK2DGeo.Vec2(bp.x + 10, bp.y + 10), 2));
};

