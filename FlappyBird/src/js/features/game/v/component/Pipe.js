/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午4:34
 * Write the description in this section.
 */
goog.provide("app.features.game.v.component.Pipe");
goog.require("org.createjs.easeljs.EaselJS");


app.features.game.v.component.Pipe = Pipe;
BOK.inherits(Pipe, createjs.Container);
function Pipe(){
    createjs.Container.call(this);

    this.pipe = new createjs.Bitmap(imgContainer['assets/img/pipe.png']);
    this.addChild(this.pipe);
}
