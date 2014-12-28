/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 18/03/14
 * Time: 10:33
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.c.GameStartCMD");
goog.require("bok.framework.core.BaseCommand");

dotasim.features.maingame.c.GameStartCMD = function()
{
    BaseCommand.call(this);
};
BOK.inherits(dotasim.features.maingame.c.GameStartCMD, BaseCommand);

/**
 * @override
 * */
dotasim.features.maingame.c.GameStartCMD.prototype.execute = function()
{
    this.retrieveProxy('UnitsProxy').initBaseUnits();

    //broad cast out put note
    this.sendNotification(MainGameFeatureNotes.getOutputNote('GAME_STARTED'));
};


