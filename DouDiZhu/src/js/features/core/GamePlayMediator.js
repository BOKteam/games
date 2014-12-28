/**
 * Created by lydashsbok on 14-7-15.
 */
goog.provide("doudizhu.features.core.GamePlayMediator");
goog.require("bok.framework.core.BaseMediator");

goog.require("doudizhu.features.core.l.GamePlay");

GamePlay = doudizhu.features.core.l.GamePlay;

doudizhu.features.core.GamePlayMediator = GamePlayMediator;

BOK.inherits(GamePlayMediator, BaseMediator);
function GamePlayMediator() {
    BaseMediator.call(this);

    this.game_ = new doudizhu.features.core.l.GamePlay();

    this.game_.addEventListener('play', Delegate.create(this, this.onGamePlay_));
    this.game_.addEventListener('pass', Delegate.create(this, this.onGamePass_));
    this.game_.addEventListener('gameFinished', Delegate.create(this, this.onGameFinished_));
    this.game_.addEventListener('gameStart', Delegate.create(this, this.onGameStart_));
    this.game_.addEventListener('nextBid', Delegate.create(this, this.onNextBid_));
    this.game_.addEventListener('nextPlayer', Delegate.create(this, this.onNextPlayer_));
}

/**
 * @override
 * */
GamePlayMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(CoreFeatureNotes.getInputNote('READY_TO_START'), this.onReadyToStart_);
    this.declareInterest(CoreFeatureNotes.getInputNote('PLAY_HAND'), this.onPlayHand_);
    this.declareInterest(CoreFeatureNotes.getInputNote('PLAYER_PASSED'), this.onPass_);
    this.declareInterest(CoreFeatureNotes.getInputNote('BID_READY_TO_START'), this.onBidStart_);
    this.declareInterest(CoreFeatureNotes.getInputNote('BID_BANKER'), this.onBidBanker_);
    this.declareInterest(GameUIFeatureNotes.getOutputNoteWithFeatureName('BACK_TO_MENU'), this.onQuitGame_, BaseMediator.SCOPE.PARENT);

    GamePlayMediator.superClass_.declareInterestedNotifications.call(this);
};

GamePlayMediator.prototype.onReadyToStart_ = function(msg) {
    this.game_.newGame(GamePlay.sideNames);

    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_INIT'),
        {   playerName: GamePlay.sideNames[0],
            deck: BOK.cloneObject(this.game_.game_.deckCopy),
            bankerCard: this.game_.game_.bankerCard,
            sideNames: GamePlay.sideNames
        });
};

GamePlayMediator.prototype.onPlayHand_ = function(msg) {
    var player = msg.body.name;
    var hand = msg.body.cards;

    this.game_.playHand(player, hand);
};

GamePlayMediator.prototype.onPass_ = function() {
    this.game_.tryToPass();
};

GamePlayMediator.prototype.onBidStart_ = function() {
    this.game_.nextBidForBanker();
};

GamePlayMediator.prototype.onBidBanker_ = function(msg) {
    this.game_.callForBanker(msg.body.name, msg.body.bid);
};

GamePlayMediator.prototype.onQuitGame_ = function(msg) {
    //set a new game when quit to prevent further messages from old game.
    this.game_.newGame(GamePlay.sideNames);
};


/************************** component event listener *****************************************/
GamePlayMediator.prototype.onGameStart_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_START'), e.body);
};

GamePlayMediator.prototype.onNextBid_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('NEXT_CALLER'), e.body);
};

GamePlayMediator.prototype.onGameFinished_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_END'), e.body);
};

GamePlayMediator.prototype.onGamePlay_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('HAND_PLAYED'), e.body);
};

GamePlayMediator.prototype.onGamePass_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('PLAYER_PASSED'), e.body);
};

GamePlayMediator.prototype.onNextPlayer_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('NEXT_PLAYER'), e.body);
};



