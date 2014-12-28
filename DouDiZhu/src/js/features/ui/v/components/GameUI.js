/**
 * Created by lydashsbok on 14-7-16.
 */

goog.provide("doudizhu.features.ui.v.components.GameUI");
goog.require("org.createjs.easeljs.EaselJS");

doudizhu.features.ui.v.components.GameUI = GameUI;
BOK.inherits(GameUI, createjs.Container);
function GameUI() {
    createjs.Container.call(this);

    //init avatar
    this.avatars_ = {
        bankerL: new createjs.Bitmap('assets/img/GameUI/Lord_L.png'),
        bankerR: new createjs.Bitmap('assets/img/GameUI/Lord_R.png'),
        playerL: new createjs.Bitmap('assets/img/GameUI/Farmer_L.png'),
        playerR: new createjs.Bitmap('assets/img/GameUI/Farmer_R.png')
    };
    BOK.each(this.avatars_, function(avatar){
        avatar.y = -70;
    });

    this.playerInfo_ = [
        new createjs.Container(),
        new createjs.Container(),
        new createjs.Container()
    ];
    BOK.each(this.playerInfo_, function(info, index){
        this.addChild(info);
        info.set(GameUI.settings.labelPos['info'+(index+1)]);
    }, this);

    //init call buttons
    this.callButtons_ = [
        new createjs.Bitmap('assets/img/GameUI/PassLord01.png'),
        new createjs.Bitmap('assets/img/GameUI/Game_BtnNor_CallScore1.png'),
        new createjs.Bitmap('assets/img/GameUI/Game_BtnNor_CallScore2.png'),
        new createjs.Bitmap('assets/img/GameUI/Game_BtnNor_CallScore3.png')
    ];
    BOK.each(this.callButtons_, function(btn, index){
        btn.x = 150 + index * 120;
        btn.y = 400;
        this.addChild(btn);
        btn.addEventListener('click', Delegate.create(this, this.onCallButtonClicked_, index));
    }, this);

    this.hintBtn_ = new createjs.Bitmap('assets/img/GameUI/Hint01.png');
    this.playBtn_ = new createjs.Bitmap('assets/img/GameUI/Out01.png');
    this.passBtn_ = new createjs.Bitmap('assets/img/GameUI/Pass01.png');
    this.passLabel_ = new createjs.Bitmap('assets/img/GameUI/Pass_Back.png');
    this.winLabel_ = new createjs.Bitmap('assets/img/GameUI/Win.png');
    this.loseLabel_ = new createjs.Bitmap('assets/img/GameUI/Lose.png');
    this.backBtn_ = new createjs.Bitmap('assets/img/GameUI/back01.png');
    this.quitBtn_ = new createjs.Bitmap('assets/img/GameUI/back01.png');
    this.passCallLabel_ = new createjs.Bitmap('assets/img/GameUI/PassLord_Back.png');


    this.winLabel_.set(GameUI.settings.labelPos.winLose);
    this.loseLabel_.set(GameUI.settings.labelPos.winLose);
    this.backBtn_.set(GameUI.settings.labelPos.backBtn);
    this.quitBtn_.set(GameUI.settings.labelPos.quitBtn);

    this.hintBtn_.set({ x: 180, y: 370 });
    this.playBtn_.set({ x: 280, y: 370 });
    this.passBtn_.set({ x: 380, y: 370 });

    this.addChild(this.hintBtn_);
    this.addChild(this.playBtn_);
    this.addChild(this.passBtn_);
    this.addChild(this.passLabel_);
    this.addChild(this.winLabel_);
    this.addChild(this.loseLabel_);
    this.addChild(this.backBtn_);
    this.addChild(this.quitBtn_);
    this.addChild(this.passCallLabel_);
}

GameUI.settings = {
    labelPos: {
        info1: {x:380, y:550},
        info2: {x:720, y:100},
        info3: {x:15, y:100},
        winLose: {x:220, y:200},
        backBtn: {x:360, y:430},
        quitBtn: {x:700, y:550},
        seat1:{x:280, y:450},
        seat2:{x:600, y:170},
        seat3:{x:50, y:170},
        duration1:{x:400, y:415},
        duration2:{x:600, y:200},
        duration3:{x:150, y:200}
    }
};


GameUI.prototype.init = function(names) {
    this.winLabel_.visible = false;
    this.loseLabel_.visible = false;
    this.backBtn_.visible = false;
    this.passLabel_.alpha = 0;
    this.passCallLabel_.alpha = 0;
    this.quitBtn_.visible = true;

    this.hideGameButtons();
    this.hideCallButtons();
    this.setPlayerNames(names);
};
/**
 * @param {Array} names
 * */
GameUI.prototype.setPlayerNames = function(names) {
    BOK.each(names, function(name, i){
        var info = this.playerInfo_[i];
        info.removeAllChildren();
        if(info.text)
            info.removeChild(info.text);
        info.text = new createjs.Text(name, "14px Arial", "#FFFFFF");
        info.addChild(info.text);
    }, this);
};

GameUI.prototype.setAvatars = function(playerSide) {
    switch(playerSide) {
        case 'banker':
            this.playerInfo_[1].addChild(this.avatars_.playerL);
            this.playerInfo_[2].addChild(this.avatars_.playerR);
           break;
        case 'player1':
            this.playerInfo_[1].addChild(this.avatars_.playerL);
            this.playerInfo_[2].addChild(this.avatars_.bankerR);
           break;
        case 'player2':
            this.playerInfo_[1].addChild(this.avatars_.bankerL);
            this.playerInfo_[2].addChild(this.avatars_.playerR);
           break;
    }
};


GameUI.prototype.showGameResult = function(isWon) {
    if(isWon) {
        this.winLabel_.visible = true;
    } else {
        this.loseLabel_.visible = true;
    }

    this.backBtn_.visible = true;
};

GameUI.prototype.showPassLabel = function(playerName) {
    this.passLabel_.alpha = 1;
    this.passLabel_.set(GameUI.settings.labelPos[playerName]);
    createjs.Tween.get(this.passLabel_).wait(700).to({alpha:0}, 250);
};

GameUI.prototype.showCallPassLabel = function(playerName) {
    this.passCallLabel_.alpha = 1;
    this.passCallLabel_.set(GameUI.settings.labelPos[playerName]);
    createjs.Tween.get(this.passCallLabel_).wait(700).to({alpha:0}, 250);
};

GameUI.prototype.showPlayButton = function() {
    this.playBtn_.visible = true;
};
GameUI.prototype.hidePlayButton = function() {
    this.playBtn_.visible = false;
};
GameUI.prototype.showPassBtn = function(){
    this.passBtn_.visible = true;
};
GameUI.prototype.showHintBtn = function(){
    this.hintBtn_.visible = true;
};
GameUI.prototype.hideGameButtons = function() {
    this.hintBtn_.visible = false;
    this.playBtn_.visible = false;
    this.passBtn_.visible = false;
    this.hintBtn_.visible = false;
};
/**
 *
 * @param {number} bid //binding score
 */
GameUI.prototype.showCallButtons = function(bid) {
    BOK.each(this.callButtons_, function(btn, index){
        if(bid == index) btn.visible = false;
        if(index == 0) btn.visible = true;
        if(index > bid) btn.visible = true;
    });
};
GameUI.prototype.hideCallButtons = function() {
    BOK.each(this.callButtons_, function(btn){ btn.visible = false;});
};


GameUI.prototype.onCallButtonClicked_ = function(bet) {
    var e = new createjs.Event('callClicked');
    e.bet = bet;
    this.dispatchEvent(e);
};

/**
 * set timeout duration in desk
 * @param  {string} name type name of player duration
 * @param {number} duration number
 */
GameUI.prototype.setRoundDuration = function(name, duration){
        var info = new createjs.Container();
        if(info.text){
            info.text.text = duration;
        }else{
            info.text = new createjs.Text(duration, "25px Arial bold", "#FFFFFF");
            info.addChild(info.text);
            this.addChild(info);
            info.set(GameUI.settings.labelPos[name]);
            createjs.Tween.get(info.text).wait(800).to({alpha:0}, 150);
        }
};