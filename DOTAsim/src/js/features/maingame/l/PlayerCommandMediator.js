/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 25/02/14
 * Time: 14:36
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.l.PlayerCommandMediator");
goog.require("bok.framework.core.BaseMediator");

/**
 *
 * @param unitsProxy {dotasim.features.maingame.m.UnitsProxy}
 * */
dotasim.features.maingame.l.PlayerCommandMediator = function(unitsProxy)
{
    BaseMediator.call(this, 'PlayerCommandMediator');
    this.unitsProxy_ = unitsProxy;
};
BOK.inherits(dotasim.features.maingame.l.PlayerCommandMediator, BaseMediator);


/**
 * @override
 * */
dotasim.features.maingame.l.PlayerCommandMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(MainGameFeatureNotes.getInputNote('DEPLOY_HERO'), this.onDeployHero_);
    this.declareInterest(MainGameFeatureNotes.getInputNote('CALL_BACK_HERO'), this.onCallBackHero_);

    dotasim.features.maingame.l.PlayerCommandMediator.superClass_.declareInterestedNotifications.call(this);
};

dotasim.features.maingame.l.PlayerCommandMediator.prototype.onCallBackHero_ = function(msg)
{
    var id = msg.body;

    this.unitsProxy_.callBackHero(id);
};

/**
 * @private
 * */
dotasim.features.maingame.l.PlayerCommandMediator.prototype.onDeployHero_ = function(msg)
{
    var id = msg.body.id;
    var lane = msg.body.lane;

    this.unitsProxy_.deployHero(id, lane);
};
