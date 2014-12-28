/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */


goog.provide("quiz.features.game.v.component.TextButton");
goog.require("org.createjs.easeljs.EaselJS");

quiz.features.game.v.component.TextButton = TextButton;
BOK.inherits(TextButton, createjs.Container);

function TextButton (width, height, bgColor, radius, textContent, textStyle, textColor, textAlign){
	createjs.Container.call(this);
	var g = new createjs.Graphics().beginFill(bgColor).drawRoundRect(0, 0, width, height, radius).endFill();
	var shape = new createjs.Shape(g);
	var text = new createjs.Text(textContent, textStyle, textColor);
	text.set({x: width / 2 , y: height / 2, textAlign: textAlign, textBaseline: "middle"});

	this.addChild(shape, text);
}
