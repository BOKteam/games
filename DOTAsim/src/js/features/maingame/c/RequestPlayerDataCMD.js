/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 12/03/14
 * Time: 19:00
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.c.RequestPlayerDataCMD");
goog.require("bok.framework.core.BaseCommand");

dotasim.features.maingame.c.RequestPlayerDataCMD = function()
{
    BaseCommand.call(this);
};
BOK.inherits(dotasim.features.maingame.c.RequestPlayerDataCMD, BaseCommand);

/**
 * @override
 * */
dotasim.features.maingame.c.RequestPlayerDataCMD.prototype.execute = function(msg)
{
    /** @type {dotasim.features.maingame.m.PlayerDataProxy}*/
    var proxy = this.retrieveProxy('PlayerDataProxy');

    msg.body.data = proxy.getPlayerData();
};
