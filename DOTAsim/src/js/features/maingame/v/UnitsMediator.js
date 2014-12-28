/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午8:10
 *
 * File over view.
 */

goog.provide("dotasim.features.maingame.v.UnitsMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("bok.cssstage.CssStage");

goog.require("bok.cssstage.widgets.EaselStage");
goog.requireAll("dotasim.features.maingame.v.components.*");

/**
 * @param {createjs.Stage} stage
 * @param {BaseProxy} unitsProxy
 * */
dotasim.features.maingame.v.UnitsMediator = function(stage, unitsProxy)
{
    BaseMediator.call(this, 'UnitsMediator');
    /** @type {dotasim.features.maingame.m.UnitsProxy}*/
    this.unitsProxy_ = unitsProxy;

    this.units_ = {};
    /** @type {createjs.Container}*/
    this.gameStage_ = new createjs.Container();
    stage.addChild(this.gameStage_);

    /** @type {createjs.Container}*/
    this.unitsLayer_ = new createjs.Container();
    this.gameStage_.addChild(this.unitsLayer_);
    /** @type {createjs.Container}*/
    this.heroesLayer_ = new createjs.Container();
    this.gameStage_.addChild(this.heroesLayer_);

    //TODO: debug content
    this.debugGridMat_ = new createjs.Shape();
    this.gameStage_.addChild(this.debugGridMat_);
    this.UNIT_SIZE = dotasim.GameCoreData.MAP_GRID_CELL_SIZE;
};
BOK.inherits(dotasim.features.maingame.v.UnitsMediator, BaseMediator);


/**
 * @override
 * */
dotasim.features.maingame.v.UnitsMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(MainGameFeatureNotes.getInputNote('GAME_START'), this.onGameStart_);
    this.declareInterest(MainGameFeatureNotes.getInputNote('GAME_PAUSE'), this.onGamePause_);
    this.declareInterest(MainGameFeatureNotes.getInputNote('GAME_RESUME'), this.onGameResume_);

    this.declareInterest(MainGameFeatureNotes.getOutputNote('GAME_FINISHED'), this.onGameEnd_);

    this.declareInterest(MainGameFeatureNotes.getInternalNote('ADD_UNIT'), this.onUnitAdded_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('FADE_AWAY_HERO'), this.onUnitFade_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('UNIT_KILLED'), this.onUnitKilled_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('UNIT_ATTACK'), this.onUnitAttack_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('DEBUG_GRID_UPDATE'), this.onDebugGridUpdate_);

    dotasim.features.maingame.v.UnitsMediator.superClass_.declareInterestedNotifications.call(this);
};


dotasim.features.maingame.v.UnitsMediator.prototype.onGameStart_ = function()
{
    //clear all existing unit for a new game
    this.unitsLayer_.removeAllChildren();
    this.heroesLayer_.removeAllChildren();
    this.units_ = {};

    this.gameTickStart_();
};

dotasim.features.maingame.v.UnitsMediator.prototype.onGameEnd_ = function()
{
    this.gameTickStop_();
};

dotasim.features.maingame.v.UnitsMediator.prototype.onGamePause_ = function()
{
    this.gameTickStop_();
};

dotasim.features.maingame.v.UnitsMediator.prototype.onGameResume_ = function()
{
    this.gameTickStart_();
};

dotasim.features.maingame.v.UnitsMediator.prototype.onDebugGridUpdate_ = function(msg)
{
    var grid = msg.body;

    this.debugGridMat_.graphics.clear();
    for(var i=0; i<grid.length; ++i)
    {
        for(var j=0; j<grid[i].length;++j)
        {
            if(grid[i][j])
            {
                this.debugGridMat_.graphics.beginFill('rgba(0,250,0,1)').drawRect(i*this.UNIT_SIZE, j*this.UNIT_SIZE, this.UNIT_SIZE, this.UNIT_SIZE)
            }
        }
    }
};

dotasim.features.maingame.v.UnitsMediator.prototype.onUnitAdded_ = function(msg)
{
    var unitData = msg.body;
    var unitID = unitData.id;

    if(!this.units_[unitID])
    {
        var unit = null;
        var displayLayer = this.unitsLayer_;
        switch(unitData.type)
        {
            case 'TOWER':
            case 'HQ':
            case 'RANGE_CREEP':
                unit = new dotasim.RangeUnit(unitData.party);
                break;
            case 'CREEP':
                unit = new dotasim.Unit(unitData.party);
                break;
            case 'HERO':
                unit = new dotasim.HeroUnit(unitData.party, unitData.name);
                displayLayer = this.heroesLayer_;
                break;
            case 'RANGE_HERO':
                unit = new dotasim.RangeHeroUnit(unitData.party, unitData.name);
                displayLayer = this.heroesLayer_;
                break;

            default:
                BOK.warn('dotasim.features.maingame.v.UnitsMediator.onUnitAdded_: unknown unit type '+unitData.type);
        }

        if(unit)
        {
            this.units_[unitID] = unit;
            this.units_[unitID].x = unitData.pos.x;
            this.units_[unitID].y = unitData.pos.y;
            displayLayer.addChild(this.units_[unitID]);
        }
    }

};

dotasim.features.maingame.v.UnitsMediator.prototype.onUnitAttack_ = function(msg)
{
    var attacker = this.units_[msg.body.attackerID];
    var atkCD = msg.body.attackCD;
    var target = this.units_[msg.body.targetID];

    if(attacker && target)
        attacker.attack(atkCD, target);
};

dotasim.features.maingame.v.UnitsMediator.prototype.onUnitKilled_ = function(msg)
{
    var id = msg.body.id;

    if(this.units_[id])
    {
        this.units_[id].die();
        delete this.units_[id];
    }
};
dotasim.features.maingame.v.UnitsMediator.prototype.onUnitFade_ = function(msg)
{
    var id = msg.body;

    if(this.units_[id])
    {
        this.units_[id].exit();
        delete this.units_[id];
    }
};

dotasim.features.maingame.v.UnitsMediator.prototype.onStageUpdate_ = function()
{
    //stage update notification to trigger all game logic update
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GAME_STAGE_UPDATE'));

    var unitsData = this.unitsProxy_.getUnitsData();
    for(var i=0; i<unitsData.length; i++)
    {
        var unitData = unitsData[i];
        var unit = this.units_[unitData.id];

        //update entity position
        if(unit)
            unit.moveTo(unitData.pos);
    }
};

dotasim.features.maingame.v.UnitsMediator.prototype.gameTickStart_ = function()
{
    if(!this.tikerListener_)
        this.tikerListener_ = Delegate.create(this, this.onStageUpdate_);
    createjs.Ticker.addEventListener("tick", this.tikerListener_);
};
dotasim.features.maingame.v.UnitsMediator.prototype.gameTickStop_ = function()
{
    createjs.Ticker.removeEventListener("tick", this.tikerListener_);
};


