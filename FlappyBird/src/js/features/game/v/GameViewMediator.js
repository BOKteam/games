/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:42
 * Write the description in this section.
 */
goog.provide("app.features.game.v.GameViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("app.features.game.v.component.Bird");

app.features.game.v.GameViewMediator = GameViewMediator;
BOK.inherits(GameViewMediator, BaseMediator);
function GameViewMediator(stage) {
    BaseMediator.call(this);

    var bg0 = new createjs.Bitmap(imgContainer['assets/img/bg.png']);
    this.bgLayer_ = new createjs.Container();
    this.bgLayer_.addChild(bg0);
    stage.addChild(this.bgLayer_);


    this.bird = new app.features.game.v.component.Bird();
    this.bird.x = 100;
    stage.addChild(this.bird);

    createjs.Ticker.addEventListener('tick', Delegate.create(this, this.update_));
    stage.addEventListener('mousedown', Delegate.create(this, this.onTap_));
}

GameViewMediator.prototype.update_ = function(){
    this.bgLayer_.x -= CONST.GAME_PLAY.SCROLL_SPEED;
    if(this.bgLayer_.x <= -CONST.BG.SEGMENT)
        this.bgLayer_.x  = 0;

    this.bird.update();
    if(this.bird.y > CONST.BG.HEIGHT)
        this.bird.y = 0;
};
GameViewMediator.prototype.onTap_ = function(){
    this.bird.flap();
};

