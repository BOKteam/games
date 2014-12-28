/**
 * @author lys.BOK
 * Date: 14-2-18
 * Time: 下午11:22
 *
 * File over view.
 */
goog.provide("dotasim.features.maingame.v.components.RangeUnit");
goog.require("dotasim.features.maingame.v.components.Unit");


dotasim.RangeUnit = function(party)
{
    dotasim.Unit.call(this, party);

    this.weaponLayer_ = new createjs.Container();
    this.addChild(this.weaponLayer_);
};
BOK.inherits(dotasim.RangeUnit, dotasim.Unit);

/**
 * @override
 * @param {Number} atkCD
 * @param {dotasim.Unit} target Target is not needed in melee attack tho.
 * */
dotasim.RangeUnit.prototype.attack = function(atkCD, target)
{
    this.rotateToFacePos(target.x, target.y);

    var spd = atkCD < 400 ? atkCD : 400;
    var vecDis = new BOK2DGeo.Vec2(target.x - this.x, target.y - this.y);
    var weapon = this.weaponLayer_;
    var bullet = new createjs.Shape();

    bullet.graphics.beginStroke("#F00")
        .moveTo(0, 0).lineTo(0, -vecDis.abs())
        .moveTo(-1, 0).lineTo(-1, -vecDis.abs())
        .moveTo(1, 0).lineTo(1, -vecDis.abs());
    //bullet.alpha = 0;
    weapon.addChild(bullet);

    createjs.Tween.get(bullet).to({alpha:1},spd*2/3).to({alpha:0},spd/3).call(function(){
        weapon.removeChild(bullet);
    });
};

