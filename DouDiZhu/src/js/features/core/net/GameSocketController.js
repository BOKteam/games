/**
 * Created by Enveesoft.
 * User: Liu Xinyi
 * Date: 14-7-23
 * Time: 下午8:28
 * Write the description in this section.
 */

goog.provide("doudizhu.features.core.net.GameSocketController");
goog.require("net.DecoratedSocketController");

doudizhu.features.core.net.GameSocketController = GameSocketController;
BOK.inherits(GameSocketController, DecoratedSocketController);
function GameSocketController() {
    DecoratedSocketController.call(this);
}

GameSocketController.prototype.playerReadyToPlay = function() {
    this.socket_.emit('readytoplay');
};

GameSocketController.prototype.playerReadyForBid = function() {
    this.socket_.emit('playerreadyforbid');
};

GameSocketController.prototype.callForBanker = function(bid) {
    this.socket_.emit('bidbanker', {bid:bid});
};

GameSocketController.prototype.playerPass = function() {
    this.socket_.emit('playerpass');
};

GameSocketController.prototype.playHand = function(name, cards) {
    this.socket_.emit('playcards', {name:name, cards:cards});
};

GameSocketController.prototype.quitGame = function() {
    this.socket_.emit('quitgame');
};

/**
 * Socket listener
 */
GameSocketController.prototype.onConnect = function() {
    this.socket_.emit('init', {types:['doudizhu']});
    console.log('connected to game server.');
};

GameSocketController.prototype.onGetPlayerInfo = function() {
    this.socket_.emit('playerinfo', {name: hudAPI ? (hudAPI.getUserName() + ',' + hudAPI.getUserInfo().seat): 'Player'+BOK.randN(1000)});
};

GameSocketController.prototype.onGameInit = function(data) {
    this.dispatchEvent(new Event('gameInit', data));
};

GameSocketController.prototype.onNextBid = function(data) {
    this.dispatchEvent(new Event('nextBid', data));
};

GameSocketController.prototype.onGameStart = function(data) {
    this.dispatchEvent(new Event('gameStart', data));
};

GameSocketController.prototype.onGameFinished = function(data) {
    this.dispatchEvent(new Event('gameFinished', data));
};

GameSocketController.prototype.onGamePass = function(data) {
    this.dispatchEvent(new Event('pass', data));
};

GameSocketController.prototype.onHandPlayed = function(data) {
    this.dispatchEvent(new Event('play', data));
};

GameSocketController.prototype.onNextPlayer = function(data) {
    this.dispatchEvent(new Event('nextPlayer', data));
};

GameSocketController.prototype.onNetworkWaiting = function(data) {
    this.dispatchEvent(new Event('networkWaiting', data));
};


