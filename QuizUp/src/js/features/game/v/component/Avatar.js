/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.Avatar");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.Avatar = Avatar;
BOK.inherits(Avatar, createjs.Container);

/**
 * @param {number} x //the position of x for avatar
 * @param {number} y //the position of y for avatar
 * @param {number} radius // the radius of avatar circle
 * @param {string} borderColor // the color string : "#ffffff"
 * @param {string} avatar //the avatar src
 */
function Avatar(radius, borderColor, avatar, scale) {
   	createjs.Container.call(this);
   	
   	this.radius_ = radius;
   	this.scale_ = scale;
   	this.borderColor_ = borderColor;
	this.outShape_ = new createjs.Shape(new createjs.Graphics().beginFill(borderColor).drawCircle(0, 0, radius + 5));

    this.addChild(this.outShape_);
   	this.setAvatar(avatar);
}

Avatar.prototype.setAvatar = function(avatar){
	if(this.inShape){
		this.removeChild(this.inShape_);
	}
	if(!avatar){
		return;
	}
	var bitImg = new createjs.Bitmap(avatar);
	bitImg.set({x:-this.radius_ ,y: -this.radius_, scaleX: this.scale_, scaleY: this.scale_});

	var avatarImg = new Image()
	avatarImg.src = avatar;

	avatarImg.onload = Delegate.create(this, function(){
		this.inShape_ = new createjs.Shape(new createjs.Graphics().beginStroke(this.borderColor_).beginBitmapFill(avatarImg, "no-repeat", bitImg.getMatrix()).drawCircle(0, 0, this.radius_).endStroke());
		this.addChild(this.inShape_);
	});

	
}