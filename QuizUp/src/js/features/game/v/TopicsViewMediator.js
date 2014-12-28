/**
 * Created by Envee.
 *
 * Date: 14-10-21
 * Time: 上午9:08
 * @author: <a href="526597516@qq.com">luyc</a>
 * @version: 0.1
 */
goog.provide("quiz.features.game.v.TopicsViewMediator");
goog.require("bok.framework.core.BaseMediator");
goog.require("org.createjs.easeljs.EaselJS");
goog.require("quiz.features.game.v.GamePlayMediator");
goog.requireAll("quiz.features.game.v.component.*");
goog.require("quiz.features.game.MainGameFeatureNotes");


quiz.features.game.v.TopicsViewMediator = TopicsViewMediator;
BOK.inherits(TopicsViewMediator, BaseMediator);

function TopicsViewMediator(stage) {
    BaseMediator.call(this);

    this.ui_ = new quiz.features.game.v.component.TopicsUI();
    this.ui_.visible = false;
    var bg = new createjs.Shape(new createjs.Graphics().beginFill('#333333').drawRect(0, 0, CONST.BG.WIDTH, CONST.BG.HEIGHT).endFill());
    stage.addChild(this.ui_);
    stage.addChildAt(bg, 0);
    this.currentPage_  = 1;
    this.totalPage_ = 1;

    this.ui_.footerBar_.history.addEventListener('click', Delegate.create(this, this.onHistoryClick_));
    this.ui_.next_.addEventListener('click', Delegate.create(this, this.onNextClick_));
    this.ui_.prev_.addEventListener('click', Delegate.create(this, this.onPrevClick_));
}


TopicsViewMediator.prototype.declareInterestedNotifications = function() {
    this.declareInterest(MainGameFeatureNotes.getInternalNote('NETWORK_WAITING'), this.onNetworkWaiting_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GAME_INIT'), this.onGameInit_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('PLAYER_LEFT_GAME'), this.onPlayerLeftGame_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('TOPIC_LIST_READY'), this.onTopicListReady_);
    this.declareInterest(MainGameFeatureNotes.getInternalNote('GO_TOPIC'), this.onGoTopic_);

    TopicsViewMediator.superClass_.declareInterestedNotifications.call(this);
};

TopicsViewMediator.prototype.onNextClick_ = function(e){
    if(this.currentPage_  >= this.totalPage_) {return false;}
    this.currentPage_ ++;
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_TOPIC'), this.currentPage_);
};

TopicsViewMediator.prototype.onPrevClick_ = function(e){
    if(this.currentPage_  <= 1) {return false;}
    this.currentPage_ --;
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GET_TOPIC'), this.currentPage_);
};
TopicsViewMediator.prototype.onGoTopic_ = function(){
    this.ui_.show();
};

TopicsViewMediator.prototype.onHistoryClick_ = function(){
    if(this.ui_.blocked)
        return;
    this.ui_.hide();
    this.sendNotification(MainGameFeatureNotes.getInternalNote('GO_HISTORY'));

};

TopicsViewMediator.prototype.onGameInit_ = function(){
    this.stopReadyActionTimeout_();
    this.ui_.visible = false;
    this.ui_.hide();
};

TopicsViewMediator.prototype.onPlayerLeftGame_ = function(){
	this.ui_.show();
};

TopicsViewMediator.prototype.onNetworkWaiting_ = function(msg) {
   // this.ui_.setWaitingNum(msg.body.waitingNum);
    if(msg.body.readyDuration != null && this.actionTimeoutId_ == null){
        this.ui_.setReadydDuration(msg.body.readyDuration);
        this.stopReadyActionTimeout_();
        this.durationNum_ =  Math.round(msg.body.readyDuration / 1000);
        this.readyActionTimeout_();
    }
};

TopicsViewMediator.prototype.onTopicListReady_ = function(msg){
    this.ui_.initTopicsAll(msg.body.topicList);
    this.totalPage_ = Math.ceil(msg.body.count / 5 );
    this.ui_.setPage(this.currentPage_, this.totalPage_);
    BOK.each(this.ui_.topicBars_, function(topic){
        topic.bar.addEventListener('click', Delegate.create(this, this.onTopicBtnClick_, topic));
    }, this);
};


TopicsViewMediator.prototype.readyActionTimeout_ = function(){
    if(this.durationNum_ <= 0){
        this.stopReadyActionTimeout_();
        return;
    }
    this.ui_.setReadydDuration(this.durationNum_);
    this.durationNum_ --;
    this.actionTimeoutId_ = setTimeout(Delegate.create(this, this.readyActionTimeout_),1000);
};

TopicsViewMediator.prototype.stopReadyActionTimeout_ = function() {
    if(this.actionTimeoutId_) {
        clearTimeout(this.actionTimeoutId_);
        this.actionTimeoutId_ = null;
    }
};

/** Event Listener */
TopicsViewMediator.prototype.onTopicBtnClick_ = function(topic){
    if(this.ui_.blocked)
        return;
    var topicName = topic.bar.getTopic();
    topic.bar.setPressedColor();
    this.ui_.showNetworkWaiting();
    this.sendNotification(MainGameFeatureNotes.getInternalNote('SET_TOPIC'), topicName);
    this.sendNotification(MainGameFeatureNotes.getInternalNote('READY_TO_START'), {topic: topicName});
};


