/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-9-22
 * Time: 上午11:42
 * Write the description in this section.
 */
goog.provide("noone.features.game.v.GameViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("noone.features.game.v.component.Road");
goog.require("noone.features.game.v.component.Runner");
goog.require("noone.features.game.v.component.Obstacle");
goog.require("noone.features.game.v.component.EndPanel");

noone.features.game.v.GameViewMediator = GameViewMediator;
BOK.inherits(GameViewMediator, BaseMediator);
function GameViewMediator(stage) {
    BaseMediator.call(this);
    this.stage_ = stage;

    this.stage_.addChild(new createjs.Shape(new createjs.Graphics().beginFill("rgba(255,255,255,1)").drawRect(0, 0, 640, 480)));

    var bg0 = new noone.features.game.v.component.Road();
    bg0.y = 160;
    var bg1 = new noone.features.game.v.component.Road();
    bg1.y = 360;
    this.bgLayer_ = new createjs.Container();
    this.bgLayer_.addChild(bg0, bg1);
    this.scoreText_ = new createjs.Text("0.00","35px dimboregular", "#FE3200");
    this.scoreText_.set({x:300,y:50, lineWidth :2,shadow:new createjs.Shadow("#000", 2, 2, 10)});

    this.runner_ = new noone.features.game.v.component.Runner(106);
    this.runner_.x = 100;
    this.runner2_ = new noone.features.game.v.component.Runner(306);
    this.runner2_.x = 100;
    this.stage_.addChild(this.bgLayer_, this.runner_, this.runner2_, this.scoreText_);
    localStorage.highScore = 0;
    //this.init();
}

/**
 * @override
 * */
GameViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getOutputNoteWithFeatureName('START_GAME'), this.init, BaseMediator.SCOPE.PARENT);
    GameViewMediator.superClass_.declareInterestedNotifications.call(this);
};

GameViewMediator.prototype.init = function(){
    this.score_ = 0;
    this.ticker_ = 80;
    this.bgLayer_.visible = true;
    this.scoreText_.visible = true;
    this.runner_.visible = true;
    this.runner2_.visible = true;
    this.scoreText_.text = "0.00";
    this.runner_.set({x: 100, y: 0});
    this.runner2_.set({x: 100, y: 106});
    this.runner_.baseY_ = 106;
    console.log(this.runner_.y);
    this.runner2_.baseY_ = 306;

    this.runner_.bird.gotoAndPlay("run");
    this.runner2_.bird.gotoAndPlay("run");
    this.obstacles_ = [];

    this.shapeos = [];

    this.stage_.addEventListener('mousedown', Delegate.create(this, this.onTap_));
    document.onkeydown = Delegate.create(this, this.onKeyDown_);
    this.tickListener_ = createjs.Ticker.addEventListener('tick', Delegate.create(this, this.update_));
    this.scoreTimer_();
};


GameViewMediator.prototype.update_ = function(){
    //generate obstacle

    if(this.ticker_ == 80){
        this.generateObstacle_();
        this.ticker_ = 0;
    }

    this.ticker_ ++;
    this.bgLayer_.x -= CONST.GAME_PLAY.SCROLL_SPEED;
    if(this.bgLayer_.x <= -CONST.BG.SEGMENT)
        this.bgLayer_.x  = 0;


    this.runner_.update();
    this.runner2_.update();

    for(var k=0;k < this.shapeos.length; k++){
        this.stage_.removeChild(this.shapeos[k]);
    }
    this.shapeos =[];
    /**add obstacle**/
    for(var i=0; i< this.obstacles_.length; i++){
        this.obstacles_[i].update();


        /*var obRect= this.obstacles_[i].getRectRange();
        var go = new createjs.Graphics().beginStroke("rgba(255,8,8,1)").drawRect(obRect.x, obRect.y, obRect.width, obRect.height).endStroke();
        var shapeo = new createjs.Shape(go);
        this.shapeos.push(shapeo);*/
        if(this.isCollide_(this.runner_.getRectRange(),this.obstacles_[i].getRectRange()) || this.isCollide_(this.runner2_.getRectRange(),this.obstacles_[i].getRectRange())){
            createjs.Ticker.setPaused(true);
            createjs.Ticker.off("tick", this.tickListener_);
            this.runner_.bird.gotoAndStop();
            this.runner2_.bird.gotoAndStop();
            this.stopScoreTimer_();
            this.gameOver_();
        }


        if(this.obstacles_[i].x <= -CONST.BG.SEGMENT)
            this.obstacles_.splice(i, 1);
    }


    /**
     * add test rect
     * **/
    /*this.stage_.removeChild(this.shape2, this.shape);
    var rec2= this.runner2_.getRectRange();
    var rec= this.runner_.getRectRange();
    var g = new createjs.Graphics().beginStroke("rgba(255,8,8,1)").drawRect(rec.x, rec.y, rec.width, rec.height).endStroke();
    var g2 = new createjs.Graphics().beginStroke("rgba(255,8,8,1)").drawRect(rec2.x, rec2.y, rec2.width, rec2.height).endStroke();
    this.shape2 = new createjs.Shape(g2);
    this.shape = new createjs.Shape(g);
    this.stage_.addChild(this.shape2, this.shape);
    for( var m=0;m < this.shapeos.length; m++){
        this.stage_.addChild(this.shapeos[m]);
    }*/
    /**test**/


};
GameViewMediator.prototype.onTap_ = function(event){
    if(event.localX < 320){
        this.runner_.flap();
    }else{
        this.runner2_.flap();
    }

};

GameViewMediator.prototype.onKeyDown_ = function (event) {
    var keynum;
    if (window.event) // IE
    {
        keynum = event.keyCode
    }
    else if (event.which) // Netscape/Firefox/Opera
    {
        keynum = event.which
    }
    if (keynum == 49) {
        this.runner_.flap();
    } else if (keynum == 50) {
        this.runner2_.flap();
    }
};

GameViewMediator.prototype.generateObstacle_ = function () {
    var obstacle1 = new noone.features.game.v.component.Obstacle();

    obstacle1.x = (640 + CONST.OBSTACLE.MIN_SUB_X + BOK.randN(CONST.OBSTACLE.MAX_SUB_X - CONST.OBSTACLE.MIN_SUB_X));
    obstacle1.y = (161 - obstacle1.height);

    var obstacle2 = new noone.features.game.v.component.Obstacle();
    obstacle2.x = (640 + CONST.OBSTACLE.MIN_SUB_X + BOK.randN(CONST.OBSTACLE.MAX_SUB_X - CONST.OBSTACLE.MIN_SUB_X));
    obstacle2.y = (361 - obstacle2.height);

    this.obstacles_.push(obstacle1, obstacle2);
    this.stage_.addChild(obstacle1, obstacle2);
    this.stage_.swapChildren(obstacle1, this.runner_);
    this.stage_.swapChildren(obstacle2, this.runner2_);
};


/**
 *
 * @param {Object} range1  {x, y, width, height}
 * @param {Object} range2  {x, y, width, height}
 */
GameViewMediator.prototype.isCollide_ = function(range1, range2){
    var xDiff =  Math.abs(range1.x - range2.x);
    var yDiff =  Math.abs(range1.y  - range2.y);

    if(range1.x >= range2.x ){
        return (xDiff <= range2.width && yDiff < range2.height);
    }else{
        return (xDiff <= range1.width && yDiff <= range1.height)
    }
};

GameViewMediator.prototype.scoreTimer_ = function(){
    this.score_ += 1;
    this.scoreText_.text = this.score_ / 100;
    this.scoreTimerId_ = setTimeout(Delegate.create(this, this.scoreTimer_),10);
};

GameViewMediator.prototype.stopScoreTimer_ = function(){
    if(this.scoreTimerId_){
        clearTimeout(this.scoreTimerId_);
        this.scoreTimerId_ = null;
    }
};

GameViewMediator.prototype.gameOver_ = function(){
    if(this.score_ > localStorage.highScore){
        localStorage.highScore = this.score_;
    }
    this.endPanel_ = new noone.features.game.v.component.EndPanel();
    this.endPanel_.setThisScore(this.score_);
    this.stage_.addChild(this.endPanel_);
    this.endPanel_.submitBtn_.addEventListener('click', Delegate.create(this, this.submitBtnClick_));
    this.endPanel_.backBtn_.addEventListener('click', Delegate.create(this, this.backBtnClick_));
    this.stage_.removeAllEventListeners ('mousedown');
};

GameViewMediator.prototype.submitBtnClick_ = function(){

};
GameViewMediator.prototype.backBtnClick_ = function(){
    this.bgLayer_.visible = false;
    this.scoreText_.visible = false;
    this.runner_.visible = false;
    this.runner2_.visible = false;
    this.endPanel_.visible = false;
    this.clearObstacles_();

    this.sendParentNotification(MainGameFeatureNotes.getOutputNoteWithFeatureName('BACK_MENU'));
};

GameViewMediator.prototype.clearObstacles_ = function(){
    for(var i=0;i < this.obstacles_.length; i++){
        this.stage_.removeChild(this.obstacles_[i]);
    }
    for(i=0;i < this.shapeos.length; i++){
        this.stage_.removeChild(this.shapeos[i]);
    }

    this.stage_.removeChild(this.shape2, this.shape);
};

