/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 11/03/14
 * Time: 15:16
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.hud.m.GameDataProxy");

dotasim.features.hud.m.GameDataProxy = function()
{
    BaseProxy.call(this, 'GameDataProxy');

    this.heroData_ = null;
    this.playerData_ = null;
};
BOK.inherits(dotasim.features.hud.m.GameDataProxy, BaseProxy);

/**
 * @param {Object} data
 * */
dotasim.features.hud.m.GameDataProxy.prototype.setPlayerData = function(data)
{
    this.playerData_ = data;
};

/**
 * @param {Array} data
 * */
dotasim.features.hud.m.GameDataProxy.prototype.setHeroData = function(data)
{
    this.heroData_ = data;
};

dotasim.features.hud.m.GameDataProxy.prototype.getPlayerData = function()
{
    return this.playerData_;
};
dotasim.features.hud.m.GameDataProxy.prototype.getPlayerHeroes = function()
{
    var heroes = [];

    BOK.each(this.heroData_, function(hero){
        if(0 == hero.party)
            heroes.push(hero);
    });

    return heroes;
};
