/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 20/02/14
 * Time: 15:10
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.hud.v.HudMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("dotasim.features.hud.v.components.GameHud");
goog.require("bok.cssstage.CssStage");

goog.require("dotasim.features.maingame.MainGameFeatureNotes");

/**
 * @param {createjs.Stage} stage
 * @param {dotasim.features.hud.m.HeroDataProxy} heroProxy
 * */
dotasim.features.hud.v.HudMediator = function(stage, heroProxy)
{
    BaseMediator.call(this, 'HudMediator');

    this.heroDataProxy_ = heroProxy;

    this.hud_ = new dotasim.features.hud.v.components.GameHud();
    this.hud_.visible = false;
    this.hud_.addEventListener('open', Delegate.create(this, this.onHudOpen_));
    this.hud_.addEventListener('hero_deployed', Delegate.create(this, this.onHeroDeployed_));
    this.hud_.addEventListener('hero_retracted', Delegate.create(this, this.onHeroRetracted_));
    stage.addChild(this.hud_);
};
BOK.inherits(dotasim.features.hud.v.HudMediator, BaseMediator);

/**
 * @override
 * */
dotasim.features.hud.v.HudMediator.prototype.declareInterestedNotifications = function()
{
    this.declareInterest(HudFeatureNotes.getInternalNote('GAME_INIT'), this.onGameInit_);

    //interest on external notification
    this.declareInterest(MainGameFeatureNotes.getOutputNoteWithFeatureName('GAME_STARTED'), this.onGameStarted_, BaseMediator.SCOPE.PARENT);

    dotasim.features.hud.v.HudMediator.superClass_.declareInterestedNotifications.call(this);
};

/**
 * Initialize game HUD before game starts
 * @private
 * */
dotasim.features.hud.v.HudMediator.prototype.onGameStarted_ = function()
{
    this.hud_.visible = true;
};
/**
 * Initialize game HUD data before game starts
 * @private
 * */
dotasim.features.hud.v.HudMediator.prototype.onGameInit_ = function()
{
    this.hud_.setHeroData(this.heroDataProxy_.getPlayerHeroes());
    this.hud_.setPlayerData(this.heroDataProxy_.getPlayerData());
};

/**
 * @private
 * */
dotasim.features.hud.v.HudMediator.prototype.onHeroRetracted_ = function(e)
{
    this.sendParentNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('CALL_BACK_HERO'), e.id);
};
/**
 * @private
 * */
dotasim.features.hud.v.HudMediator.prototype.onHeroDeployed_ = function(e)
{
    this.sendParentNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('DEPLOY_HERO'), {id:e.id, lane:e.lane});
};
/**
 * @private
 * */
dotasim.features.hud.v.HudMediator.prototype.onHudOpen_ = function()
{
};
