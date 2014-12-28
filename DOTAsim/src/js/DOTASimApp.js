/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 04/10/13
 * Time: 16:19
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.DOTASimApp");

goog.require("bok.framework.App");
goog.require("bok.features.gamesave.GameSaveFeature");
goog.require("dotasim.features.maingame.MainGameFeature");
goog.require("dotasim.features.hud.HudFeature");


/**
 * @param {createjs.Stage} stage a div element on DOM which will be used as game stage.
 * @constructor
 * */
function DOTASimApp(stage)
{
    App.call(this);

    //init mvc features
    this.addFeature(new GameSaveFeature());
    this.addFeature(new dotasim.features.maingame.MainGameFeature(stage));
    this.addFeature(new dotasim.features.hud.HudFeature(stage));
}
BOK.inherits(DOTASimApp, App);

/**
 * @override
 * */
DOTASimApp.prototype.initialize = function()
{
    DOTASimApp.superClass_.initialize.call(this);

    //window.addEventListener('focus', Delegate.create(this, this.onWindowFocus_));
    //window.addEventListener('blur', Delegate.create(this, this.onWindowBlur_));
};

DOTASimApp.prototype.onWindowFocus_ = function()
{
    this.sendNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('GAME_RESUME'));
};

DOTASimApp.prototype.onWindowBlur_ = function()
{
    this.sendNotification(MainGameFeatureNotes.getInputNoteWithFeatureName('GAME_PAUSE'));
};
