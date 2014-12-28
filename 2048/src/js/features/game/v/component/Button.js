/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.features.game.v.component.Button");
goog.require("org.createjs.easeljs.EaselJS");

tzfe.features.game.v.component.Button = Button;
BOK.inherits(Button, createjs.Container);

function Button(width, height, value) {
    createjs.Container.call(this);

    var text = new createjs.Text(value, "bold 12px ClearSans", "#FFFFFF").set({x: width /2, y: height / 2, textAlign: 'center', textBaseline:'middle' });
    var buttonBack = new createjs.Shape(new createjs.Graphics().beginFill('#8F7A66').drawRoundRect(0, 0, width, height ,2));
    this.addChild(buttonBack, text);
}
