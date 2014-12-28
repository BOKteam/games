/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 11/03/14
 * Time: 14:53
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.c.RequestHeroDataCMD");
goog.require("bok.framework.core.BaseCommand");

dotasim.features.maingame.c.RequestHeroDataCMD = function()
{
    BaseCommand.call(this);
};
BOK.inherits(dotasim.features.maingame.c.RequestHeroDataCMD, BaseCommand);

/**
 * @override
 * */
dotasim.features.maingame.c.RequestHeroDataCMD.prototype.execute = function(msg)
{
    /** @type {dotasim.features.maingame.m.UnitsProxy}*/
    var unitsProxy = this.retrieveProxy('UnitsProxy');

    msg.body.data = unitsProxy.getHeroesData();
};
