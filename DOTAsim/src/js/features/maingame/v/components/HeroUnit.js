/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 24/02/14
 * Time: 14:03
 * To change this template use File | Settings | File Templates.
 */

goog.provide("dotasim.features.maingame.v.components.HeroUnit");
goog.require("dotasim.features.maingame.v.components.Unit");

dotasim.HeroUnit = function(party)
{
    dotasim.Unit.call(this, party);

    //hero unit is slightly bigger
    this.scaleX = this.scaleY = 1.3;
};
BOK.inherits(dotasim.HeroUnit, dotasim.Unit);



