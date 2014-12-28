/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 12/03/14
 * Time: 14:15
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.c.PlayerGainGoldCMD");
goog.require("bok.framework.core.BaseCommand");

dotasim.features.maingame.c.PlayerGainGoldCMD = function()
{
    BaseCommand.call(this);
};
BOK.inherits(dotasim.features.maingame.c.PlayerGainGoldCMD, BaseCommand);

/**
 * @override
 * */
dotasim.features.maingame.c.PlayerGainGoldCMD.prototype.execute = function(msg)
{
    this.retrieveProxy('PlayerDataProxy').receiveGold(msg.body);
};

