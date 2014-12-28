/**
 * Created by Envee.
 *
 * Date: 14-10-9
 * Time: 下午3:10
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("noone.features.game.v.component.Obstacle");
goog.require("org.createjs.easeljs.EaselJS");

noone.features.game.v.component.Obstacle = Obstacle;
BOK.inherits(Obstacle, createjs.Container);
function Obstacle(){
    createjs.Container.call(this);
    this.height = CONST.OBSTACLE.MIN_HEIGHT + BOK.randN(CONST.OBSTACLE.MAX_HEIGHT - CONST.OBSTACLE.MIN_HEIGHT);
    this.width = CONST.OBSTACLE.MIN_WIDTH + BOK.randN(CONST.OBSTACLE.MAX_WIDTH - CONST.OBSTACLE.MIN_WIDTH);
    this.obstacle = new createjs.Shape(new createjs.Graphics().beginFill("rgba(215,125,86,1)").drawRect(0, 0, this.width, this.height));
    this.addChild(this.obstacle);
}

Obstacle.prototype.update = function() {
    this.x -= CONST.GAME_PLAY.SCROLL_SPEED;
    this.setBounds(this.x, this.y, this.width, this.height);
};

Obstacle.prototype.getRectRange = function() {
    return this.getBounds();
};
