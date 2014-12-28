/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 18/03/14
 * Time: 11:20
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.c.GameFinishedCMD");
goog.require("bok.framework.core.BaseCommand");
goog.require("bok.features.gamesave.GameSaveFeatureNotes");

dotasim.features.maingame.c.GameFinishedCMD = function()
{
    BaseCommand.call(this);
};
BOK.inherits(dotasim.features.maingame.c.GameFinishedCMD, BaseCommand);

/**
 * @override
 * */
dotasim.features.maingame.c.GameFinishedCMD.prototype.execute = function()
{
    //save player data after game
    var playerData = this.retrieveProxy('PlayerDataProxy').getPlayerData();
    this.sendParentNotification(GameSaveFeatureNotes.getInputNoteWithFeatureName('QUICK_SAVE'), playerData);
};

