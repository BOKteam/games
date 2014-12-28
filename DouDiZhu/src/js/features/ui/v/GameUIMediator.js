/**
 * Created by lydashsbok on 14-7-16.
 */
goog.provide("doudizhu.features.ui.v.GameUIMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("doudizhu.features.ui.v.components.GameUI");
doudizhu.features.ui.v.GameUIMediator = GameUIMediator;

BOK.inherits(GameUIMediator, BaseMediator);
function GameUIMediator(stage) {
    BaseMediator.call(this);

    this.ui_ = new doudizhu.features.ui.v.components.GameUI();
    this.playerName_ = '';
    this.sideNames_ = [];
    this.playerSelectedHand_ = null;
    this.playerSuggesedHand_ = null;
    this.playerSide_ = '';
    this.currentPlayer_ = null;

    stage.addChild(this.ui_);

    //reg events
    this.ui_.playBtn_.addEventListener('click', Delegate.create(this, this.onPlayClicked_));
    this.ui_.passBtn_.addEventListener('click', Delegate.create(this, this.onPassClicked_));
    this.ui_.hintBtn_.addEventListener('click', Delegate.create(this, this.onHintClicked_));
    this.ui_.backBtn_.addEventListener('click', Delegate.create(this, this.onBackClicked_));
    this.ui_.addEventListener('callClicked', Delegate.create(this, this.onCallClicked_));
    this.ui_.quitBtn_.addEventListener('click', Delegate.create(this, this.onQuitClicked_));
}

/**
 * @override
 * */
GameUIMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getOutputNoteWithFeatureName('HAND_SELECTED'), this.onSelected_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('HAND_PLAYED'), this.onHandPlayed_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('PLAYER_PASSED'), this.onPlayerPassed_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_INIT'), this.onGameInit_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_START'), this.onGameStart_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('GAME_END'), this.onGameEnd_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('NEXT_CALLER'), this.onNextCaller_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(CoreFeatureNotes.getOutputNoteWithFeatureName('NEXT_PLAYER'), this.onNextPlayer_, BaseMediator.SCOPE.PARENT);
    this.declareInterest(GameUIFeatureNotes.getOutputNoteWithFeatureName('BACK_TO_MENU'), this.stopActionTimeout_, BaseMediator.SCOPE.PARENT);

    GameUIMediator.superClass_.declareInterestedNotifications.call(this);
};

GameUIMediator.prototype.onSelected_ = function(msg) {
    var handInfo = msg.body;
    this.playerSelectedHand_ = handInfo;

    if(this.currentPlayer_ != handInfo.name)
        return;

    if(handInfo.cards.length) {
        this.ui_.showPlayButton();
    } else {
        this.ui_.hidePlayButton();
    }
};
GameUIMediator.prototype.onGameInit_ = function(msg) {
    this.playerName_ = msg.body.playerName;
    var sideNames = BOK.cloneObject(msg.body.sideNames);
    while(sideNames[0] != this.playerName_) {
        sideNames.push(sideNames.shift());
    }
    this.sideNames_ = sideNames;

    this.ui_.init(sideNames);
};

GameUIMediator.prototype.onGameStart_ = function(msg) {
    var info = msg.body;
    if(info.bankerName == this.playerName_) {
        this.playerSide_ = 'banker';
    } else if(info.player1Name == this.playerName_){
        this.playerSide_ = 'player1';
    } else if(info.player2Name == this.playerName_){
        this.playerSide_ = 'player2';
    }

    this.ui_.setAvatars(this.playerSide_);
};

GameUIMediator.prototype.onNextCaller_ = function(msg) {
    var lastCaller = null;
    var name = msg.body.name;
    var seatNumber = 1;
    BOK.each(this.sideNames_, function(sideName, index){
        if(sideName == name){
            lastCaller = this.sideNames_[(index - 1) < 0 ? 2 : (index - 1)];
            return BOK.BREAK;
        }
    }, this);
    BOK.each(this.sideNames_, function(sideName, index){
        if(sideName == lastCaller){
            seatNumber = ((index + 1) > 3 ? 1 : (index + 1));
            return BOK.BREAK;
        }
    }, this);

    if(msg.body.lastCallerPassed){
        this.ui_.showCallPassLabel("seat" + seatNumber);
    }
    if(this.playerName_ == msg.body.name) {
        this.ui_.showCallButtons(msg.body.bid);
    }
};

GameUIMediator.prototype.onHandPlayed_ = function() {
    this.playerSelectedHand_ = null;
    this.playerSuggesedHand_ = null;
};

GameUIMediator.prototype.onPlayerPassed_ = function(msg) {
    //find which seat passed
    var name = this.playerSide_;
    var seatNumber = 1;
    while(msg.body.name != name && seatNumber <= 3) {
        name = CoreRule._getInstance_().getNextPlayer(name);
        seatNumber++;
    }
    this.ui_.showPassLabel('seat'+seatNumber);
};

GameUIMediator.prototype.onGameEnd_ = function(msg) {
    var result = msg.body;

    this.ui_.showGameResult((result.bankerWon && this.playerSide_ == 'banker') ||
        (!result.bankerWon && this.playerSide_ != 'banker'));
    this.stopActionTimeout_();
};
GameUIMediator.prototype.onNextPlayer_ = function(msg){
    this.currentPlayer_ =  msg.body.name;
    this.playerSuggesedHand_ = msg.body.suggestCards;

    if(msg.body.roundDuration != null && msg.body.roundDuration != undefined){
        var name = this.playerSide_;
        var seatNumber = 1;
        while(this.currentPlayer_!= name && seatNumber <= 3) {
            name = CoreRule._getInstance_().getNextPlayer(name);
            seatNumber++;
        }
        this.durationName = 'duration'+seatNumber;
        this.stopActionTimeout_();
        this.durationNum =  msg.body.roundDuration / 1000;
        this.playerActionTimeout_();
    }

    if( this.currentPlayer_ == this.playerSide_){
        this.ui_.showHintBtn();
        if(msg.body.allowPass){
            this.ui_.showPassBtn();
        }

        if (this.playerSelectedHand_ && this.playerSelectedHand_.cards.length) {
            this.ui_.showPlayButton();
        }
    } else {
        this.ui_.hideGameButtons();
    }
};
/**
 * play timeout start
 **/
GameUIMediator.prototype.playerActionTimeout_ = function(){
    if(this.durationNum <= 0){
        this.stopActionTimeout_();
        return;
    }
    this.ui_.setRoundDuration(this.durationName, this.durationNum);
    this.durationNum --;
    this.actionTimeoutId_ = setTimeout(Delegate.create(this, this.playerActionTimeout_),1000);
};
/**
 * clear  play timeout
 * */
GameUIMediator.prototype.stopActionTimeout_ = function() {
    if(this.actionTimeoutId_) {
        clearTimeout(this.actionTimeoutId_);
        this.actionTimeoutId_ = null;
    }
};
//************************//
//event listeners
GameUIMediator.prototype.onBackClicked_ = function() {
    this.sendNotification(GameUIFeatureNotes.getOutputNote('BACK_TO_MENU'))
};

GameUIMediator.prototype.onPlayClicked_ = function() {
    if(this.playerSelectedHand_){
        this.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('PLAY_HAND'), BOK.cloneObject(this.playerSelectedHand_));
    }
};

GameUIMediator.prototype.onPassClicked_ = function() {
    this.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('PLAYER_PASSED'));
};

GameUIMediator.prototype.onHintClicked_ = function() {
    if(this.playerSuggesedHand_ != null){
            this.sendParentNotification(GameUIFeatureNotes.getOutputNoteWithFeatureName('PLAY_HINT'), {name:this.playerSide_, cards: this.playerSuggesedHand_ });
    }else{
        this.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('PLAYER_PASSED'));
    }
};

GameUIMediator.prototype.onCallClicked_ = function(e) {
    this.ui_.hideCallButtons();
    this.sendParentNotification(CoreFeatureNotes.getInputNoteWithFeatureName('BID_BANKER'), {name: this.playerName_, bid: e.bet});
};

GameUIMediator.prototype.onQuitClicked_ = function() {
    this.sendNotification(GameUIFeatureNotes.getOutputNote('BACK_TO_MENU'));
};


