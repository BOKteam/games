/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.GamePlayMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("quiz.features.game.MainGameFeatureNotes");
goog.require("quiz.features.game.net.QuizSocketController");


quiz.features.game.v.GamePlayMediator = GamePlayMediator;
BOK.inherits(GamePlayMediator, BaseMediator);

function GamePlayMediator() {
	BaseMediator.call(this);

    this.con_ = new quiz.features.game.net.QuizSocketController() 
    this.con_.init('/socket', {port: 8081});

    this.con_.addEventListener('gameInit', Delegate.create(this, this.onGameInit_));
    this.con_.addEventListener('gameStart', Delegate.create(this, this.onGameStart_));
    this.con_.addEventListener('gameFinished', Delegate.create(this, this.onGameFinished_));
    this.con_.addEventListener('networkWaiting', Delegate.create(this, this.onNetworkWaiting_));
    this.con_.addEventListener('checkAnsewrtResult', Delegate.create(this, this.onCheckAnsertResult_));
    this.con_.addEventListener('getResult', Delegate.create(this, this.onGetResult_));
    this.con_.addEventListener('nextQuestion', Delegate.create(this, this.onNextQuestionInfo_));
    this.con_.addEventListener('topicList', Delegate.create(this, this.onTopicList_));
    this.con_.addEventListener('historyList', Delegate.create(this, this.onHistoryList_));
    this.con_.addEventListener('rematchTopicResult', Delegate.create(this, this.onRematchTopicResult_));
}

/**
 * @override
 * */
GamePlayMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('READY_TO_START'), this.onReadyToStart_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('SELECT_OPTION'), this.onSelectOption_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('FINISH_SELECT'), this.onFinishSelect_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('PLAYER_LEFT_GAME'), this.onPlayerLeftGame_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_START'), this.onGameStart_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('SET_TOPIC'), this.onSetTopic_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GET_HISTORY'), this.onGetHistory_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GET_TOPIC'), this.onGetTopic_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('REMATCH_SELECT_TOPIC'), this.onRematchSelectTopic_);

    GamePlayMediator.superClass_.declareInterestedNotifications.call(this);
};

/******************************** Notes listener     ******************************************************/

GamePlayMediator.prototype.onReadyToStart_ = function(msg){
    this.con_.readyToPlay(msg.body);
};

GamePlayMediator.prototype.onSelectOption_ = function(msg){
    this.con_.selectOption(msg.body);
};

GamePlayMediator.prototype.onFinishSelect_ = function(msg){
    this.con_.finishSelect(msg.body);
};

GamePlayMediator.prototype.onPlayerLeftGame_ = function(){
    this.con_.quitGame();  
};

GamePlayMediator.prototype.onGameStart_ = function(msg){
    this.con_.gameStart(msg.body);  
};
GamePlayMediator.prototype.onSetTopic_ = function(msg){
    this.con_.setTopic(msg.body);  
};

GamePlayMediator.prototype.onGetHistory_ = function(msg){
    this.con_.getHistory(msg.body);  
};
GamePlayMediator.prototype.onGetTopic_ = function(msg){
    this.con_.getTopicByPage(msg.body);  
};

GamePlayMediator.prototype.onRematchSelectTopic_ = function(msg){
    this.con_.rematchSelectTopic(msg.body);  
};  
/******************************** conn event listener******************************************************/

GamePlayMediator.prototype.onGameInit_ = function(e){
	this.sendNotification(MainGameFeatureNotes.getInternalNote('GAME_INIT'), e.body);
};

GamePlayMediator.prototype.onGameStart_ = function(){
	
};

GamePlayMediator.prototype.onGameFinished_ = function(){
	
};

GamePlayMediator.prototype.onNetworkWaiting_ = function(e){
	this.sendNotification(MainGameFeatureNotes.getInternalNote('NETWORK_WAITING'), e.body);
};

GamePlayMediator.prototype.onCheckAnsertResult_ = function(e){
    this.sendNotification(MainGameFeatureNotes.getInternalNote('CHECK_ANSWER_RESULT'), e.body);
};

GamePlayMediator.prototype.onGetResult_ = function(e){
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_RESULT'), e.body);
};

GamePlayMediator.prototype.onNextQuestionInfo_ = function(e){
     this.sendNotification(MainGameFeatureNotes.getInternalNote('NEXT_QUESTION_INFO'), e.body);
};

GamePlayMediator.prototype.onTopicList_ = function(e){
    this.sendNotification(MainGameFeatureNotes.getInternalNote('TOPIC_LIST_READY'), e.body);
};

GamePlayMediator.prototype.onHistoryList_ = function(e){
    this.sendNotification(MainGameFeatureNotes.getInternalNote('HISTORY_LIST_READY'), e.body);
};

GamePlayMediator.prototype.onRematchTopicResult_ = function(e){
    this.sendNotification(MainGameFeatureNotes.getInternalNote('REMATCH_TOPIC_RESULT'), e.body);
};