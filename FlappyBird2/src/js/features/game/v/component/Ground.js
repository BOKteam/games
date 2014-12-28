/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午4:34
 * Write the description in this section.
 */
goog.provide("app.features.game.v.component.Ground");
goog.require("org.createjs.easeljs.EaselJS");


app.features.game.v.component.Ground = Ground;
BOK.inherits(Ground, createjs.Container);
function Ground(){
    createjs.Container.call(this);

    this.grounds = [];
    for (var i = 0; i < 2; i++){
        this.grounds[i] = new createjs.Bitmap(imgContainer['assets/img/ground.png']);
        this.grounds[i].x = i * CONST.CANVAS.WIDTH;
        this.grounds[i].y = CONST.BG.HEIGHT;
        this.addChild(this.grounds[i]);
    }
}
