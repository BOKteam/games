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
goog.require("app.features.game.v.component.Pipe");
goog.require("app.features.game.v.component.Ground");

app.features.game.v.GameViewMediator = GameViewMediator;
BOK.inherits(GameViewMediator, BaseMediator);
function GameViewMediator(stage) {
    BaseMediator.call(this);
    this.stage_ = stage;
    this.obstacles_ = [];
    if(!localStorage.getItem('slappyFishHighStep')) localStorage.setItem('slappyFishHighStep',0);

    var bg = new createjs.Bitmap(imgContainer['assets/img/bg.png']);
    this.obstacleLayer_ = new createjs.Container();
    this.ground = new app.features.game.v.component.Ground();
    this.logo = new createjs.Bitmap(imgContainer['assets/img/logo.png']);
    //game over
    this.gameOverLayer_ = new createjs.Container();
    var gameOverBg = new createjs.Bitmap(imgContainer['assets/img/gameoverbg.png']);
    var gameOver = new createjs.Bitmap(imgContainer['assets/img/gameover.png']);
    gameOverBg.set({ x:197, y:119});
    gameOver.set({ x:327, y:140});
    this.stepText = new createjs.Text("","40px gameFont", "#000");
    this.stepText.set({x:300,y:300});
    this.stepHighText = new createjs.Text("","40px gameFont", "#000");
    this.stepHighText.set({x:300,y:350});
    this.gameOverLayer_.addChild(gameOverBg, gameOver, this.stepText, this.stepHighText);
    // step
    this.step = new createjs.Text(0,"25px gameFont", "#000");
    this.step.set({x:440,y:10});

    this.bird = new app.features.game.v.component.Bird();
    this.ready = new createjs.Bitmap(imgContainer['assets/img/getready.png']);
    this.ready.set({ x:250, y:200});

    this.stage_.addChild(bg, this.obstacleLayer_, this.ground,this.logo,this.gameOverLayer_,this.bird, this.ready, this.step);
    this.init();
}

GameViewMediator.prototype.init = function(){
    this.step.text = 0;
    this.ticker_ = this.tickerMax_ = 90;
    this.bird.set({ x:CONST.BIRD.OFFSETX, y:200 });
    this.bird.resetPos(0,0);
    this.bird.bird.gotoAndPlay("run");
    this.logo.set({ x:275, y:0});
    this.ready.set({alpha:1});
    this.stepHigher = localStorage.getItem('slappyFishHighStep');

    this.logo.visible = true;
    this.bird.visible = false;
    this.ready.visible = false;
    this.gameOverLayer_.visible = false;
    this.step.visible = false;

    createjs.Tween.get(this.logo).to({y:200}, 500);
    this.stage_.addEventListener('mousedown', Delegate.create(this, this.onReady_));
};

GameViewMediator.prototype.onReady_ = function(){
    this.bird.visible = true;
    this.ready.visible = true;
    this.logo.visible = false;

    this.stage_.removeAllEventListeners('mousedown');
    createjs.Tween.get(this.ready).to({alpha:0}, 2000).call(Delegate.create(this, this.onPlay_));
};

GameViewMediator.prototype.onPlay_ = function(){
    this.step.visible = true;
    this.playStatus = true;
    this.tickListener = createjs.Ticker.addEventListener('tick', Delegate.create(this, this.update_));
    this.stage_.addEventListener('mousedown', Delegate.create(this, this.onTap_));
};

GameViewMediator.prototype.onTap_ = function(){
    if(this.playStatus){
        this.bird.flap();
    }
};

GameViewMediator.prototype.onGameOver_ = function(){
    this.playStatus = false;
    this.step.visible = false;
    this.bird.set({ x:CONST.BIRD.OFFSETX + CONST.BIRD.WIDTH });
    this.bird.bird.gotoAndStop();
    this.bird.resetPos(90,0);
    createjs.Ticker.off("tick", this.tickListener);
    createjs.Tween.get(this.bird).to({y:CONST.BG.HEIGHT - CONST.BIRD.HEIGHTVERTICAL}, 1000).call(Delegate.create(this, this.onGameOverShow_));
};

GameViewMediator.prototype.onGameOverShow_ = function(){
    this.stepText.text = "step: " + this.step.text;
    this.stepHighText.text = "Higher step: " + localStorage.getItem('slappyFishHighStep');
    this.bird.visible = false;
    this.gameOverLayer_.visible = true;
    this.clearAllObstacle();
    this.stage_.removeAllEventListeners('mousedown');
    this.stage_.addEventListener('mousedown', Delegate.create(this, this.init));
};

GameViewMediator.prototype.generateObstacle_ = function(){
    var pipe = new app.features.game.v.component.Pipe();
    this.obstacleLayer_.addChild(pipe);
    this.obstacles_.push(pipe);
};

GameViewMediator.prototype.clearAllObstacle = function(){
    BOK.each(this.obstacles_, function(item, index){
        this.obstacleLayer_.removeChild(item);
        this.obstacles_.splice(index, 1);
    }, this);
};

GameViewMediator.prototype.update_ = function(){
    this.bird.update();
    // ground
    this.ground.x -= CONST.GAME_PLAY.SCROLL_SPEED;
    if(this.ground.x <= -CONST.CANVAS.WIDTH)
        this.ground.x  = 0;
    //generate obstacle
    if(this.ticker_ == this.tickerMax_){
        this.generateObstacle_();
        this.ticker_ = 0;
    }
    this.ticker_ ++;
    //loop obstacle
    BOK.each(this.obstacles_, function(item, index){
        if(item){
            item.updateAndCheckRemove();
            var offsetx = - CONST.CANVAS.WIDTH + CONST.BIRD.WIDTHLEAN;
            // through pipe
            if(item.x <= offsetx + CONST.BIRD.OFFSETX && (item.x >= offsetx - CONST.PIPE.WIDTH)){
                if(this.bird.y < item.posY || this.bird.y > item.posY + CONST.PIPE.APART - CONST.BIRD.HEIGHT){
                    this.onGameOver_();
                }
            }
            // step++
            if(item.x <= offsetx- CONST.PIPE.WIDTH  && item.x > offsetx - CONST.PIPE.WIDTH- CONST.GAME_PLAY.SCROLL_SPEED){
                this.step.text++;
                if(this.step.text > this.stepHigher){
                    localStorage.setItem('slappyFishHighStep',this.step.text);
                }
            }
            // before the first pipe arrive
            if(item.x >= offsetx + CONST.BIRD.OFFSETX){
                if(this.bird.y < 0 || this.bird.y > CONST.BG.HEIGHT - CONST.BIRD.HEIGHTLEAN){
                    this.onGameOver_();
                }
            }
            // remove pipe
            if(item.x <= - CONST.CANVAS.WIDTH - CONST.PIPE.WIDTH){
                this.obstacleLayer_.removeChild(item);
                this.obstacles_.splice(index, 1);
            }
        };
    }, this);
};




