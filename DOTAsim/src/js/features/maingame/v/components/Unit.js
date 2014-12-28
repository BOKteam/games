/**
 * @author lys.BOK
 * Date: 14-2-9
 * Time: 下午8:18
 *
 * File over view.
 */

goog.provide("dotasim.features.maingame.v.components.Unit");
goog.require("org.createjs.easeljs.EaselJS");


/**
 * @param {Number} party Decides the color of unit
 * */
dotasim.Unit = function(party)
{
    createjs.Container.call(this);

    var colour = party ? 'rgba(255,50,50,1)' : 'rgba(50,50,255,1)';

    var i=0;
    this.unitShapes_ = [];
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill(colour).drawRect(0, 0, dotasim.Unit.WIDTH/2, dotasim.Unit.HEIGHT/2);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill(colour).drawRect(dotasim.Unit.WIDTH/2, 0, dotasim.Unit.WIDTH/2, dotasim.Unit.HEIGHT/2);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill(colour).drawRect(0, dotasim.Unit.HEIGHT/2, dotasim.Unit.WIDTH/2, dotasim.Unit.HEIGHT/2);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill(colour).drawRect(dotasim.Unit.WIDTH/2, dotasim.Unit.HEIGHT/2, dotasim.Unit.WIDTH/2, dotasim.Unit.HEIGHT/2);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill('rgba(20,20,20,1)').drawRect((dotasim.Unit.WIDTH-dotasim.Unit.EDGE)/2, -2, dotasim.Unit.EDGE, dotasim.Unit.HEIGHT);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill('rgba(20,20,20,1)').drawRect(0, 0, dotasim.Unit.WIDTH, dotasim.Unit.EDGE);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill('rgba(20,20,20,1)').drawRect(0, dotasim.Unit.HEIGHT-dotasim.Unit.EDGE, dotasim.Unit.WIDTH, dotasim.Unit.EDGE);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill('rgba(20,20,20,1)').drawRect(0, 0, dotasim.Unit.EDGE, dotasim.Unit.HEIGHT);
    this.unitShapes_[i] = new createjs.Shape();
    this.unitShapes_[i++].graphics.beginFill('rgba(20,20,20,1)').drawRect(dotasim.Unit.WIDTH-dotasim.Unit.EDGE, 0, dotasim.Unit.EDGE, dotasim.Unit.HEIGHT);
    this.root_ = new createjs.Container();
    this.root_.x = -dotasim.Unit.WIDTH/2;
    this.root_.y = -dotasim.Unit.HEIGHT/2;

    for(i=0; i<this.unitShapes_.length; ++i)
        this.root_.addChild(this.unitShapes_[i]);
    this.addChild(this.root_);
};
dotasim.Unit.WIDTH = 10;
dotasim.Unit.HEIGHT = 10;
dotasim.Unit.EDGE = 2;
BOK.inherits(dotasim.Unit, createjs.Container);

/**
 * @param {Object} pos expected format {x:Number, y:Number}
 * */
dotasim.Unit.prototype.moveTo = function(pos)
{
    if(this.x != pos.x || this.y != pos.y)
    {
        this.rotateToFacePos(pos.x, pos.y);
        this.x = pos.x;
        this.y = pos.y;
    }
};

/**
 * @param {Number} atkCD
 * @param {dotasim.Unit} target Target is not needed in melee attack tho.
 * */
dotasim.Unit.prototype.attack = function(atkCD, target)
{
    this.rotateToFacePos(target.x, target.y);
    var spd = atkCD < 400 ? atkCD : 400;
    var rootY = this.root_.y;

    createjs.Tween.get(this.root_).to({y:rootY-4},spd/2).to({y:rootY},spd/2);
};

dotasim.Unit.prototype.exit = function()
{
    var parent = this.parent;
    var self = this;
    createjs.Tween.get(this).to({alpha:0}, 500).call(function(){parent.removeChild(self)});
};

dotasim.Unit.prototype.die = function()
{
    for(var i=0; i<this.unitShapes_.length; ++i)
        createjs.Tween.get(this.unitShapes_[i]).to({x:BOK.randN(200)-100,y:BOK.randN(200)-100, alpha:0}, 500);

    var parent = this.parent;
    var self = this;
    createjs.Tween.get({}).wait(500).call(function(){parent.removeChild(self)});
};

dotasim.Unit.prototype.rotateToFacePos = function(x, y)
{
    this.rotation = new BOK2DGeo.Vec2(x - this.x, y - this.y).toAngle();
};