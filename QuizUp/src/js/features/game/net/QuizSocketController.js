/**
 * Created by Envee.
 *
 * Date: 14-10-23
 * Time: 上午11:16
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */

goog.provide("quiz.features.game.net.QuizSocketController");
goog.require("net.DecoratedSocketController");

quiz.features.game.net.QuizSocketController = QuizSocketController;
BOK.inherits(QuizSocketController, DecoratedSocketController);
function QuizSocketController() {
    DecoratedSocketController.call(this);
}

/**
 * @param {Object} data // the player info  data format: 
 {
	name: {string},
	seat: {string}

 }	
 */
QuizSocketController.prototype.readyToPlay = function(data) {
    this.socket_.emit('readytoplay', data);
};

QuizSocketController.prototype.quitGame = function() {
    this.socket_.emit('quitgame');
};

/**
 * @param {string} // the name of selected topic
 */
QuizSocketController.prototype.setTopic = function(data){
	this.socket_.emit('settopic', data);	
};

QuizSocketController.prototype.selectOption = function(data){
    this.socket_.emit('selectoption', data);    
};

QuizSocketController.prototype.finishSelect = function(data){
    this.socket_.emit('finishselect', data);    
};

QuizSocketController.prototype.gameStart = function(data){
    this.socket_.emit('gameStart', data);    
};

QuizSocketController.prototype.getHistory = function(data){
    this.socket_.emit('gethistory', data);    
};

QuizSocketController.prototype.getTopicByPage = function(page){
    this.socket_.emit('gettopic', page);    
};

QuizSocketController.prototype.rematchSelectTopic = function(data){
    this.socket_.emit('rematchselecttopic', data);    
};
/**********************socket listener**************/
QuizSocketController.prototype.onConnect = function() {
    this.socket_.emit('init', {types:['quizup']});
};

QuizSocketController.prototype.onGetPlayerInfo = function() {
    hudAPI = appConfig.hudAPI;
    //this.socket_.emit('playerinfo', {name: hudAPI ? (hudAPI.getUserName()): 'Player'+BOK.randN(1000),  seat: hudAPI ? hudAPI.getUserInfo().seat : BOK.randN(1000), avatar: "assets/img/avatar" + BOK.randN(10) + ".jpg"});

    this.socket_.emit('playerinfo', appConfig.player);
};

QuizSocketController.prototype.onGameInit = function(data) {
    this.dispatchEvent(new Event('gameInit', data));
};

QuizSocketController.prototype.onNetworkWaiting = function(data) {
    this.dispatchEvent(new Event('networkWaiting', data));
};

QuizSocketController.prototype.onCheckAnswerResult = function(data) {
    this.dispatchEvent(new Event('checkAnsewrtResult', data));
};

QuizSocketController.prototype.onGetResult = function(data) {
    this.dispatchEvent(new Event('getResult', data));
};

QuizSocketController.prototype.onNextQuestion = function(data) {
    this.dispatchEvent(new Event('nextQuestion', data));
};

QuizSocketController.prototype.onTopicList = function(data) {
    this.dispatchEvent(new Event('topicList', data));
};

QuizSocketController.prototype.onHistoryList = function(data) {
    this.dispatchEvent(new Event('historyList', data));
};

QuizSocketController.prototype.onMatchTopicResult = function(data) {
    this.dispatchEvent(new Event('rematchTopicResult', data));
};