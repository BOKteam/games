/**
 * Created by Envee.
 *
 * Date: 14-11-18
 * Time: pm 10:12
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.HistoryViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.requireAll("quiz.features.game.v.component.*");


quiz.features.game.v.HistoryViewMediator = HistoryViewMediator;
BOK.inherits(HistoryViewMediator, BaseMediator);

function HistoryViewMediator(stage) {
	BaseMediator.call(this);

	this.ui_ = new quiz.features.game.v.component.HistoryUI();
	this.ui_.visible = false;
	stage.addChild(this.ui_);

	this.ui_.back_.addEventListener('click', Delegate.create(this, this.onBackClick_));
}

HistoryViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GO_HISTORY'), this.onGoHistory_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('HISTORY_LIST_READY'), this.onHistoryListReady_);

    HistoryViewMediator.superClass_.declareInterestedNotifications.call(this);
};

HistoryViewMediator.prototype.onGoHistory_ = function(){
	this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_HISTORY'), {playerId:appConfig.player.name});
};
HistoryViewMediator.prototype.onHistoryListReady_ = function(msg){
	var topicList = msg.body;
	this.ui_.initHistoryList(topicList);
	this.ui_.show();
};

HistoryViewMediator.prototype.onBackClick_ = function(e){
	this.ui_.hide();
	this.sendNotification(MainGameFeatureNotes.getInternalNote('GO_TOPIC'));
}