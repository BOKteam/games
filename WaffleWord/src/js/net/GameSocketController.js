/**
 * Created by Envee.
 *
 * Date: 14-8-20
 * Time: 上午9:33
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("waffleword.net.GameSocketController");
goog.require("net.DecoratedSocketController");

waffleword.net.GameSocketController = GameSocketController;
BOK.inherits(GameSocketController, DecoratedSocketController);
function GameSocketController() {
    DecoratedSocketController.call(this);
}

GameSocketController.prototype.hostFinished = function(language, raw) {
    this.socket_.emit('hostfinished',{language: language,board:raw});
};

GameSocketController.prototype.slaveFinished = function() {
    this.socket_.emit('slavefinished');
};

GameSocketController.prototype.readyToPlay = function(data) {
    this.socket_.emit('readytoplay', data);
};

GameSocketController.prototype.selectRightCell = function(word) {
    this.socket_.emit('selectrightcell', word);
};

GameSocketController.prototype.refreshOpScore = function( opScore) {
    this.socket_.emit('refreshopscore', {opScore: opScore});
};

GameSocketController.prototype.gameFinished = function(data) {
    this.socket_.emit('gamefinished',data);
};

GameSocketController.prototype.playerLeftGame = function() {
    this.socket_.emit('quitgame');
};

GameSocketController.prototype.onePlayerInfoReady = function(data) {
    this.socket_.emit('playerinfo', data);
};

GameSocketController.prototype.cancelReady = function() {
    this.socket_.emit('quitgame');
};
/**
 * Socket listener
 */
GameSocketController.prototype.onConnect = function() {
    this.socket_.emit('init', {types:['waffle']});
};

GameSocketController.prototype.onGameStart = function() {
    this.dispatchEvent(new Event('gameStart'));
};

GameSocketController.prototype.onAllPlayerInfo = function(data) {
    this.dispatchEvent(new Event('allPlayerInfo', data));
};

GameSocketController.prototype.onSelectInfo = function(data) {
    this.dispatchEvent(new Event('selectinfo', data));
};

GameSocketController.prototype.onWaitingMakeRaw = function(data) {
    this.dispatchEvent(new Event('waitingMakeRaw', data));
};

GameSocketController.prototype.onWaitingSlave = function(data) {
    this.dispatchEvent(new Event('waitingSlave', data));
};

GameSocketController.prototype.onPartnerRight = function(data) {
    this.dispatchEvent(new Event('partnerRight', data));
};

GameSocketController.prototype.onRefreshOpScore = function(data) {
    this.dispatchEvent(new Event('refreshOpScore', data));
};

GameSocketController.prototype.onNetworkWaiting = function(data) {
    this.dispatchEvent(new Event('networkWaiting', data));
};

GameSocketController.prototype.onStartSingleGame = function(data) {
    this.dispatchEvent(new Event('startSingleGame', data));
};

GameSocketController.prototype.onGetResult = function(data) {
    this.dispatchEvent(new Event('getResult', data));
};

GameSocketController.prototype.onPlayerDisconnect = function() {
    this.dispatchEvent(new Event('playerDisconnect'));
};

GameSocketController.prototype.onReplayNetworkWaiting = function(data) {
    this.dispatchEvent(new Event('replayNetworkWaiting', data));
};
