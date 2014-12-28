/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 下午1:59
 * Write the description in this section.
 */
goog.provide("app.features.game.v.component.Bird");
goog.require("org.createjs.easeljs.EaselJS");

app.features.game.v.component.Bird = Bird;
BOK.inherits(Bird, createjs.Container);
function Bird(){
    createjs.Container.call(this);

    this.dropSpeed = 0;

    this.bird = new createjs.Sprite(Bird.AnimSheet, 'run');
    this.addChild(this.bird);

    this.bird.addEventListener('animationend', Delegate.create(this, this.onFlapFinished_));
}

Bird.AnimSheet = new createjs.SpriteSheet({
    framerate:15,
    images: ['assets/img/clumsy.png'],
    frames: {width:85, height:60},
    animations: {run:[0,2]}
});

Bird.prototype.flap = function() {
    this.dropSpeed = CONST.GAME_PLAY.FLAP_SPEED;
};

Bird.prototype.update = function() {
    this.dropSpeed += CONST.GAME_PLAY.GRAVITY;
    this.y += this.dropSpeed;

    this.bird.rotation = Math.min((this.dropSpeed/CONST.GAME_PLAY.FLAP_SPEED) * CONST.BIRD.FACE_ROTATE_MIN, CONST.BIRD.FACE_ROTATE_MAX);
};

Bird.prototype.onFlapFinished_ = function() {
};

