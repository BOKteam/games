/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 19/02/14
 * Time: 14:07
 *
 * Logic execution layer.
 * Process mainly overall in game logic.
 */
goog.provide("dotasim.features.maingame.l.MapControlMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.underscore._");

/**
 *
 * @param unitsProxy {dotasim.features.maingame.m.UnitsProxy}
 * */
dotasim.features.maingame.l.MapControlMediator = function(unitsProxy, playerProxy)
{
    BaseMediator.call(this, 'MapControlMediator');

    this.unitsProxy_ = unitsProxy;
    this.playerProxy_ = playerProxy;
    this.FRAME_TIME = createjs.Ticker.getInterval();
    this.resetControl_();
};
BOK.inherits(dotasim.features.maingame.l.MapControlMediator, BaseMediator);

/**
 * @override
 * */
dotasim.features.maingame.l.MapControlMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(MainGameFeatureNotes.getInputNote('GAME_START'), this.onGameStart_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('UNIT_KILLED'), this.onUnitKilled_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_STAGE_UPDATE'), this.onStageUpdate_);

    dotasim.features.maingame.l.MapControlMediator.superClass_.declareInterestedNotifications.call(this);
};



dotasim.features.maingame.l.MapControlMediator.prototype.resetControl_ = function()
{
    this.spawnWaitedTime_ =0;
    this.rebornHeroes_ = [];
};

dotasim.features.maingame.l.MapControlMediator.prototype.onGameStart_ = function(msg)
{
    this.resetControl_();
};

dotasim.features.maingame.l.MapControlMediator.prototype.onUnitKilled_ = function(msg)
{
    var deadUnit = msg.body;

    //When any unit dead get heroes of oppersite party and grant RXP
    var heroes = this.unitsProxy_.getActiveHeroes(deadUnit.party ? 0 : 1);
    BOK.each(heroes, function(hero){
        //Gain EXP if active hero is in range
        if(dotasim.model.State.ACTIVE == hero.state
            && BOK2DGeo.dist(deadUnit.pos, hero.pos) < dotasim.GamePlayData.GENERAL.EXP_RECEIVE_RANGE)
        {
            hero.exp += 1;
            if(hero.exp > 10)
            {
                hero.level++;
                hero.exp = 0;
                hero.atk += 0.5;
            }
        }

        //Gain gold if killed by ally hero
        if(this.playerProxy_.getPlayerParty() == hero.party && deadUnit.killedBy == hero.id)
        {
            this.sendNotification(MainGameFeatureNotes.getInternalNote('PLAYER_GAIN_GOLD'), 1);
        }
    }, this);

    //Handle special die
    switch(deadUnit.type)
    {
        case 'HERO':
        case 'RANGE_HERO':
            this.heroKilled_(deadUnit);
            break;

        case 'HQ':
            this.sendNotification(MainGameFeatureNotes.getOutputNote('GAME_FINISHED'));
            break;
    }
};

dotasim.features.maingame.l.MapControlMediator.prototype.heroKilled_ = function(heroData)
{
    heroData.rebornTime = dotasim.GamePlayData.GENERAL.HERO_REBORN_TIME * heroData.level;
    this.rebornHeroes_.push(heroData);
};

dotasim.features.maingame.l.MapControlMediator.prototype.onStageUpdate_ = function()
{
    //spwan creeps periodically
    var SPAWN_CD = dotasim.GamePlayData.GENERAL.CREEP_SPAWN_CD;
    this.spawnWaitedTime_ += this.FRAME_TIME;
    if(this.spawnWaitedTime_ >= SPAWN_CD)
    {
        this.spawnWaitedTime_ = 0;
        this.unitsProxy_.spawnCreeps();
    }

    //handle hero reborn
    if(!_.isEmpty(this.rebornHeroes_))
    {
        BOK.each(this.rebornHeroes_, function(hero, index){
            hero.rebornTime -= this.FRAME_TIME;

            if(hero.rebornTime <= 0)
            {
                this.unitsProxy_.rebornHero(hero.id);
                this.rebornHeroes_.splice(index,1);
            }
        }, this);
    }
};
