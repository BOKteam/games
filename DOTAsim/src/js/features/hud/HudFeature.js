/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 20/02/14
 * Time: 15:07
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.hud.HudFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("dotasim.features.hud.HudFeatureNotes");
goog.require("dotasim.features.hud.m.GameDataProxy");
goog.require("dotasim.features.hud.v.HudMediator");

goog.require("dotasim.features.maingame.MainGameFeatureNotes");

/**
 * @param {createjs.Stage} stage
 * */
dotasim.features.hud.HudFeature = function(stage)
{
    MVCFeature.call(this, 'HudFeature');

    //add proxies
    this.dataProxy_ = new dotasim.features.hud.m.GameDataProxy();
    this.addProxy(this.dataProxy_);

    //add mediators
    this.addMediator(new dotasim.features.hud.v.HudMediator(stage, this.dataProxy_))
};
BOK.inherits(dotasim.features.hud.HudFeature, MVCFeature);

/**
 * @override
 * */
dotasim.features.hud.HudFeature.prototype.initialize = function()
{
    dotasim.features.hud.HudFeature.superClass_.initialize.call(this);

    var container = {data:null};
    this.sendParentNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('REQUEST_HERO_DATA'), container);
    this.dataProxy_.setHeroData(container.data);

    this.sendParentNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('REQUEST_PLAYER_DATA'), container);
    this.dataProxy_.setPlayerData(container.data);

    this.sendNotification(HudFeatureNotes.getInternalNote('GAME_INIT'));
};

