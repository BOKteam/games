/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午8:52
 *
 * File over view.
 *
 * Physical execution layer.
 */

goog.provide("dotasim.features.maingame.l.LogicMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("bok.util.BOK2DGeo");

/**
 *
 * @param unitsProxy {dotasim.features.maingame.m.UnitsProxy}
 * @constructor
 * */
dotasim.features.maingame.l.LogicMediator = function(unitsProxy)
{
    BaseMediator.call(this, 'LogicMediator');
    /** @type {dotasim.features.maingame.m.UnitsProxy}*/
    this.unitsProxy_ = unitsProxy;
    this.FRAME_TIME = createjs.Ticker.getInterval();

    //this map prevent units stand on same spot
    //it's a 160X120 grid, each cell stands for a 4X4 map size
    this.unitStandMap_ = [];
    for(var i=0; i<640/dotasim.GameCoreData.MAP_GRID_CELL_SIZE;++i)
        this.unitStandMap_[i] = [];
};
BOK.inherits(dotasim.features.maingame.l.LogicMediator, BaseMediator);

/**
 * @override
 * */
dotasim.features.maingame.l.LogicMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_STAGE_UPDATE'), this.onStageUpdate_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('UNIT_KILLED'), this.onUnitKilled_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('FADE_AWAY_HERO'), this.onHeroRetracted__);

    dotasim.features.maingame.l.LogicMediator.superClass_.declareInterestedNotifications.call(this);
};

/**
 * @private
 * */
dotasim.features.maingame.l.LogicMediator.prototype.onUnitKilled_ = function(msg)
{
    var unitData = msg.body;
    if(unitData.occupyGridPos)
        this.unitStandMap_[unitData.occupyGridPos.x][unitData.occupyGridPos.y] = undefined;
};
/**
 * @private
 * */
dotasim.features.maingame.l.LogicMediator.prototype.onHeroRetracted__ = function(msg)
{
    var unitData = this.unitsProxy_.getHeroByID(msg.body);
    //clear temp priority destination
    unitData.priorityPos = undefined;
    this.unitStandMap_[unitData.occupyGridPos.x][unitData.occupyGridPos.y] = undefined;
};
/**
 * @private
 * */
dotasim.features.maingame.l.LogicMediator.prototype.onStageUpdate_ = function()
{
    /** @type {Array} */
    var data = this.unitsProxy_.getUnitsData();

    for(var i=0; i<data.length; i++)
    {
        var unitData = data[i];
        switch(unitData.action.type)
        {
            case 'IDLE':
                break;

            case 'MOVE':
                if(this.unitMoveToPos(unitData, unitData.action.target))
                    unitData.action.type = 'IDLE';
                break;

            case 'ATTACK':
                var targetUnit = this.unitsProxy_.getUnitByID(unitData.action.target);
                if(this.unitAttackTarget(unitData, targetUnit))
                    unitData.action.type = 'IDLE';
                break;

            case 'MOVE_ATTACK':
                var attackTatget = this.scanForAttackTarget_(data, unitData);
                if(attackTatget)
                    this.unitAttackTarget(unitData, attackTatget);
                else if(this.unitMoveToPos(unitData, unitData.action.target))
                    unitData.action.type = 'IDLE';
                break;

            case 'DIE':
                this.unitsProxy_.killUnitByID(unitData.id);
                break;
        }

        //run atk CD regardless of action
        if(unitData.atkCDCur && unitData.atkCDCur > 0)
        {
            unitData.atkCDCur -= this.FRAME_TIME;
        }
    }


    //TODO: debug use, please delete on release
    if(LOCAL_DEBUG.SHOW_GRID)
        this.sendNotification(MainGameFeatureNotes.getInternalNote('DEBUG_GRID_UPDATE'), this.unitStandMap_);
};

/**
 * @return {Boolean} if the target is killed after attack
 * */
dotasim.features.maingame.l.LogicMediator.prototype.unitAttackTarget = function(unitData, targetUnitData)
{
    //if unit already dead, return true.
    if(!targetUnitData)
        return true;

    var unitKilled = false;
    if(BOK2DGeo.dist(unitData.pos, targetUnitData.pos) > unitData.atkRange)
        this.unitMoveToPos(unitData, targetUnitData.pos);
    else
    {
        if(!unitData.atkCDCur || unitData.atkCDCur < 0)
        {
            //commit attack
            targetUnitData.hp -= unitData.atk;
            if(targetUnitData.hp <= 0){
                targetUnitData.action.type = 'DIE';
                targetUnitData.killedBy = unitData.id;
                unitKilled = true;
            }

            unitData.atkCDCur = unitData.atkCD;
            this.sendNotification(MainGameFeatureNotes.getInternalNote('UNIT_ATTACK'),
                {attackerID: unitData.id, attackCD:unitData.atkCD, targetID:targetUnitData.id});
        }
    }

    return unitKilled;
};

/**
 * @return {Boolean} if the target arrived on target pos
 * */
dotasim.features.maingame.l.LogicMediator.prototype.unitMoveToPos = function(unit, pos)
{
    if(pos.x == unit.pos.x && pos.y == unit.pos.y)
        return true;

    //if unit have priority pos (meaning unit is trying to bypass some blocker)
    //ignore given pos and use priority instead
    if(unit.priorityPos)
        pos = unit.priorityPos;

    var curPos = unit.pos;
    var spd = unit.speed;
    var vecDis = BOK2DGeo.Vec2['-'](pos, curPos);
    var vecAng = new BOK2DGeo.Vec2(vecDis);
    vecAng.normalize();
    var vecSpd = new BOK2DGeo.Vec2(vecAng);
    vecSpd.scale(spd);
    var distance = vecDis.abs();

    if(distance <= spd)
    {
        return this.fitUnitToPos_(unit, pos);
    }
    else
    {
        this.fitUnitToPos_(unit, BOK2DGeo.Vec2['+'](curPos, vecSpd));
        return false;
    }
};

/**
 * @private
 * */
dotasim.features.maingame.l.LogicMediator.prototype.scanForAttackTarget_ = function(allUnits, scaningUnit)
{
    var potentialTatget = {target:null, dis:Infinity};

    //scan for targets
    for(var i=0; i<allUnits.length; ++i)
    {
        var unit = allUnits[i];
        if(unit.party != scaningUnit.party)
        {
            var dis = new BOK2DGeo.Vec2(unit.pos.x - scaningUnit.pos.x, unit.pos.y - scaningUnit.pos.y).abs();
            if(dis < dotasim.GamePlayData.GENERAL.MOVE_ATTACK_SCAN_RANGE)
            {
                if(!potentialTatget.target || potentialTatget.dis > dis)
                {
                    potentialTatget.target = unit;
                    potentialTatget.dis = dis;
                }
            }
        }
    }

    return potentialTatget.target;
};
/**
 * @private
 * @return {boolean} true if the actually landing pos is the expected pos.
 *                  false meaning expected pos is taken and had to relocate.
 * */
dotasim.features.maingame.l.LogicMediator.prototype.fitUnitToPos_ = function(unit, expectedPos, rotatedDegree)
{
    if(undefined == rotatedDegree)
        rotatedDegree = 0;

    var V2 = BOK2DGeo.Vec2;
    //clear old occupying pos
    if(unit.occupyGridPos)
    {
        this.unitStandMap_[unit.occupyGridPos.x][unit.occupyGridPos.y] = undefined;
    }

    //try to fit to new pos
    var GRID_CELL_SIZE = dotasim.GameCoreData.MAP_GRID_CELL_SIZE;
    var gridPos = new V2(Math.floor(expectedPos.x / GRID_CELL_SIZE), Math.floor(expectedPos.y / GRID_CELL_SIZE));
    if(this.unitStandMap_[gridPos.x][gridPos.y])
    {
        if(Math.abs(rotatedDegree) < 360)
        {
            //if new pos is taken, try to by pass blocker by rotating moving direction
            //each unit have a random rotate direction preference, if not have, assign one.
            var stepDegree = 15 * (unit.rotatePreference || (unit.rotatePreference = BOK.randN(2) ? 1 : -1));

            var vecDis = V2['-'](expectedPos, unit.pos).rotate(stepDegree);
            var vecDisPrio = new V2(vecDis);
            //set priority destination as 5 steps tpwards rotated angle to by pass blocker
            unit.priorityPos = V2['+'](unit.pos, vecDisPrio.scale(10));
            //check modified destination
            this.fitUnitToPos_(unit, V2['+'](unit.pos, vecDis), rotatedDegree + stepDegree);
        }
        else
        {
            //if unit is fully surrounded, stand still.
            return false;
        }
    }
    else
    {
        //release priority pos if arrived
        if(unit.priorityPos && expectedPos.x == unit.priorityPos.x && expectedPos.y == unit.priorityPos.y)
            unit.priorityPos = undefined;

        unit.pos.x = expectedPos.x;
        unit.pos.y = expectedPos.y;
        unit.occupyGridPos = gridPos;
        this.unitStandMap_[gridPos.x][gridPos.y] = unit;
        return true;
    }

    return false;
};
