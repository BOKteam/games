/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 12/03/14
 * Time: 14:10
 * To change this template use File | Settings | File Templates.
 */

goog.provide("dotasim.features.maingame.m.PlayerDataProxy");
goog.require("bok.framework.core.BaseProxy");

dotasim.features.maingame.m.PlayerDataProxy = function()
{
    BaseProxy.call(this, 'PlayerDataProxy');

    this.playerData_ = {
        gold: 0,
        party: 0
    };
};
BOK.inherits(dotasim.features.maingame.m.PlayerDataProxy, BaseProxy);

/**
 * @param {number} amount
 * */
dotasim.features.maingame.m.PlayerDataProxy.prototype.receiveGold = function(amount)
{
    this.playerData_.gold += amount;
};

/**
 *
 * */
dotasim.features.maingame.m.PlayerDataProxy.prototype.getPlayerParty = function()
{
    return this.playerData_.party;
};

/**
 *
 * */
dotasim.features.maingame.m.PlayerDataProxy.prototype.getPlayerData = function()
{
    return this.playerData_;
};

/**
 *  @param {Object} data
 * */
dotasim.features.maingame.m.PlayerDataProxy.prototype.setPlayerData = function(data)
{
    if(data)
        this.playerData_ = data;
};
