/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("tzfe.features.game.v.component.GameOver");
goog.require("tzfe.features.game.v.component.Button");
goog.require("org.createjs.easeljs.EaselJS");

tzfe.features.game.v.component.GameOver = GameOver;
BOK.inherits(GameOver, createjs.Container);

function GameOver() {
    createjs.Container.call(this);
    var bg = new createjs.Shape(new createjs.Graphics().beginFill('rgba(255,255,255,0.5)').drawRect(0, 0, CONST.GRID.WIDTH, CONST.GRID.HEIGHT ));
    var text = new createjs.Text("GameOver", "bold 50px ClearSans", "#8F7A66").set({x: CONST.GRID.WIDTH /2, y: CONST.GRID.WIDTH / 2 , textAlign: 'center', textBaseline:'bottom' });
    this.buttonReplay = new tzfe.features.game.v.component.Button(140, 40, "Try Again");
    this.buttonReplay.set({x: CONST.GRID.WIDTH /2 - 70, y:CONST.GRID.HEIGHT / 2 + 50});
    this.addChild(bg, text, this.buttonReplay);
}
