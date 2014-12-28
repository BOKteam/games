/**
 * @author lys.BOK
 * Date: 14-2-7
 * Time: 下午9:43
 *
 * File over view.
 */
goog.provide("dotasim.features.maingame.MainGameFeature");
goog.require("bok.framework.core.MVCFeature");
goog.require("bok.features.gamesave.GameSaveFeatureNotes");

goog.require("dotasim.features.maingame.MainGameFeatureNotes");
goog.requireAll("dotasim.features.maingame.l.*");
goog.requireAll("dotasim.features.maingame.m.*");
goog.requireAll("dotasim.features.maingame.v.*");
goog.requireAll("dotasim.features.maingame.c.*");


/**
 * @param {createjs.Stage} stage
 * */
dotasim.features.maingame.MainGameFeature = function(stage)
{
    MVCFeature.call(this, 'MainGameFeature');

    this.unitsProxy_ = new dotasim.features.maingame.m.UnitsProxy(dotasim.assets.data.setup.InitialUnitsData);
    this.playerProxy_ = new dotasim.features.maingame.m.PlayerDataProxy();
    this.addProxy(this.unitsProxy_);
    this.addProxy(this.playerProxy_);

    this.addMediator(new dotasim.features.maingame.v.UnitsMediator(stage, this.unitsProxy_));
    this.addMediator(new dotasim.features.maingame.l.LogicMediator(this.unitsProxy_));
    this.addMediator(new dotasim.features.maingame.l.AIMediator(this.unitsProxy_));
    this.addMediator(new dotasim.features.maingame.l.MapControlMediator(this.unitsProxy_, this.playerProxy_));
    this.addMediator(new dotasim.features.maingame.l.PlayerCommandMediator(this.unitsProxy_));

    this.addCommand(MainGameFeatureNotes.getInputNote('REQUEST_PLAYER_DATA'), dotasim.features.maingame.c.RequestPlayerDataCMD);
    this.addCommand(MainGameFeatureNotes.getInputNote('REQUEST_HERO_DATA'), dotasim.features.maingame.c.RequestHeroDataCMD);
    this.addCommand(MainGameFeatureNotes.getInputNote('GAME_START'), dotasim.features.maingame.c.GameStartCMD);
    this.addCommand(MainGameFeatureNotes.getOutputNote('GAME_FINISHED'), dotasim.features.maingame.c.GameFinishedCMD);
    this.addCommand(MainGameFeatureNotes.getInternalNote('PLAYER_GAIN_GOLD'), dotasim.features.maingame.c.PlayerGainGoldCMD);
};
BOK.inherits(dotasim.features.maingame.MainGameFeature, MVCFeature);

/**
 * this is called after setup to initiate feature.
 * @override
 * */
dotasim.features.maingame.MainGameFeature.prototype.initialize = function()
{
    //load saved player data
    var saveContainer = {data:null};
    this.sendParentNotification(GameSaveFeatureNotes.getInputNoteWithFeatureName('QUICK_LOAD'), saveContainer);
    this.playerProxy_.setPlayerData(saveContainer.data);

    dotasim.features.maingame.MainGameFeature.superClass_.initialize.call(this);
	
	this.sendNotification(MainGameFeatureNotes.getInputNote('GAME_START'));
};
