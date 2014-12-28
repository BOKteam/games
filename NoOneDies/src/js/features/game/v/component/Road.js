/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-10-9
 * Time: 下午2:02
 * Write the description in this section.
 */


goog.provide("noone.features.game.v.component.Road");
goog.require("org.createjs.easeljs.EaselJS");


noone.features.game.v.component.Road = Road;
BOK.inherits(Road, createjs.Container);
function Road(){
    createjs.Container.call(this);

    this.roadPieces_ = [];
    for(var i =0; i<Road.MAX_PIECE; ++i) {
        this.roadPieces_[i] = new createjs.Bitmap(imgContainer['assets/img/road.png']);
        this.roadPieces_[i].x = i * CONST.ROAD.WIDTH;
        this.addChild(this.roadPieces_[i]);
    }
}

Road.MAX_PIECE = 7;

