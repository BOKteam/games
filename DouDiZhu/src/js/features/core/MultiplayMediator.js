/**
 * Created by lys.
 * User: Liu Xinyi
 * Date: 14-7-23
 * Time: 下午8:24
 * Write the description in this section.
 */

goog.provide("doudizhu.features.core.MultiplayMediator");
goog.require("bok.framework.core.BaseMediator");

goog.require("doudizhu.features.core.net.GameSocketController");

doudizhu.features.core.MultiplayMediator = MultiplayMediator;
BOK.inherits(MultiplayMediator, BaseMediator);
function MultiplayMediator() {
    BaseMediator.call(this);

    this.con_ = new doudizhu.features.core.net.GameSocketController();
    this.con_.init('/socket', {port: 8081});

    this.con_.addEventListener('gameInit', Delegate.create(this, this.onGameInit_));
    this.con_.addEventListener('nextBid', Delegate.create(this, this.onNextBid_));
    this.con_.addEventListener('gameStart', Delegate.create(this, this.onGameStart_));
    this.con_.addEventListener('pass', Delegate.create(this, this.onGamePass_));
    this.con_.addEventListener('play', Delegate.create(this, this.onHandPlayed_));
    this.con_.addEventListener('nextPlayer', Delegate.create(this, this.onNextPlayer_));
    this.con_.addEventListener('gameFinished', Delegate.create(this, this.onGameFinished_));
    this.con_.addEventListener('networkWaiting', Delegate.create(this, this.onNetworkWaiting_));
}

/**
 * @override
 * */
MultiplayMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(CoreFeatureNotes.getInputNote('READY_TO_START'), this.onReadyToStart_);
    this.declareInterest(CoreFeatureNotes.getInputNote('PLAY_HAND'), this.onPlayHand_);
    this.declareInterest(CoreFeatureNotes.getInputNote('PLAYER_PASSED'), this.onPass_);
    this.declareInterest(CoreFeatureNotes.getInputNote('BID_READY_TO_START'), this.onBidStart_);
    this.declareInterest(CoreFeatureNotes.getInputNote('BID_BANKER'), this.onBidBanker_);
    this.declareInterest(GameUIFeatureNotes.getOutputNoteWithFeatureName('BACK_TO_MENU'), this.onQuitGame_, BaseMediator.SCOPE.PARENT);

    MultiplayMediator.superClass_.declareInterestedNotifications.call(this);
};


/******************************** Notes listener     ******************************************************/
MultiplayMediator.prototype.onReadyToStart_ = function() {
    this.con_.playerReadyToPlay();
};

MultiplayMediator.prototype.onBidStart_ = function() {
    this.con_.playerReadyForBid();
};

MultiplayMediator.prototype.onBidBanker_ = function(msg) {
    this.con_.callForBanker(msg.body.bid);
};

MultiplayMediator.prototype.onPass_ = function() {
    this.con_.playerPass();
};

MultiplayMediator.prototype.onPlayHand_ = function(msg) {
    this.con_.playHand(msg.body.name, msg.body.cards);
};


MultiplayMediator.prototype.onQuitGame_ = function() {
    this.con_.quitGame();
};
/******************************** conn event listener******************************************************/
MultiplayMediator.prototype.onGameInit_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_INIT'), e.body);
};

MultiplayMediator.prototype.onNextBid_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('NEXT_CALLER'), e.body);
};

MultiplayMediator.prototype.onGameStart_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_START'), e.body);
};

MultiplayMediator.prototype.onGameFinished_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('GAME_END'), e.body);
};

MultiplayMediator.prototype.onGamePass_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('PLAYER_PASSED'), e.body);
};

MultiplayMediator.prototype.onHandPlayed_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('HAND_PLAYED'), e.body);
};

MultiplayMediator.prototype.onNextPlayer_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('NEXT_PLAYER'), e.body);
};

MultiplayMediator.prototype.onNetworkWaiting_ = function(e) {
    this.sendNotification(CoreFeatureNotes.getOutputNote('NETWORK_WAITING'), e.body);
};
