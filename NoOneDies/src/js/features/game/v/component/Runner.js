/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午1:59
 * Write the description in this section.
 */
goog.provide("noone.features.game.v.component.Runner");
goog.require("org.createjs.easeljs.EaselJS");

noone.features.game.v.component.Runner = Runner;
BOK.inherits(Runner, createjs.Container);
function Runner(y){
    createjs.Container.call(this);

    this.dropSpeed = 0;
    this.baseY_ = y;
    this.bird = new createjs.Sprite(Runner.AnimSheet, 'run');

    this.addChild(this.bird);
    this.boundsRect_ = this.getBounds();

    this.bird.addEventListener('animationend', Delegate.create(this, this.onFlapFinished_));
}

Runner.AnimSheet = new createjs.SpriteSheet({
    framerate:20,
    images: ['assets/img/dh01_01.png','assets/img/dh01_02.png','assets/img/dh01_03.png','assets/img/dh01_04.png','assets/img/dh01_05.png', 'assets/img/dh01_06.png'],
    frames: {x:-5,width:37, height:55},
    animations: {run:[0,5,true,0.5], stop:[0]}
});

Runner.prototype.flap = function() {
    if(this.y == this.baseY_)
        this.dropSpeed = CONST.GAME_PLAY.FLAP_SPEED;

};

Runner.prototype.update = function() {
    this.dropSpeed += CONST.GAME_PLAY.GRAVITY;
    this.y += this.dropSpeed;
    this.setRectRange(this.x + 5,  this.y + 5, this.getBounds().width - 5,  this.getBounds().height -5);

    if(this.y > this.baseY_) {
        this.y = this.baseY_;
        this.dropSpeed = 0;
    }
    //this.bird.rotation = Math.min((this.dropSpeed/CONST.GAME_PLAY.FLAP_SPEED) * CONST.BIRD.FACE_ROTATE_MIN, CONST.BIRD.FACE_ROTATE_MAX);
};

Runner.prototype.onFlapFinished_ = function() {
};

Runner.prototype.getRectRange = function() {
    return this.boundsRect_;
};

Runner.prototype.setRectRange = function(x, y, width, height) {
    this.boundsRect_ = {x: x, y: y, width: width, height: height};
};