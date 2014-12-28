/**
 * Created by JetBrains PhpStorm.
 * User: xliu
 * Date: 19/03/14
 * Time: 10:15
 * To change this template use File | Settings | File Templates.
 */
goog.provide("dotasim.features.maingame.v.components.RangeHeroUnit");
goog.require("dotasim.features.maingame.v.components.RangeUnit");

dotasim.RangeHeroUnit = function(party)
{
    dotasim.RangeUnit.call(this, party);

    //hero unit is slightly bigger
    this.scaleX = this.scaleY = 1.3;
};
BOK.inherits(dotasim.RangeHeroUnit, dotasim.RangeUnit);


