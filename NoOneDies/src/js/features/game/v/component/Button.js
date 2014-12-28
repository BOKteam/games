/**
 * Created by Envee.
 *
 * Date: 14-10-11
 * Time: 下午4:48
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("noone.features.game.v.component.Button");
goog.require("org.createjs.easeljs.EaselJS");

noone.features.game.v.component.Button = Button;
BOK.inherits(Button, createjs.Container);

function Button(text){
    createjs.Container.call(this);

    this.text = new createjs.Text(text,"15px bold dimboregular", "#fff").set({x:30, y:4});

    this.addChild(new createjs.Shape(new createjs.Graphics().beginFill('#000').drawRoundRect(0,0,120,30,30)));
    this.addChild(this.text);
    this.addEventListener('mouseover', Delegate.create(this, this.onMouseOver_ ));
    this.addEventListener('mouseout', Delegate.create(this, this.onMouseOut_ ))
}

Button.prototype.onMouseOver_ = function(){
    this.mask_ = new createjs.Shape(new createjs.Graphics().beginFill('rgba(255,255,255,0.5)').drawRoundRect(0,0,120,30,30));
    this.addChild(this.mask_);
};

Button.prototype.onMouseOut_ = function(){
    this.removeChild(this.mask_);
};
