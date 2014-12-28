/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午10:49
 *
 * Command layer, write units command ONLY.
 */
goog.provide("dotasim.features.maingame.l.AIMediator");
goog.require("bok.framework.core.BaseMediator");

dotasim.features.maingame.l.AIMediator = function(unitsProxy)
{
    BaseMediator.call(this, 'AIMediator');
    /** @type {dotasim.features.maingame.m.UnitsProxy}*/
    this.unitsProxy_ = unitsProxy;
};
BOK.inherits(dotasim.features.maingame.l.AIMediator, BaseMediator);

/**
 * @override
 * */
dotasim.features.maingame.l.AIMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_STAGE_UPDATE'), this.onStageUpdate_);

    dotasim.features.maingame.l.AIMediator.superClass_.declareInterestedNotifications.call(this);
};

dotasim.features.maingame.l.AIMediator.prototype.onStageUpdate_ = function()
{
    var unitsData = this.unitsProxy_.getUnitsData();
    for(var i=0; i<unitsData.length; i++)
    {
        var unit = unitsData[i];
        //handle different types of units
        switch (unit.type)
        {
            case 'CREEP':
            case 'RANGE_CREEP':
            case 'HERO':
            case 'RANGE_HERO':
                //skip if already under command
                if('IDLE' != unit.action.type
                    && (unit.party ? unit.pos.x > unit.action.target.x : unit.pos.x < unit.action.target.x))
                    break;

                //do not handle units that are not assigned to any lane
                if(undefined == unit.lane)
                    break;

                unit.action.type = 'MOVE_ATTACK';
                if(0 == unit.party)
                {
                    if(unit.pos.x < 100)
                        unit.action.target = {x:100, y:80+unit.lane*160};
                    else if(unit.pos.x < 530)
                        unit.action.target = {x:530, y:80+unit.lane*160};
                    else
                        unit.action.target = {x:610, y:240};
                }
                else if(1 == unit.party)
                {
                    if(unit.pos.x > 530)
                        unit.action.target = {x:530, y:80+unit.lane*160};
                    else if(unit.pos.x > 100)
                        unit.action.target = {x:100, y:80+unit.lane*160};
                    else
                        unit.action.target = {x:30, y:240};
                }
                break;

            case 'TOWER':
                if('IDLE' == unit.action.type)
                {
                    unit.action.type = 'MOVE_ATTACK';
                    unit.action.target = {x:unit.pos.x, y:unit.pos.y};
                }
                break;
        }
    }
};
