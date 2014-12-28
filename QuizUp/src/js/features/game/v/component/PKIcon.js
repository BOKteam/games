/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.PKIcon");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.PKIcon = PKIcon;
BOK.inherits(PKIcon, createjs.Container);

function PKIcon(size){
	createjs.Container.call(this);

	var g = new createjs.Graphics().beginFill("#ffffff").moveTo (size / 2, 0).lineTo (size / 2, size / 3).lineTo (size , size / 3).lineTo (size /2, size).lineTo (size /2, 2* size / 3).lineTo (0, 2*size / 3).lineTo(size / 2, 0).endFill();
	var shape = new createjs.Shape(g);
	this.addChild(shape);
}